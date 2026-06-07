#include "runtime_fixture.h"
#include <gtest/gtest.h>

using UpdateBangFunction = RooTest::RuntimeTestFixture;

TEST_F(UpdateBangFunction, mutates_existing_map_key)
{
  runtime.eval("(def my-map {:count 2 :other 99})");

  auto result = runtime.eval("(update! my-map :count (fn [x] (+ x 1)))");

  EXPECT_EQ(*result, *runtime.eval("{:count 3 :other 99}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:count 3 :other 99}");
}

TEST_F(UpdateBangFunction, mutates_missing_map_key_with_nil_current_value)
{
  runtime.eval("(def my-map {:a 1})");

  auto result = runtime.eval("(update! my-map :b (fn [x] (if (nil? x) 44 (+ x 1))))");

  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 44}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:a 1 :b 44}");
}

TEST_F(UpdateBangFunction, passes_extra_args_to_update_function)
{
  runtime.eval("(def my-map {:count 2})");

  auto result = runtime.eval("(update! my-map :count [+ 10 5])");

  EXPECT_EQ(*result, *runtime.eval("{:count 17}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:count 17}");
}

TEST_F(UpdateBangFunction, mutates_sequence_by_index)
{
  runtime.eval("(def my-vec [1 2 3])");

  auto result = runtime.eval("(update! my-vec 1 [* 10])");

  EXPECT_EQ(result->to_string(), "[1 20 3]");
  EXPECT_EQ(runtime.lookup("my-vec")->to_string(), "[1 20 3]");
}

TEST_F(UpdateBangFunction, mutates_multiple_keys_in_one_call)
{
  runtime.eval("(def my-map {:a 1 :b 2})");

  auto result = runtime.eval("(update! my-map :a (fn [x] (+ x 1)) :b [+ 10])");

  EXPECT_EQ(*result, *runtime.eval("{:a 2 :b 12}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:a 2 :b 12}");
}
