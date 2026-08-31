#include <roo/exception.h>

#include "gmock/gmock.h"
#include "host/test_adapters/vehicle_native_adapters.h"
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

TEST_F(UpdateInBangFunction, vector_shorthand_lowers_special_forms)
{
  runtime.eval("(def my-map {:nested {:value nil}})");

  auto result = runtime.eval("(update-in! my-map [:nested :value] [or 7])");

  EXPECT_EQ(result->to_string(), "{:nested {:value 7}}");
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:nested {:value 7}}");
}

TEST_F(UpdateInBangFunction, vector_shorthand_works_through_thread_first)
{
  runtime.eval("(def my-map {:nested {:value nil}})");

  auto result = runtime.eval("(-> my-map (update-in! [:nested :value] [or 7]))");

  EXPECT_EQ(result->to_string(), "{:nested {:value 7}}");
  EXPECT_EQ(runtime.lookup("my-map")->to_string(), "{:nested {:value 7}}");
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

TEST_F(UpdateInBangFunction, creates_missing_maps_below_a_nil_target)
{
  runtime.eval("(def target nil)");

  auto result =
    runtime.eval("(update-in! target [:nested :count] (fn [x] (if (nil? x) 1 x)))");

  EXPECT_EQ(*result, *runtime.eval("{:nested {:count 1}}"));
  EXPECT_EQ(*runtime.lookup("target"), *Roo::Constant::NIL);
}

TEST_F(UpdateInBangFunction, creates_a_missing_intermediate_map)
{
  runtime.eval("(def target {:a 1})");
  auto target = runtime.lookup("target");

  auto result = runtime.eval(
    "(update-in! target [:nested :count] (fn [x] (if (nil? x) 2 x)))");

  EXPECT_EQ(result, target);
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :nested {:count 2}}"));
}

TEST_F(UpdateInBangFunction, creates_a_missing_path_when_updater_returns_nil)
{
  runtime.eval("(def target {})");

  auto result = runtime.eval("(update-in! target [:missing] (fn [value] value))");

  EXPECT_EQ(*result, *runtime.eval("{:missing nil}"));
}

TEST_F(UpdateInBangFunction, updates_through_read_only_native_property)
{
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), nullptr);
  runtime.eval(R"(
    (def vehicle
      (vehicle/make-vehicle
        {:model {:model-name "Roadster" :seats 2}
         :reg-number {:letters "ABC" :numbers "123"}}))
  )");
  auto vehicle = runtime.lookup("vehicle");

  auto result = runtime.eval("(update-in! vehicle [:model :seats] [+ 2])");

  EXPECT_EQ(result, vehicle);
  EXPECT_EQ(runtime.eval("(:seats (:model vehicle))")->i64(), 4);
}

TEST_F(UpdateInBangFunction, does_not_write_back_same_object_to_read_only_native_property)
{
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), nullptr);
  runtime.eval(R"(
    (def vehicle
      (vehicle/make-vehicle
        {:model {:model-name "Roadster" :seats 2}
         :reg-number {:letters "ABC" :numbers "123"}}))
  )");
  auto vehicle = runtime.lookup("vehicle");

  auto result = runtime.eval(
    "(update-in! vehicle [:model] (fn [model] (assoc! model :seats 4)))");

  EXPECT_EQ(result, vehicle);
  EXPECT_EQ(runtime.eval("(:seats (:model vehicle))")->i64(), 4);
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
