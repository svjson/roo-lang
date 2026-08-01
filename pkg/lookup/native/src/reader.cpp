#include "reader.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <system_error>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/reader.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

#include "source_values.h"
#include <lookup/native.h>

namespace Roo::Lookup
{
  /** ReadFileFormsFunction - lookup.reader/read-file-forms! */
  FUNC_IMPL(ReadFileFormsFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&ReadFileFormsFunction::exec_read_file_forms))))

  EXEC_BODY(ReadFileFormsFunction, exec_read_file_forms)
  {
    const std::filesystem::path path = args[0]->str();
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec))
    {
      if (ec)
      {
        throw RooException("Could not inspect file: " + path.string() + ": " + ec.message());
      }
      throw RooException("Not a regular file: " + path.string());
    }

    SourceMap source_map;
    const uint32_t source_id = source_map.intern_file(path.string());
    Reader reader;
    sptr_ast_node_v forms;
    try
    {
      forms = reader.read_sexps(read_file(path), source_id, true);
    }
    catch (const std::exception& e)
    {
      throw RooException("Could not read Roo forms from " + path.string() + ": " + e.what());
    }

    sptr_val_v entries;
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
