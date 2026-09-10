#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/reader.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

#include <proofread/native.h>
#include <roo-package/native_abi.h>

namespace Roo::Proofread
{
  namespace
  {
    struct CheckError
    {
      std::string path;
      std::string message;
    };

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

    void check_file(const std::filesystem::path& path, SourceMap& source_map)
    {
      std::error_code ec;
      if (!std::filesystem::is_regular_file(path, ec))
      {
        if (ec)
        {
          throw std::runtime_error("Could not inspect file: " + path.string() + ": " +
                                   ec.message());
        }
        throw std::runtime_error("Not a regular file: " + path.string());
      }

      const std::string source = read_file(path);
      const uint32_t source_id = source_map.intern_file(path.string());
      Reader reader;
      reader.read_sexps(source, source_id, true);
    }

    void check_source(const std::string& path,
                      const std::string& source,
                      SourceMap& source_map)
    {
      const uint32_t source_id = source_map.intern_file(path);
      Reader reader;
      reader.read_sexps(source, source_id, true);
    }

    std::vector<std::string> string_args(const sptr_val& arg)
    {
      std::vector<std::string> values;
      for (const auto& value : arg->elements())
      {
        values.push_back(value->str());
      }
      return values;
    }

    size_t check_files(const std::vector<std::filesystem::path>& files,
                       std::vector<CheckError>& errors)
    {
      SourceMap source_map;
      size_t checked_count = 0;
      for (const auto& file : files)
      {
        try
        {
          check_file(file, source_map);
          checked_count++;
        }
        catch (const std::exception& e)
        {
          errors.push_back({file.string(), e.what()});
        }
      }
      return checked_count;
    }

    sptr_val error_details_value(const std::vector<CheckError>& errors)
    {
      sptr_val_v values;
      values.reserve(errors.size());
      for (const CheckError& error : errors)
      {
        values.push_back(Value::map({Value::keyword("path"),
                                     Value::string(error.path),
                                     Value::keyword("message"),
                                     Value::string(error.message)}));
      }
      return Value::vector(std::move(values));
    }

    sptr_val check_result(size_t checked_count, const std::vector<CheckError>& errors)
    {
      return Value::map({Value::keyword("checked"),
                         Value::number(static_cast<int>(checked_count)),
                         Value::keyword("errors"),
                         Value::number(static_cast<int>(errors.size())),
                         Value::keyword("error-details"),
                         error_details_value(errors)});
    }

    class FailFunction : public Function
    {
     public:
      FailFunction()
        : Function(SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&FailFunction::exec_fail))))
      {
      }

      static sptr_val make() { return Value::executable(std::make_shared<FailFunction>()); }

      sptr_val exec_fail(Context&, sptr_val_v& args)
      {
        std::cerr << args[0]->str() << std::endl;
        std::exit(EXIT_FAILURE);
      }
    };

    class CheckFilesResultFunction : public Function
    {
     public:
      CheckFilesResultFunction()
        : Function(SIG((FN_ARGS((&Type::VECTOR)),
                        EXEC_DISPATCH(&CheckFilesResultFunction::exec_check_files_result))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<CheckFilesResultFunction>());
      }

      sptr_val exec_check_files_result(Context&, sptr_val_v& args)
      {
        std::vector<std::filesystem::path> files;
        for (const auto& file : string_args(args[0]))
        {
          files.emplace_back(file);
        }

        std::vector<CheckError> errors;
        const size_t checked_count = check_files(files, errors);
        return check_result(checked_count, errors);
      }
    };

    class ReadStdinFunction : public Function
    {
     public:
      ReadStdinFunction()
        : Function(SIG((NO_ARGS, EXEC_DISPATCH(&ReadStdinFunction::exec_read_stdin))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<ReadStdinFunction>());
      }

      sptr_val exec_read_stdin(Context&, sptr_val_v&)
      {
        std::ostringstream contents;
        contents << std::cin.rdbuf();
        if (std::cin.bad())
        {
          throw RooException("Could not read standard input");
        }
        return Value::string(contents.str());
      }
    };

    class CheckSourceResultFunction : public Function
    {
     public:
      CheckSourceResultFunction()
        : Function(
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&CheckSourceResultFunction::exec_check_source_result))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<CheckSourceResultFunction>());
      }

      sptr_val exec_check_source_result(Context&, sptr_val_v& args)
      {
        const std::string path = args[0]->str();
        SourceMap source_map;
        std::vector<CheckError> errors;
        size_t checked_count = 0;
        try
        {
          check_source(path, args[1]->str(), source_map);
          checked_count = 1;
        }
        catch (const std::exception& e)
        {
          errors.push_back({path, e.what()});
        }
        return check_result(checked_count, errors);
      }
    };
  } // namespace

  std::unique_ptr<Namespace> make_native_namespace()
  {
    auto ns = std::make_unique<Namespace>("proofread.native");
    ns->set_origin(Namespace::Origin::native());
    ns->store("check-files-result!", CheckFilesResultFunction::make());
    ns->store("check-source-result!", CheckSourceResultFunction::make());
    ns->store("fail!", FailFunction::make());
    ns->store("read-stdin!", ReadStdinFunction::make());
    return ns;
  }

  std::vector<std::unique_ptr<Namespace>> make_native_namespaces()
  {
    std::vector<std::unique_ptr<Namespace>> namespaces;
    namespaces.push_back(make_native_namespace());
    return namespaces;
  }
} // namespace Roo::Proofread

namespace
{
  int load_proofread_native(const RooNativeHostV1* host)
  {
    auto ns = Roo::Proofread::make_native_namespace();
    return host->register_namespace(host->user, ns.release());
  }

  const char* proofread_native_last_error()
  {
    return "";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1* roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1),
    "proofread-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_proofread_native,
    proofread_native_last_error,
  };
  return &package;
}
