
#include <lisple/adapter.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

#include "host/test_adapters/vehicle_host_adapters.h"

/*
 * ===================================
 * std::vector<int>
 * ===================================
 */

TEST(StdVectorAdapter_int, size)
{
  // Given
  std::vector<int> int_v{100, 0, 90, 10, 80, 20};

  Lisple::StdVectorAdapter<int> adapter(int_v);

  // Then
  EXPECT_EQ(adapter.size(), 6);
}

TEST(StdVectorAdapter_int, get_children)
{
  // Given
  std::vector<int> int_v{100, 0, 90, 10, 80, 20};

  Lisple::StdVectorAdapter<int> adapter(int_v);

  // Then
  EXPECT_THAT(adapter.get_children(),
              ElementsAre(Lisple::Number::make(100),
                          Lisple::Number::make(0),
                          Lisple::Number::make(90),
                          Lisple::Number::make(10),
                          Lisple::Number::make(80),
                          Lisple::Number::make(20)));
}

TEST(StdVectorAdapter_int, to_string)
{
  // Given
  std::vector<int> int_v{100, 0, 90, 10, 80, 20};

  Lisple::StdVectorAdapter<int> adapter(int_v);

  // Then
  EXPECT_EQ(adapter.to_string(), "[100 0 90 10 80 20]");
}

TEST(StdVectorAdapter_int, append)
{
  // Given
  std::vector<int> int_v{100, 0, 90, 10, 80, 20};

  Lisple::StdVectorAdapter<int> adapter(int_v);

  // When
  adapter.append(Lisple::Number::make(70));

  // Then
  EXPECT_THAT(int_v, ElementsAre(100, 0, 90, 10, 80, 20, 70));

  EXPECT_THAT(adapter.get_children(),
              ElementsAre(Lisple::Number::make(100),
                          Lisple::Number::make(0),
                          Lisple::Number::make(90),
                          Lisple::Number::make(10),
                          Lisple::Number::make(80),
                          Lisple::Number::make(20),
                          Lisple::Number::make(70)));
  EXPECT_EQ(adapter.to_string(), "[100 0 90 10 80 20 70]");
}

/*
 * ===================================
 * std::vector<std::string>
 * ===================================
 */

TEST(StdVectorAdapter_string, size)
{
  // Given
  std::vector<std::string> string_v{"The", "Quick", "Brown", "Fox", "Ate", "A"};

  Lisple::StdVectorAdapter<std::string> adapter(string_v);

  // Then
  EXPECT_EQ(adapter.size(), 6);
}

TEST(StdVectorAdapter_string, get_children)
{
  // Given
  std::vector<std::string> string_v{"The", "Quick", "Brown", "Fox", "Ate", "A"};

  Lisple::StdVectorAdapter<std::string> adapter(string_v);

  // Then
  EXPECT_EQ(adapter.size(), 6);
  EXPECT_EQ(*adapter.get_children()[0], Lisple::String("The"));
  EXPECT_EQ(*adapter.get_children()[1], Lisple::String("Quick"));
  EXPECT_EQ(*adapter.get_children()[2], Lisple::String("Brown"));
  EXPECT_EQ(*adapter.get_children()[3], Lisple::String("Fox"));
  EXPECT_EQ(*adapter.get_children()[4], Lisple::String("Ate"));
  EXPECT_EQ(*adapter.get_children()[5], Lisple::String("A"));
}

TEST(StdVectorAdapter_string, to_string)
{
  // Given
  std::vector<std::string>
    string_v{"The", "Quick", "Brown", "Fox", "Ate", "A", "Cheese Muffin"};

  Lisple::StdVectorAdapter<std::string> adapter(string_v);

  // Then
  EXPECT_EQ(adapter.to_string(),
            R"(["The" "Quick" "Brown" "Fox" "Ate" "A" "Cheese Muffin"])");
}

