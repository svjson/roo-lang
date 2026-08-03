#include <algorithm>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <glob.h>
#endif

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

    bool has_glob_meta(const std::string& pattern)
    {
      return pattern.find_first_of("*?[") != std::string::npos;
    }

    bool proofread_file(const std::filesystem::path& path)
    {
      const auto extension = path.extension();
      return extension == ".roo" || extension == ".edn";
    }

    bool ignored_directory(const std::filesystem::path& path)
    {
      const std::string name = path.filename().string();
      return name.empty() || name[0] == '.' || name == "build" ||
             name.rfind("build-", 0) == 0 || name == "dist";
    }

    bool package_root(const std::filesystem::path& path)
    {
      std::error_code ec;
      return std::filesystem::is_regular_file(path / "package.edn", ec);
    }

    void append_file(std::vector<std::filesystem::path>& files,
                     std::set<std::string>& seen,
                     std::filesystem::path path)
    {
      path = path.lexically_normal();
      if (seen.insert(path.string()).second)
      {
        files.push_back(path);
      }
    }

    void append_package_files(std::vector<std::filesystem::path>& files,
                              const std::filesystem::path& root);

#ifdef _WIN32
    std::string windows_error_message(DWORD error)
    {
      if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
      {
        return {};
      }

      LPSTR buffer = nullptr;
      const DWORD size =
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                         FORMAT_MESSAGE_IGNORE_INSERTS,
                       nullptr,
                       error,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       reinterpret_cast<LPSTR>(&buffer),
                       0,
                       nullptr);
      std::string message =
        size == 0 ? "Windows error " + std::to_string(error) : std::string(buffer, size);
      if (buffer != nullptr)
      {
        LocalFree(buffer);
      }
      while (!message.empty() && (message.back() == '\n' || message.back() == '\r'))
      {
        message.pop_back();
      }
      return message;
    }
