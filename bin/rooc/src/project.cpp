#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>

#include <roo/exception.h>
#include <roo/io/dir_root_file_system.h>
#include <rooc/project.h>

namespace Rooc
{
  namespace
  {
    std::string sanitize_target_name(std::string value)
    {
      if (value.empty())
      {
        return "roo_app";
      }

      for (char& c : value)
      {
        const bool allowed = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                             (c >= '0' && c <= '9') || c == '_';
        if (!allowed)
        {
          c = '_';
        }
      }
      if (value.front() >= '0' && value.front() <= '9')
      {
        value.insert(value.begin(), '_');
      }
      return value;
    }

    std::string read_file(const std::filesystem::path& path)
    {
      std::ifstream file(path, std::ios::binary);
      if (!file)
      {
        throw Roo::RooException("Could not read file: " + path.string());
      }
      std::ostringstream stream;
      stream << file.rdbuf();
      return stream.str();
    }

    std::string generic_normal(const std::filesystem::path& path)
    {
      return path.lexically_normal().generic_string();
    }

    bool is_roo_source(const std::filesystem::path& path)
    {
      return path.extension() == ".roo" || path.extension() == ".roo" ||
             path.extension() == ".lspl";
    }

    void add_unique_file(std::vector<EmbeddedFile>& files,
                         std::map<std::string, size_t>& indexes,
                         const std::string& key,
                         const std::string& source)
    {
      if (key.empty() || indexes.count(key))
      {
        return;
      }
      indexes[key] = files.size();
      files.push_back(EmbeddedFile{key, source});
    }

    std::vector<EmbeddedFile> collect_sources(const Roo::Package::LoadPlan& plan)
    {
      std::vector<EmbeddedFile> files;
      std::map<std::string, size_t> indexes;

      for (const auto& load_path_str : plan.load_paths)
      {
        const std::filesystem::path load_path(load_path_str);
        std::error_code ec;
        if (!std::filesystem::is_directory(load_path, ec))
        {
          throw Roo::RooException("Package load root is not a directory: " +
                                  load_path.string());
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(load_path))
        {
          if (!entry.is_regular_file() || !is_roo_source(entry.path()))
          {
            continue;
          }

          const std::string source = read_file(entry.path());
          add_unique_file(files, indexes, generic_normal(entry.path()), source);
          add_unique_file(files,
                          indexes,
                          generic_normal(std::filesystem::relative(entry.path(), load_path)),
                          source);
        }
      }

      return files;
    }

    std::string default_executable_name(const Roo::Package::LoadPlan& plan)
    {
      const auto root_package = std::find_if(
        plan.packages.begin(),
        plan.packages.end(),
        [&](const auto& package) { return package.package_root == plan.package_root; });
      return root_package == plan.packages.end() ? "roo_app" : root_package->name;
    }
  } // namespace

  GeneratedProject prepare_project(const Options& options)
  {
    Roo::DirRootFileSystem manifest_fs("/");
    const auto package_dir =
      std::filesystem::absolute(options.package_dir).lexically_normal();
    auto plan = Roo::Package::resolve_load_plan(manifest_fs, package_dir.string());

    std::string executable_name = options.executable_name.empty()
                                    ? default_executable_name(plan)
                                    : options.executable_name;
    auto files = collect_sources(plan);
    return GeneratedProject{
      sanitize_target_name(executable_name),
      std::move(plan),
      std::move(files),
    };
  }
} // namespace Rooc
