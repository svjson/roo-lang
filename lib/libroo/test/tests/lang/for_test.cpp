#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ForForm = RooTest::RuntimeTestFixture;
TEST_F(ForForm, transform_vector_of_int)
{
  // Given
  runtime.eval("(def my-vector [1 2 3 4 5 6])");

  // When
  auto retval = runtime.eval("(for [num my-vector] (* num 2))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6 8 10 12]");
}

TEST_F(ForForm, with_map_destructuring)
{
  // Given
  runtime.eval("(def my-vector [{:a 1 :b 10} {:a 5 :b 4}])");

  // When
  auto retval = runtime.eval("(for [{:keys [a b]} my-vector] (+ a b))");

  // Then
  ASSERT_EQ(retval->to_string(), "[11 9]");
}

TEST_F(ForForm, each_char_in_string)
{
  // Given
  auto retval = runtime.eval("(for [ch \"Beer Gnome!\"] ch)");

  // Then
  ASSERT_EQ(retval->to_string(), "['B' 'e' 'e' 'r' ' ' 'G' 'n' 'o' 'm' 'e' '!']");
}