#endif

    std::vector<std::filesystem::path> expand_pattern(const std::string& pattern)
    {
      if (!has_glob_meta(pattern))
      {
        return {pattern};
      }

#ifdef _WIN32
      WIN32_FIND_DATAA find_data;
      std::filesystem::path input(pattern);
      const std::filesystem::path parent = input.parent_path();
      const std::string native_pattern = input.make_preferred().string();
      HANDLE handle = FindFirstFileA(native_pattern.c_str(), &find_data);
      if (handle == INVALID_HANDLE_VALUE)
      {
        const std::string message = windows_error_message(GetLastError());
        if (message.empty())
        {
          throw std::runtime_error("No files matched pattern: " + pattern);
        }
        throw std::runtime_error("Could not expand pattern: " + pattern + ": " + message);
      }

      std::vector<std::filesystem::path> paths;
      do
      {
        const std::string filename = find_data.cFileName;
        if (filename != "." && filename != "..")
        {
          paths.push_back((parent / filename).lexically_normal());
        }
      } while (FindNextFileA(handle, &find_data));

      const DWORD last_error = GetLastError();
      FindClose(handle);
      if (last_error != ERROR_NO_MORE_FILES)
      {
        throw std::runtime_error("Could not expand pattern: " + pattern + ": " +
                                 windows_error_message(last_error));
      }
      if (paths.empty())
      {
        throw std::runtime_error("No files matched pattern: " + pattern);
      }
#else
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
#endif

      std::sort(paths.begin(),
                paths.end(),
                [](const auto& lhs, const auto& rhs)
                { return lhs.generic_string() < rhs.generic_string(); });
      return paths;
    }

    std::vector<std::filesystem::path> directory_files(const std::filesystem::path& root)
    {
      std::error_code ec;
      std::vector<std::filesystem::path> files;
      std::filesystem::recursive_directory_iterator entry(root, ec);
      if (ec)
      {
        throw std::runtime_error("Could not inspect directory: " + root.string() + ": " +
                                 ec.message());
      }

      const std::filesystem::recursive_directory_iterator end;
      while (entry != end)
      {
        const std::filesystem::path path = entry->path();
        if (std::filesystem::is_directory(path, ec))
        {
          if (ignored_directory(path))
          {
            entry.disable_recursion_pending();
          }
          else if (package_root(path))
          {
            append_package_files(files, path);
            entry.disable_recursion_pending();
          }
        }
        else if (std::filesystem::is_regular_file(path, ec))
        {
          if (proofread_file(path))
          {
            files.push_back(path.lexically_normal());
          }
        }
        else if (ec)
        {
          throw std::runtime_error("Could not inspect file: " + path.string() + ": " +
                                   ec.message());
        }

        entry.increment(ec);
        if (ec)
        {
          throw std::runtime_error("Could not inspect directory: " + root.string() + ": " +
                                   ec.message());
        }
      }

      std::sort(files.begin(),
                files.end(),
                [](const auto& lhs, const auto& rhs)
                { return lhs.generic_string() < rhs.generic_string(); });
      return files;
    }

    void append_package_files(std::vector<std::filesystem::path>& files,
                              const std::filesystem::path& root)
    {
      std::error_code ec;
      const std::filesystem::path manifest = root / "package.edn";
      if (std::filesystem::is_regular_file(manifest, ec))
      {
        files.push_back(manifest.lexically_normal());
      }

      const std::filesystem::path src = root / "src";
      if (std::filesystem::is_directory(src, ec))
      {
        for (const auto& file : directory_files(src))
        {
          files.push_back(file);
        }
      }
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

    std::vector<std::filesystem::path> collect_files(
      const std::vector<std::string>& patterns,
      std::vector<std::string>& errors)
    {
      std::vector<std::filesystem::path> files;
      std::set<std::string> seen;

      for (const auto& pattern : patterns)
      {
        try
        {
          for (auto path : expand_pattern(pattern))
          {
            path = path.lexically_normal();
            std::error_code ec;
            if (std::filesystem::is_directory(path, ec))
            {
              std::vector<std::filesystem::path> files_to_append;
              if (package_root(path))
              {
                append_package_files(files_to_append, path);
              }
              else
              {
                files_to_append = directory_files(path);
              }
              for (const auto& file : files_to_append)
              {
                append_file(files, seen, file);
              }
            }
            else if (ec)
            {
              throw std::runtime_error("Could not inspect file: " + path.string() + ": " +
                                       ec.message());
            }
            else
            {
              append_file(files, seen, path);
            }
          }
        }
        catch (const std::exception& e)
        {
          errors.push_back(e.what());
        }
      }

      if (files.empty() && errors.empty())
      {
        errors.push_back("No files matched.");
      }

      return files;
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

    void print_errors(const std::vector<std::string>& errors)
    {
      for (const auto& error : errors)
      {
        std::cerr << error << "\n";
      }
    }

    void fail_with_errors(const std::vector<std::string>& errors)
    {
      if (errors.empty())
      {
        return;
      }

      print_errors(errors);
      throw RooException("proofread: checks failed");
    }

    std::vector<std::string> error_messages(const std::vector<CheckError>& errors)
    {
      std::vector<std::string> messages;
      messages.reserve(errors.size());
      for (const CheckError& error : errors)
      {
        messages.push_back(error.path + ": " + error.message);
      }
      return messages;
    }

    void fail_with_check_errors(const std::vector<CheckError>& errors)
    {
      fail_with_errors(error_messages(errors));
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

    sptr_val file_vector(const std::vector<std::filesystem::path>& files)
    {
      sptr_val_v values;
      values.reserve(files.size());
      for (const auto& file : files)
      {
        values.push_back(Value::string(file.string()));
      }
      return Value::vector(std::move(values));
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
        std::vector<std::string> collect_errors;
        const std::vector<std::filesystem::path> files =
          collect_files(patterns, collect_errors);
        std::vector<CheckError> check_errors;
        const size_t checked_count = check_files(files, check_errors);
        std::vector<std::string> errors = collect_errors;
        for (const std::string& message : error_messages(check_errors))
        {
          errors.push_back(message);
        }
        fail_with_errors(errors);
        return Value::number(static_cast<int>(checked_count));
      }
    };

    class FilesFunction : public Function
    {
     public:
      FilesFunction()
        : Function(
            SIG((FN_ARGS((&Type::VECTOR)), EXEC_DISPATCH(&FilesFunction::exec_files))))
      {
      }

      static sptr_val make() { return Value::executable(std::make_shared<FilesFunction>()); }

      sptr_val exec_files(Context&, sptr_val_v& args)
      {
        std::vector<std::string> errors;
        std::vector<std::filesystem::path> files =
          collect_files(string_args(args[0]), errors);
        fail_with_errors(errors);
        return file_vector(files);
      }
    };

    class CheckFilesFunction : public Function
    {
     public:
      CheckFilesFunction()
        : Function(SIG((FN_ARGS((&Type::VECTOR)),
                        EXEC_DISPATCH(&CheckFilesFunction::exec_check_files))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<CheckFilesFunction>());
      }

      sptr_val exec_check_files(Context&, sptr_val_v& args)
      {
        std::vector<std::filesystem::path> files;
        for (const auto& file : string_args(args[0]))
        {
          files.emplace_back(file);
        }

        std::vector<CheckError> errors;
        const size_t checked_count = check_files(files, errors);
        fail_with_check_errors(errors);
        return Value::number(static_cast<int>(checked_count));
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
  } // namespace

  std::unique_ptr<Namespace> make_native_namespace()
  {
    auto ns = std::make_unique<Namespace>("proofread.native");
    ns->set_origin(Namespace::Origin::native());
    ns->store("check!", CheckFunction::make());
    ns->store("check-files!", CheckFilesFunction::make());
    ns->store("check-files-result!", CheckFilesResultFunction::make());
    ns->store("fail!", FailFunction::make());
    ns->store("files!", FilesFunction::make());
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
