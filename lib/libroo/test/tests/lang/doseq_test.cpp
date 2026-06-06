#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DoseqForm = RooTest::RuntimeTestFixture;

TEST_F(DoseqForm, returns_nil_and_does_not_retain_body_values)
{
  EXPECT_EQ(runtime.eval("(doseq [num [1 2 3]] (* num 2))")->to_string(), "nil");
}

TEST_F(DoseqForm, executes_body_for_each_item)
{
  runtime.eval("(def results [])");

  EXPECT_EQ(runtime.eval("(doseq [num [1 2 3]] (append! results (* num 2)))")->to_string(),
            "nil");
  EXPECT_EQ(runtime.eval("results")->to_string(), "[2 4 6]");
}

TEST_F(DoseqForm, supports_map_destructuring)
{
  runtime.eval("(def sums [])");

  runtime.eval("(doseq [{:keys [a b]} [{:a 1 :b 10} {:a 5 :b 4}]] "
               "  (append! sums (+ a b)))");

  EXPECT_EQ(runtime.eval("sums")->to_string(), "[11 9]");
}

TEST_F(DoseqForm, each_char_in_string)
{
  runtime.eval("(def chars [])");

  runtime.eval(R"((doseq [ch "Roo"] (append! chars ch)))");

  EXPECT_EQ(runtime.eval("chars")->to_string(), "['R' 'o' 'o']");
}
