#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "runtime_fixture.h"
#include <gtest/gtest.h>
#include <roo/adapter.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/seq.h>
#include <roo/runtime/value.h>

using NativeStdVectorAdapter_int = RooTest::RuntimeTestFixture;
using NativeStdMapAdapter_int_string = RooTest::RuntimeTestFixture;
using NativeStdMapAdapter_int_const_string = RooTest::RuntimeTestFixture;
using NativeStdMapAdapter_uint8_short = RooTest::RuntimeTestFixture;
TEST_F(NativeStdVectorAdapter_int, get_set_children_and_count)
{
  std::vector<int> values = {1, 2, 3};
  Roo::sptr_val adapter = Roo::NativeStdVectorAdapter<int>::make_ref(values);

  EXPECT_EQ(Roo::count(*adapter), 3);
  EXPECT_EQ(*Roo::get_child(*adapter, 1), *Roo::Value::number(2));

  Roo::sptr_val updated = Roo::Value::number(8);
  Roo::Dict::set_property(adapter, Roo::Value::number(1), updated);

  EXPECT_EQ(values.at(1), 8);
  EXPECT_EQ(*Roo::get_child(*adapter, 1), *Roo::Value::number(8));
  EXPECT_EQ(adapter->nobj()->to_string(), "[1 8 3]");
}

TEST_F(NativeStdVectorAdapter_int, for_iterates_native_vector)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  auto result = runtime.eval("(for [value values] (* value 2))");

  EXPECT_EQ(result->to_string(), "[2 4 6]");
}

TEST_F(NativeStdMapAdapter_int_string, get_set_keys_and_count)
{
  std::map<int, std::string> values = {{1, "one"}, {2, "two"}};
  Roo::sptr_val adapter = Roo::NativeStdMapAdapter<int, std::string>::make_ref(values);

  EXPECT_EQ(Roo::count(*adapter), 2);
  EXPECT_EQ(*Roo::Dict::get_property(adapter, Roo::Value::number(1)),
            *Roo::Value::string("one"));

  Roo::sptr_val updated = Roo::Value::string("three");
  Roo::Dict::set_property(adapter, Roo::Value::number(3), updated);

  EXPECT_EQ(values.at(3), "three");
  EXPECT_EQ(*Roo::Dict::get_property(adapter, Roo::Value::number(3)),
            *Roo::Value::string("three"));
  EXPECT_EQ(Roo::Dict::map_sptr_keys(adapter).size(), 3);
  EXPECT_EQ(adapter->nobj()->to_string(), R"({1 "one" 2 "two" 3 "three"})");
}

TEST_F(NativeStdMapAdapter_int_string, script_usage)
{
  std::map<int, std::string> values = {{1, "one"}, {2, "two"}, {3, "three"}};

  runtime.get_current_namespace().store(
    "my-map",
    Roo::NativeStdMapAdapter<int, std::string>::make_ref(values));

  EXPECT_EQ(runtime.eval("my-map")->type, Roo::Value::Type::NATIVE_OBJECT);
  EXPECT_EQ(*runtime.eval("(str my-map)"),
            *Roo::Value::string(R"({1 "one" 2 "two" 3 "three"})"));

  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Roo::Value::string("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Roo::Value::string("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Roo::Value::string("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map \"SEMPRINI!\")"), *Roo::Constant::NIL);

  EXPECT_EQ(*runtime.eval("(count my-map)"), *Roo::Value::number(3));
  runtime.eval(R"((def updated-map (assoc my-map 4 "four")))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Roo::Value::number(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Roo::Value::number(3));

  runtime.eval(R"((assoc! my-map 8 "eight"))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Roo::Value::number(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Roo::Value::number(4));
  EXPECT_EQ(*runtime.eval("(get my-map 8)"), *Roo::Value::string("eight"));
}

TEST_F(NativeStdMapAdapter_int_const_string, script_usage)
{
  std::map<int, const std::string> values = {{1, "one"}, {2, "two"}, {3, "three"}};

  runtime.get_current_namespace().store(
    "my-map",
    Roo::NativeStdMapAdapter<int, const std::string>::make_ref(values));

  EXPECT_EQ(runtime.eval("my-map")->type, Roo::Value::Type::NATIVE_OBJECT);
  EXPECT_EQ(*runtime.eval("(str my-map)"),
            *Roo::Value::string(R"({1 "one" 2 "two" 3 "three"})"));

  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Roo::Value::string("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Roo::Value::string("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Roo::Value::string("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map \"SEMPRINI!\")"), *Roo::Constant::NIL);

  EXPECT_EQ(*runtime.eval("(count my-map)"), *Roo::Value::number(3));
  runtime.eval(R"((def updated-map (assoc my-map 4 "four")))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Roo::Value::number(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Roo::Value::number(3));

  runtime.eval(R"((assoc! my-map 8 "eight"))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Roo::Value::number(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Roo::Value::number(4));
  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Roo::Value::string("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Roo::Value::string("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Roo::Value::string("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map 8)"), *Roo::Value::string("eight"));
}

TEST_F(NativeStdMapAdapter_uint8_short, get_set_numeric_map)
{
  std::map<uint8_t, short> values = {{1, 8}, {2, 16}};
  Roo::sptr_val adapter = Roo::NativeStdMapAdapter<uint8_t, short>::make_ref(values);

  EXPECT_EQ(*Roo::Dict::get_property(adapter, Roo::Value::number(2)),
            *Roo::Value::number(16));

  Roo::sptr_val updated = Roo::Value::number(24);
  Roo::Dict::set_property(adapter, Roo::Value::number(3), updated);

  EXPECT_EQ(values.at(3), 24);
  EXPECT_EQ(*Roo::Dict::get_property(adapter, Roo::Value::number(3)),
            *Roo::Value::number(24));
}
