#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <lisplec/builder.h>
#include <lisplec/generator.h>
#include <lisplec/project.h>

using ::testing::HasSubstr;

namespace
{
  std::filesystem::path repo_root()
  {
    return std::filesystem::path(LISPLEC_TEST_REPO_ROOT);
  }

  std::filesystem::path build_root()
  {
    return std::filesystem::path(LISPLEC_TEST_BUILD_ROOT);
  }

  std::string read_file(const std::filesystem::path& path)
  {
    std::ifstream file(path, std::ios::binary);
    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
  }

  void write_file(const std::filesystem::path& path, const std::string& source)
  {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
      throw std::runtime_error("Could not write file: " + path.string());
    }
    file << source;
  }

  Lisplec::Options options_for(const std::filesystem::path& build_dir)
  {
    Lisplec::Options options;
    options.command = "generate";
    options.package_dir =
      repo_root() / "lib/lisple-package/test/tests/assets/packages/cafe-register";
    options.build_dir = build_dir;
    options.executable_name = "test_generated_cafe";
    return options;
  }

  Lisplec::Options main_app_options_for(const std::filesystem::path& build_dir)
  {
    Lisplec::Options options;
    options.command = "build";
    options.package_dir = repo_root() / "bin/lisplec/test/assets/main-app";
    options.build_dir = build_dir;
    return options;
  }

  Lisplec::Options main_app_options_for(const std::filesystem::path& build_dir,
                                        const std::filesystem::path& package_dir)
  {
    Lisplec::Options options;
    options.command = "build";
    options.package_dir = package_dir;
    options.build_dir = build_dir;
    return options;
  }

  Lisplec::Options dynamic_smoke_options_for(const std::filesystem::path& build_dir)
  {
    Lisplec::Options options;
    options.command = "build";
    options.package_dir = repo_root() / "pkg/proof/test/assets/dynamic-smoke";
    options.build_dir = build_dir;
    options.executable_name = "dynamic_smoke_compiled";
    return options;
  }

  std::string shell_arg(const std::string& value)
  {
    std::string result = "\"";
    for (const char c : value)
    {
      if (c == '"' || c == '\\' || c == '$' || c == '`')
      {
        result += '\\';
      }
      result += c;
    }
    result += "\"";
    return result;
  }

  std::string shell_arg(const std::filesystem::path& path)
  {
    return shell_arg(path.generic_string());
  }

  void run_command(const std::string& command)
  {
    ASSERT_EQ(std::system(command.c_str()), 0) << command;
  }

  void create_main_app_fixture(const std::filesystem::path& package_dir,
                               const std::string& main_source)
  {
    std::filesystem::remove_all(package_dir);
    write_file(package_dir / "package.edn",
               R"({:name main-app
 :version "0.1.0"
 :load-roots ["src"]
 :main main.app/main})");
    write_file(package_dir / "src/main/app.lisple", main_source);
  }

  void create_run_tool_app_fixture(const std::filesystem::path& root)
  {
    std::filesystem::remove_all(root);
    write_file(root / "runner/package.edn",
               R"({:name runner
 :version "0.1.0"
 :load-roots ["src"]
 :tools {run runner.tool/run}})");
    write_file(root / "runner/src/runner/tool.lisple",
               R"((ns runner.tool
  (:require [lisple.io :as io]))

(defun run [context]
  (io/spit! "run-tool.txt"
            (str (:package-name context) ":"
                 (:tool-package context) ":"
                 (:tool-name context) ":"
                 (:message (:config context)))))
)");
    write_file(root / "app/package.edn",
               R"({:name run-app
 :version "0.1.0"
 :dependencies {runner {:path "../runner"}}
 :load-roots ["src"]
 :config {runner {:message "ok"}}
 :run runner})");
    write_file(root / "app/src/run/app.lisple", "(ns run.app)\n");
  }

  void run_generated_executable(const Lisplec::GeneratedProject& project,
                                const std::filesystem::path& build_dir,
                                const std::filesystem::path& run_dir,
                                const std::vector<std::string>& args = {})
  {
    const auto executable =
      build_dir / "build" /
      (project.executable_name + std::string(LISPLEC_TEST_EXECUTABLE_SUFFIX));
    std::string command = std::string(LISPLEC_TEST_CMAKE_COMMAND) + " -E chdir " +
                          shell_arg(run_dir) + " " + shell_arg(executable);
    for (const auto& arg : args)
    {
      command += " " + shell_arg(arg);
    }
    run_command(command);
  }
} // namespace

