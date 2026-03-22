#include "test_host_objects.h"
#include <gtest/gtest.h>

#include "lisple/form.h"
#include "lisple/runtime/value.h"

TEST(HostTypeRef, is_type)
{
  // Given
  Lisple::sptr_sobject adapter =
    Tests::VehicleAdapter::make<Tests::Vehicle>("Spruttibangbang", 8);

  Lisple::sptr_sobject other_adapter =
    Tests::BookAdapter::make<Tests::Book>("How to befriend hostile organisms",
                                          120.0,
                                          38388383,
                                          "M. Elting Face",
                                          "8888-1000");

  // Then
  EXPECT_TRUE(Tests::VEHICLE_TYPE.is_type_of(*adapter));
  EXPECT_FALSE(Tests::VEHICLE_TYPE.is_type_of(Lisple::String("a string!")));
  EXPECT_FALSE(Tests::VEHICLE_TYPE.is_type_of(Lisple::Number(8)));
  EXPECT_FALSE(Tests::VEHICLE_TYPE.is_type_of(*other_adapter));
}

TEST(HostTypeRef, is_type__RTWrapper)
{
  // Given
  Lisple::sptr_sobject adapter =
    Tests::VehicleAdapter::make<Tests::Vehicle>("Spruttibangbang", 8);

  Lisple::sptr_sobject other_adapter =
    Tests::BookAdapter::make<Tests::Book>("How to befriend hostile organisms",
                                          120.0,
                                          38388383,
                                          "M. Elting Face",
                                          "8888-1000");

  Lisple::sptr_rtval vehicle_val = Lisple::RTValue::object(adapter);
  Lisple::sptr_rtval book_val = Lisple::RTValue::object(other_adapter);

  Lisple::sptr_sobject wrapped_vehicle =
    std::make_shared<Lisple::RuntimeValueWrapper>(vehicle_val);
  Lisple::sptr_sobject wrapped_book =
    std::make_shared<Lisple::RuntimeValueWrapper>(book_val);

  // Then
  EXPECT_TRUE(Tests::VEHICLE_TYPE.is_type_of(*wrapped_vehicle));

  EXPECT_FALSE(Tests::VEHICLE_TYPE.is_type_of(Lisple::String("a string!")));
  EXPECT_FALSE(Tests::VEHICLE_TYPE.is_type_of(Lisple::Number(8)));
  EXPECT_FALSE(Tests::VEHICLE_TYPE.is_type_of(*wrapped_book));
}
