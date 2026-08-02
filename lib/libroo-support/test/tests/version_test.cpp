#include <stdexcept>
#include <string>

#include <gtest/gtest.h>
#include <roo-support/version.h>

namespace
{
  using Roo::Support::parse_version;
  using Roo::Support::parse_version_constraint;
  using Roo::Support::satisfies_version_constraint;
  using Roo::Support::Version;
  using Roo::Support::version_to_string;

  Version version(const std::string& text)
  {
    return parse_version(text);
  }

  bool satisfies(const std::string& version_text, const std::string& constraint_text)
  {
    return satisfies_version_constraint(version(version_text),
                                        parse_version_constraint(constraint_text));
  }
} // namespace

TEST(SemanticVersion, parses_core_version)
{
  auto parsed = parse_version("1.2.3");

  EXPECT_EQ(parsed.major, 1u);
  EXPECT_EQ(parsed.minor, 2u);
  EXPECT_EQ(parsed.patch, 3u);
  EXPECT_TRUE(parsed.prerelease.empty());
  EXPECT_TRUE(parsed.build_metadata.empty());
  EXPECT_EQ(version_to_string(parsed), "1.2.3");
}

TEST(SemanticVersion, parses_prerelease_and_build_metadata)
{
  auto parsed = parse_version("0.1.0-alpha.1+build.7");

  EXPECT_EQ(parsed.major, 0u);
  EXPECT_EQ(parsed.minor, 1u);
  EXPECT_EQ(parsed.patch, 0u);
  ASSERT_EQ(parsed.prerelease.size(), 2u);
  EXPECT_FALSE(parsed.prerelease[0].numeric);
  EXPECT_EQ(parsed.prerelease[0].text, "alpha");
  EXPECT_TRUE(parsed.prerelease[1].numeric);
  EXPECT_EQ(parsed.prerelease[1].number, 1u);
  EXPECT_EQ(parsed.build_metadata, "build.7");
  EXPECT_EQ(version_to_string(parsed), "0.1.0-alpha.1+build.7");
}

TEST(SemanticVersion, rejects_invalid_versions)
{
  EXPECT_THROW(parse_version(""), std::invalid_argument);
  EXPECT_THROW(parse_version("1.2"), std::invalid_argument);
  EXPECT_THROW(parse_version("1.2.3.4"), std::invalid_argument);
  EXPECT_THROW(parse_version("01.2.3"), std::invalid_argument);
  EXPECT_THROW(parse_version("1.02.3"), std::invalid_argument);
  EXPECT_THROW(parse_version("1.2.03"), std::invalid_argument);
  EXPECT_THROW(parse_version("1.2.3-"), std::invalid_argument);
  EXPECT_THROW(parse_version("1.2.3-alpha..1"), std::invalid_argument);
  EXPECT_THROW(parse_version("1.2.3-alpha.01"), std::invalid_argument);
  EXPECT_THROW(parse_version("1.2.3+"), std::invalid_argument);
  EXPECT_THROW(parse_version("1.2.3+build..1"), std::invalid_argument);
}

TEST(SemanticVersion, compares_core_versions)
{
  EXPECT_LT(version("1.2.3"), version("1.2.4"));
  EXPECT_LT(version("1.2.3"), version("1.3.0"));
  EXPECT_LT(version("1.2.3"), version("2.0.0"));
  EXPECT_GT(version("2.0.0"), version("1.9.9"));
}

TEST(SemanticVersion, compares_prerelease_versions_by_semver_precedence)
{
  EXPECT_LT(version("1.0.0-alpha"), version("1.0.0-alpha.1"));
  EXPECT_LT(version("1.0.0-alpha.1"), version("1.0.0-alpha.beta"));
  EXPECT_LT(version("1.0.0-alpha.beta"), version("1.0.0-beta"));
  EXPECT_LT(version("1.0.0-beta"), version("1.0.0-beta.2"));
  EXPECT_LT(version("1.0.0-beta.2"), version("1.0.0-beta.11"));
  EXPECT_LT(version("1.0.0-beta.11"), version("1.0.0-rc.1"));
  EXPECT_LT(version("1.0.0-rc.1"), version("1.0.0"));
}

TEST(SemanticVersion, ignores_build_metadata_for_precedence)
{
  EXPECT_EQ(version("1.2.3+build.1"), version("1.2.3+build.2"));
  EXPECT_EQ(version("1.2.3-alpha+build.1"), version("1.2.3-alpha+build.2"));
}

TEST(SemanticVersionConstraint, parses_exact_versions)
{
  auto constraint = parse_version_constraint("0.1.0-alpha.1");

  ASSERT_EQ(constraint.comparators.size(), 1u);
  EXPECT_TRUE(satisfies("0.1.0-alpha.1", "0.1.0-alpha.1"));
  EXPECT_FALSE(satisfies("0.1.0-alpha.2", "0.1.0-alpha.1"));
}

TEST(SemanticVersionConstraint, supports_single_comparators)
{
  EXPECT_TRUE(satisfies("0.1.0", ">=0.1.0-alpha.1"));
  EXPECT_TRUE(satisfies("0.1.0-alpha.1", ">=0.1.0-alpha.1"));
  EXPECT_FALSE(satisfies("0.1.0-alpha.0", ">=0.1.0-alpha.1"));

  EXPECT_TRUE(satisfies("0.1.0-alpha.1", "<0.1.0"));
  EXPECT_FALSE(satisfies("0.1.0", "<0.1.0"));

  EXPECT_TRUE(satisfies("0.1.0", "=0.1.0"));
  EXPECT_FALSE(satisfies("0.1.1", "=0.1.0"));
}

TEST(SemanticVersionConstraint, supports_space_separated_and_constraints)
{
  EXPECT_TRUE(satisfies("0.1.0-alpha.1", ">=0.1.0-alpha.1 <0.2.0"));
  EXPECT_TRUE(satisfies("0.1.9", ">=0.1.0-alpha.1 <0.2.0"));
  EXPECT_FALSE(satisfies("0.2.0", ">=0.1.0-alpha.1 <0.2.0"));
  EXPECT_FALSE(satisfies("0.1.0-alpha.0", ">=0.1.0-alpha.1 <0.2.0"));
}

TEST(SemanticVersionConstraint, trims_outer_whitespace)
{
  EXPECT_TRUE(satisfies("0.1.0", "  >=0.1.0   <0.2.0  "));
}

TEST(SemanticVersionConstraint, supports_whitespace_between_operator_and_version)
{
  EXPECT_TRUE(satisfies("0.1.0", ">= 0.1.0 < 0.2.0"));
}

TEST(SemanticVersionConstraint, rejects_invalid_constraints)
{
  EXPECT_THROW(parse_version_constraint(""), std::invalid_argument);
  EXPECT_THROW(parse_version_constraint(">"), std::invalid_argument);
  EXPECT_THROW(parse_version_constraint("=>0.1.0"), std::invalid_argument);
  EXPECT_THROW(parse_version_constraint(">=0.1"), std::invalid_argument);
  EXPECT_THROW(parse_version_constraint(">=0.1.0 || <0.2.0"), std::invalid_argument);
}
