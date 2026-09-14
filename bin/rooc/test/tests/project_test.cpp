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
  std::filesystem::path test_root()
  {
    return std::filesystem::path(ROOC_TEST_ROOT);
  }

  std::filesystem::path staged_package_repository_root()
  {
    return std::filesystem::path(ROOC_TEST_STAGED_PACKAGE_REPOSITORY_ROOT);
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
  auto options = options_for(test_root() / "assets/packages/cafe-register");

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

TEST(RoocProject, resolves_versioned_dependency_from_explicit_repository)
{
  // Given
  const auto root =
    std::filesystem::temp_directory_path() / "rooc-project-explicit-repository-test";
  std::filesystem::remove_all(root);
  write_file(root / "app/package.edn",
             "{:name app :version \"0.1.0\" :dependencies {util \"1.0.0\"} "
             ":load-roots [\"src\"]}\n");
  write_file(root / "app/src/app.roo", "(ns app)\n");
  write_file(root / "repository/util/1.0.0/package.edn",
             "{:name util :version \"1.0.0\" :dependencies [] "
             ":load-roots [\"src\"]}\n");
  write_file(root / "repository/util/1.0.0/src/util.roo", "(ns util)\n");
  auto options = options_for(root / "app");
  options.package_repository_roots.push_back((root / "repository").string());

  // When
  auto project = Rooc::prepare_project(options);

  // Then
  EXPECT_THAT(project.files, Contains(Field(&Rooc::EmbeddedFile::key, "util.roo")));
  EXPECT_THAT(project.plan.package_roots,
              Contains((root / "repository/util/1.0.0").generic_string()));
}

TEST(RoocProject, sanitizes_explicit_executable_name)
{
  // Given
  auto options = options_for(test_root() / "assets/packages/cafe-register");
  options.executable_name = "123 cafe-register";

  // When
  auto project = Rooc::prepare_project(options);

  // Then
  EXPECT_EQ(project.executable_name, "_123_cafe_register");
}

// This test intentionally consumes Proof and its dependencies from the repository package
// stage prepared by stage_proof_package.
TEST(RoocStagedPackageIntegration, prepares_proof_with_native_dependencies)
{
  // Given
  auto options =
    options_for(staged_package_repository_root() / "proof/test/assets/dynamic-smoke");
  options.package_repository_roots.push_back(staged_package_repository_root().string());

  // When
  auto project = Rooc::prepare_project(options);

  // Then
  EXPECT_THAT(project.plan.native_libraries,
              Contains(Field(&Roo::Package::NativeLibrary::name, "proof-native")));
  EXPECT_THAT(project.plan.native_namespaces, Contains("proof.syntax"));
  EXPECT_THAT(project.plan.package_roots,
              Contains((staged_package_repository_root() / "cli-trooper").generic_string()));
  EXPECT_THAT(project.files, Contains(Field(&Rooc::EmbeddedFile::key, "core.roo")));
}
