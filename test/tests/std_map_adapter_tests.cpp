
#include <lisple/adapter.h>
#include <lisple/impl.h>
#include <lisple/runtime.h>

#include "host/test_adapters/vehicle_host_adapters.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

/*
 * ===================================
 * std::map<int, string>
 * ===================================
 */

TEST(StdMapAdapter_int_string, get_sptr_property)
{
  // Given
  std::map<int, std::string> std_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  Lisple::StdMapAdapter<int, std::string> adapter(std_map);

  // Then
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(1)), Lisple::String("one"));
  EXPECT_EQ(Lisple::str_val(*adapter.get_sptr_property(*Lisple::Number::make(1))), "one");
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(2)), Lisple::String("two"));
  EXPECT_EQ(Lisple::str_val(*adapter.get_sptr_property(*Lisple::Number::make(2))), "two");
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(3)), Lisple::String("three"));
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(4)), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(5)), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(0)), *Lisple::NIL);

  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::String::make("one")), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::NIL), *Lisple::NIL);
}

TEST(StdMapAdapter_int_string, set_property)
{
  // Given
  std::map<int, std::string> std_map = {};

  Lisple::StdMapAdapter<int, std::string> adapter(std_map);

  // When
  Lisple::sptr_sobject value = Lisple::String::make("five");
  adapter.set_property(*Lisple::Number::make(5), value);

  // Then
  ASSERT_EQ(std_map.count(5), 1);
  ASSERT_EQ(std_map.at(5), "five");
}

TEST(StdMapAdapter_int_string, keys)
{
  // Given
  std::map<int, std::string> std_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  Lisple::StdMapAdapter<int, std::string> adapter(std_map);

  // Then
  EXPECT_THAT(adapter.keys(),
              (Lisple::sptr_sobject_v{Lisple::Number::make(1),
                                      Lisple::Number::make(2),
                                      Lisple::Number::make(3)}));
}

TEST(StdMapAdapter_int_string, to_string)
{
  // Given
  std::map<int, std::string> std_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  Lisple::StdMapAdapter<int, std::string> adapter(std_map);

  // Then
  EXPECT_THAT(adapter.to_string(), R"({1 "one" 2 "two" 3 "three"})");
}

TEST(StdMapAdapter_int_string, script_usage)
{
  // Given
  std::map<int, std::string> std_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  std::shared_ptr<Lisple::StdMapAdapter<int, std::string>> adapter =
    std::make_shared<Lisple::StdMapAdapter<int, std::string>>(std_map);

  Lisple::Runtime runtime;
  runtime.get_current_namespace().store(Lisple::Word("my-map"), adapter);

  // Then
  EXPECT_EQ(runtime.eval("my-map"), adapter);

  EXPECT_EQ(*runtime.eval("(str my-map)"),
            *Lisple::String::make(R"({1 "one" 2 "two" 3 "three"})"))
    << "Why though?";
  EXPECT_EQ(runtime.eval("(str my-map)")->to_string(), R"("{1 "one" 2 "two" 3 "three"}")");

  /* Get keys */
  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Lisple::String::make("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Lisple::String::make("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Lisple::String::make("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Lisple::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map \"SEMPRINI!\")"), *Lisple::NIL);

  /* assoc and count */
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::Number::make(3));
  runtime.eval(R"((def updated-map (assoc my-map 4 "four")))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Lisple::Number::make(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::Number::make(3));

  /* assoc! and count */
  runtime.eval(R"((assoc! my-map 8 "eight"))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"),
            *Lisple::Number::make(4)); // Map counts both keys and values for now
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::Number::make(4));
  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Lisple::String::make("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Lisple::String::make("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Lisple::String::make("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Lisple::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map 8)"), *Lisple::String::make("eight"));
}

/*
 * ===================================
 * std::map<uint8_t, short>
 * ===================================
 */

