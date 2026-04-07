
#include <lisple/runtime.h>

#include "host/test_adapters/vehicle_host_adapters.h"
#include "test_adapters/vehicle_native_adapters.h"
#include <gtest/gtest.h>

TEST(VehicleAdapter_usage, make_with_explicit_make_functions)
{
  // Given
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  Lisple::Runtime runtime(std::move(namespaces), nullptr);

  // When
  runtime.eval(R"(
(def my-vehicle
  (vehicle/make-vehicle
    {:model (vehicle/make-vehicle-model
              {:model-name "Vroom Deluxe" :seats 2})
     :reg-number (vehicle/make-reg-number
              {:letters "XYZ" :numbers "123" })}))
                )");

  auto vehicle = runtime.eval("my-vehicle");
  auto model = runtime.eval("(:model my-vehicle)");
  auto num = runtime.eval("(:reg-number my-vehicle)");

  // Then
  ASSERT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(*vehicle));
  ASSERT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*model));
  ASSERT_TRUE(LispleTest::REGNUM_TYPE.is_type_of(*num));
}

TEST(VehicleAdapter_usage, make_with_coercion)
{
  // Given
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  Lisple::Runtime runtime(std::move(namespaces), nullptr);

  // When
  runtime.eval(R"(
(def my-vehicle
  (vehicle/make-vehicle
    {:model {:model-name "Vroom Deluxe" :seats 2}
     :reg-number {:letters "XYZ" :numbers "123" }}))
                )");

  auto vehicle = runtime.eval("my-vehicle");
  auto model = runtime.eval("(:model my-vehicle)");
  auto num = runtime.eval("(:reg-number my-vehicle)");

  // Then
  ASSERT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(*vehicle));
  ASSERT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*model));
  ASSERT_TRUE(LispleTest::REGNUM_TYPE.is_type_of(*num));
}
