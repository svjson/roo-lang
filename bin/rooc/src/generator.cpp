#include <filesystem>
#include <fstream>
#include <sstream>

#include <roo/exception.h>

#include <rooc/generator.h>

namespace Rooc
{
  namespace
  {
    void write_file(const std::filesystem::path& path, const std::string& contents)
    {
      std::filesystem::create_directories(path.parent_path());
      std::ofstream file(path, std::ios::binary);
      if (!file)
      {
        throw Roo::RooException("Could not write file: " + path.string());
      }
      file << contents;
    }

    std::string cpp_string_literal(const std::string& value)
    {
      std::ostringstream out;
      out << '"';
      for (const unsigned char c : value)
      {
        switch (c)
        {
        case '\\':
          out << "\\\\";
          break;
        case '"':
          out << "\\\"";
          break;
        case '\n':
          out << "\\n";
          break;
        case '\r':
          out << "\\r";
          break;
        case '\t':
          out << "\\t";
          break;
        default:
          if (c < 0x20 || c > 0x7e)
          {
            out << '\\' << static_cast<char>('0' + ((c >> 6) & 0x7))
                << static_cast<char>('0' + ((c >> 3) & 0x7))
                << static_cast<char>('0' + (c & 0x7));
          }
          else
          {
            out << c;
          }
          break;
        }
      }
      out << '"';
      return out.str();
    }

    std::filesystem::path repo_source_root()
    {
      return std::filesystem::path(ROO_SOURCE_ROOT).lexically_normal();
    }

    std::filesystem::path repo_build_root()
    {
      return std::filesystem::path(ROO_BUILD_ROOT).lexically_normal();
    }

    std::string platform_library_file_name(const std::string& name)
    {
#if defined(_WIN32)
      return name + ".dll";
#elif defined(__APPLE__)
      return "lib" + name + ".dylib";
#else
      return "lib" + name + ".so";
#endif
    }

    std::string platform_import_library_file_name(const std::string& name)
    {
#if defined(_WIN32)
      return name + ".lib";
#else
      return platform_library_file_name(name);
#endif
    }

    std::filesystem::path build_shared_library_path(const std::string& directory,
                                                    const std::string& name)
    {
      return repo_build_root() / directory / platform_library_file_name(name);
    }

    std::filesystem::path build_import_library_path(const std::string& directory,
                                                    const std::string& name)
    {
      return repo_build_root() / directory / platform_import_library_file_name(name);
    }

    bool is_installed_rooc(const Options& options)
    {
      return options.executable_path.parent_path().filename() == "bin";
    }

    std::filesystem::path rooc_prefix(const Options& options)
    {
      if (is_installed_rooc(options))
      {
        return options.executable_path.parent_path().parent_path().lexically_normal();
      }
      return options.executable_path.parent_path().lexically_normal();
    }

    std::filesystem::path installed_shared_library_path(const std::filesystem::path& prefix,
                                                        const std::string& name)
    {
#if defined(_WIN32)
      return prefix / "bin" / platform_library_file_name(name);
#else
      return prefix / "lib" / platform_library_file_name(name);
#endif
    }

    std::filesystem::path installed_import_library_path(const std::filesystem::path& prefix,
                                                        const std::string& name)
    {
#if defined(_WIN32)
      return prefix / "lib" / platform_import_library_file_name(name);
#else
      return installed_shared_library_path(prefix, name);
#endif
    }

    std::filesystem::path native_library_path(const Roo::Package::NativeLibrary& library)
    {
      if (!library.path.empty())
      {
        std::error_code ec;
        if (std::filesystem::is_directory(library.path, ec))
        {
          return std::filesystem::path(library.path) /
                 platform_library_file_name(library.name);
        }
        return library.path;
      }

      const auto file_name = platform_library_file_name(library.name);
      if (library.package_root.empty() || library.package_root == ".")
      {
        return file_name;
      }
      return std::filesystem::path(library.package_root) / file_name;
    }