TEST(StdMapAdapter_uint8_short, get_sptr_property)
{
  // Given
  std::map<uint8_t, short> std_map = {{0x01, 8}, {0x02, 16}, {0x03, 24}};

  Lisple::StdMapAdapter<uint8_t, short> adapter(std_map);

  // Then
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(1)), *Lisple::Number::make(8));
  EXPECT_EQ(Lisple::str_val(*adapter.get_sptr_property(*Lisple::Number::make(1))), "8");
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(2)), *Lisple::Number::make(16));
  EXPECT_EQ(Lisple::str_val(*adapter.get_sptr_property(*Lisple::Number::make(2))), "16");
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(3)), *Lisple::Number::make(24));
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(4)), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(5)), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(0)), *Lisple::NIL);

  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::String::make("8")), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::NIL), *Lisple::NIL);
}

TEST(StdMapAdapter_uint8_short, make_ref)
{
  // Given
  std::map<uint8_t, short> std_map = {{0x01, 8}, {0x02, 16}, {0x03, 24}};

  // When
  auto adapter = Lisple::StdMapUint8ToShort::make_ref(std_map);

  // Then
  EXPECT_EQ(*adapter->get_sptr_property(*Lisple::Number::make(1)), *Lisple::Number::make(8));
}

/*
 * ===================================
 * std::map<RegNumber, Vehicle>
 * ===================================
 */
