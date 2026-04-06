
#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/runtime.h>
#include <lisple/type.h>

#include "host/test_adapters/vehicle_host_adapters.h"
#include "test_host_objects.h"
#include <gtest/gtest.h>

TEST(HostObjectAdapter_Vehicle, type)
{
  // Given
  Lisple::sptr_sobject obj =
    LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Runaway Train", 500);
  LispleTest::VehicleAdapter& adapter = obj->as<LispleTest::VehicleAdapter>();

  // Then
  EXPECT_EQ(obj->get_type(), Lisple::Form::HOST_OBJECT);
  EXPECT_EQ(adapter.get_type(), Lisple::Form::HOST_OBJECT);
  EXPECT_EQ(adapter.get_host_type(), &LispleTest::VEHICLE_TYPE);

  EXPECT_TRUE(Lisple::Type::HOST_OBJECT.is_type_of(adapter));
  EXPECT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(adapter));

  EXPECT_FALSE(Lisple::Type::ARRAY.is_type_of(adapter));
  EXPECT_FALSE(Lisple::Type::SEQ.is_type_of(adapter));
  EXPECT_FALSE(Lisple::Type::MAP.is_type_of(adapter));
  EXPECT_FALSE(Tests::PRODUCT.is_type_of(adapter));
}

// Tmp changed to use get_sptr_property, because get_property in its current form does
// not work for temporary objects.
TEST(HostObjectAdapter_Vehicle, get_property)
{
  // Given
  Lisple::sptr_sobject adapter =
    LispleTest::VehicleAdapter::make<LispleTest::Vehicle>("Runaway Train", 500);

  // Then
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("model-name")),
            Lisple::String("Runaway Train"));
  std::cout << "Get seats" << std::endl;
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("seats")), Lisple::Number(500));
  std::cout << "Non-existent" << std::endl;
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("Non-existent")), *Lisple::NIL);
}

TEST(HostObjectAdapter_Clothing, type)
{
  // Given
  Lisple::sptr_sobject obj = Tests::ClothingAdapter::make<LispleTest::Clothing>("Silly Hat",
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
  EXPECT_FALSE(LispleTest::VEHICLE_TYPE.is_type_of(adapter));
}

// Tmp changed to use get_sptr_property, because get_property in its current form does
// not work for temporary objects.
TEST(HostObjectAdapter_Clothing, get_property)
{
  // Given
  Lisple::sptr_sobject adapter =
    Tests::ClothingAdapter::make<LispleTest::Clothing>("Mankini",
                                                       150.50,
                                                       889922123,
                                                       "XS",
                                                       "Silk");

  // Then
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("name")), Lisple::String("Mankini"));
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("price")), Lisple::Number(150.50));
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("sku")), Lisple::Number(889922123));
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("size")), Lisple::String("XS"));
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("material")), Lisple::String("Silk"));
  EXPECT_EQ(*adapter->get_sptr_property(Lisple::Key("non-existent")), *Lisple::NIL);
}
