#include <filesystem>
#include <fstream>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <rooc/project.h>

using ::testing::Contains;
using ::testing::Field;

namespace
{
  std::filesystem::path repo_root()
  {
    return std::filesystem::path(ROOC_TEST_REPO_ROOT);
  }

  std::filesystem::path package_stage_root()
  {
    return std::filesystem::path(ROOC_TEST_PACKAGE_STAGE_ROOT);
  }

  Rooc::Options options_for(const std::filesystem::path& package_dir)
  {
    Rooc::Options options;
    options.command = "generate";
    options.package_dir = package_dir;
    options.build_dir = ".rooc";
    return options;
  }

  void write_file(const std::filesystem::path& path, const std::string& contents)
  {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path);
    out << contents;
  }
} // namespace

TEST(RoocProject, prepares_pure_package_with_file_dependency)
{
  // Given
  auto options =
    options_for(repo_root() / "lib/libroo-package/test/tests/assets/packages/cafe-register");

  // When
  auto project = Rooc::prepare_project(options);

  // Then
  EXPECT_EQ(project.executable_name, "cafe_register");
  EXPECT_THAT(project.plan.entry_points, Contains("cafe.run"));
  EXPECT_THAT(project.files, Contains(Field(&Rooc::EmbeddedFile::key, "cafe/run.roo")));
  EXPECT_THAT(project.files, Contains(Field(&Rooc::EmbeddedFile::key, "recipe/book.roo")));
}

TEST(RoocProject, collects_roo_source_files)
{
  // Given
  const auto root = std::filesystem::temp_directory_path() / "rooc-project-roo-source-test";
  std::filesystem::remove_all(root);
  write_file(root / "package.edn",
             "{:name roo-source-app\n"
             " :version \"0.1.0\"\n"
             " :load-roots [\"src\"]\n"
             " :entry-points [main.app]}\n");
  write_file(root / "src/main/app.roo", "(ns main.app)\n");
  auto options = options_for(root);

  // When
  auto project = Rooc::prepare_project(options);

  // Then
  EXPECT_THAT(project.files, Contains(Field(&Rooc::EmbeddedFile::key, "main/app.roo")));
}

TEST(RoocProject, sanitizes_explicit_executable_name)
{
  // Given
  auto options =
    options_for(repo_root() / "lib/libroo-package/test/tests/assets/packages/cafe-register");
  options.executable_name = "123 cafe-register";

  // When
  auto project = Rooc::prepare_project(options);

  // Then
  EXPECT_EQ(project.executable_name, "_123_cafe_register");
}

TEST(RoocProject, prepares_package_with_native_dependency)
{
  // Given
  auto options = options_for(package_stage_root() / "proof/test/assets/dynamic-smoke");

  // When
  auto project = Rooc::prepare_project(options);

  // Then
  EXPECT_THAT(project.plan.native_libraries,
              Contains(Field(&Roo::Package::NativeLibrary::name, "proof-native")));
  EXPECT_THAT(project.plan.native_namespaces, Contains("proof.syntax"));
  EXPECT_THAT(project.files, Contains(Field(&Rooc::EmbeddedFile::key, "proof/core.roo")));
}
