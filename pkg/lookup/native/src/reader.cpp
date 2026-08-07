#include "reader.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <system_error>

#include <roo/context.h>
#include <roo/reader.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

#include "source_values.h"
#include <lookup/native.h>

namespace Roo::Lookup
{
  namespace
  {
    sptr_val read_file_forms_error_entry(const std::string& path, const std::string& message)
    {
      return Value::map({Value::keyword("error"), Value::string(message), Value::keyword("path"), Value::string(path)});
    }
  } // namespace

  /** ReadFileFormsFunction - lookup.reader/read-file-forms! */
  FUNC_IMPL(ReadFileFormsFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&ReadFileFormsFunction::exec_read_file_forms))))

  EXEC_BODY(ReadFileFormsFunction, exec_read_file_forms)
  {
    const std::filesystem::path path = args[0]->str();
    const std::string path_text = path.string();
    std::error_code ec;
    sptr_val_v entries;

    if (!std::filesystem::is_regular_file(path, ec))
    {
      if (ec)
      {
        return Value::vector(
          {read_file_forms_error_entry(path_text, "Could not inspect file: " + path_text + ": " + ec.message())});
      }
      return Value::vector(
        {read_file_forms_error_entry(path_text, "Not a regular file: " + path_text)});
    }

    SourceMap source_map;
    sptr_ast_node_v forms;
    try
    {
      const uint32_t source_id = source_map.intern_file(path.string());
      Reader reader;
      forms = reader.read_sexps(read_file(path), source_id, true);
    }
    catch (const std::exception& e)
    {
      return Value::vector({read_file_forms_error_entry(
        path_text,
        "Could not read Roo forms from " + path_text + ": " + e.what())});
    }

    entries.reserve(forms.size());
    for (const auto& form : forms)
    {
      entries.push_back(form_entry_value(source_map, form));
    }
    return Value::vector(entries);
  }

  /** ReadStdinFunction - lookup.reader/read-stdin! */
  FUNC_IMPL(ReadStdinFunction,
            SIG((NO_ARGS, EXEC_DISPATCH(&ReadStdinFunction::exec_read_stdin))))

  EXEC_BODY(ReadStdinFunction, exec_read_stdin)
  {
    std::ostringstream contents;
    contents << std::cin.rdbuf();
    return Value::string(contents.str());
  }

  std::unique_ptr<Namespace> make_reader_namespace()
  {
    auto ns = std::make_unique<Namespace>("lookup.reader");
    ns->set_origin(Namespace::Origin::native());
    ns->store("read-file-forms!", ReadFileFormsFunction::make());
    ns->store("read-stdin!", ReadStdinFunction::make());
    return ns;
  }
} // namespace Roo::Lookup
