#include <map>
#include <string>
#include <vector>

#include <lisple/adapter.h>
#include <lisple/runtime/value.h>

#include "host/test_adapters/vectorgfx_native_adapters.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Semantic_StructuralEquality = LispleTest::RuntimeTestFixture;

TEST_F(Semantic_StructuralEquality, native_adapter_compares_equal_to_map_shape)
{
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::PointNamespace>("pixils.point"));
  auto& runtime = use_runtime_with(std::move(namespaces), nullptr);

  runtime.eval("(def point (pixils.point/make-point {:x 10 :y 15}))");

  EXPECT_EQ(*runtime.eval("(= point {:x 10 :y 15})"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= {:x 10 :y 15} point)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= point (pixils.point/make-point {:x 10 :y 15}))"),
            *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(= point {:x 10 :y 16})"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= point {:x 10})"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= point {:x 10 :y 15 :z 0})"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(Semantic_StructuralEquality, vectors_compare_nested_native_adapter_to_map_shape)
{
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::PointNamespace>("pixils.point"));
  auto& runtime = use_runtime_with(std::move(namespaces), nullptr);

  runtime.eval("(def point (pixils.point/make-point {:x 10 :y 15}))");

  EXPECT_EQ(*runtime.eval("(= [point] [{:x 10 :y 15}])"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= [{:x 10 :y 15}] [point])"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= [{:point point}] [{:point {:x 10 :y 15}}])"),
            *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(= [point] [{:x 10 :y 16}])"), *Lisple::Constant::BOOL_FALSE);
}

TEST_F(Semantic_StructuralEquality, native_std_vector_compares_equal_to_vector_shape)
{
  std::vector<int> values = {1, 2, 3};
  std::vector<int> same_values = {1, 2, 3};
  std::vector<int> different_values = {1, 2, 4};

  runtime.get_current_namespace().store(
    "native-values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));
  runtime.get_current_namespace().store(
    "same-native-values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(same_values));
  runtime.get_current_namespace().store(
    "different-native-values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(different_values));

  EXPECT_EQ(*runtime.eval("(= native-values [1 2 3])"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= [1 2 3] native-values)"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= native-values same-native-values)"),
            *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval("(= native-values [1 2 4])"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= native-values different-native-values)"),
            *Lisple::Constant::BOOL_FALSE);
}

TEST_F(Semantic_StructuralEquality, native_std_map_compares_equal_to_map_shape)
{
  std::map<int, std::string> values = {{1, "one"}, {2, "two"}};
  std::map<int, std::string> same_values = {{1, "one"}, {2, "two"}};
  std::map<int, std::string> different_values = {{1, "one"}, {2, "too"}};

  runtime.get_current_namespace().store(
    "native-map",
    Lisple::NativeStdMapAdapter<int, std::string>::make_ref(values));
  runtime.get_current_namespace().store(
    "same-native-map",
    Lisple::NativeStdMapAdapter<int, std::string>::make_ref(same_values));
  runtime.get_current_namespace().store(
    "different-native-map",
    Lisple::NativeStdMapAdapter<int, std::string>::make_ref(different_values));

  EXPECT_EQ(*runtime.eval(R"((= native-map {1 "one" 2 "two"}))"),
            *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval(R"((= {1 "one" 2 "two"} native-map))"),
            *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*runtime.eval("(= native-map same-native-map)"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*runtime.eval(R"((= native-map {1 "one" 2 "too"}))"),
            *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*runtime.eval("(= native-map different-native-map)"),
            *Lisple::Constant::BOOL_FALSE);
}
