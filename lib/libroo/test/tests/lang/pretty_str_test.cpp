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

TEST_F(PrettyStrFunction, indent_zero_is_legitimate_and_adds_no_indentation)
{
  // Nesting is still conveyed by brackets alone - 0 is a valid indent
  // level, just not a visually indented one.
  EXPECT_EQ(*runtime.eval(R"((pretty-str {:a 1 :b [true nil]} {:indent 0}))"),
            *Roo::Value::string(R"({
:a 1
:b [
true
nil
]
})"));
}

TEST_F(PrettyStrFunction, width_collapses_content_that_fits_on_one_line)
{
  EXPECT_EQ(*runtime.eval(R"((pretty-str {:a 1 :b [true nil]} {:width 40}))"),
            *Roo::Value::string("{:a 1 :b [true nil]}"));
}

TEST_F(PrettyStrFunction, width_only_breaks_collections_that_do_not_fit)
{
  EXPECT_EQ(
    *runtime.eval(
      R"((pretty-str {:short [1 2 3] :long "a-value-long-enough-to-force-the-outer-map-to-break"} {:width 60}))"),
    *Roo::Value::string(R"({
  :short [1 2 3]
  :long "a-value-long-enough-to-force-the-outer-map-to-break"
})"));
}

TEST_F(PrettyStrFunction, max_depth_truncates_collections_nested_past_the_limit)
{
  EXPECT_EQ(*runtime.eval(R"((pretty-str {:a 1 :b [1 2] :c {:x 1}} {:max-depth 1}))"),
            *Roo::Value::string(R"({
  :a 1
  :b [...]
  :c {...}
})"));
}

TEST_F(PrettyStrFunction, max_depth_zero_truncates_the_root_collection)
{
  EXPECT_EQ(*runtime.eval(R"((pretty-str {:a 1} {:max-depth 0}))"),
            *Roo::Value::string("{...}"));
}

TEST_F(PrettyStrFunction, max_elements_truncates_vector_contents)
{
  EXPECT_EQ(*runtime.eval(R"((pretty-str [1 2 3 4 5] {:max-elements 2 :width 40}))"),
            *Roo::Value::string("[1 2 ...]"));
}

TEST_F(PrettyStrFunction, max_elements_truncates_map_entries)
{
  EXPECT_EQ(*runtime.eval(R"((pretty-str {:a 1 :b 2 :c 3} {:max-elements 1 :width 40}))"),
            *Roo::Value::string("{:a 1 ...}"));
}

TEST_F(PrettyStrFunction, rejects_negative_width)
{
  EXPECT_THROW(runtime.eval(R"((pretty-str {:a 1} {:width -1}))"), Roo::TypeError);
}

TEST_F(PrettyStrFunction, rejects_negative_max_depth)
{
  EXPECT_THROW(runtime.eval(R"((pretty-str {:a 1} {:max-depth -1}))"), Roo::TypeError);
}
