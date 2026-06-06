#include "runtime_fixture.h"
#include <gtest/gtest.h>

using PrettyStrFunction = RooTest::RuntimeTestFixture;

TEST_F(PrettyStrFunction, formats_nested_values)
{
  EXPECT_EQ(*runtime.eval(R"((pretty-str {:a 1 :b [true nil]}))"), *Roo::Value::string(R"({
  :a 1
  :b [
    true
    nil
  ]
})"));
}

TEST_F(PrettyStrFunction, supports_custom_indent)
{
  EXPECT_EQ(*runtime.eval(R"((pretty-str {:a 1 :b [true nil]} {:indent 4}))"),
            *Roo::Value::string(R"({
    :a 1
    :b [
        true
        nil
    ]
})"));
}
