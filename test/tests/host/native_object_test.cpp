
#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/runtime.h>
#include <lisple/runtime/dict.h>
#include <lisple/type.h>

#include "host/test_adapters/vehicle_host_adapters.h"
#include "host/test_adapters/vehicle_native_adapters.h"
#include "test_host_objects.h"
#include <gtest/gtest.h>

TEST(NativeObjectAdapter_Vehicle, type)
{
  // Given
  Lisple::sptr_rtval val =
    LispleTest::Native::VehicleAdapter::make_unique("Runaway Train", 500);
  LispleTest::Native::VehicleAdapter& adapter =
    val->adapter<LispleTest::Native::VehicleAdapter>();

  // Then
  EXPECT_EQ(val->type, Lisple::RTValue::Type::NATIVE_OBJECT);
  EXPECT_EQ(adapter.get_host_type(), &LispleTest::VEHICLE_TYPE);

  EXPECT_TRUE(Lisple::Type::NATIVE_OBJECT.is_type_of(*val));
  EXPECT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(*val));

  EXPECT_FALSE(Lisple::Type::ARRAY.is_type_of(*val));
  EXPECT_FALSE(Lisple::Type::SEQ.is_type_of(*val));
  EXPECT_FALSE(Lisple::Type::MAP.is_type_of(*val));
  EXPECT_FALSE(Tests::PRODUCT.is_type_of(*val));
}

TEST(NativeObjectAdapter_Vehicle, get_property)
{
  // Given
  Lisple::sptr_rtval val =
    LispleTest::Native::VehicleAdapter::make_unique("Runaway Train", 500);
  LispleTest::Native::VehicleAdapter& adapter =
    val->adapter<LispleTest::Native::VehicleAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_model_name(), *Lisple::RTValue::string("Runaway Train"));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("model-name")),
            *Lisple::RTValue::string("Runaway Train"));

  EXPECT_EQ(*adapter.get_seats(), *Lisple::RTValue::number(500));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("seats")),
            *Lisple::RTValue::number(500));

  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("does-not-exist")),
            *Lisple::Constant::NIL);
}
