#include "runtime_fixture.h"
#include <gtest/gtest.h>

using StringStartsWithPFunction = RooTest::RuntimeTestFixture;

TEST_F(StringStartsWithPFunction, tests_string_prefixes)
{
  EXPECT_EQ(*runtime.eval(R"((roo.string/starts-with? "Normal Club" "Normal"))"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((roo.string/starts-with? "Normal Club" "Club"))"),
            *Roo::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval(R"((roo.string/starts-with? "Normal Club" ""))"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((roo.string/starts-with? "Normal" "Normal Club"))"),
            *Roo::Constant::BOOL_FALSE);
}
