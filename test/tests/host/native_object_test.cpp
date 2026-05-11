
#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/host/accessor.h>
#include <lisple/host/object.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/runtime.h>
#include <lisple/runtime/dict.h>
#include <lisple/type.h>

#include "host/test_adapters/optional_native_adapters.h"
#include "host/test_adapters/vehicle_host_adapters.h"
#include "host/test_adapters/vehicle_impl.h"
#include "host/test_adapters/vehicle_native_adapters.h"
#include "test_host_objects.h"
#include <gtest/gtest.h>

namespace
{
  struct ByteBox
  {
    uint8_t value = 0;
  };

  inline Lisple::HostTypeRef BYTE_BOX("ByteBox");

  NATIVE_ADAPTER(ByteBoxAdapter, ByteBox, (value), (value));

  NATIVE_ADAPTER_IMPL(ByteBoxAdapter, ByteBox, &BYTE_BOX, (rw, "value", value));

  NOBJ_PROP_GET_SET__FIELD(ByteBoxAdapter, value);
} // namespace

TEST(NativeObjectAdapter_VehicleModel, type)
{
  // Given
  Lisple::sptr_rtval val =
    LispleTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  LispleTest::Native::VehicleModelAdapter& adapter =
    val->adapter<LispleTest::Native::VehicleModelAdapter>();

  // Then
  EXPECT_EQ(val->type, Lisple::RTValue::Type::NATIVE_OBJECT);
  EXPECT_EQ(adapter.get_host_type(), &LispleTest::VEHICLE_MODEL_TYPE);

  EXPECT_TRUE(Lisple::Type::NATIVE_OBJECT.is_type_of(*val));
  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*val));

  EXPECT_FALSE(Lisple::Type::ARRAY.is_type_of(*val));
  EXPECT_FALSE(Lisple::Type::SEQ.is_type_of(*val));
  EXPECT_FALSE(Lisple::Type::MAP.is_type_of(*val));
  EXPECT_FALSE(Tests::PRODUCT.is_type_of(*val));
}

TEST(NativeObjectAdapter_PrimitiveField, get_set_uint8_field)
{
  // Given
  Lisple::sptr_rtval val = ByteBoxAdapter::make_unique();
  ByteBoxAdapter& adapter = val->adapter<ByteBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_value(), *Lisple::RTValue::number(0));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("value")),
            *Lisple::RTValue::number(0));

  // When
  auto value = Lisple::RTValue::number(255);
  adapter.set_value(nullptr, value);

  // Then
  EXPECT_EQ(adapter.get_object().value, 255);
  EXPECT_EQ(*adapter.get_value(), *Lisple::RTValue::number(255));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("value")),
            *Lisple::RTValue::number(255));
}

TEST(NativeObjectAdapter_OptionalField, get_set_optional_field)
{
  // Given
  Lisple::sptr_rtval val = LispleTest::Native::OptionalBoxAdapter::make_unique();
  LispleTest::Native::OptionalBoxAdapter& adapter =
    val->adapter<LispleTest::Native::OptionalBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_count(), *Lisple::Constant::NIL);
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("count")),
            *Lisple::Constant::NIL);

  // When
  auto count = Lisple::RTValue::number(7);
  adapter.set_count(nullptr, count);

  // Then
  ASSERT_TRUE(adapter.get_object().count.has_value());
  EXPECT_EQ(*adapter.get_object().count, 7);
  EXPECT_EQ(*adapter.get_count(), *Lisple::RTValue::number(7));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("count")),
            *Lisple::RTValue::number(7));

  // When
  auto nil = Lisple::Constant::NIL;
  adapter.set_count(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().count.has_value());
  EXPECT_EQ(*adapter.get_count(), *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_OptionalAdapterField, get_set_optional_adapter_field)
{
  // Given
  Lisple::sptr_rtval val = LispleTest::Native::OptionalChildBoxAdapter::make_unique();
  LispleTest::Native::OptionalChildBoxAdapter& adapter =
    val->adapter<LispleTest::Native::OptionalChildBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_child(), *Lisple::Constant::NIL);
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("child")),
            *Lisple::Constant::NIL);

  // When
  auto child = LispleTest::Native::OptionalChildAdapter::make_unique();
  auto child_value = Lisple::RTValue::number(42);
  child->adapter<LispleTest::Native::OptionalChildAdapter>().set_value(nullptr, child_value);
  adapter.set_child(nullptr, child);

  // Then
  ASSERT_TRUE(adapter.get_object().child.has_value());
  EXPECT_EQ(adapter.get_object().child->value, 42);
  EXPECT_EQ(adapter.get_child()
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            42);

  // When
  auto nil = Lisple::Constant::NIL;
  adapter.set_child(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().child.has_value());
  EXPECT_EQ(*adapter.get_child(), *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_PointerAdapterField, get_set_pointer_adapter_field)
{
  // Given
  Lisple::sptr_rtval child = LispleTest::Native::OptionalChildAdapter::make_unique();
  auto child_value = Lisple::RTValue::number(42);
  child->adapter<LispleTest::Native::OptionalChildAdapter>().set_value(nullptr, child_value);

  Lisple::sptr_rtval val = LispleTest::Native::OptionalChildPtrBoxAdapter::make_unique();
  LispleTest::Native::OptionalChildPtrBoxAdapter& adapter =
    val->adapter<LispleTest::Native::OptionalChildPtrBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_child(), *Lisple::Constant::NIL);

  // When
  adapter.set_child(nullptr, child);

  // Then
  ASSERT_NE(adapter.get_object().child, nullptr);
  EXPECT_EQ(adapter.get_object().child->value, 42);
  EXPECT_EQ(adapter.get_child()
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            42);

  // When
  auto nil = Lisple::Constant::NIL;
  adapter.set_child(nullptr, nil);

  // Then
  EXPECT_EQ(adapter.get_object().child, nullptr);
  EXPECT_EQ(*adapter.get_child(), *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_VehicleModel, get_property)
{
  // Given
  Lisple::sptr_rtval val =
    LispleTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  LispleTest::Native::VehicleModelAdapter& adapter =
    val->adapter<LispleTest::Native::VehicleModelAdapter>();

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

TEST(NativeObjectAdapter_VehicleModel, set_property)
{
  // Given
  Lisple::sptr_rtval val =
    LispleTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  LispleTest::Native::VehicleModelAdapter& adapter =
    val->adapter<LispleTest::Native::VehicleModelAdapter>();

  VehicleModel& vehicle = adapter.get_object();
  EXPECT_EQ(vehicle.get_model_name(), "Runaway Train");
  EXPECT_EQ(vehicle.get_seats(), 500);

  // When
  vehicle.set_seats(800);

  // Then
  EXPECT_EQ(*adapter.get_seats(), *Lisple::RTValue::number(800));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("seats")),
            *Lisple::RTValue::number(800));
  EXPECT_EQ(vehicle.get_seats(), 800);
}
