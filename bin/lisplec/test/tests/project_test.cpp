#include <filesystem>
#include <fstream>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <lisplec/project.h>

using ::testing::Contains;
using ::testing::Field;

namespace
{
  std::filesystem::path repo_root()
  {
    return std::filesystem::path(LISPLEC_TEST_REPO_ROOT);
  }

  Lisplec::Options options_for(const std::filesystem::path& package_dir)
  {
    Lisplec::Options options;
    options.command = "generate";
    options.package_dir = package_dir;
    options.build_dir = ".lisplec";
    return options;
  }

  void write_file(const std::filesystem::path& path, const std::string& contents)
  {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path);
    out << contents;
  }
} // namespace

TEST(LisplecProject, prepares_pure_package_with_file_dependency)
{
  // Given
  auto options =
    options_for(repo_root() / "lib/lisple-package/test/tests/assets/packages/cafe-register");

  // When
  auto project = Lisplec::prepare_project(options);

  // Then
  EXPECT_EQ(project.executable_name, "cafe_register");
  EXPECT_THAT(project.plan.entry_points, Contains("cafe.run"));
  EXPECT_THAT(project.files,
              Contains(Field(&Lisplec::EmbeddedFile::key, "cafe/run.lisple")));
  EXPECT_THAT(project.files,
              Contains(Field(&Lisplec::EmbeddedFile::key, "recipe/book.lisple")));
}

TEST(LisplecProject, collects_roo_source_files)
{
  // Given
  const auto root =
    std::filesystem::temp_directory_path() / "lisplec-project-roo-source-test";
  std::filesystem::remove_all(root);
  write_file(root / "package.edn",
             "{:name roo-source-app\n"
             " :version \"0.1.0\"\n"
             " :load-roots [\"src\"]\n"
             " :entry-points [main.app]}\n");
  write_file(root / "src/main/app.roo", "(ns main.app)\n");
  auto options = options_for(root);

  // When
  auto project = Lisplec::prepare_project(options);

  // Then
  EXPECT_THAT(project.files, Contains(Field(&Lisplec::EmbeddedFile::key,
                                            "main/app.roo")));
}

TEST(LisplecProject, sanitizes_explicit_executable_name)
{
  // Given
  auto options =
    options_for(repo_root() / "lib/lisple-package/test/tests/assets/packages/cafe-register");
  options.executable_name = "123 cafe-register";

  // When
  auto project = Lisplec::prepare_project(options);

  // Then
  EXPECT_EQ(project.executable_name, "_123_cafe_register");
}

TEST(LisplecProject, prepares_package_with_native_dependency)
{
  // Given
  auto options = options_for(repo_root() / "pkg/proof/test/assets/dynamic-smoke");

  // When
  auto project = Lisplec::prepare_project(options);

  // Then
  EXPECT_THAT(project.plan.native_libraries,
              Contains(Field(&Lisple::Package::NativeLibrary::name, "proof-native")));
  EXPECT_THAT(project.plan.native_namespaces, Contains("proof.syntax"));
  EXPECT_THAT(project.files, Contains(Field(&Lisplec::EmbeddedFile::key,
                                            "proof/core.lisple")));
}