TEST(LisplecGenerator, writes_generated_project_files)
{
  // Given
  const auto build_dir = build_root() / "lisplec-gtest-generated";
  auto options = options_for(build_dir);
  auto project = Lisplec::prepare_project(options);

  // When
  Lisplec::generate_project(options, project);

  // Then
  EXPECT_TRUE(std::filesystem::is_regular_file(build_dir / "CMakeLists.txt"));
  EXPECT_TRUE(std::filesystem::is_regular_file(build_dir / "src/main.cpp"));
  EXPECT_TRUE(std::filesystem::is_regular_file(build_dir / "src/embedded_file_system.h"));
  EXPECT_TRUE(std::filesystem::is_regular_file(build_dir / "src/embedded_file_system.cpp"));
  EXPECT_TRUE(std::filesystem::is_regular_file(build_dir / "src/embedded_sources.h"));
  EXPECT_TRUE(std::filesystem::is_regular_file(build_dir / "src/embedded_sources.cpp"));
}

TEST(LisplecGenerator, generated_project_splits_bootstrap_runtime_and_embedded_sources)
{
  // Given
  const auto build_dir = build_root() / "lisplec-gtest-generated";
  auto options = options_for(build_dir);
  auto project = Lisplec::prepare_project(options);

  // When
  Lisplec::generate_project(options, project);

  // Then
  const std::string main_cpp = read_file(build_dir / "src/main.cpp");
  const std::string embedded_sources_cpp = read_file(build_dir / "src/embedded_sources.cpp");
  const std::string embedded_file_system_cpp =
    read_file(build_dir / "src/embedded_file_system.cpp");

  EXPECT_THAT(main_cpp, HasSubstr("LisplecGenerated::EmbeddedFileSystem namespace_fs"));
  EXPECT_THAT(main_cpp, HasSubstr("FileSystemNamespaceSource"));
  EXPECT_THAT(main_cpp,
              HasSubstr("LoadedNativePackages native_packages;\n"
                        "    Lisple::Runtime runtime"));
  EXPECT_THAT(embedded_file_system_cpp, HasSubstr("EmbeddedFileSystem::read"));
  EXPECT_THAT(embedded_sources_cpp, HasSubstr("cafe/run.lisple"));
  EXPECT_THAT(embedded_sources_cpp, HasSubstr("recipe/book.lisple"));
  EXPECT_THAT(embedded_sources_cpp, HasSubstr("embedded_native_libraries"));
  EXPECT_THAT(embedded_sources_cpp, HasSubstr("embedded_main_function"));
  EXPECT_THAT(embedded_sources_cpp, HasSubstr("embedded_load_plan"));
}

TEST(LisplecGenerator, generated_executable_invokes_main_function)
{
  // Given
  const auto build_dir = build_root() / "lisplec-gtest-main-build";
  const auto run_dir = build_root() / "lisplec-gtest-main-run";
  auto options = main_app_options_for(build_dir);
  auto project = Lisplec::prepare_project(options);
  std::filesystem::create_directories(run_dir);
  std::filesystem::remove(run_dir / "main-ran.txt");

  // When
  Lisplec::generate_project(options, project);
  Lisplec::build_project(options, project);
  run_generated_executable(project, build_dir, run_dir);

  // Then
  EXPECT_EQ(read_file(run_dir / "main-ran.txt"), "ok");
}

TEST(LisplecGenerator, generated_executable_passes_no_args_to_zero_arity_main)
{
  // Given
  const auto package_dir = build_root() / "lisplec-gtest-main-zero-package";
  const auto build_dir = build_root() / "lisplec-gtest-main-zero-build";
  const auto run_dir = build_root() / "lisplec-gtest-main-zero-run";
  create_main_app_fixture(package_dir,
                          R"((ns main.app)

(defun main []
  (lisple.io/spit! "main-ran.txt" "zero"))
)");
  auto options = main_app_options_for(build_dir, package_dir);
  auto project = Lisplec::prepare_project(options);
  std::filesystem::create_directories(run_dir);
  std::filesystem::remove(run_dir / "main-ran.txt");

  // When
  Lisplec::generate_project(options, project);
  Lisplec::build_project(options, project);
  run_generated_executable(project, build_dir, run_dir, {"alpha", "beta"});

  // Then
  EXPECT_EQ(read_file(run_dir / "main-ran.txt"), "zero");
}

