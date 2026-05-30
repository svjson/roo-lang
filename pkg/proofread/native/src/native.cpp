#include <algorithm>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <glob.h>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <proofread/native.h>
#include <roo-package/native_abi.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/reader.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

namespace Roo::Proofread
{
  namespace
  {
    bool has_glob_meta(const std::string& pattern)
    {
      return pattern.find_first_of("*?[") != std::string::npos;
    }

    std::vector<std::filesystem::path> expand_pattern(const std::string& pattern)
    {
      if (!has_glob_meta(pattern))
      {
        return {pattern};
      }

      glob_t matches{};
      const int result = glob(pattern.c_str(), GLOB_TILDE, nullptr, &matches);
      std::vector<std::filesystem::path> paths;

      if (result == 0)
      {
        for (size_t i = 0; i < matches.gl_pathc; ++i)
        {
          paths.emplace_back(matches.gl_pathv[i]);
        }
      }

      globfree(&matches);

      if (result == GLOB_NOMATCH)
      {
        throw std::runtime_error("No files matched pattern: " + pattern);
      }
      if (result != 0)
      {
        throw std::runtime_error("Could not expand pattern: " + pattern);
      }

      std::sort(paths.begin(), paths.end());
      return paths;
    }

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

    std::vector<std::string> string_args(const sptr_val& arg)
    {
      std::vector<std::string> values;
      for (const auto& value : arg->elements())
      {
        values.push_back(value->str());
      }
      return values;
    }

    class FailFunction : public Function
    {
     public:
      FailFunction()
        : Function(SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&FailFunction::exec_fail))))
      {
      }

      static sptr_val make() { return Value::executable(std::make_shared<FailFunction>()); }

      sptr_val exec_fail(Context&, sptr_val_v& args) { throw RooException(args[0]->str()); }
    };

    class CheckFunction : public Function
    {
     public:
      CheckFunction()
        : Function(
            SIG((FN_ARGS((&Type::VECTOR)), EXEC_DISPATCH(&CheckFunction::exec_check))))
      {
      }

      static sptr_val make() { return Value::executable(std::make_shared<CheckFunction>()); }

      sptr_val exec_check(Context&, sptr_val_v& args)
      {
        const std::vector<std::string> patterns = string_args(args[0]);
        std::vector<std::filesystem::path> files;
        std::set<std::string> seen;
        std::vector<std::string> errors;

        for (const auto& pattern : patterns)
        {
          try
          {
            for (auto path : expand_pattern(pattern))
            {
              path = path.lexically_normal();
              if (seen.insert(path.string()).second)
              {
                files.push_back(path);
              }
            }
          }
          catch (const std::exception& e)
          {
            errors.push_back(e.what());
          }
        }

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
            errors.push_back(file.string() + ": " + e.what());
          }
        }

        if (checked_count == 0 && errors.empty())
        {
          errors.push_back("No files matched.");
        }

        if (!errors.empty())
        {
          for (const auto& error : errors)
          {
            std::cerr << error << "\n";
          }
          throw RooException("proofread: checks failed");
        }

        return Value::number(static_cast<int>(checked_count));
      }
    };
  } // namespace

  std::unique_ptr<Namespace> make_native_namespace()
  {
    auto ns = std::make_unique<Namespace>("proofread.native");
    ns->set_origin(Namespace::Origin::native());
    ns->store("check!", CheckFunction::make());
    ns->store("fail!", FailFunction::make());
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

  void unload_proofread_native() {}

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
    unload_proofread_native,
    proofread_native_last_error,
  };
  return &package;
}
