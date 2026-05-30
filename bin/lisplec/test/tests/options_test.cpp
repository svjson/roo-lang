#include <string>

#include <lisple/exception.h>

#include <gtest/gtest.h>
#include <lisplec/options.h>

namespace
{
  Lisplec::Options parse(std::initializer_list<const char*> args)
  {
    std::vector<char*> argv;
    argv.reserve(args.size());
    for (const char* arg : args)
    {
      argv.push_back(const_cast<char*>(arg));
    }
    return Lisplec::parse_args(static_cast<int>(argv.size()), argv.data());
  }
} // namespace

TEST(LisplecOptions, parses_generate_command_with_defaults)
{
  // When
  auto options = parse({"lisplec", "generate", "pkg/app"});

  // Then
  EXPECT_EQ(options.command, "generate");
  EXPECT_EQ(options.package_dir.generic_string(), "pkg/app");
  EXPECT_EQ(options.build_dir.generic_string(), ".rooc");
  EXPECT_TRUE(options.executable_name.empty());
}

TEST(LisplecOptions, parses_build_dir_and_name)
{
  // When
  auto options =
    parse({"lisplec", "build", "pkg/app", "--build-dir", "out/lisplec", "--name", "app"});

  // Then
  EXPECT_EQ(options.command, "build");
  EXPECT_EQ(options.package_dir.generic_string(), "pkg/app");
  EXPECT_EQ(options.build_dir.generic_string(), "out/lisplec");
  EXPECT_EQ(options.executable_name, "app");
}

TEST(LisplecOptions, parses_equals_form_options)
{
  // When
  auto options =
    parse({"lisplec", "build", "pkg/app", "--build-dir=out/lisplec", "--name=app"});

  // Then
  EXPECT_EQ(options.build_dir.generic_string(), "out/lisplec");
  EXPECT_EQ(options.executable_name, "app");
}

TEST(LisplecOptions, rejects_unknown_command)
{
  // Then
  EXPECT_THROW(parse({"lisplec", "run", "pkg/app"}), Lisple::LispleException);
}

TEST(LisplecOptions, rejects_unknown_option)
{
  // Then
  EXPECT_THROW(parse({"lisplec", "build", "pkg/app", "--wat"}), Lisple::LispleException);
}
