
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Semantic_FunctionArgumentDestructuring = LispleTest::RuntimeTestFixture;
TEST_F(Semantic_FunctionArgumentDestructuring, destructure_literal_map)
{
  // Given
  runtime.eval(R"(

  (defun my-func [{:keys [records mode]}]
    {:records records
     :old-mode mode
     :mode :mode/SELECT})
                )");

  // When
  Lisple::sptr_val result = runtime.eval(R"(
    (my-func {:request-id 16
              :mode :mode/UPDATE
              :records [{:record-id 8 :state :state/NEW}
                        {:record-id 9 :state :state/NEW}]})
                  )");

  // Then
  EXPECT_EQ(result->to_string(),
            "{:records [{:record-id 8 :state :state/NEW} {:record-id 9 :state "
            ":state/NEW}] :old-mode :mode/UPDATE :mode :mode/SELECT}");
}

TEST_F(Semantic_FunctionArgumentDestructuring,
       nil_argument_is_bound_and_shadows_global_symbol)
{
  // Given
  runtime.eval(R"(

  (def global-var {:request-id 16
                   :mode :mode/UPDATE
                   :records [{:record-id 8 :state :state/NEW}
                             {:record-id 9 :state :state/NEW}]})

  (defun my-func [{:keys [records mode] :as global-var}]
    global-var)

                )");

  // When
  Lisple::sptr_val result = runtime.eval("(my-func nil)");

  // Then
  EXPECT_EQ(result->to_string(), "nil");
}
