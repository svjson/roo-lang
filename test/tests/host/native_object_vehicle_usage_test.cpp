
#include <lisple/runtime.h>

#include "host/test_adapters/vectorgfx_native_adapters.h"
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
  auto stringified = runtime.eval("(str my-vehicle)");

  // Then
  ASSERT_TRUE(LispleTest::VEHICLE_TYPE.is_type_of(*vehicle));
  ASSERT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*model));
  ASSERT_TRUE(LispleTest::REGNUM_TYPE.is_type_of(*num));
  ASSERT_EQ(stringified->str(),
            "{:model {:model-name \"Vroom Deluxe\" :seats 2} :reg-number {:letters \"XYZ\" "
            ":numbers \"123\"}}");
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

TEST(VehicleModelAdapter_usage, write_mutable_property_with_assoc_bang)
{
  // Given
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  Lisple::Runtime runtime(std::move(namespaces), nullptr);

  runtime.eval(
    "(def model (vehicle/make-vehicle-model {:model-name \"Spruttibangbang\" :seats 2}))");

  // When
  runtime.eval("(assoc! model :seats 8)");

  // Then
  EXPECT_EQ(runtime.eval("(str model)")->str(),
            "{:model-name \"Spruttibangbang\" :seats 8}");
}

TEST(VehicleModelAdapter_usage, produce_modified_copy_with_assoc)
{
  // Given
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  Lisple::Runtime runtime(std::move(namespaces), nullptr);

  runtime.eval(
    "(def model (vehicle/make-vehicle-model {:model-name \"Spruttibangbang\" :seats 2}))");

  // When
  auto result = runtime.eval("(assoc model :seats 8)");

  // Then
  //  - Original is unmodified
  EXPECT_EQ(runtime.eval("(str model)")->str(),
            "{:model-name \"Spruttibangbang\" :seats 2}");
  //  - Result contains new property
  EXPECT_EQ(result->to_string(), "{:model-name \"Spruttibangbang\" :seats 8}");
}

TEST(NativeObjectAdapter_usage, rt_dispatch_coerces_args_before_ast_fallback)
{
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::PointNamespace>("pixils.point"));
  Lisple::Runtime runtime(std::move(namespaces), nullptr);

  auto result = runtime.eval(R"(
    (pixils.point/plus
      {:x 1 :y 2}
      {:x 3 :y 4})
  )");

  ASSERT_TRUE(LispleTest::Native::POINT.is_type_of(*result));
  auto& point = result->adapter<LispleTest::Native::PointAdapter>().get_object();
  EXPECT_EQ(point.x, 4);
  EXPECT_EQ(point.y, 6);
}
