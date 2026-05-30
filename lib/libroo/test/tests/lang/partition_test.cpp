#include <vector>

#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/adapter.h>

using PartitionFunction = RooTest::RuntimeTestFixture;

TEST_F(PartitionFunction, partitions_vector)
{
  EXPECT_EQ(runtime.eval("(partition 2 [1 2 3])")->to_string(), "[[1 2] [3]]");
}

TEST_F(PartitionFunction, partitions_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((partition 2 "abcd"))")->to_string(), "[['a' 'b'] ['c' 'd']]");
}

TEST_F(PartitionFunction, partitions_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(partition 2 {:a 1 :b 2})")->to_string(), "[[:a 1] [:b 2]]");
}

TEST_F(PartitionFunction, partitions_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(partition 2 values)")->to_string(), "[[1 2] [3]]");
}