TEST(StdMapAdapter_RegNumber_Vehicle, get_sptr_property)
{
  // Given
  std::map<LispleTest::RegNumber, LispleTest::Vehicle> std_map = {
    {LispleTest::RegNumber("ABC", "123"), LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {LispleTest::RegNumber("SMK", "847"), LispleTest::Vehicle("Snail", 5)},
    {LispleTest::RegNumber("EMP", "443"), LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<LispleTest::RegNumber,
                        LispleTest::Vehicle,
                        LispleTest::RegNumberAdapter,
                        LispleTest::VehicleAdapter>
    adapter(std_map);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(
    *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"));
  EXPECT_EQ(*snail, *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Snail", 5));
  EXPECT_EQ(snail->to_string(), R"({:model-name "Snail" :seats 5})");
}

TEST(StdMapAdapter_RegNumber_Vehicle, set_property)
{
  // Given
  std::map<LispleTest::RegNumber, LispleTest::Vehicle> std_map;

  Lisple::StdMapAdapter<LispleTest::RegNumber,
                        LispleTest::Vehicle,
                        LispleTest::RegNumberAdapter,
                        LispleTest::VehicleAdapter>
    adapter(std_map);

  // When
  Lisple::sptr_sobject value =
    LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("The Vroom-Vroom", 2);
  adapter.set_property(
    *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"),
    value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(LispleTest::RegNumber("SMK", "847")), 1);
  ASSERT_EQ(std_map.at(LispleTest::RegNumber("SMK", "847")),
            LispleTest::Vehicle("The Vroom-Vroom", 2));
}

TEST(StdMapAdapter_RegNumber_Vehicle, keys)
{
  // Given
  std::map<LispleTest::RegNumber, LispleTest::Vehicle> std_map = {
    {LispleTest::RegNumber("ABC", "123"), LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {LispleTest::RegNumber("SMK", "847"), LispleTest::Vehicle("Snail", 5)},
    {LispleTest::RegNumber("EMP", "443"), LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<LispleTest::RegNumber,
                        LispleTest::Vehicle,
                        LispleTest::RegNumberAdapter,
                        LispleTest::VehicleAdapter>
    adapter(std_map);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("ABC", "123"));
  EXPECT_EQ(keys[2]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"));
  EXPECT_EQ(keys[1]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("EMP", "443"));
}

TEST(StdMapAdapter_RegNumber_Vehicle, to_string)
{
  // Given
  std::map<LispleTest::RegNumber, LispleTest::Vehicle> std_map = {
    {LispleTest::RegNumber("ABC", "123"), LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {LispleTest::RegNumber("SMK", "847"), LispleTest::Vehicle("Snail", 5)},
    {LispleTest::RegNumber("EMP", "443"), LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<LispleTest::RegNumber,
                        LispleTest::Vehicle,
                        LispleTest::RegNumberAdapter,
                        LispleTest::VehicleAdapter>
    adapter(std_map);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(
    adapter.to_string(),
    std::string(
      R"({{:letters "ABC" :numbers "123"} {:model-name "The Vroom-Vroom" :seats 2} )") +
      std::string(R"({:letters "EMP" :numbers "443"} {:model-name "Comfort" :seats 4} )") +
      std::string(R"({:letters "SMK" :numbers "847"} {:model-name "Snail" :seats 5}})"));
}

/*
 * ===================================
 * std::map<long, Vehicle>
 * ===================================
 */
TEST(StdMapAdapter_long_Vehicle, get_sptr_property)
{
  // Given
  std::map<long, LispleTest::Vehicle> std_map = {
    {1L, LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {2L, LispleTest::Vehicle("Snail", 5)},
    {3L, LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<long, LispleTest::Vehicle, LispleTest::VehicleAdapter> adapter(
    std_map);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(*Lisple::Number::make(2L));
  EXPECT_EQ(*snail, *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Snail", 5));
  EXPECT_EQ(snail->to_string(), R"({:model-name "Snail" :seats 5})");
}

TEST(StdMapAdapter_long_Vehicle, set_property)
{
  // Given
  std::map<long, LispleTest::Vehicle> std_map;

  Lisple::StdMapAdapter<long, LispleTest::Vehicle, LispleTest::VehicleAdapter> adapter(
    std_map);

  // When
  Lisple::sptr_sobject value =
    LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("The Vroom-Vroom", 2);
  adapter.set_property(*Lisple::Number::make(1L), value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(1L), 1);
  ASSERT_EQ(std_map.at(1L), LispleTest::Vehicle("The Vroom-Vroom", 2));
}

TEST(StdMapAdapter_long_Vehicle, keys)
{
  // Given
  std::map<long, LispleTest::Vehicle> std_map = {
    {1L, LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {2L, LispleTest::Vehicle("Snail", 5)},
    {3L, LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<long, LispleTest::Vehicle, LispleTest::VehicleAdapter> adapter(
    std_map);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<Lisple::Number>(), *Lisple::Number::make(1L));
  EXPECT_EQ(keys[1]->as<Lisple::Number>(), *Lisple::Number::make(2L));
  EXPECT_EQ(keys[2]->as<Lisple::Number>(), *Lisple::Number::make(3L));
}

TEST(StdMapAdapter_long_Vehicle, to_string)
{
  // Given
  std::map<long, LispleTest::Vehicle> std_map = {
    {1L, LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {2L, LispleTest::Vehicle("Snail", 5)},
    {3L, LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<long, LispleTest::Vehicle, LispleTest::VehicleAdapter> adapter(
    std_map);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(adapter.to_string(),
            std::string(R"({1 {:model-name "The Vroom-Vroom" :seats 2} )") +
              std::string(R"(2 {:model-name "Snail" :seats 5} )") +
              std::string(R"(3 {:model-name "Comfort" :seats 4}})"));
}

/*
 * ===================================
 * std::map<RegNumber, short>
 * ===================================
 */
TEST(StdMapAdapter_RegNumber_short, get_sptr_property)
{
  // Given
  std::map<LispleTest::RegNumber, short> std_map = {
    {LispleTest::RegNumber("ABC", "123"), 10},
    {LispleTest::RegNumber("SMK", "847"), 32123},
    {LispleTest::RegNumber("EMP", "443"), 193}};

  Lisple::StdMapAdapter<LispleTest::RegNumber, short, LispleTest::RegNumberAdapter> adapter(
    std_map);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(
    *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"));
  EXPECT_EQ(*snail, *Lisple::Number::make(32123));
  EXPECT_EQ(snail->to_string(), "32123");
}

TEST(StdMapAdapter_RegNumber_short, set_property)
{
  // Given
  std::map<LispleTest::RegNumber, short> std_map;

  Lisple::StdMapAdapter<LispleTest::RegNumber, short, LispleTest::RegNumberAdapter> adapter(
    std_map);

  // When
  Lisple::sptr_sobject value = Lisple::Number::make(static_cast<short>(32123));
  adapter.set_property(
    *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"),
    value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(LispleTest::RegNumber("SMK", "847")), 1);
  ASSERT_EQ(std_map.at(LispleTest::RegNumber("SMK", "847")), static_cast<short>(32123));
}

TEST(StdMapAdapter_RegNumber_short, keys)
{
  // Given
  std::map<LispleTest::RegNumber, short> std_map = {
    {LispleTest::RegNumber("ABC", "123"), 10},
    {LispleTest::RegNumber("SMK", "847"), 32123},
    {LispleTest::RegNumber("EMP", "443"), 193}};

  Lisple::StdMapAdapter<LispleTest::RegNumber, short, LispleTest::RegNumberAdapter> adapter(
    std_map);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("ABC", "123"));
  EXPECT_EQ(keys[2]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"));
  EXPECT_EQ(keys[1]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("EMP", "443"));
}

TEST(StdMapAdapter_RegNumber_short, to_string)
{
  // Given
  std::map<LispleTest::RegNumber, short> std_map = {
    {LispleTest::RegNumber("ABC", "123"), 10},
    {LispleTest::RegNumber("SMK", "847"), 32123},
    {LispleTest::RegNumber("EMP", "443"), 193}};

  Lisple::StdMapAdapter<LispleTest::RegNumber, short, LispleTest::RegNumberAdapter> adapter(
    std_map);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(adapter.to_string(),
            std::string(R"({{:letters "ABC" :numbers "123"} 10 )") +
              std::string(R"({:letters "EMP" :numbers "443"} 193 )") +
              std::string(R"({:letters "SMK" :numbers "847"} 32123})"));
}

/*
 * ===================================
 * std::map<int, const string>
 * ===================================
 */
TEST(StdMapAdapter_int_const_string, get_sptr_property)
{

  // Given
  std::map<int, const std::string> std_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  Lisple::StdMapAdapter<int, const std::string> adapter(std_map);

  // Then
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(1)), Lisple::String("one"));
  EXPECT_EQ(Lisple::str_val(*adapter.get_sptr_property(*Lisple::Number::make(1))), "one");
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(2)), Lisple::String("two"));
  EXPECT_EQ(Lisple::str_val(*adapter.get_sptr_property(*Lisple::Number::make(2))), "two");
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(3)), Lisple::String("three"));
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(4)), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(5)), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::Number::make(0)), *Lisple::NIL);

  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::String::make("one")), *Lisple::NIL);
  EXPECT_EQ(*adapter.get_sptr_property(*Lisple::NIL), *Lisple::NIL);
}

TEST(StdMapAdapter_int_const_string, set_property)
{
  // Given
  std::map<int, const std::string> std_map = {};

  Lisple::StdMapAdapter<int, const std::string> adapter(std_map);

  // When
  Lisple::sptr_sobject value = Lisple::String::make("five");
  adapter.set_property(*Lisple::Number::make(5), value);

  // Then
  ASSERT_EQ(std_map.count(5), 1);
  ASSERT_EQ(std_map.at(5), "five");
}

TEST(StdMapAdapter_int_const_string, keys)
{
  // Given
  std::map<int, const std::string> std_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  Lisple::StdMapAdapter<int, const std::string> adapter(std_map);

  // Then
  EXPECT_THAT(adapter.keys(),
              (Lisple::sptr_sobject_v{Lisple::Number::make(1),
                                      Lisple::Number::make(2),
                                      Lisple::Number::make(3)}));
}

TEST(StdMapAdapter_int_const_string, to_string)
{
  // Given
  std::map<int, const std::string> std_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  Lisple::StdMapAdapter<int, const std::string> adapter(std_map);

  // Then
  EXPECT_THAT(adapter.to_string(), R"({1 "one" 2 "two" 3 "three"})");
}

TEST(StdMapAdapter_int_const_string, script_usage)
{
  // Given
  std::map<int, const std::string> std_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  std::shared_ptr<Lisple::StdMapAdapter<int, const std::string>> adapter =
    std::make_shared<Lisple::StdMapAdapter<int, const std::string>>(std_map);

  Lisple::Runtime runtime;
  runtime.get_current_namespace().store("my-map", adapter);

  // Then
  EXPECT_EQ(runtime.eval("my-map"), adapter);

  EXPECT_EQ(*runtime.eval("(str my-map)"),
            *Lisple::String::make(R"({1 "one" 2 "two" 3 "three"})"));

  EXPECT_EQ(runtime.eval("(str my-map)")->to_string(), R"("{1 "one" 2 "two" 3 "three"}")");

  /* Get keys */
  EXPECT_EQ(runtime.eval("(get my-map 1)")->to_string(),
            Lisple::String::make("one")->to_string());
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Lisple::String::make("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Lisple::String::make("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Lisple::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map \"SEMPRINI!\")"), *Lisple::NIL);

  /* assoc and count */
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::Number::make(3));
  runtime.eval(R"((def updated-map (assoc my-map 4 "four")))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Lisple::Number::make(4));
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::Number::make(3));

  /* assoc! and count */
  runtime.eval(R"((assoc! my-map 8 "eight"))");
  EXPECT_EQ(*runtime.eval("(count updated-map)"),
            *Lisple::Number::make(4)); // Map counts both keys and values for now
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::Number::make(4));
  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Lisple::String::make("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Lisple::String::make("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Lisple::String::make("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Lisple::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map 8)"), *Lisple::String::make("eight"));
}

/*
 * ===================================
 * std::map<RegNumber, const int>
 * ===================================
 */
TEST(StdMapAdapter_RegNumber_const_int, get_sptr_property)
{
  // Given
  std::map<LispleTest::RegNumber, const int> std_map = {
    {LispleTest::RegNumber("ABC", "123"), 10},
    {LispleTest::RegNumber("SMK", "847"), 32123},
    {LispleTest::RegNumber("EMP", "443"), 193}};

  Lisple::StdMapAdapter<LispleTest::RegNumber, const int, LispleTest::RegNumberAdapter>
    adapter(std_map);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(
    *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"));
  EXPECT_EQ(*snail, *Lisple::Number::make(32123));
  EXPECT_EQ(snail->to_string(), "32123");
}

TEST(StdMapAdapter_RegNumber_const_int, set_property)
{
  // Given
  std::map<LispleTest::RegNumber, const int> std_map;

  Lisple::StdMapAdapter<LispleTest::RegNumber, const int, LispleTest::RegNumberAdapter>
    adapter(std_map);

  // When
  Lisple::sptr_sobject value = Lisple::Number::make(32123);
  adapter.set_property(
    *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"),
    value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(LispleTest::RegNumber("SMK", "847")), 1);
  ASSERT_EQ(std_map.at(LispleTest::RegNumber("SMK", "847")), 32123);
}

TEST(StdMapAdapter_RegNumber_const_int, keys)
{
  // Given
  std::map<LispleTest::RegNumber, const int> std_map = {
    {LispleTest::RegNumber("ABC", "123"), 10},
    {LispleTest::RegNumber("SMK", "847"), 32123},
    {LispleTest::RegNumber("EMP", "443"), 193}};

  Lisple::StdMapAdapter<LispleTest::RegNumber, const int, LispleTest::RegNumberAdapter>
    adapter(std_map);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("ABC", "123"));
  EXPECT_EQ(keys[2]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"));
  EXPECT_EQ(keys[1]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("EMP", "443"));
}

TEST(StdMapAdapter_RegNumber_const_int, to_string)
{
  // Given
  std::map<LispleTest::RegNumber, const int> std_map = {
    {LispleTest::RegNumber("ABC", "123"), 10},
    {LispleTest::RegNumber("SMK", "847"), 32123},
    {LispleTest::RegNumber("EMP", "443"), 193}};

  Lisple::StdMapAdapter<LispleTest::RegNumber, const int, LispleTest::RegNumberAdapter>
    adapter(std_map);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(adapter.to_string(),
            std::string(R"({{:letters "ABC" :numbers "123"} 10 )") +
              std::string(R"({:letters "EMP" :numbers "443"} 193 )") +
              std::string(R"({:letters "SMK" :numbers "847"} 32123})"));
}

/*
 * ===================================
 * std::map<RegNumber, const Vehicle>
 * ===================================
 */
TEST(StdMapAdapter_RegNumber_const_Vehicle, get_sptr_property)
{
  // Given
  std::map<LispleTest::RegNumber, const LispleTest::Vehicle> std_map = {
    {LispleTest::RegNumber("ABC", "123"), LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {LispleTest::RegNumber("SMK", "847"), LispleTest::Vehicle("Snail", 5)},
    {LispleTest::RegNumber("EMP", "443"), LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<LispleTest::RegNumber,
                        const LispleTest::Vehicle,
                        LispleTest::RegNumberAdapter,
                        LispleTest::VehicleAdapter>
    adapter(std_map);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(
    *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"));
  EXPECT_EQ(*snail, *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Snail", 5));
  EXPECT_EQ(snail->to_string(), R"({:model-name "Snail" :seats 5})");
}

TEST(StdMapAdapter_RegNumber_const_Vehicle, set_property)
{
  // Given
  std::map<LispleTest::RegNumber, const LispleTest::Vehicle> std_map;

  Lisple::StdMapAdapter<LispleTest::RegNumber,
                        const LispleTest::Vehicle,
                        LispleTest::RegNumberAdapter,
                        LispleTest::VehicleAdapter>
    adapter(std_map);

  // When
  Lisple::sptr_sobject value =
    LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("The Vroom-Vroom", 2);
  adapter.set_property(
    *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"),
    value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(LispleTest::RegNumber("SMK", "847")), 1);
  ASSERT_EQ(std_map.at(LispleTest::RegNumber("SMK", "847")),
            LispleTest::Vehicle("The Vroom-Vroom", 2));
}

TEST(StdMapAdapter_RegNumber_const_Vehicle, keys)
{
  // Given
  std::map<LispleTest::RegNumber, const LispleTest::Vehicle> std_map = {
    {LispleTest::RegNumber("ABC", "123"), LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {LispleTest::RegNumber("SMK", "847"), LispleTest::Vehicle("Snail", 5)},
    {LispleTest::RegNumber("EMP", "443"), LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<LispleTest::RegNumber,
                        const LispleTest::Vehicle,
                        LispleTest::RegNumberAdapter,
                        LispleTest::VehicleAdapter>
    adapter(std_map);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("ABC", "123"));
  EXPECT_EQ(keys[2]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("SMK", "847"));
  EXPECT_EQ(keys[1]->as<LispleTest::RegNumberAdapter>(),
            *LispleTest::RegNumberAdapter::make<LispleTest::RegNumber>("EMP", "443"));
}

TEST(StdMapAdapter_RegNumber_const_Vehicle, to_string)
{
  // Given
  std::map<LispleTest::RegNumber, const LispleTest::Vehicle> std_map = {
    {LispleTest::RegNumber("ABC", "123"), LispleTest::Vehicle("The Vroom-Vroom", 2)},
    {LispleTest::RegNumber("SMK", "847"), LispleTest::Vehicle("Snail", 5)},
    {LispleTest::RegNumber("EMP", "443"), LispleTest::Vehicle("Comfort", 4)}};

  Lisple::StdMapAdapter<LispleTest::RegNumber,
                        const LispleTest::Vehicle,
                        LispleTest::RegNumberAdapter,
                        LispleTest::VehicleAdapter>
    adapter(std_map);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(
    adapter.to_string(),
    std::string(
      R"({{:letters "ABC" :numbers "123"} {:model-name "The Vroom-Vroom" :seats 2} )") +
      std::string(R"({:letters "EMP" :numbers "443"} {:model-name "Comfort" :seats 4} )") +
      std::string(R"({:letters "SMK" :numbers "847"} {:model-name "Snail" :seats 5}})"));
}
