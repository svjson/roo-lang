
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <lisple/runtime.h>
#include <lisple/adapter.h>
#include <lisple/impl.h>

#include "test_host_objects.h"

using namespace ::testing;

template class Lisple::StdMapAdapter<int, std::string>;

/*
 * ===================================
 * std::map<int, string>
 * ===================================
 */

TEST(StdMapAdapter_int_string, get_sptr_property)
{
  // Given
  std::map<int, std::string> std_map = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };

  Lisple::StdMapAdapter<int, std::string> adapter(&Tests::MAP_INT_TO_STRING,
                                                  std_map,
                                                  &Lisple::Type::NUMBER,
                                                  &Lisple::Type::STRING);

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

  Lisple::StdMapAdapter<int, std::string> adapter(&Tests::MAP_INT_TO_STRING,
                                                  std_map,
                                                  &Lisple::Type::NUMBER,
                                                  &Lisple::Type::STRING);

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
  std::map<int, std::string> std_map = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };

  Lisple::StdMapAdapter<int, std::string> adapter(&Tests::MAP_INT_TO_STRING,
                                                  std_map,
                                                  &Lisple::Type::NUMBER,
                                                  &Lisple::Type::STRING);

  // Then
  EXPECT_THAT(adapter.keys(), (Lisple::sptr_sobject_v {
        Lisple::Number::make(1),
        Lisple::Number::make(2),
        Lisple::Number::make(3)}));
}

TEST(StdMapAdapter_int_string, to_string)
{
  // Given
  std::map<int, std::string> std_map = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };

  Lisple::StdMapAdapter<int, std::string> adapter(&Tests::MAP_INT_TO_STRING,
                                                  std_map,
                                                  &Lisple::Type::NUMBER,
                                                  &Lisple::Type::STRING);

  // Then
  EXPECT_THAT(adapter.to_string(), R"({1 "one" 2 "two" 3 "three"})");
}

TEST(StdMapAdapter_int_string, script_usage)
{
  // Given
  std::map<int, std::string> std_map = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };

  std::shared_ptr<Lisple::StdMapAdapter<int, std::string>> adapter =
    std::make_shared<Lisple::StdMapAdapter<int, std::string>>(&Tests::MAP_INT_TO_STRING,
                                                              std_map,
                                                              &Lisple::Type::NUMBER,
                                                              &Lisple::Type::STRING);

  Lisple::Runtime runtime;
  runtime.get_current_namespace().store(Lisple::Word("my-map"), adapter);

  // Then
  EXPECT_EQ(runtime.eval("my-map"), adapter);

  EXPECT_EQ(*runtime.eval("(str my-map)"), *Lisple::String::make(R"({1 "one" 2 "two" 3 "three"})")) << "Why though?";
  EXPECT_EQ(runtime.eval("(str my-map)")->to_string(),
            R"("{1 "one" 2 "two" 3 "three"}")");

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
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Lisple::Number::make(4)); // Map counts both keys and values for now
  EXPECT_EQ(*runtime.eval("(count my-map)"), *Lisple::Number::make(4));
  EXPECT_EQ(*runtime.eval("(get my-map 1)"), *Lisple::String::make("one"));
  EXPECT_EQ(*runtime.eval("(get my-map 2)"), *Lisple::String::make("two"));
  EXPECT_EQ(*runtime.eval("(get my-map 3)"), *Lisple::String::make("three"));
  EXPECT_EQ(*runtime.eval("(get my-map 4)"), *Lisple::NIL);
  EXPECT_EQ(*runtime.eval("(get my-map 8)"), *Lisple::String::make("eight"));
}


/*
 * ===================================
 * std::map<RegNumber, Vehicle>
 * ===================================
 */
