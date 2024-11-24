
#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include <lisple/runtime.h>
#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/type.h>

#include "test_host_objects.h"
#include "runtime_fixture.h"

TEST(HostObjectAdapter_Vehicle, type)
{
  // Given
  Lisple::sptr_sobject obj = Tests::VehicleAdapter::make<Tests::Vehicle>("Runaway Train", 500);
  Tests::VehicleAdapter& adapter = obj->as<Tests::VehicleAdapter>();

  // Then
  EXPECT_EQ(obj->get_type(), Lisple::Form::HOST_OBJECT);
  EXPECT_EQ(adapter.get_type(), Lisple::Form::HOST_OBJECT);
  EXPECT_EQ(adapter.get_host_type(), &Tests::VEHICLE_TYPE);

  EXPECT_TRUE(Lisple::Type::HOST_OBJECT.is_type_of(adapter));
  EXPECT_TRUE(Tests::VEHICLE_TYPE.is_type_of(adapter));

  EXPECT_FALSE(Lisple::Type::ARRAY.is_type_of(adapter));
  EXPECT_FALSE(Lisple::Type::SEQ.is_type_of(adapter));
  EXPECT_FALSE(Lisple::Type::MAP.is_type_of(adapter));
  EXPECT_FALSE(Tests::PRODUCT.is_type_of(adapter));
}

TEST(HostObjectAdapter_Vehicle, get_property)
{
  // Given
  Lisple::sptr_sobject adapter = Tests::VehicleAdapter::make<Tests::Vehicle>("Runaway Train", 500);

  // Then
  EXPECT_EQ(adapter->get_property(Lisple::Key("model-name")), Lisple::String("Runaway Train"));
  EXPECT_EQ(adapter->get_property(Lisple::Key("seats")), Lisple::Number(500));
  EXPECT_EQ(adapter->get_property(Lisple::Key("Non-existent")), *Lisple::NIL);
}

TEST(HostObjectAdapter_Clothing, type)
{
  // Given
  Lisple::sptr_sobject obj = Tests::ClothingAdapter::make<Tests::Clothing>("Silly Hat",
                                                                           20.0,
                                                                           159124743,
                                                                           "L",
                                                                           "Paper");
  Tests::ClothingAdapter& adapter = obj->as<Tests::ClothingAdapter>();

  // Then
  EXPECT_EQ(obj->get_type(), Lisple::Form::HOST_OBJECT);
  EXPECT_EQ(adapter.get_type(), Lisple::Form::HOST_OBJECT);
  EXPECT_EQ(adapter.get_host_type(), &Tests::CLOTHING);

  EXPECT_TRUE(Lisple::Type::HOST_OBJECT.is_type_of(adapter));
  EXPECT_TRUE(Tests::CLOTHING.is_type_of(adapter));
  EXPECT_TRUE(Tests::PRODUCT.is_type_of(adapter));

  EXPECT_FALSE(Lisple::Type::ARRAY.is_type_of(adapter));
  EXPECT_FALSE(Lisple::Type::SEQ.is_type_of(adapter));
  EXPECT_FALSE(Lisple::Type::MAP.is_type_of(adapter));
  EXPECT_FALSE(Tests::VEHICLE_TYPE.is_type_of(adapter));
}

TEST(HostObjectAdapter_Clothing, get_property)
{
  // Given
  Lisple::sptr_sobject adapter = Tests::ClothingAdapter::make<Tests::Clothing>("Mankini",
                                                                               150.50,
                                                                               889922123,
                                                                               "XS",
                                                                               "Silk");

  // Then
  EXPECT_EQ(adapter->get_property(Lisple::Key("name")), Lisple::String("Mankini"));
  EXPECT_EQ(adapter->get_property(Lisple::Key("price")), Lisple::Number(150.50));
  EXPECT_EQ(adapter->get_property(Lisple::Key("sku")), Lisple::Number(889922123));
  EXPECT_EQ(adapter->get_property(Lisple::Key("size")), Lisple::String("XS"));
  EXPECT_EQ(adapter->get_property(Lisple::Key("material")), Lisple::String("Silk"));
  EXPECT_EQ(adapter->get_property(Lisple::Key("non-existent")), *Lisple::NIL);

}
