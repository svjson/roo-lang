
#include <gtest/gtest.h>
#include <lisple/runtime.h>

TEST(Semantic_FunctionArgumentDestructuring, destructure_literal_map)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"(

  (defun my-func [{:keys [records mode]}]
    {:records records
     :old-mode mode
     :mode :mode/SELECT})
                )");

  Lisple::sptr_sobject result = runtime.eval(R"(
    (my-func {:request-id 16
              :mode :mode/UPDATE
              :records [{:record-id 8 :state :state/NEW}
                        {:record-id 9 :state :state/NEW}]})
                  )");

  EXPECT_EQ(result->to_string(),
            "{:records [{:record-id 8 :state :state/NEW} {:record-id 9 :state "
            ":state/NEW}] :old-mode :mode/UPDATE :mode :mode/SELECT}");
}
