#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <lookup/native.h>
#include <roo-package/native_abi.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/reader.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

namespace Roo::Lookup
{
  namespace
  {
    std::string read_file(const std::filesystem::path& path)
    {
      errno = 0;
      std::ifstream input(path, std::ios::binary);
      if (!input)
      {
        std::string message = "Could not read file: " + path.string();
        if (errno != 0)
        {
          message += ": ";
          message += std::strerror(errno);
        }
        throw std::runtime_error(message);
      }

      std::ostringstream contents;
      contents << input.rdbuf();
      if (input.bad())
      {
        throw std::runtime_error("Could not read file: " + path.string());
      }
      return contents.str();
    }

    sptr_val source_position_value(const SourcePosition& position)
    {
      if (!position.valid())
      {
        return Constant::NIL;
      }

      return Value::map({
        Value::keyword("line"),
        Value::number(static_cast<int>(position.line)),
        Value::keyword("column"),
        Value::number(static_cast<int>(position.column)),
      });
    }

    sptr_val source_value(const SourceMap& source_map, const SourceRef& source)
    {
      return Value::map({
        Value::keyword("path"),
        Value::string(source.valid() ? source_map.file_name(source.file_id) : ""),
        Value::keyword("line"),
        source.valid() ? Value::number(static_cast<int>(source.span.start.line))
                       : Constant::NIL,
        Value::keyword("column"),
        source.valid() ? Value::number(static_cast<int>(source.span.start.column))
                       : Constant::NIL,
        Value::keyword("start"),
        source_position_value(source.span.start),
        Value::keyword("end"),
        source_position_value(source.span.end),
      });
    }

    sptr_val form_entry_value(const SourceMap& source_map, const sptr_ast_node& form)
    {
      return Value::map({
        Value::keyword("form"),
        to_rt_value(*form),
        Value::keyword("source"),
        source_value(source_map, form->get_source()),
      });
    }

    class ReadFileFormsFunction : public Function
    {
     public:
      ReadFileFormsFunction()
        : Function(SIG((FN_ARGS((&Type::STRING)),
                        EXEC_DISPATCH(&ReadFileFormsFunction::exec_read_file_forms))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<ReadFileFormsFunction>());
      }

      sptr_val exec_read_file_forms(Context&, sptr_val_v& args)
      {
        const std::filesystem::path path = args[0]->str();
        std::error_code ec;
        if (!std::filesystem::is_regular_file(path, ec))
        {
          if (ec)
          {
            throw RooException("Could not inspect file: " + path.string() + ": " +
                               ec.message());
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
          throw RooException("Could not read Roo forms from " + path.string() + ": " +
                             e.what());
        }

        sptr_val_v entries;
        entries.reserve(forms.size());
        for (const auto& form : forms)
        {
          entries.push_back(form_entry_value(source_map, form));
        }
        return Value::vector(entries);
      }
    };
  } // namespace

  std::unique_ptr<Namespace> make_native_namespace()
  {
    auto ns = std::make_unique<Namespace>("lookup.native");
    ns->set_origin(Namespace::Origin::native());
    ns->store("read-file-forms!", ReadFileFormsFunction::make());
    return ns;
  }

  std::vector<std::unique_ptr<Namespace>> make_native_namespaces()
  {
    std::vector<std::unique_ptr<Namespace>> namespaces;
    namespaces.push_back(make_native_namespace());
    return namespaces;
  }
} // namespace Roo::Lookup

namespace
{
  int load_lookup_native(const RooNativeHostV1* host)
  {
    auto ns = Roo::Lookup::make_native_namespace();
    return host->register_namespace(host->user, ns.release());
  }

  void unload_lookup_native() {}

  const char* lookup_native_last_error()
  {
    return "";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1* roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1),
    "lookup-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_lookup_native,
    unload_lookup_native,
    lookup_native_last_error,
  };
  return &package;
}
