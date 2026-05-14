#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <lisple/adapter.h>
#include "runtime_fixture.h"
#include <lisple/runtime/dict.h>
#include <lisple/runtime/seq.h>
#include <lisple/runtime/value.h>

#include <gtest/gtest.h>


using NativeStdVectorAdapter_int = LispleTest::RuntimeTestFixture;
using NativeStdMapAdapter_int_string = LispleTest::RuntimeTestFixture;
using NativeStdMapAdapter_int_const_string = LispleTest::RuntimeTestFixture;
using NativeStdMapAdapter_uint8_short = LispleTest::RuntimeTestFixture;
TEST_F(NativeStdVectorAdapter_int, get_set_children_and_count)
{
  std::vector<int> values = {1, 2, 3};
  Lisple::sptr_rtval adapter = Lisple::NativeStdVectorAdapter<int>::make_ref(values);

  EXPECT_EQ(Lisple::count(*adapter), 3);
  EXPECT_EQ(*Lisple::get_child(*adapter, 1), *Lisple::RTValue::number(2));

  Lisple::sptr_rtval updated = Lisple::RTValue::number(8);
  Lisple::Dict::set_property(adapter, Lisple::RTValue::number(1), updated);

  EXPECT_EQ(values.at(1), 8);
  EXPECT_EQ(*Lisple::get_child(*adapter, 1), *Lisple::RTValue::number(8));
  EXPECT_EQ(adapter->nobj()->to_string(), "[1 8 3]");
}

TEST_F(NativeStdVectorAdapter_int, for_iterates_native_vector)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Lisple::NativeStdVectorAdapter<int>::make_ref(values));

  auto result = runtime.eval("(for [value values] (* value 2))");

  EXPECT_EQ(result->to_string(), "[2 4 6]");
}

TEST_F(NativeStdMapAdapter_int_string, get_set_keys_and_count)
{
  std::map<int, std::string> values = {{1, "one"}, {2, "two"}};
  Lisple::sptr_rtval adapter =
    Lisple::NativeStdMapAdapter<int, std::string>::make_ref(values);

  EXPECT_EQ(Lisple::count(*adapter), 2);
  EXPECT_EQ(*Lisple::Dict::get_property(adapter, Lisple::RTValue::number(1)),
            *Lisple::RTValue::string("one"));

  Lisple::sptr_rtval updated = Lisple::RTValue::string("three");
  Lisple::Dict::set_property(adapter, Lisple::RTValue::number(3), updated);

  EXPECT_EQ(values.at(3), "three");
  EXPECT_EQ(*Lisple::Dict::get_property(adapter, Lisple::RTValue::number(3)),
            *Lisple::RTValue::string("three"));
  EXPECT_EQ(Lisple::Dict::map_sptr_keys(adapter).size(), 3);
  EXPECT_EQ(adapter->nobj()->to_string(), R"({1 "one" 2 "two" 3 "three"})");
}

TEST_F(NativeStdMapAdapter_int_string, script_usage)
{
  std::map<int, std::string> values = {{1, "one"}, {2, "two"}, {3, "three"}};

  runtime.get_current_namespace().store(
    "my-map",
    Lisple::NativeStdMapAdapter<int, std::string>::make_ref(values));

  EXPECT_EQ(runtime.eval("my-map")->type, Lisple::RTValue::Type::NATIVE_OBJECT);
  EXPECT_EQ(*runtime.eval("(str my-map)"),
            *Lisple::RTValue::string(R"({1 "one" 2 "two" 3 "three"})"));

  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Lisple::RTValue::string("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Lisple::RTValue::string("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Lisple::RTValue::string("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map \"SEMPRINI!\")"), *Lisple::Constant::NIL);

  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::RTValue::number(3));
  runtime.eval(R"((def updated-map (assoc my-map 4 "four")))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Lisple::RTValue::number(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::RTValue::number(3));

  runtime.eval(R"((assoc! my-map 8 "eight"))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Lisple::RTValue::number(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::RTValue::number(4));
  EXPECT_EQ(*runtime.eval("(get my-map 8)"), *Lisple::RTValue::string("eight"));
}

TEST_F(NativeStdMapAdapter_int_const_string, script_usage)
{
  std::map<int, const std::string> values = {{1, "one"}, {2, "two"}, {3, "three"}};

  runtime.get_current_namespace().store(
    "my-map",
    Lisple::NativeStdMapAdapter<int, const std::string>::make_ref(values));

  EXPECT_EQ(runtime.eval("my-map")->type, Lisple::RTValue::Type::NATIVE_OBJECT);
  EXPECT_EQ(*runtime.eval("(str my-map)"),
            *Lisple::RTValue::string(R"({1 "one" 2 "two" 3 "three"})"));

  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Lisple::RTValue::string("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Lisple::RTValue::string("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Lisple::RTValue::string("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map \"SEMPRINI!\")"), *Lisple::Constant::NIL);

  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::RTValue::number(3));
  runtime.eval(R"((def updated-map (assoc my-map 4 "four")))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Lisple::RTValue::number(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::RTValue::number(3));

  runtime.eval(R"((assoc! my-map 8 "eight"))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Lisple::RTValue::number(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::RTValue::number(4));
  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Lisple::RTValue::string("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Lisple::RTValue::string("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Lisple::RTValue::string("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map 8)"), *Lisple::RTValue::string("eight"));
}

TEST_F(NativeStdMapAdapter_uint8_short, get_set_numeric_map)
{
  std::map<uint8_t, short> values = {{1, 8}, {2, 16}};
  Lisple::sptr_rtval adapter = Lisple::NativeStdMapAdapter<uint8_t, short>::make_ref(values);

  EXPECT_EQ(*Lisple::Dict::get_property(adapter, Lisple::RTValue::number(2)),
            *Lisple::RTValue::number(16));

  Lisple::sptr_rtval updated = Lisple::RTValue::number(24);
  Lisple::Dict::set_property(adapter, Lisple::RTValue::number(3), updated);

  EXPECT_EQ(values.at(3), 24);
  EXPECT_EQ(*Lisple::Dict::get_property(adapter, Lisple::RTValue::number(3)),
            *Lisple::RTValue::number(24));
}
