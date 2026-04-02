#include "lisple/form.h"
#include "lisple/runtime/value.h"

#include "test_adapters/vehicle_host_adapters.h"
#include "test_adapters/vehicle_native_adapters.h"
#include "test_host_objects.h"
#include <gtest/gtest.h>

TEST(HostTypeRef, is_type__ast_host_object)
{
  // Given
  Lisple::sptr_sobject adapter =
    LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Spruttibangbang", 8);

  Lisple::sptr_sobject other_adapter =
    Tests::BookAdapter::make<Tests::Book>("How to befriend hostile organisms",
                                          120.0,
                                          38388383,
                                          "M. Elting Face",
                                          "8888-1000");

  // Then
  EXPECT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(*adapter));
  EXPECT_FALSE(LispleTest::VEHICLE_TYPE.is_type_of(Lisple::String("a string!")));
  EXPECT_FALSE(LispleTest::VEHICLE_TYPE.is_type_of(Lisple::Number(8)));
  EXPECT_FALSE(LispleTest::VEHICLE_TYPE.is_type_of(*other_adapter));
}

TEST(HostTypeRef, is_type__native_object)
{
  // Given
  Lisple::sptr_rtval adapter =
    LispleTest::Native::VehicleAdapter::make_unique("Spruttibangbang", 8);

  // Then
  EXPECT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(*adapter));
}

TEST(HostTypeRef, is_type__RTWrapped_native_object)
{
  // Given
  Lisple::sptr_rtval adapter =
    LispleTest::Native::VehicleAdapter::make_unique("Spruttibangbang", 8);

  Lisple::sptr_sobject wrapped = Lisple::RuntimeValueWrapper::make(adapter);

  // Then
  EXPECT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(*wrapped));
}

TEST(HostTypeRef, is_type__RTWrapper)
{
  // Given
  Lisple::sptr_sobject adapter =
    LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Spruttibangbang", 8);

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
  EXPECT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(*wrapped_vehicle));

  EXPECT_FALSE(LispleTest::VEHICLE_TYPE.is_type_of(Lisple::String("a string!")));
  EXPECT_FALSE(LispleTest::VEHICLE_TYPE.is_type_of(Lisple::Number(8)));
  EXPECT_FALSE(LispleTest::VEHICLE_TYPE.is_type_of(*wrapped_book));
}
