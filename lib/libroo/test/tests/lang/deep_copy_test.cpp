#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DeepCopyFunction = RooTest::RuntimeTestFixture;

TEST_F(DeepCopyFunction, recursively_detaches_mutable_collection_structure)
{
  EXPECT_EQ(runtime
              .eval(R"((do
                         (def original {:nested {:count 1}})
                         (def copied (deep-copy original))
                         (assoc-in! copied [:nested :count] 2)
                         [original copied]))")
              ->to_string(),
            "[{:nested {:count 1}} {:nested {:count 2}}]");
}

TEST_F(DeepCopyFunction, preserves_aliases_within_the_copied_graph)
{
  EXPECT_EQ(runtime
              .eval(R"((do
                         (def shared {:count 1})
                         (def copied (deep-copy [shared shared]))
                         (assoc! (head copied) :count 2)
                         [shared copied]))")
              ->to_string(),
            "[{:count 1} [{:count 2} {:count 2}]]");
}

TEST_F(DeepCopyFunction, accepts_and_shares_executable_leaves)
{
  EXPECT_EQ(runtime.eval("(= identity (deep-copy identity))")->to_string(), "true");
}

TEST_F(DeepCopyFunction, rejects_cyclic_collection_graphs)
{
  EXPECT_THROW(runtime.eval("(do (def cyclic [nil]) (assoc! cyclic 0 cyclic) (deep-copy cyclic))"),
               Roo::RooException);
}