    std::string sanitize_target_name(std::string value)
    {
      if (value.empty())
      {
        return "native_library";
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

    std::string cmake_path(const std::filesystem::path& path)
    {
      return path.lexically_normal().generic_string();
    }

    std::filesystem::path logical_package_root(const Roo::Package::PackageInfo& package)
    {
      return std::filesystem::path("roo-packages") / package.name / package.version;
    }

    std::filesystem::path logical_plan_path(const Roo::Package::LoadPlan& plan,
                                            const std::string& value)
    {
      if (value.empty())
      {
        return {};
      }

      const std::filesystem::path path(value);
      if (!path.is_absolute())
      {
        return path.lexically_normal();
      }

      for (const auto& package : plan.packages)
      {
        const std::filesystem::path relative =
          path.lexically_relative(std::filesystem::path(package.package_root));
        if (!relative.empty() &&
            (relative.begin() == relative.end() || *relative.begin() != ".."))
        {
          return (logical_package_root(package) / relative).lexically_normal();
        }
      }

      throw Roo::RooException("Cannot embed path outside the resolved package plan: " +
                              value);
    }

    GeneratedProject logical_embedded_project(const GeneratedProject& project)
    {
      GeneratedProject embedded = project;
      for (auto& file : embedded.files)
      {
        file.key = logical_plan_path(project.plan, file.key).generic_string();
      }
      embedded.plan.package_root =
        logical_plan_path(project.plan, project.plan.package_root).generic_string();
      for (size_t i = 0; i < embedded.plan.package_roots.size(); ++i)
      {
        embedded.plan.package_roots[i] =
          logical_plan_path(project.plan, project.plan.package_roots[i]).generic_string();
      }
      for (size_t i = 0; i < embedded.plan.load_paths.size(); ++i)
      {
        embedded.plan.load_paths[i] =
          logical_plan_path(project.plan, project.plan.load_paths[i]).generic_string();
      }
      for (size_t i = 0; i < embedded.plan.namespace_roots.size(); ++i)
      {
        embedded.plan.namespace_roots[i].path =
          logical_plan_path(project.plan, project.plan.namespace_roots[i].path)
            .generic_string();
      }
      for (size_t i = 0; i < embedded.plan.native_libraries.size(); ++i)
      {
        embedded.plan.native_libraries[i].path =
          logical_plan_path(project.plan, project.plan.native_libraries[i].path)
            .generic_string();
        embedded.plan.native_libraries[i].package_root =
          logical_plan_path(project.plan, project.plan.native_libraries[i].package_root)
            .generic_string();
      }
      for (size_t i = 0; i < embedded.plan.packages.size(); ++i)
      {
        embedded.plan.packages[i].package_root =
          logical_plan_path(project.plan, project.plan.packages[i].package_root)
            .generic_string();
        for (size_t j = 0; j < embedded.plan.packages[i].load_roots.size(); ++j)
        {
          embedded.plan.packages[i].load_roots[j] =
            logical_plan_path(project.plan, project.plan.packages[i].load_roots[j])
              .generic_string();
        }
      }
      return embedded;
    }

    std::string generated_embedded_sources_h()
    {
      std::ostringstream out;
      out << "#ifndef ROOC_GENERATED_EMBEDDED_SOURCES_H\n"
             "#define ROOC_GENERATED_EMBEDDED_SOURCES_H\n\n"
             "#include <span>\n"
             "#include <string>\n"
             "#include <vector>\n\n"
             "#include <roo/io/embedded_file_system.h>\n"
             "#include <roo/namespace_source.h>\n"
             "#include <roo-package/manifest.h>\n\n"
             "namespace RoocGenerated\n"
             "{\n"
             "  std::span<const Roo::EmbeddedFile> embedded_files();\n"
             "  std::vector<Roo::NamespaceRoot> embedded_namespace_roots();\n"
             "  std::vector<std::string> embedded_autoloads();\n"
             "  std::vector<std::string> embedded_entry_points();\n"
             "  std::vector<Roo::Package::NativeLibrary> embedded_native_libraries();\n"
             "  std::string embedded_main_function();\n"
             "  Roo::Package::LoadPlan embedded_load_plan();\n"
             "} // namespace RoocGenerated\n\n"
             "#endif\n";
      return out.str();
    }

    std::string generated_embedded_sources_cpp(const GeneratedProject& project)
    {
      std::ostringstream out;
      out << "#include \"embedded_sources.h\"\n\n"
             "#include <array>\n"
             "#include <utility>\n\n"
             "namespace\n"
             "{\n"
             "  constexpr std::array<Roo::EmbeddedFile, "
          << project.files.size() << "> EMBEDDED_FILES = {{\n";

      for (const auto& file : project.files)
      {
        out << "    {" << cpp_string_literal(file.key) << ", "
            << cpp_string_literal(file.source) << "},\n";
      }

      out << "  }};\n"
             "} // namespace\n\n"
             "namespace RoocGenerated\n"
             "{\n"
             "  std::span<const Roo::EmbeddedFile> embedded_files()\n"
             "  {\n"
             "    return EMBEDDED_FILES;\n"
             "  }\n\n"
             "  std::vector<Roo::NamespaceRoot> embedded_namespace_roots()\n"
             "  {\n"
             "    return {\n";

      for (const auto& root : project.plan.namespace_roots)
      {
        out << "      Roo::NamespaceRoot{" << cpp_string_literal(root.ns_prefix) << ", "
            << cpp_string_literal(root.path) << "},\n";
      }

      out << "    };\n"
             "  }\n\n";

      out << "  std::vector<std::string> embedded_autoloads()\n"
             "  {\n"
             "    return {";
      for (size_t i = 0; i < project.plan.autoloads.size(); ++i)
      {
        if (i > 0)
        {
          out << ", ";
        }
        out << cpp_string_literal(project.plan.autoloads[i]);
      }
      out << "};\n"
             "  }\n\n";

      out << "  std::vector<std::string> embedded_entry_points()\n"
             "  {\n"
             "    return {";
      for (size_t i = 0; i < project.plan.entry_points.size(); ++i)
      {
        if (i > 0)
        {
          out << ", ";
        }
        out << cpp_string_literal(project.plan.entry_points[i]);
      }
      out << "};\n"
             "  }\n"
             "\n"
             "  std::vector<Roo::Package::NativeLibrary> embedded_native_libraries()\n"
             "  {\n"
             "    std::vector<Roo::Package::NativeLibrary> libraries;\n";
      for (const auto& library : project.plan.native_libraries)
      {
        out << "    {\n"
               "      Roo::Package::NativeLibrary library;\n"
               "      library.name = "
            << cpp_string_literal(library.name)
            << ";\n"
               "      library.version = "
            << cpp_string_literal(library.version)
            << ";\n"
               "      library.path = "
            << cpp_string_literal(library.path)
            << ";\n"
               "      library.package_root = "
            << cpp_string_literal(library.package_root)
            << ";\n"
               "      library.namespaces = {";
        for (size_t i = 0; i < library.namespaces.size(); ++i)
        {
          if (i > 0)
          {
            out << ", ";
          }
          out << cpp_string_literal(library.namespaces[i]);
        }
        out << "};\n"
               "      libraries.push_back(std::move(library));\n"
               "    }\n";
      }
      out << "    return libraries;\n"
             "  }\n"
             "\n"
             "  std::string embedded_main_function()\n"
             "  {\n"
             "    return "
          << cpp_string_literal(project.plan.main)
          << ";\n"
             "  }\n"
             "\n"
             "  Roo::Package::LoadPlan embedded_load_plan()\n"
             "  {\n"
             "    Roo::Package::LoadPlan plan;\n"
             "    plan.package_root = "
          << cpp_string_literal(project.plan.package_root)
          << ";\n"
             "    plan.package_roots = {";
      for (size_t i = 0; i < project.plan.package_roots.size(); ++i)
      {
        if (i > 0)
        {
          out << ", ";
        }
        out << cpp_string_literal(project.plan.package_roots[i]);
      }
      out << "};\n"
             "    plan.load_paths = {";
      for (size_t i = 0; i < project.plan.load_paths.size(); ++i)
      {
        if (i > 0)
        {
          out << ", ";
        }
        out << cpp_string_literal(project.plan.load_paths[i]);
      }
      out << "};\n"
             "    plan.namespace_roots = embedded_namespace_roots();\n"
             "    plan.native_namespaces = {";
      for (size_t i = 0; i < project.plan.native_namespaces.size(); ++i)
      {
        if (i > 0)
        {
          out << ", ";
        }
        out << cpp_string_literal(project.plan.native_namespaces[i]);
      }
      out << "};\n"
             "    plan.native_libraries = embedded_native_libraries();\n"
             "    plan.autoloads = embedded_autoloads();\n"
             "    plan.entry_points = embedded_entry_points();\n"
             "    plan.main = embedded_main_function();\n"
             "    plan.run = "
          << cpp_string_literal(project.plan.run) << ";\n";
      for (const auto& package : project.plan.packages)
      {
        out << "    {\n"
               "      Roo::Package::PackageInfo package;\n"
               "      package.name = "
            << cpp_string_literal(package.name)
            << ";\n"
               "      package.version = "
            << cpp_string_literal(package.version)
            << ";\n"
               "      package.package_root = "
            << cpp_string_literal(package.package_root)
            << ";\n"
               "      package.load_roots = {";
        for (size_t i = 0; i < package.load_roots.size(); ++i)
        {
          if (i > 0)
          {
            out << ", ";
          }
          out << cpp_string_literal(package.load_roots[i]);
        }
        out << "};\n"
               "      package.config = {";
        size_t pair_index = 0;
        for (const auto& [key, value] : package.config)
        {
          if (pair_index++ > 0)
          {
            out << ", ";
          }
          out << "{" << cpp_string_literal(key) << ", " << cpp_string_literal(value) << "}";
        }
        out << "};\n"
               "      package.tools = {";
        pair_index = 0;
        for (const auto& [key, value] : package.tools)
        {
          if (pair_index++ > 0)
          {
            out << ", ";
          }
          out << "{" << cpp_string_literal(key) << ", " << cpp_string_literal(value) << "}";
        }
        out << "};\n"
               "      plan.packages.push_back(std::move(package));\n"
               "    }\n";
      }
      out << "    return plan;\n"
             "  }\n"
             "} // namespace RoocGenerated\n";

      return out.str();
    }

    std::string generated_main_cpp()
    {
      std::ostringstream out;
      out
        << "#include <algorithm>\n"
           "#include <cstdlib>\n"
           "#include <exception>\n"
           "#include <filesystem>\n"
           "#include <iostream>\n"
           "#include <memory>\n"
           "#include <string>\n"
           "#include <utility>\n"
           "#include <vector>\n\n"
           "#include <roo/exception.h>\n"
           "#include <roo/io/dir_root_file_system.h>\n"
           "#include <roo/io/embedded_file_system.h>\n"
           "#include <roo/io/file_system_namespace_source.h>\n"
           "#include <roo/runtime.h>\n"
           "#include <roo-package/application.h>\n"
           "#include <roo-package/runtime_environment.h>\n\n"
           "#include \"embedded_sources.h\"\n\n"
           "namespace\n"
           "{\n"
           "  std::filesystem::path executable_path(const char* argv0)\n"
           "  {\n"
           "    if (!argv0 || argv0[0] == '\\0')\n"
           "    {\n"
           "      return {};\n"
           "    }\n"
           "    const std::filesystem::path requested(argv0);\n"
           "    if (requested.has_parent_path())\n"
           "    {\n"
           "      return std::filesystem::weakly_canonical(requested);\n"
           "    }\n"
           "    const char* path_value = std::getenv(\"PATH\");\n"
           "    if (path_value)\n"
           "    {\n"
           "      const std::string paths(path_value);\n"
           "#if defined(_WIN32)\n"
           "      constexpr char delimiter = ';';\n"
           "#else\n"
           "      constexpr char delimiter = ':';\n"
           "#endif\n"
           "      std::size_t begin = 0;\n"
           "      while (begin <= paths.size())\n"
           "      {\n"
           "        const std::size_t end = paths.find(delimiter, begin);\n"
           "        const std::string directory = paths.substr(begin, end - begin);\n"
           "        const std::filesystem::path candidate =\n"
           "          (directory.empty() ? std::filesystem::path(\".\")\n"
           "                             : std::filesystem::path(directory)) / requested;\n"
           "        if (std::filesystem::exists(candidate))\n"
           "        {\n"
           "          return std::filesystem::weakly_canonical(candidate);\n"
           "        }\n"
           "        if (end == std::string::npos)\n"
           "        {\n"
           "          break;\n"
           "        }\n"
           "        begin = end + 1;\n"
           "      }\n"
           "    }\n"
           "    return std::filesystem::absolute(requested);\n"
           "  }\n\n"
           "  void relocate_native_libraries(Roo::Package::LoadPlan& plan,\n"
           "                                 const char* argv0)\n"
           "  {\n"
           "    const std::filesystem::path executable = executable_path(argv0);\n"
           "    if (executable.empty())\n"
           "    {\n"
           "      return;\n"
           "    }\n"
           "    const std::filesystem::path repository =\n"
           "      executable.parent_path().parent_path() / \"share/roo/pkg\";\n"
           "    for (auto& library : plan.native_libraries)\n"
           "    {\n"
           "      const auto package = std::find_if(\n"
           "        plan.packages.begin(), plan.packages.end(),\n"
           "        [&library](const Roo::Package::PackageInfo& candidate)\n"
           "        {\n"
           "          return candidate.package_root == library.package_root;\n"
           "        });\n"
           "      if (package == plan.packages.end())\n"
           "      {\n"
           "        continue;\n"
           "      }\n"
           "      std::error_code error;\n"
           "      const std::filesystem::path relative = library.path.empty()\n"
           "        ? std::filesystem::path{}\n"
           "        : std::filesystem::path(library.path).lexically_relative(\n"
           "            library.package_root);\n"
           "      const auto relocate_to = [&library, &relative, &error](\n"
           "                                 const std::filesystem::path& root)\n"
           "      {\n"
           "        if (!std::filesystem::is_directory(root, error))\n"
           "        {\n"
           "          return false;\n"
           "        }\n"
           "        if (!library.path.empty())\n"
           "        {\n"
           "          const std::filesystem::path path = root / relative;\n"
           "          if (!std::filesystem::exists(path, error))\n"
           "          {\n"
           "            return false;\n"
           "          }\n"
           "          library.path = path.string();\n"
           "        }\n"
           "        library.package_root = root.string();\n"
           "        return true;\n"
           "      };\n"
           "      const std::filesystem::path installed_root =\n"
           "        repository / package->name / package->version;\n"
           "      if (!relocate_to(installed_root))\n"
           "      {\n"
           "        relocate_to(executable.parent_path() / package->package_root);\n"
           "      }\n"
           "    }\n"
           "  }\n\n"
           "  void load_namespaces(Roo::Runtime& runtime,\n"
           "                       const std::vector<std::string>& namespaces,\n"
           "                       const std::string& loader_ns,\n"
           "                       const std::string& source_name)\n"
           "  {\n"
           "    for (const auto& ns : namespaces)\n"
           "    {\n"
           "      runtime.eval(\"(ns \" + loader_ns + \" (:require \" + ns + \"))\", "
           "source_name);\n"
           "    }\n"
           "  }\n\n"
           "} // namespace\n\n"
           "int main(int argc, char** argv)\n"
           "{\n"
           "  try\n"
           "  {\n"
           "    Roo::Package::LoadPlan package_plan =\n"
           "      RoocGenerated::embedded_load_plan();\n"
           "    relocate_native_libraries(package_plan, argc > 0 ? argv[0] : nullptr);\n"
           "    Roo::Package::ApplicationRuntimeSpec runtime_spec;\n"
           "    runtime_spec.load_plan = package_plan;\n"
           "    runtime_spec.make_inputs = []()\n"
           "    {\n"
           "      Roo::Package::RuntimeEnvironmentInputs inputs;\n"
           "      inputs.file_system = std::make_unique<Roo::DirRootFileSystem>(\n"
           "        std::vector<std::string>{std::filesystem::current_path().string(), "
           "\"/\"});\n"
           "      auto namespace_fs = std::make_unique<Roo::EmbeddedFileSystem>(\n"
           "        RoocGenerated::embedded_files());\n"
           "      inputs.namespace_source = "
           "std::make_unique<Roo::FileSystemNamespaceSource>(\n"
           "        namespace_fs.get());\n"
           "      inputs.supporting_file_systems.push_back(std::move(namespace_fs));\n"
           "      return inputs;\n"
           "    };\n"
           "    runtime_spec.autoload_loader_namespace = \"roo.compiled.autoload\";\n"
           "    Roo::WorkerEnvironmentFactory environment_factory =\n"
           "      Roo::Package::make_application_runtime_factory(std::move(runtime_spec));\n"
           "    std::unique_ptr<Roo::WorkerEnvironment> environment = "
           "environment_factory();\n"
           "    Roo::Runtime& runtime = environment->runtime();\n"
           "    runtime.worker_registry().register_environment(\n"
           "      \"application\", environment_factory);\n\n"
           "    if (package_plan.entry_points.empty() && package_plan.main.empty() &&\n"
           "        package_plan.run.empty())\n"
           "    {\n"
           "      throw Roo::RooException(\"Compiled package has no :run, :main, or "
           ":entry-points in package.edn.\");\n"
           "    }\n"
           "    if (!package_plan.run.empty())\n"
           "    {\n"
           "      return Roo::Package::Application::exit_code(\n"
           "        Roo::Package::Application::invoke_tool(runtime, package_plan,\n"
           "                                               package_plan.run,\n"
           "                                               argc,\n"
           "                                               argv));\n"
           "    }\n"
           "    else\n"
           "    {\n"
           "      load_namespaces(runtime, package_plan.entry_points, "
           "\"roo.compiled.entry\", \"<package-entry>\");\n"
           "      return Roo::Package::Application::exit_code(\n"
           "        Roo::Package::Application::invoke_main(runtime, package_plan.main, "
           "argc, "
           "argv));\n"
           "    }\n"
           "  }\n"
           "  catch (const std::exception& e)\n"
           "  {\n"
           "    std::cerr << e.what() << std::endl;\n"
           "    return 1;\n"
           "  }\n"
           "  return 0;\n"
           "}\n";

      return out.str();
    }

    std::string generated_cmake(const Options& options, const GeneratedProject& project)
    {
      const auto prefix = rooc_prefix(options);
      const bool installed = is_installed_rooc(options);
      const auto roo_lib = installed ? installed_shared_library_path(prefix, "roo")
                                     : build_shared_library_path("lib/libroo", "roo");
      [[maybe_unused]] const auto roo_import_lib =
        installed ? installed_import_library_path(prefix, "roo")
                  : build_import_library_path("lib/libroo", "roo");
      const auto package_lib =
        installed ? installed_shared_library_path(prefix, "roo-package")
                  : build_shared_library_path("lib/libroo-package", "roo-package");
      [[maybe_unused]] const auto package_import_lib =
        installed ? installed_import_library_path(prefix, "roo-package")
                  : build_import_library_path("lib/libroo-package", "roo-package");
      const auto roo_include_dir =
        installed ? prefix / "include" : repo_source_root() / "lib/libroo/include";
      const auto package_include_dir =
        installed ? prefix / "include" : repo_source_root() / "lib/libroo-package/include";
      std::ostringstream out;
      out << "cmake_minimum_required(VERSION 3.20)\n\n"
             "project("
          << project.executable_name
          << " VERSION 0.1 LANGUAGES CXX)\n\n"
             "set(CMAKE_CXX_STANDARD 20)\n"
             "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
             "set(CMAKE_CXX_EXTENSIONS OFF)\n\n"
             "if(APPLE)\n"
             "  set(ROOC_INSTALL_RPATH \"@loader_path/../lib\")\n"
             "elseif(UNIX)\n"
             "  set(ROOC_INSTALL_RPATH \"$ORIGIN/../lib\")\n"
             "else()\n"
             "  set(ROOC_INSTALL_RPATH \"\")\n"
             "endif()\n\n"
             "add_executable("
          << project.executable_name
          << "\n"
             "  src/main.cpp\n"
             "  src/embedded_sources.cpp\n"
             ")\n\n"
             "add_library(roo_shared_imported SHARED IMPORTED)\n"
             "set_target_properties(roo_shared_imported PROPERTIES\n"
             "    IMPORTED_LOCATION "
          << cpp_string_literal(cmake_path(roo_lib))
          << "\n"
#if defined(_WIN32)
             "    IMPORTED_IMPLIB "
          << cpp_string_literal(cmake_path(roo_import_lib))
          << "\n"
#endif
             "    INTERFACE_INCLUDE_DIRECTORIES "
          << cpp_string_literal(cmake_path(roo_include_dir))
          << "\n"
             "  )\n"
             "add_library(roo_package_shared_imported SHARED IMPORTED)\n"
             "set_target_properties(roo_package_shared_imported PROPERTIES\n"
             "    IMPORTED_LOCATION "
          << cpp_string_literal(cmake_path(package_lib))
          << "\n"
#if defined(_WIN32)
             "    IMPORTED_IMPLIB "
          << cpp_string_literal(cmake_path(package_import_lib))
          << "\n"
#endif
             "    INTERFACE_INCLUDE_DIRECTORIES "
          << cpp_string_literal(cmake_path(package_include_dir))
          << "\n"
             "  )\n"
             "\n";
      for (size_t i = 0; i < project.plan.native_libraries.size(); ++i)
      {
        const auto& library = project.plan.native_libraries[i];
        const auto target_name =
          "native_library_" + std::to_string(i) + "_" + sanitize_target_name(library.name);
        const auto path = native_library_path(library);
        out << "add_library(" << target_name
            << " SHARED IMPORTED)\n"
               "set_target_properties("
            << target_name
            << " PROPERTIES\n"
               "    IMPORTED_LOCATION "
            << cpp_string_literal(cmake_path(path))
            << "\n"
               "  )\n\n";
        const auto build_local_path = logical_plan_path(project.plan, path.string());
        out << "add_custom_command(TARGET " << project.executable_name
            << " POST_BUILD\n"
               "  COMMAND ${CMAKE_COMMAND} -E make_directory\n"
               "          \"$<TARGET_FILE_DIR:"
            << project.executable_name << ">/"
            << cmake_path(build_local_path.parent_path())
            << "\"\n"
               "  COMMAND ${CMAKE_COMMAND} -E copy_if_different\n"
               "          "
            << cpp_string_literal(cmake_path(path))
            << "\n"
               "          \"$<TARGET_FILE_DIR:"
            << project.executable_name << ">/"
            << cmake_path(build_local_path)
            << "\"\n"
               ")\n\n";
      }
      out << "  target_link_libraries(" << project.executable_name
          << " PRIVATE roo_shared_imported roo_package_shared_imported)\n"
             "\n"
#if defined(_WIN32)
             "add_custom_command(TARGET "
          << project.executable_name
          << " POST_BUILD\n"
             "  COMMAND ${CMAKE_COMMAND} -E copy_if_different\n"
             "          $<TARGET_FILE:roo_shared_imported>\n"
             "          $<TARGET_FILE:roo_package_shared_imported>\n"
             "          $<TARGET_FILE_DIR:"
          << project.executable_name
          << ">\n"
             ")\n\n"
#endif
             "set_target_properties("
          << project.executable_name
          << " PROPERTIES\n"
             "  BUILD_RPATH "
          << cpp_string_literal(
               [&]()
               {
                 std::string rpath = cmake_path(roo_lib.parent_path()) + ";" +
                                     cmake_path(package_lib.parent_path());
                 for (const auto& library : project.plan.native_libraries)
                 {
                   rpath += ";" + cmake_path(native_library_path(library).parent_path());
                 }
                 return rpath;
               }())
          << "\n"
             "  INSTALL_RPATH \"${ROOC_INSTALL_RPATH}\"\n"
             ")\n\n"
             "install(TARGETS "
          << project.executable_name
          << "\n"
             "  RUNTIME DESTINATION bin\n"
             ")\n";
      return out.str();
    }
  } // namespace

  void generate_project(const Options& options, const GeneratedProject& project)
  {
    const GeneratedProject embedded_project = logical_embedded_project(project);
    write_file(options.build_dir / "CMakeLists.txt", generated_cmake(options, project));
    write_file(options.build_dir / "src/main.cpp", generated_main_cpp());
    write_file(options.build_dir / "src/embedded_sources.h", generated_embedded_sources_h());
    write_file(options.build_dir / "src/embedded_sources.cpp",
               generated_embedded_sources_cpp(embedded_project));
  }
} // namespace Rooc
