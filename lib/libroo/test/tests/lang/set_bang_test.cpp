#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SetBangMacro = RooTest::RuntimeTestFixture;
TEST_F(SetBangMacro, set_global_value)
{
  // Given
  runtime.eval("(def my-global-var 10)");

  // When
  runtime.eval("(set! [my-global-var] 50)");

  // Then
  auto global_var = runtime.get_current_namespace().lookup("my-global-var");
  ASSERT_EQ(global_var->i64(), 50);
}

TEST_F(SetBangMacro, set_parent_scope_value)
{
  // Given
  auto my_var = runtime.eval("(let [my-var 10] (do (set! [my-var] 20) my-var))");

  // Then
  EXPECT_EQ(my_var->i64(), 20);
}

TEST_F(SetBangMacro, set_global_map_value)
{
  // Given
  runtime.eval("(def global-map {:key1 10 :key2 20})");

  // When
  runtime.eval("(set! [:key2 global-map] \"Number 9\")");

  // Then
  auto global_map = runtime.get_current_namespace().lookup("global-map");
  auto expected_map = runtime.eval("{:key1 10 :key2 \"Number 9\"}");
  ASSERT_EQ(*global_map, *expected_map);
}
