#include <string>

#include <roo/exception.h>

#include <gtest/gtest.h>
#include <rooc/options.h>

namespace
{
  Rooc::Options parse(std::initializer_list<const char*> args)
  {
    std::vector<char*> argv;
    argv.reserve(args.size());
    for (const char* arg : args)
    {
      argv.push_back(const_cast<char*>(arg));
    }
    return Rooc::parse_args(static_cast<int>(argv.size()), argv.data());
  }
} // namespace

TEST(RoocOptions, parses_generate_command_with_defaults)
{
  // When
  auto options = parse({"rooc", "generate", "pkg/app"});

  // Then
  EXPECT_EQ(options.command, "generate");
  EXPECT_EQ(options.package_dir.generic_string(), "pkg/app");
  EXPECT_EQ(options.build_dir.generic_string(), ".rooc");
  EXPECT_TRUE(options.executable_name.empty());
}

TEST(RoocOptions, parses_build_dir_and_name)
{
  // When
  auto options =
    parse({"rooc", "build", "pkg/app", "--build-dir", "out/rooc", "--name", "app"});

  // Then
  EXPECT_EQ(options.command, "build");
  EXPECT_EQ(options.package_dir.generic_string(), "pkg/app");
  EXPECT_EQ(options.build_dir.generic_string(), "out/rooc");
  EXPECT_EQ(options.executable_name, "app");
}

TEST(RoocOptions, parses_equals_form_options)
{
  // When
  auto options = parse({"rooc", "build", "pkg/app", "--build-dir=out/rooc", "--name=app"});

  // Then
  EXPECT_EQ(options.build_dir.generic_string(), "out/rooc");
  EXPECT_EQ(options.executable_name, "app");
}

TEST(RoocOptions, parses_package_repository_roots)
{
  // When
  auto options = parse({"rooc",
                        "build",
                        "pkg/app",
                        "-R",
                        "out/repo",
                        "--package-repository=vendor/repo"});

  // Then
  EXPECT_EQ(options.package_repository_roots,
            (std::vector<std::string>{"out/repo", "vendor/repo"}));
}

TEST(RoocOptions, rejects_missing_package_repository_root)
{
  // Then
  EXPECT_THROW(parse({"rooc", "build", "pkg/app", "--package-repository"}),
               Roo::RooException);
}

TEST(RoocOptions, rejects_old_repo_option)
{
  // Then
  EXPECT_THROW(parse({"rooc", "build", "pkg/app", "--repo", "out/repo"}),
               Roo::RooException);
}

TEST(RoocOptions, rejects_unknown_command)
{
  // Then
  EXPECT_THROW(parse({"rooc", "run", "pkg/app"}), Roo::RooException);
}

TEST(RoocOptions, rejects_unknown_option)
{
  // Then
  EXPECT_THROW(parse({"rooc", "build", "pkg/app", "--wat"}), Roo::RooException);
}
