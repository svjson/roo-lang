#include "runtime_fixture.h"
#include <gtest/gtest.h>

using JoinPath = RooTest::RuntimeTestFixture;

TEST_F(JoinPath, joins_path_components)
{
  EXPECT_EQ(runtime.eval(R"((roo.io/join-path "assets" "config.edn"))")->to_string(),
            R"("assets/config.edn")");
}

TEST_F(JoinPath, joins_multiple_path_segments)
{
  EXPECT_EQ(
    runtime.eval(R"((roo.io/join-path "assets" "images" "icons" "save.svg"))")
      ->to_string(),
    R"("assets/images/icons/save.svg")");
}

TEST_F(JoinPath, ignores_nil_path_segments)
{
  EXPECT_EQ(runtime.eval(
               R"((roo.io/join-path nil "assets" nil "images" "logo.svg" nil))")
              ->to_string(),
            R"("assets/images/logo.svg")");
}
