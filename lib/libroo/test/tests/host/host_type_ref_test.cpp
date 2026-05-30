#include "roo/form.h"
#include "roo/runtime/value.h"

#include "host/test_adapters/product_native_adapters.h"
#include "test_adapters/vehicle_native_adapters.h"
#include <gtest/gtest.h>

TEST(HostTypeRef, is_type__native_object)
{
  // Given
  Roo::sptr_val adapter =
    RooTest::Native::VehicleModelAdapter::make_unique("Spruttibangbang", 8);

  // Then
  EXPECT_TRUE(RooTest::VEHICLE_MODEL_TYPE.is_type_of(*adapter));
}

TEST(HostTypeRef, is_type__sub_type_is_base_type)
{
  // Given
  Roo::sptr_val adapter = RooTest::Native::ClothingAdapter::make_unique("Cheese Trousers",
                                                                        200.0,
                                                                        7774443,
                                                                        "m",
                                                                        "Maasdamer");

  // Then
  ASSERT_TRUE(RooTest::Native::PRODUCT.is_type_of(*adapter));
}

TEST(HostTypeRef, is_type__RTWrapped_native_object)
{
  // Given
  Roo::sptr_val adapter =
    RooTest::Native::VehicleModelAdapter::make_unique("Spruttibangbang", 8);

  Roo::sptr_ast_node wrapped = Roo::AST::RuntimeValueWrapper::make(adapter);

  // Then
  EXPECT_TRUE(RooTest::VEHICLE_MODEL_TYPE.is_type_of(*wrapped));
}
