
#include "host/test_adapters/vectorgfx_impl.h"
#include "host/test_adapters/vectorgfx_native_adapters.h"
#include "roo/runtime/value.h"
#include "runtime_fixture.h"
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>
#include <roo/context.h>
#include <roo/host/schema.h>

using MapSchema = RooTest::RuntimeTestFixture;
using MapSchema_Inspector = RooTest::RuntimeTestFixture;
using namespace ::testing;

struct MapSchemaFixture
{
  Roo::Runtime runtime;
  Roo::Context ctx;
  Roo::MapSchema schema;

 public:
  MapSchemaFixture(Roo::MapSchema schema)
    : runtime()
    , ctx(runtime)
    , schema(std::move(schema))
  {
  }

  MapSchemaFixture(std::vector<std::unique_ptr<Roo::Namespace>> ns, Roo::MapSchema schema)
    : runtime(std::move(ns), nullptr)
    , ctx(runtime)
    , schema(std::move(schema))
  {
  }
};

TEST_F(MapSchema, bind_with_required_key_present_yields_inspector)
{
  // Given
  MapSchemaFixture fixture(Roo::MapSchema({{"amount", &Roo::Type::NUMBER}}));
  Roo::sptr_val map_value =
    Roo::Value::map({Roo::Value::keyword("amount"), Roo::Value::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);

  // Then
  EXPECT_TRUE(&inspector);
}

TEST_F(MapSchema, bind_without_required_key_throws_TypeError)
{
  // Given
  MapSchemaFixture fixture(Roo::MapSchema({{"amount", &Roo::Type::NUMBER}}));
  Roo::sptr_val map_value =
    Roo::Value::map({Roo::Value::keyword("some-value"), Roo::Value::number(10)});

  // When/Then
  EXPECT_THAT([&]() { auto inspector = fixture.schema.bind(fixture.ctx, *map_value); },
              ThrowsMessage<Roo::TypeError>(HasSubstr("is missing required key")));
}

TEST_F(MapSchema, bind_with_omitted_optional_key_yields_inspector)
{
  // Given
  MapSchemaFixture fixture(
    Roo::MapSchema({{"instances", &Roo::Type::NUMBER}}, {{"amount", &Roo::Type::NUMBER}}));
  Roo::sptr_val map_value =
    Roo::Value::map({Roo::Value::keyword("instances"), Roo::Value::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);

  // Then
  EXPECT_TRUE(&inspector);
}

TEST_F(MapSchema_Inspector, bound_inspector__get_numbers)
{
  MapSchemaFixture fixture(Roo::MapSchema({{"amount", &Roo::Type::NUMBER}}));
  Roo::sptr_val map_value =
    Roo::Value::map({Roo::Value::keyword("amount"), Roo::Value::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  uint8_t ui8 = inspector.ui8("amount");
  int i32 = inspector.i32("amount");
  long i64 = inspector.i64("amount");
  float f32 = inspector.f32("amount");
  double f64 = inspector.f64("amount");

  // Then
  EXPECT_EQ(ui8, 10);
  EXPECT_EQ(i32, 10);
  EXPECT_EQ(i64, 10L);
  EXPECT_EQ(f32, 10.0f);
  EXPECT_EQ(f64, 10.0);
}

TEST_F(MapSchema_Inspector, bound_inspector__get_optional_number)
{
  MapSchemaFixture fixture(Roo::MapSchema({}, {{"amount", &Roo::Type::NUMBER}}));
  Roo::sptr_val map_value =
    Roo::Value::map({Roo::Value::keyword("amount"), Roo::Value::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<int> amount = inspector.optional<int>("amount");

  // Then
  EXPECT_THAT(amount, Optional(10));
}

TEST_F(MapSchema_Inspector, bound_inspector__get_missing_optional_number)
{
  MapSchemaFixture fixture(Roo::MapSchema({}, {{"amount", &Roo::Type::NUMBER}}));
  Roo::sptr_val map_value = Roo::Value::map({});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<int> amount = inspector.optional<int>("amount");

  // Then
  EXPECT_EQ(amount, std::nullopt);
}

TEST_F(MapSchema_Inspector, bound_inspector__get_optional_number_with_default)
{
  MapSchemaFixture fixture(Roo::MapSchema({}, {{"amount", &Roo::Type::NUMBER}}));
  Roo::sptr_val map_value = Roo::Value::map({});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<int> amount = inspector.optional<int>("amount", 10);

  // Then
  EXPECT_THAT(amount, Optional(10));
}

TEST_F(MapSchema_Inspector, bound_inspector__get_optional_string)
{
  MapSchemaFixture fixture(Roo::MapSchema({}, {{"name", &Roo::Type::STRING}}));
  Roo::sptr_val map_value =
    Roo::Value::map({Roo::Value::keyword("name"), Roo::Value::string("A")});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<std::string> name = inspector.optional<std::string>("name");

  // Then
  EXPECT_THAT(name, Optional(std::string("A")));
}

TEST_F(MapSchema_Inspector, bound_inspector__get_optional_value)
{
  MapSchemaFixture fixture(Roo::MapSchema({}, {{"amount", &Roo::Type::NUMBER}}));
  Roo::sptr_val map_value =
    Roo::Value::map({Roo::Value::keyword("amount"), Roo::Value::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<Roo::sptr_val> amount = inspector.optional<Roo::sptr_val>("amount");

  // Then
  EXPECT_TRUE(amount.has_value());
  EXPECT_EQ(amount.value()->i32(), 10);
}

TEST_F(MapSchema_Inspector, bound_inspected__get_native_object)
{
  MapSchemaFixture fixture(Roo::MapSchema({{"point", &RooTest::Native::POINT}}));
  Roo::sptr_val map_value = Roo::Value::map(
    {Roo::Value::keyword("point"), RooTest::Native::PointAdapter::make_unique(15, 30)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  RooTest::Point& point = inspector.obj<RooTest::Point>("point");

  // Then
  EXPECT_EQ(point.x, 15.0);
  EXPECT_EQ(point.y, 30.0);
}

TEST_F(MapSchema_Inspector, bound_inspected__get_optional_native_object)
{
  MapSchemaFixture fixture(Roo::MapSchema({}, {{"point", &RooTest::Native::POINT}}));
  Roo::sptr_val map_value = Roo::Value::map(
    {Roo::Value::keyword("point"), RooTest::Native::PointAdapter::make_unique(15, 30)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<RooTest::Point> point = inspector.optional<RooTest::Point>("point");

  // Then
  ASSERT_TRUE(point.has_value());
  EXPECT_EQ(point->x, 15.0);
  EXPECT_EQ(point->y, 30.0);
}

TEST_F(MapSchema_Inspector, bound_inspected__get_native_object__with_default)
{
  MapSchemaFixture fixture(Roo::MapSchema({{"point", &RooTest::Native::POINT}}));

  const RooTest::Point ZERO_ZERO(0, 0);

  Roo::sptr_val map_value = Roo::Value::map(
    {Roo::Value::keyword("point"), RooTest::Native::PointAdapter::make_unique(15, 30)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  const RooTest::Point& point = inspector.obj<RooTest::Point>("point", ZERO_ZERO);

  // Then
  EXPECT_EQ(point.x, 15.0);
  EXPECT_EQ(point.y, 30.0);
}

TEST_F(MapSchema_Inspector, bound_inspected__get_native_object_by_coercion)
{
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::PointNamespace>("pixils.point"));

  MapSchemaFixture fixture(std::move(namespaces),
                           Roo::MapSchema({{"point", &RooTest::Native::POINT}}));
  Roo::sptr_val map_value = Roo::Value::map({Roo::Value::keyword("point"),
                                             Roo::Value::map({Roo::Value::keyword("x"),
                                                              Roo::Value::number(15),
                                                              Roo::Value::keyword("y"),
                                                              Roo::Value::number(30)})});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  RooTest::Point& point = inspector.obj<RooTest::Point>("point");

  // Then
  EXPECT_EQ(point.x, 15.0);
  EXPECT_EQ(point.y, 30.0);
}

TEST_F(MapSchema_Inspector, bound_inspected__get_optional_native_object_by_coercion)
{
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::PointNamespace>("pixils.point"));

  MapSchemaFixture fixture(std::move(namespaces),
                           Roo::MapSchema({}, {{"point", &RooTest::Native::POINT}}));
  Roo::sptr_val map_value = Roo::Value::map({Roo::Value::keyword("point"),
                                             Roo::Value::map({Roo::Value::keyword("x"),
                                                              Roo::Value::number(15),
                                                              Roo::Value::keyword("y"),
                                                              Roo::Value::number(30)})});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<RooTest::Point> point = inspector.optional<RooTest::Point>("point");

  // Then
  ASSERT_TRUE(point.has_value());
  EXPECT_EQ(point->x, 15.0);
  EXPECT_EQ(point->y, 30.0);
}

TEST_F(MapSchema_Inspector, bound_inspected__get_native_object_by_coercion__with_default)
{
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::PointNamespace>("pixils.point"));

  const RooTest::Point ZERO_ZERO(0, 0);

  MapSchemaFixture fixture(std::move(namespaces),
                           Roo::MapSchema({{"point", &RooTest::Native::POINT}}));
  Roo::sptr_val map_value = Roo::Value::map({Roo::Value::keyword("point"),
                                             Roo::Value::map({Roo::Value::keyword("x"),
                                                              Roo::Value::number(15),
                                                              Roo::Value::keyword("y"),
                                                              Roo::Value::number(30)})});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  const RooTest::Point& point = inspector.obj<RooTest::Point>("point", ZERO_ZERO);

  // Then
  EXPECT_EQ(point.x, 15.0);
  EXPECT_EQ(point.y, 30.0);
}

TEST_F(MapSchema_Inspector,
       bound_inspected__get_missing_native_object_by_coercion__with_default)
{
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::PointNamespace>("pixils.point"));

  const RooTest::Point ZERO_ZERO(0, 0);

  MapSchemaFixture fixture(std::move(namespaces),
                           Roo::MapSchema({}, {{"point", &RooTest::Native::POINT}}));
  Roo::sptr_val map_value = Roo::Value::map({});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  const RooTest::Point& point = inspector.obj<RooTest::Point>("point", ZERO_ZERO);

  // Then
  EXPECT_EQ(point.x, 0);
  EXPECT_EQ(point.y, 0);
}
