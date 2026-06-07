#include <roo/exception.h>

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using UpdateInBangFunction = RooTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(UpdateInBangFunction, mutates_existing_nested_map_key)
{
  runtime.eval("(def my-map {:nested {:count 2} :other 99})");

  auto result = runtime.eval("(update-in! my-map [:nested :count] (fn [x] (+ x 1)))");

  EXPECT_EQ(*result, *runtime.eval("{:nested {:count 3} :other 99}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:nested {:count 3} :other 99}");
}

TEST_F(UpdateInBangFunction, passes_extra_args_to_update_function)
{
  runtime.eval("(def my-map {:nested {:count 2}})");

  auto result = runtime.eval("(update-in! my-map [:nested :count] [+ 10 5])");

  EXPECT_EQ(*result, *runtime.eval("{:nested {:count 17}}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:nested {:count 17}}");
}

TEST_F(UpdateInBangFunction, mutates_nested_sequence_by_index)
{
  runtime.eval("(def my-map {:vec [1 2 3]})");

  auto result = runtime.eval("(update-in! my-map [:vec 1] [* 10])");

  EXPECT_EQ(result->to_string(), "{:vec [1 20 3]}");
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:vec [1 20 3]}");
}

TEST_F(UpdateInBangFunction, mutates_root_sequence_by_path)
{
  runtime.eval("(def my-vec [1 2 3])");

  auto result = runtime.eval("(update-in! my-vec [1] [* 10])");

  EXPECT_EQ(result->to_string(), "[1 20 3]");
  EXPECT_EQ(runtime.lookup("my-vec")->to_string(), "[1 20 3]");
}

TEST_F(UpdateInBangFunction, mutates_multiple_paths_in_one_call)
{
  runtime.eval("(def my-map {:a 1 :nested {:count 2}})");

  auto result =
    runtime.eval("(update-in! my-map [:a] (fn [x] (+ x 1)) [:nested :count] [+ 10])");

  EXPECT_EQ(*result, *runtime.eval("{:a 2 :nested {:count 12}}"));
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:a 2 :nested {:count 12}}");
}

TEST_F(UpdateInBangFunction, throws_on_non_sequence_path)
{
  runtime.eval("(def my-map {:a 1})");

  EXPECT_THAT(
    [this]() { runtime.eval("(update-in! my-map :a (fn [x] x))"); },
    ThrowsMessage<Roo::TypeError>(HasSubstr("Path for update-in! must be a sequence")));
}

TEST_F(UpdateInBangFunction, throws_on_empty_path)
{
  runtime.eval("(def my-map {:a 1})");

  EXPECT_THAT([this]() { runtime.eval("(update-in! my-map [] (fn [x] x))"); },
              ThrowsMessage<Roo::InvocationException>(
                HasSubstr("Path for update-in! cannot be empty.")));
}
