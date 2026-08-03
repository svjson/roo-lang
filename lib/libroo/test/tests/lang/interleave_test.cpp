#include <vector>

#include <roo/adapter.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using InterleaveFunction = RooTest::RuntimeTestFixture;

TEST_F(InterleaveFunction, interleaves_vectors)
{
  EXPECT_EQ(runtime.eval("(interleave [1 2 3] [:a :b :c])")->to_string(),
            "[1 :a 2 :b 3 :c]");
}

TEST_F(InterleaveFunction, keeps_remaining_values_from_longer_sequences)
{
  EXPECT_EQ(runtime.eval("(interleave [1 2 3] [:a])")->to_string(), "[1 :a 2 3]");
  EXPECT_EQ(runtime.eval("(interleave [1] [:a :b :c])")->to_string(), "[1 :a :b :c]");
}

TEST_F(InterleaveFunction, interleaves_strings_as_char_sequences)
{
  EXPECT_EQ(runtime.eval(R"((interleave "ab" [1 2]))")->to_string(), "['a' 1 'b' 2]");
}

TEST_F(InterleaveFunction, interleaves_maps_as_interleaved_sequences)
{
  EXPECT_EQ(runtime.eval("(interleave {:a 1 :b 2} [:x :y :z :q])")->to_string(),
            "[:a :x 1 :y :b :z 2 :q]");
}

TEST_F(InterleaveFunction, interleaves_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(interleave values [:a :b])")->to_string(), "[1 :a 2 :b 3]");
}

TEST_F(InterleaveFunction, returns_empty_vector_for_no_arguments)
{
  EXPECT_EQ(runtime.eval("(interleave)")->to_string(), "[]");
}