TEST(LisplecGenerator, generated_executable_passes_cli_args_vector_to_one_arity_main)
{
  // Given
  const auto package_dir = build_root() / "lisplec-gtest-main-one-package";
  const auto build_dir = build_root() / "lisplec-gtest-main-one-build";
  const auto run_dir = build_root() / "lisplec-gtest-main-one-run";
  create_main_app_fixture(package_dir,
                          R"((ns main.app)

(defun main [args]
  (lisple.io/spit! "main-ran.txt"
                   (str (count args) ":" (nth args 0) ":" (nth args 1))))
)");
  auto options = main_app_options_for(build_dir, package_dir);
  auto project = Lisplec::prepare_project(options);
  std::filesystem::create_directories(run_dir);
  std::filesystem::remove(run_dir / "main-ran.txt");

  // When
  Lisplec::generate_project(options, project);
  Lisplec::build_project(options, project);
  run_generated_executable(project, build_dir, run_dir, {"alpha", "beta"});

  // Then
  EXPECT_EQ(read_file(run_dir / "main-ran.txt"), "2:alpha:beta");
}

TEST(LisplecGenerator, generated_executable_pads_main_arity_with_nil_values)
{
  // Given
  const auto package_dir = build_root() / "lisplec-gtest-main-padded-package";
  const auto build_dir = build_root() / "lisplec-gtest-main-padded-build";
  const auto run_dir = build_root() / "lisplec-gtest-main-padded-run";
  create_main_app_fixture(package_dir,
                          R"((ns main.app)

(defun main [args x y z]
  (lisple.io/spit! "main-ran.txt"
                   (str (count args) ":"
                        (nth args 0) ":"
                        (if x "x" "nil") ":"
                        (if y "y" "nil") ":"
                        (if z "z" "nil"))))
)");
  auto options = main_app_options_for(build_dir, package_dir);
  auto project = Lisplec::prepare_project(options);
  std::filesystem::create_directories(run_dir);
  std::filesystem::remove(run_dir / "main-ran.txt");

  // When
  Lisplec::generate_project(options, project);
  Lisplec::build_project(options, project);
  run_generated_executable(project, build_dir, run_dir, {"alpha"});

  // Then
  EXPECT_EQ(read_file(run_dir / "main-ran.txt"), "1:alpha:nil:nil:nil");
}

TEST(LisplecGenerator, generated_executable_invokes_run_tool)
{
  // Given
  const auto fixture_root = build_root() / "lisplec-gtest-run-tool-fixture";
  const auto package_dir = fixture_root / "app";
  const auto build_dir = build_root() / "lisplec-gtest-run-tool-build";
  const auto run_dir = build_root() / "lisplec-gtest-run-tool-run";
  create_run_tool_app_fixture(fixture_root);
  auto options = main_app_options_for(build_dir, package_dir);
  auto project = Lisplec::prepare_project(options);
  std::filesystem::create_directories(run_dir);
  std::filesystem::remove(run_dir / "run-tool.txt");

  // When
  Lisplec::generate_project(options, project);
  Lisplec::build_project(options, project);
  run_generated_executable(project, build_dir, run_dir);

  // Then
  EXPECT_EQ(read_file(run_dir / "run-tool.txt"), "run-app:runner:run:ok");
}

TEST(LisplecGenerator, generated_executable_loads_native_package_dependencies)
{
  // Given
  const auto build_dir = build_root() / "lisplec-gtest-native-build";
  const auto run_dir = build_root() / "lisplec-gtest-native-run";
  auto options = dynamic_smoke_options_for(build_dir);
  auto project = Lisplec::prepare_project(options);
  std::filesystem::create_directories(run_dir);

  // When
  Lisplec::generate_project(options, project);
  Lisplec::build_project(options, project);
  run_generated_executable(project, build_dir, run_dir);

  // Then
  SUCCEED();
}