TEST(StdMapAdapter_RegNumber_Vehicle, get_sptr_property)
{
  // Given
  std::map<Tests::RegNumber, Tests::Vehicle> std_map = {
    {Tests::RegNumber("ABC", "123"), Tests::Vehicle("The Vroom-Vroom", 2) },
    {Tests::RegNumber("SMK", "847"), Tests::Vehicle("Snail", 5) },
    {Tests::RegNumber("EMP", "443"), Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        Tests::Vehicle,
                        Tests::RegNumberAdapter,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_REGNUM_TO_VEHICLE,
                                                       std_map,
                                                       &Tests::REGNUM_TYPE,
                                                       &Tests::VEHICLE_TYPE);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(*Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"));
  EXPECT_EQ(*snail, *Tests::VehicleAdapter::make<Tests::Vehicle>("Snail", 5));
  EXPECT_EQ(snail->to_string(),
            R"({:model-name "Snail" :seats 5})");
}

TEST(StdMapAdapter_RegNumber_Vehicle, set_property)
{
  // Given
  std::map<Tests::RegNumber, Tests::Vehicle> std_map;

  Lisple::StdMapAdapter<Tests::RegNumber,
                        Tests::Vehicle,
                        Tests::RegNumberAdapter,
                        Tests::VehicleAdapter>
    adapter(&Tests::MAP_REGNUM_TO_VEHICLE,
            std_map,
            &Tests::REGNUM_TYPE,
            &Tests::VEHICLE_TYPE);

  // When
  Lisple::sptr_sobject value = Tests::VehicleAdapter::make<Tests::Vehicle>("The Vroom-Vroom", 2);
  adapter.set_property(*Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"), value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(Tests::RegNumber("SMK", "847")), 1);
  ASSERT_EQ(std_map.at(Tests::RegNumber("SMK", "847")), Tests::Vehicle("The Vroom-Vroom", 2));
}

TEST(StdMapAdapter_RegNumber_Vehicle, keys)
{
  // Given
  std::map<Tests::RegNumber, Tests::Vehicle> std_map = {
    {Tests::RegNumber("ABC", "123"), Tests::Vehicle("The Vroom-Vroom", 2) },
    {Tests::RegNumber("SMK", "847"), Tests::Vehicle("Snail", 5) },
    {Tests::RegNumber("EMP", "443"), Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        Tests::Vehicle,
                        Tests::RegNumberAdapter,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_REGNUM_TO_VEHICLE,
                                                       std_map,
                                                       &Tests::REGNUM_TYPE,
                                                       &Tests::VEHICLE_TYPE);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("ABC", "123"));
  EXPECT_EQ(keys[2]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"));
  EXPECT_EQ(keys[1]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("EMP", "443"));
}

TEST(StdMapAdapter_RegNumber_Vehicle, to_string)
{
  // Given
  std::map<Tests::RegNumber, Tests::Vehicle> std_map = {
    {Tests::RegNumber("ABC", "123"), Tests::Vehicle("The Vroom-Vroom", 2) },
    {Tests::RegNumber("SMK", "847"), Tests::Vehicle("Snail", 5) },
    {Tests::RegNumber("EMP", "443"), Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        Tests::Vehicle,
                        Tests::RegNumberAdapter,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_REGNUM_TO_VEHICLE,
                                                       std_map,
                                                       &Tests::REGNUM_TYPE,
                                                       &Tests::VEHICLE_TYPE);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(adapter.to_string(),
            std::string(R"({{:letters "ABC" :numbers "123"} {:model-name "The Vroom-Vroom" :seats 2} )") +
            std::string( R"({:letters "EMP" :numbers "443"} {:model-name "Comfort" :seats 4} )") +
            std::string( R"({:letters "SMK" :numbers "847"} {:model-name "Snail" :seats 5}})"));
}


/*
 * ===================================
 * std::map<long, Vehicle>
 * ===================================
 */
TEST(StdMapAdapter_long_Vehicle, get_sptr_property)
{
  // Given
  std::map<long, Tests::Vehicle> std_map = {
    {1L, Tests::Vehicle("The Vroom-Vroom", 2) },
    {2L, Tests::Vehicle("Snail", 5) },
    {3L, Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<long,
                        Tests::Vehicle,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_LONG_TO_VEHICLE,
                                                       std_map,
                                                       &Lisple::Type::NUMBER,
                                                       &Tests::VEHICLE_TYPE);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(*Lisple::Number::make(2L));
  EXPECT_EQ(*snail, *Tests::VehicleAdapter::make<Tests::Vehicle>("Snail", 5));
  EXPECT_EQ(snail->to_string(),
            R"({:model-name "Snail" :seats 5})");
}

TEST(StdMapAdapter_long_Vehicle, set_property)
{
  // Given
  std::map<long, Tests::Vehicle> std_map;

  Lisple::StdMapAdapter<long,
                        Tests::Vehicle,
                        Tests::VehicleAdapter>
    adapter(&Tests::MAP_REGNUM_TO_VEHICLE,
            std_map,
            &Lisple::Type::NUMBER,
            &Tests::VEHICLE_TYPE);

  // When
  Lisple::sptr_sobject value = Tests::VehicleAdapter::make<Tests::Vehicle>("The Vroom-Vroom", 2);
  adapter.set_property(*Lisple::Number::make(1L), value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(1L), 1);
  ASSERT_EQ(std_map.at(1L), Tests::Vehicle("The Vroom-Vroom", 2));
}

TEST(StdMapAdapter_long_Vehicle, keys)
{
  // Given
  std::map<long, Tests::Vehicle> std_map = {
    {1L, Tests::Vehicle("The Vroom-Vroom", 2) },
    {2L, Tests::Vehicle("Snail", 5) },
    {3L, Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<long,
                        Tests::Vehicle,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_LONG_TO_VEHICLE,
                                                       std_map,
                                                       &Lisple::Type::NUMBER,
                                                       &Tests::VEHICLE_TYPE);

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
  std::map<long, Tests::Vehicle> std_map = {
    {1L, Tests::Vehicle("The Vroom-Vroom", 2) },
    {2L, Tests::Vehicle("Snail", 5) },
    {3L, Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<long,
                        Tests::Vehicle,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_LONG_TO_VEHICLE,
                                                       std_map,
                                                       &Lisple::Type::NUMBER,
                                                       &Tests::VEHICLE_TYPE);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(adapter.to_string(),
            std::string(R"({1 {:model-name "The Vroom-Vroom" :seats 2} )") +
            std::string( R"(2 {:model-name "Snail" :seats 5} )") +
            std::string( R"(3 {:model-name "Comfort" :seats 4}})"));
}


/*
 * ===================================
 * std::map<RegNumber, short>
 * ===================================
 */
TEST(StdMapAdapter_RegNumber_short, get_sptr_property)
{
  // Given
  std::map<Tests::RegNumber, short> std_map = {
    {Tests::RegNumber("ABC", "123"), 10 },
    {Tests::RegNumber("SMK", "847"), 32123 },
    {Tests::RegNumber("EMP", "443"), 193 }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        short,
                        Tests::RegNumberAdapter> adapter(&Tests::MAP_REGNUM_TO_SHORT,
                                                         std_map,
                                                         &Tests::REGNUM_TYPE,
                                                         &Lisple::Type::NUMBER);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(*Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"));
  EXPECT_EQ(*snail, *Lisple::Number::make(32123));
  EXPECT_EQ(snail->to_string(),
            "32123");
}

TEST(StdMapAdapter_RegNumber_short, set_property)
{
  // Given
  std::map<Tests::RegNumber, short> std_map;

  Lisple::StdMapAdapter<Tests::RegNumber,
                        short,
                        Tests::RegNumberAdapter>
    adapter(&Tests::MAP_REGNUM_TO_SHORT,
            std_map,
            &Tests::REGNUM_TYPE,
            &Lisple::Type::NUMBER);

  // When
  Lisple::sptr_sobject value = Lisple::Number::make(static_cast<short>(32123));
  adapter.set_property(*Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"), value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(Tests::RegNumber("SMK", "847")), 1);
  ASSERT_EQ(std_map.at(Tests::RegNumber("SMK", "847")), static_cast<short>(32123));
}

TEST(StdMapAdapter_RegNumber_short, keys)
{
  // Given
  std::map<Tests::RegNumber, short> std_map = {
    {Tests::RegNumber("ABC", "123"), 10 },
    {Tests::RegNumber("SMK", "847"), 32123 },
    {Tests::RegNumber("EMP", "443"), 193 }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        short,
                        Tests::RegNumberAdapter> adapter(&Tests::MAP_REGNUM_TO_SHORT,
                                                         std_map,
                                                         &Tests::REGNUM_TYPE,
                                                         &Lisple::Type::NUMBER);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("ABC", "123"));
  EXPECT_EQ(keys[2]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"));
  EXPECT_EQ(keys[1]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("EMP", "443"));
}

TEST(StdMapAdapter_RegNumber_short, to_string)
{
  // Given
  std::map<Tests::RegNumber, short> std_map = {
    {Tests::RegNumber("ABC", "123"), 10 },
    {Tests::RegNumber("SMK", "847"), 32123 },
    {Tests::RegNumber("EMP", "443"), 193 }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        short,
                        Tests::RegNumberAdapter> adapter(&Tests::MAP_REGNUM_TO_SHORT,
                                                         std_map,
                                                         &Tests::REGNUM_TYPE,
                                                         &Lisple::Type::NUMBER);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(adapter.to_string(),
            std::string(R"({{:letters "ABC" :numbers "123"} 10 )") +
            std::string( R"({:letters "EMP" :numbers "443"} 193 )") +
            std::string( R"({:letters "SMK" :numbers "847"} 32123})"));
}


/*
 * ===================================
 * std::map<int, const string>
 * ===================================
 */
TEST(StdMapAdapter_int_const_string, get_sptr_property)
{


  // Given
  std::map<int, const std::string> std_map = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };

  Lisple::StdMapAdapter<int, const std::string> adapter(&Tests::MAP_INT_TO_CONST_STRING,
                                                        std_map,
                                                        &Lisple::Type::NUMBER,
                                                        &Lisple::Type::STRING);

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

  Lisple::StdMapAdapter<int, const std::string> adapter(&Tests::MAP_INT_TO_CONST_STRING,
                                                        std_map,
                                                        &Lisple::Type::NUMBER,
                                                        &Lisple::Type::STRING);

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
  std::map<int, const std::string> std_map = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };

  Lisple::StdMapAdapter<int, const std::string> adapter(&Tests::MAP_INT_TO_CONST_STRING,
                                                        std_map,
                                                        &Lisple::Type::NUMBER,
                                                        &Lisple::Type::STRING);

  // Then
  EXPECT_THAT(adapter.keys(), (Lisple::sptr_sobject_v {
        Lisple::Number::make(1),
        Lisple::Number::make(2),
        Lisple::Number::make(3)}));
}

TEST(StdMapAdapter_int_const_string, to_string)
{
  // Given
  std::map<int, const std::string> std_map = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };

  Lisple::StdMapAdapter<int, const std::string> adapter(&Tests::MAP_INT_TO_CONST_STRING,
                                                        std_map,
                                                        &Lisple::Type::NUMBER,
                                                        &Lisple::Type::STRING);

  // Then
  EXPECT_THAT(adapter.to_string(), R"({1 "one" 2 "two" 3 "three"})");
}

TEST(StdMapAdapter_int_const_string, script_usage)
{
  // Given
  std::map<int, const std::string> std_map = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };

  std::shared_ptr<Lisple::StdMapAdapter<int, const std::string>> adapter =
    std::make_shared<Lisple::StdMapAdapter<int, const std::string>>(&Tests::MAP_INT_TO_CONST_STRING,
                                                                    std_map,
                                                                    &Lisple::Type::NUMBER,
                                                                    &Lisple::Type::STRING);

  Lisple::Runtime runtime;
  runtime.get_current_namespace().store(Lisple::Word("my-map"), adapter);

  // Then
  EXPECT_EQ(runtime.eval("my-map"), adapter);

  EXPECT_EQ(*runtime.eval("(str my-map)"), *Lisple::String::make(R"({1 "one" 2 "two" 3 "three"})")) << "Why though?";
  EXPECT_EQ(runtime.eval("(str my-map)")->to_string(),
            R"("{1 "one" 2 "two" 3 "three"}")");

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
  EXPECT_EQ(*runtime.eval("(count updated-map)"), *Lisple::Number::make(4)); // Map counts both keys and values for now
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
  std::map<Tests::RegNumber, const int> std_map = {
    {Tests::RegNumber("ABC", "123"), 10 },
    {Tests::RegNumber("SMK", "847"), 32123 },
    {Tests::RegNumber("EMP", "443"), 193 }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        const int,
                        Tests::RegNumberAdapter> adapter(&Tests::MAP_REGNUM_TO_CONST_INT,
                                                         std_map,
                                                         &Tests::REGNUM_TYPE,
                                                         &Lisple::Type::NUMBER);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(*Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"));
  EXPECT_EQ(*snail, *Lisple::Number::make(32123));
  EXPECT_EQ(snail->to_string(),
            "32123");
}

TEST(StdMapAdapter_RegNumber_const_int, set_property)
{
  // Given
  std::map<Tests::RegNumber, const int> std_map;

  Lisple::StdMapAdapter<Tests::RegNumber,
                        const int,
                        Tests::RegNumberAdapter>
    adapter(&Tests::MAP_REGNUM_TO_CONST_INT,
            std_map,
            &Tests::REGNUM_TYPE,
            &Lisple::Type::NUMBER);

  // When
  Lisple::sptr_sobject value = Lisple::Number::make(32123);
  adapter.set_property(*Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"), value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(Tests::RegNumber("SMK", "847")), 1);
  ASSERT_EQ(std_map.at(Tests::RegNumber("SMK", "847")), 32123);
}

TEST(StdMapAdapter_RegNumber_const_int, keys)
{
  // Given
  std::map<Tests::RegNumber, const int> std_map = {
    {Tests::RegNumber("ABC", "123"), 10 },
    {Tests::RegNumber("SMK", "847"), 32123 },
    {Tests::RegNumber("EMP", "443"), 193 }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        const int,
                        Tests::RegNumberAdapter> adapter(&Tests::MAP_REGNUM_TO_CONST_INT,
                                                         std_map,
                                                         &Tests::REGNUM_TYPE,
                                                         &Lisple::Type::NUMBER);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("ABC", "123"));
  EXPECT_EQ(keys[2]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"));
  EXPECT_EQ(keys[1]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("EMP", "443"));
}

TEST(StdMapAdapter_RegNumber_const_int, to_string)
{
  // Given
  std::map<Tests::RegNumber, const int> std_map = {
    {Tests::RegNumber("ABC", "123"), 10 },
    {Tests::RegNumber("SMK", "847"), 32123 },
    {Tests::RegNumber("EMP", "443"), 193 }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        const int,
                        Tests::RegNumberAdapter> adapter(&Tests::MAP_REGNUM_TO_CONST_INT,
                                                         std_map,
                                                         &Tests::REGNUM_TYPE,
                                                         &Lisple::Type::NUMBER);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(adapter.to_string(),
            std::string(R"({{:letters "ABC" :numbers "123"} 10 )") +
            std::string( R"({:letters "EMP" :numbers "443"} 193 )") +
            std::string( R"({:letters "SMK" :numbers "847"} 32123})"));
}

/*
 * ===================================
 * std::map<RegNumber, const Vehicle>
 * ===================================
 */
TEST(StdMapAdapter_RegNumber_const_Vehicle, get_sptr_property)
{
  // Given
  std::map<Tests::RegNumber, const Tests::Vehicle> std_map = {
    {Tests::RegNumber("ABC", "123"), Tests::Vehicle("The Vroom-Vroom", 2) },
    {Tests::RegNumber("SMK", "847"), Tests::Vehicle("Snail", 5) },
    {Tests::RegNumber("EMP", "443"), Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        const Tests::Vehicle,
                        Tests::RegNumberAdapter,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_REGNUM_TO_CONST_VEHICLE,
                                                       std_map,
                                                       &Tests::REGNUM_TYPE,
                                                       &Tests::VEHICLE_TYPE);

  // Then
  Lisple::sptr_sobject snail = adapter.get_sptr_property(*Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"));
  EXPECT_EQ(*snail, *Tests::VehicleAdapter::make<Tests::Vehicle>("Snail", 5));
  EXPECT_EQ(snail->to_string(),
            R"({:model-name "Snail" :seats 5})");
}

TEST(StdMapAdapter_RegNumber_const_Vehicle, set_property)
{
  // Given
  std::map<Tests::RegNumber, const Tests::Vehicle> std_map;

  Lisple::StdMapAdapter<Tests::RegNumber,
                        const Tests::Vehicle,
                        Tests::RegNumberAdapter,
                        Tests::VehicleAdapter>
    adapter(&Tests::MAP_REGNUM_TO_CONST_VEHICLE,
            std_map,
            &Tests::REGNUM_TYPE,
            &Tests::VEHICLE_TYPE);

  // When
  Lisple::sptr_sobject value = Tests::VehicleAdapter::make<Tests::Vehicle>("The Vroom-Vroom", 2);
  adapter.set_property(*Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"), value);

  // Then
  ASSERT_EQ(std_map.size(), 1);
  ASSERT_EQ(std_map.count(Tests::RegNumber("SMK", "847")), 1);
  ASSERT_EQ(std_map.at(Tests::RegNumber("SMK", "847")), Tests::Vehicle("The Vroom-Vroom", 2));
}

TEST(StdMapAdapter_RegNumber_const_Vehicle, keys)
{
  // Given
  std::map<Tests::RegNumber, const Tests::Vehicle> std_map = {
    {Tests::RegNumber("ABC", "123"), Tests::Vehicle("The Vroom-Vroom", 2) },
    {Tests::RegNumber("SMK", "847"), Tests::Vehicle("Snail", 5) },
    {Tests::RegNumber("EMP", "443"), Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        const Tests::Vehicle,
                        Tests::RegNumberAdapter,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_REGNUM_TO_CONST_VEHICLE,
                                                       std_map,
                                                       &Tests::REGNUM_TYPE,
                                                       &Tests::VEHICLE_TYPE);

  Lisple::sptr_sobject_v keys = adapter.keys();

  // Then

  ASSERT_EQ(keys.size(), 3);

  // FIXME: This depends on the order of keys from std::map
  EXPECT_EQ(keys[0]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("ABC", "123"));
  EXPECT_EQ(keys[2]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("SMK", "847"));
  EXPECT_EQ(keys[1]->as<Tests::RegNumberAdapter>(), *Tests::RegNumberAdapter::make<Tests::RegNumber>("EMP", "443"));
}

TEST(StdMapAdapter_RegNumber_const_Vehicle, to_string)
{
  // Given
  std::map<Tests::RegNumber, const Tests::Vehicle> std_map = {
    {Tests::RegNumber("ABC", "123"), Tests::Vehicle("The Vroom-Vroom", 2) },
    {Tests::RegNumber("SMK", "847"), Tests::Vehicle("Snail", 5) },
    {Tests::RegNumber("EMP", "443"), Tests::Vehicle("Comfort", 4) }
  };

  Lisple::StdMapAdapter<Tests::RegNumber,
                        const Tests::Vehicle,
                        Tests::RegNumberAdapter,
                        Tests::VehicleAdapter> adapter(&Tests::MAP_REGNUM_TO_CONST_VEHICLE,
                                                       std_map,
                                                       &Tests::REGNUM_TYPE,
                                                       &Tests::VEHICLE_TYPE);

  // Then

  // FIXME: This depends on the order of keys from std::map
  ASSERT_EQ(adapter.to_string(),
            std::string(R"({{:letters "ABC" :numbers "123"} {:model-name "The Vroom-Vroom" :seats 2} )") +
            std::string( R"({:letters "EMP" :numbers "443"} {:model-name "Comfort" :seats 4} )") +
            std::string( R"({:letters "SMK" :numbers "847"} {:model-name "Snail" :seats 5}})"));
}
