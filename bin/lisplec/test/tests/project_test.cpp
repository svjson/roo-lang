#include <filesystem>
#include <string>

#include <lisple/exception.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <lisplec/project.h>

using ::testing::Contains;
using ::testing::Field;
using ::testing::HasSubstr;

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

TEST(LisplecProject, rejects_native_package_dependencies)
{
  // Given
  auto options = options_for(repo_root() / "pkg/proof/test/assets/dynamic-smoke");

  // When / Then
  try
  {
    (void)Lisplec::prepare_project(options);
    FAIL() << "Expected native package rejection";
  }
  catch (const Lisple::LispleException& e)
  {
    EXPECT_THAT(std::string(e.what()), HasSubstr("Native libraries"));
    EXPECT_THAT(std::string(e.what()), HasSubstr("proof-native"));
  }
}
