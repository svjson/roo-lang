#include <filesystem>
#include <fstream>
#include <sstream>

#include <lisple/exception.h>

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
        throw Lisple::LispleException("Could not write file: " + path.string());
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

    std::filesystem::path shared_library_path(const std::string& directory,
                                              const std::string& name)
    {
#if defined(_WIN32)
      return repo_build_root() / directory / (name + ".dll");
#elif defined(__APPLE__)
      return repo_build_root() / directory / ("lib" + name + ".dylib");
#else
      return repo_build_root() / directory / ("lib" + name + ".so");
#endif
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

    std::string generated_embedded_file_system_h()
    {
      std::ostringstream out;
      out << "#ifndef ROOC_GENERATED_EMBEDDED_FILE_SYSTEM_H\n"
             "#define ROOC_GENERATED_EMBEDDED_FILE_SYSTEM_H\n\n"
             "#include <map>\n"
             "#include <string>\n\n"
             "#include <lisple/io/file_system.h>\n\n"
             "namespace RoocGenerated\n"
             "{\n"
             "  class EmbeddedFileSystem : public Lisple::FileSystem\n"
             "  {\n"
             "    std::map<std::string, std::string> files_;\n\n"
             "   public:\n"
             "    explicit EmbeddedFileSystem(std::map<std::string, std::string> files);\n\n"
             "    const std::string read(const std::string& file_name) override;\n"
             "  };\n"
             "} // namespace RoocGenerated\n\n"
             "#endif\n";
      return out.str();
    }

    std::string generated_embedded_file_system_cpp()
    {
      std::ostringstream out;
      out
        << "#include \"embedded_file_system.h\"\n\n"
           "#include <filesystem>\n"
           "#include <utility>\n"
           "\n"
           "#include <lisple/exception.h>\n\n"
           "namespace\n"
           "{\n"
           "  std::string normalize_path(const std::string& path)\n"
           "  {\n"
           "    return std::filesystem::path(path).lexically_normal().generic_string();\n"
           "  }\n\n"
           "} // namespace\n\n"
           "namespace RoocGenerated\n"
           "{\n"
           "  EmbeddedFileSystem::EmbeddedFileSystem(std::map<std::string, std::string> "
           "files)\n"
           "    : files_(std::move(files))\n"
           "  {\n"
           "  }\n\n"
           "  const std::string EmbeddedFileSystem::read(const std::string& file_name)\n"
           "  {\n"
           "    const auto key = normalize_path(file_name);\n"
           "    auto it = files_.find(key);\n"
           "    if (it == files_.end())\n"
           "    {\n"
           "      throw Lisple::LispleException(\"Embedded namespace source not found: \" + "
           "file_name);\n"
           "    }\n"
           "    return it->second;\n"
           "  }\n"
           "} // namespace RoocGenerated\n";
      return out.str();
    }

    std::string generated_embedded_sources_h()
    {
      std::ostringstream out;
      out << "#ifndef ROOC_GENERATED_EMBEDDED_SOURCES_H\n"
             "#define ROOC_GENERATED_EMBEDDED_SOURCES_H\n\n"
             "#include <map>\n"
             "#include <string>\n"
             "#include <vector>\n\n"
             "#include <lisple/namespace_source.h>\n"
             "#include <roo-package/manifest.h>\n\n"
             "namespace RoocGenerated\n"
             "{\n"
             "  std::map<std::string, std::string> embedded_files();\n"
             "  std::vector<Lisple::NamespaceRoot> embedded_namespace_roots();\n"
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
             "#include <utility>\n\n"
             "namespace RoocGenerated\n"
             "{\n"
             "  std::map<std::string, std::string> embedded_files()\n"
             "  {\n"
             "    return {\n";

      for (const auto& file : project.files)
      {
        out << "      {" << cpp_string_literal(file.key) << ", "
            << cpp_string_literal(file.source) << "},\n";
      }

      out << "    };\n"
             "  }\n\n"
             "  std::vector<Lisple::NamespaceRoot> embedded_namespace_roots()\n"
             "  {\n"
             "    return {\n";

      for (const auto& root : project.plan.namespace_roots)
      {
        out << "      Lisple::NamespaceRoot{" << cpp_string_literal(root.ns_prefix) << ", "
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
        << "#include <exception>\n"
           "#include <filesystem>\n"
           "#include <iostream>\n"
           "#include <memory>\n"
           "#include <string>\n"
           "#include <vector>\n\n"
           "#include <lisple/exception.h>\n"
           "#include <lisple/io/dir_root_file_system.h>\n"
           "#include <lisple/io/file_system_namespace_source.h>\n"
           "#include <lisple/runtime.h>\n"
           "#include <roo-package/application.h>\n\n"
           "#include <roo-package/native_loader.h>\n\n"
           "#include \"embedded_file_system.h\"\n"
           "#include \"embedded_sources.h\"\n\n"
           "namespace\n"
           "{\n"
           "  void load_namespaces(Lisple::Runtime& runtime,\n"
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
           "    Lisple::DirRootFileSystem app_fs({std::filesystem::current_path().string(), "
           "\"/\"});\n"
           "    RoocGenerated::EmbeddedFileSystem namespace_fs(\n"
           "      RoocGenerated::embedded_files());\n"
           "    auto namespace_source = "
           "std::make_unique<Lisple::FileSystemNamespaceSource>(&namespace_fs);\n"
           "    Roo::Package::LoadPlan package_plan =\n"
           "      RoocGenerated::embedded_load_plan();\n"
           "    Roo::Package::LoadedNativePackages native_packages;\n"
           "    Lisple::Runtime runtime(&app_fs, std::move(namespace_source));\n"
           "    runtime.set_call_stack_diagnostics(true);\n"
           "    runtime.set_namespace_roots(package_plan.namespace_roots);\n\n"
           "    native_packages = Roo::Package::load_native_libraries(runtime, "
           "package_plan);\n\n"
           "    load_namespaces(runtime,\n"
           "                    package_plan.autoloads,\n"
           "                    \"roo.compiled.autoload\",\n"
           "                    \"<package-autoload>\");\n\n"
           "    if (package_plan.entry_points.empty() && package_plan.main.empty() &&\n"
           "        package_plan.run.empty())\n"
           "    {\n"
           "      throw Lisple::LispleException(\"Compiled package has no :run, :main, or "
           ":entry-points in package.edn.\");\n"
           "    }\n"
           "    if (!package_plan.run.empty())\n"
           "    {\n"
           "      Roo::Package::Application::invoke_tool(runtime, package_plan,\n"
           "                                                package_plan.run);\n"
           "    }\n"
           "    else\n"
           "    {\n"
           "      load_namespaces(runtime, package_plan.entry_points, "
           "\"roo.compiled.entry\", \"<package-entry>\");\n"
           "      Roo::Package::Application::invoke_main(runtime, package_plan.main, "
           "argc, "
           "argv);\n"
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

    std::string generated_cmake(const GeneratedProject& project)
    {
      const auto lisple_lib = shared_library_path("lib/liblisple", "lisple");
      const auto package_lib = shared_library_path("lib/libroo-package", "roo-package");
      std::ostringstream out;
      out << "cmake_minimum_required(VERSION 3.20)\n\n"
             "project("
          << project.executable_name
          << " VERSION 0.1 LANGUAGES CXX)\n\n"
             "set(CMAKE_CXX_STANDARD 20)\n"
             "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
             "set(CMAKE_CXX_EXTENSIONS OFF)\n\n"
             "add_executable("
          << project.executable_name
          << "\n"
             "  src/main.cpp\n"
             "  src/embedded_file_system.cpp\n"
             "  src/embedded_sources.cpp\n"
             ")\n\n"
             "add_library(lisple_shared_imported SHARED IMPORTED)\n"
             "set_target_properties(lisple_shared_imported PROPERTIES\n"
             "    IMPORTED_LOCATION "
          << cpp_string_literal(cmake_path(lisple_lib))
          << "\n"
             "    INTERFACE_INCLUDE_DIRECTORIES "
          << cpp_string_literal(cmake_path(repo_source_root() / "lib/liblisple/include"))
          << "\n"
             "  )\n"
             "add_library(roo_package_shared_imported SHARED IMPORTED)\n"
             "set_target_properties(roo_package_shared_imported PROPERTIES\n"
             "    IMPORTED_LOCATION "
          << cpp_string_literal(cmake_path(package_lib))
          << "\n"
             "    INTERFACE_INCLUDE_DIRECTORIES "
          << cpp_string_literal(
               cmake_path(repo_source_root() / "lib/libroo-package/include"))
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
      }
      out << "  target_link_libraries(" << project.executable_name
          << " PRIVATE lisple_shared_imported roo_package_shared_imported";
      for (size_t i = 0; i < project.plan.native_libraries.size(); ++i)
      {
        const auto& library = project.plan.native_libraries[i];
        out << " native_library_" << i << "_" << sanitize_target_name(library.name);
      }
      out << ")\n"
             "\n"
             "set_target_properties("
          << project.executable_name
          << " PROPERTIES\n"
             "  BUILD_RPATH "
          << cpp_string_literal(
               [&]()
               {
                 std::string rpath = cmake_path(lisple_lib.parent_path()) + ";" +
                                     cmake_path(package_lib.parent_path());
                 for (const auto& library : project.plan.native_libraries)
                 {
                   rpath += ";" + cmake_path(native_library_path(library).parent_path());
                 }
                 return rpath;
               }())
          << "\n"
             ")\n";
      return out.str();
    }
  } // namespace

  void generate_project(const Options& options, const GeneratedProject& project)
  {
    write_file(options.build_dir / "CMakeLists.txt", generated_cmake(project));
    write_file(options.build_dir / "src/main.cpp", generated_main_cpp());
    write_file(options.build_dir / "src/embedded_file_system.h",
               generated_embedded_file_system_h());
    write_file(options.build_dir / "src/embedded_file_system.cpp",
               generated_embedded_file_system_cpp());
    write_file(options.build_dir / "src/embedded_sources.h", generated_embedded_sources_h());
    write_file(options.build_dir / "src/embedded_sources.cpp",
               generated_embedded_sources_cpp(project));
  }
} // namespace Rooc
