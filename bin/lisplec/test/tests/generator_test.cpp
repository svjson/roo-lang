#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
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
  EXPECT_THAT(embedded_file_system_cpp, HasSubstr("EmbeddedFileSystem::read"));
  EXPECT_THAT(embedded_sources_cpp, HasSubstr("cafe/run.lisple"));
  EXPECT_THAT(embedded_sources_cpp, HasSubstr("recipe/book.lisple"));
}