TEST(StdVectorAdapter_string, append)
{
  // Given
  std::vector<std::string> string_v{"The", "Quick", "Brown", "Fox", "Ate", "A"};

  Lisple::StdVectorAdapter<std::string> adapter(string_v);

  // When
  adapter.append(Lisple::String::make("Horse"));

  // Then
  EXPECT_THAT(string_v, ElementsAre("The", "Quick", "Brown", "Fox", "Ate", "A", "Horse"));

  EXPECT_EQ(adapter.size(), 7);
  EXPECT_EQ(*adapter.get_children()[0], Lisple::String("The"));
  EXPECT_EQ(*adapter.get_children()[1], Lisple::String("Quick"));
  EXPECT_EQ(*adapter.get_children()[2], Lisple::String("Brown"));
  EXPECT_EQ(*adapter.get_children()[3], Lisple::String("Fox"));
  EXPECT_EQ(*adapter.get_children()[4], Lisple::String("Ate"));
  EXPECT_EQ(*adapter.get_children()[5], Lisple::String("A"));
  EXPECT_EQ(*adapter.get_children()[6], Lisple::String("Horse"));
}

/*
 * ===================================
 * std::vector<Vehicle>
 * ===================================
 */

TEST(StdVectorAdapter_Vehicle, size)
{
  // Given
  std::vector<LispleTest::Vehicle> obj_v{LispleTest::Vehicle("Dreamy Boom-Boom", 4),
                                         LispleTest::Vehicle("Flying Saucer", 12),
                                         LispleTest::Vehicle("Something Fast", 2)};

  Lisple::StdVectorAdapter<LispleTest::Vehicle, LispleTest::VehicleAdapter> adapter(obj_v);

  // Then
  EXPECT_EQ(adapter.size(), 3);
}

TEST(StdVectorAdapter_Vehicle, get_children)
{
  // Given
  std::vector<LispleTest::Vehicle> obj_v{LispleTest::Vehicle("Dreamy Boom-Boom", 4),
                                         LispleTest::Vehicle("Flying Saucer", 12),
                                         LispleTest::Vehicle("Something Fast", 2)};

  Lisple::StdVectorAdapter<LispleTest::Vehicle, LispleTest::VehicleAdapter> adapter(obj_v);

  // Then
  EXPECT_EQ(adapter.size(), 3);
  EXPECT_EQ(*adapter.get_children()[0],
            *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Dreamy Boom-Boom", 4));
  EXPECT_EQ(*adapter.get_children()[1],
            *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Flying Saucer", 12));
  EXPECT_EQ(*adapter.get_children()[2],
            *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Something Fast", 2));
}

TEST(StdVectorAdapter_Vehicle, to_string)
{
  // Given
  std::vector<LispleTest::Vehicle> obj_v{LispleTest::Vehicle("Dreamy Boom-Boom", 4),
                                         LispleTest::Vehicle("Flying Saucer", 12),
                                         LispleTest::Vehicle("Something Fast", 2)};

  Lisple::StdVectorAdapter<LispleTest::Vehicle, LispleTest::VehicleAdapter> adapter(obj_v);

  // Then
  EXPECT_EQ(adapter.to_string(),
            "[{:model-name \"Dreamy Boom-Boom\" :seats 4} {:model-name \"Flying Saucer\" "
            ":seats 12} {:model-name \"Something Fast\" :seats 2}]");
}

TEST(StdVectorAdapter_Vehicle, append)
{
  // Given
  std::vector<LispleTest::Vehicle> obj_v{LispleTest::Vehicle("Dreamy Boom-Boom", 4),
                                         LispleTest::Vehicle("Flying Saucer", 12),
                                         LispleTest::Vehicle("Something Fast", 2)};

  Lisple::StdVectorAdapter<LispleTest::Vehicle, LispleTest::VehicleAdapter> adapter(obj_v);

  // When
  adapter.append(LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Aniara", 40000));

  // Then
  EXPECT_THAT(obj_v,
              ElementsAre(LispleTest::Vehicle("Dreamy Boom-Boom", 4),
                          LispleTest::Vehicle("Flying Saucer", 12),
                          LispleTest::Vehicle("Something Fast", 2),
                          LispleTest::Vehicle("Aniara", 40000)));

  EXPECT_EQ(adapter.size(), 4);
  EXPECT_EQ(*adapter.get_children()[0],
            *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Dreamy Boom-Boom", 4));
  EXPECT_EQ(*adapter.get_children()[1],
            *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Flying Saucer", 12));
  EXPECT_EQ(*adapter.get_children()[2],
            *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Something Fast", 2));
  EXPECT_EQ(*adapter.get_children()[3],
            *LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Aniara", 40000));
}
