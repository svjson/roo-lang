
#include "lisple/runtime/value.h"

#include <lisple/context.h>
#include <lisple/host/schema.h>
#include <lisple/runtime.h>

#include "host/test_adapters/vectorgfx_impl.h"
#include "host/test_adapters/vectorgfx_native_adapters.h"
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>

using namespace ::testing;

struct MapSchemaFixture
{
  Lisple::Runtime runtime;
  Lisple::Context ctx = (runtime);
  Lisple::MapSchema schema;

 public:
  MapSchemaFixture(Lisple::MapSchema schema)
    : schema(std::move(schema))
  {
  }

  MapSchemaFixture(std::vector<std::unique_ptr<Lisple::Namespace>> ns,
                   Lisple::MapSchema schema)
    : runtime(std::move(ns), nullptr)
    , schema(std::move(schema))
  {
  }
};

TEST(MapSchema, bind_with_required_key_present_yields_inspector)
{
  // Given
  MapSchemaFixture fixture(Lisple::MapSchema({{"amount", &Lisple::Type::NUMBER}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("amount"), Lisple::RTValue::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);

  // Then
  EXPECT_TRUE(&inspector);
}

TEST(MapSchema, bind_without_required_key_throws_TypeError)
{
  // Given
  MapSchemaFixture fixture(Lisple::MapSchema({{"amount", &Lisple::Type::NUMBER}}));
  Lisple::sptr_rtval map_value = Lisple::RTValue::map(
    {Lisple::RTValue::keyword("some-value"), Lisple::RTValue::number(10)});

  // When/Then
  EXPECT_THAT([&]() { auto inspector = fixture.schema.bind(fixture.ctx, *map_value); },
              ThrowsMessage<Lisple::TypeError>(HasSubstr("is missing required key")));
}

TEST(MapSchema, bind_with_omitted_optional_key_yields_inspector)
{
  // Given
  MapSchemaFixture fixture(Lisple::MapSchema({{"instances", &Lisple::Type::NUMBER}},
                                             {{"amount", &Lisple::Type::NUMBER}}));
  Lisple::sptr_rtval map_value = Lisple::RTValue::map(
    {Lisple::RTValue::keyword("instances"), Lisple::RTValue::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);

  // Then
  EXPECT_TRUE(&inspector);
}

TEST(MapSchema_Inspector, bound_inspector__get_numbers)
{
  MapSchemaFixture fixture(Lisple::MapSchema({{"amount", &Lisple::Type::NUMBER}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("amount"), Lisple::RTValue::number(10)});

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

TEST(MapSchema_Inspector, bound_inspector__get_optional_number)
{
  MapSchemaFixture fixture(Lisple::MapSchema({}, {{"amount", &Lisple::Type::NUMBER}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("amount"), Lisple::RTValue::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<int> amount = inspector.optional<int>("amount");

  // Then
  EXPECT_THAT(amount, Optional(10));
}

TEST(MapSchema_Inspector, bound_inspector__get_missing_optional_number)
{
  MapSchemaFixture fixture(Lisple::MapSchema({}, {{"amount", &Lisple::Type::NUMBER}}));
  Lisple::sptr_rtval map_value = Lisple::RTValue::map({});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<int> amount = inspector.optional<int>("amount");

  // Then
  EXPECT_EQ(amount, std::nullopt);
}

TEST(MapSchema_Inspector, bound_inspector__get_optional_number_with_default)
{
  MapSchemaFixture fixture(Lisple::MapSchema({}, {{"amount", &Lisple::Type::NUMBER}}));
  Lisple::sptr_rtval map_value = Lisple::RTValue::map({});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<int> amount = inspector.optional<int>("amount", 10);

  // Then
  EXPECT_THAT(amount, Optional(10));
}

TEST(MapSchema_Inspector, bound_inspector__get_optional_string)
{
  MapSchemaFixture fixture(Lisple::MapSchema({}, {{"name", &Lisple::Type::STRING}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("name"), Lisple::RTValue::string("A")});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<std::string> name = inspector.optional<std::string>("name");

  // Then
  EXPECT_THAT(name, Optional(std::string("A")));
}

TEST(MapSchema_Inspector, bound_inspector__get_optional_value)
{
  MapSchemaFixture fixture(Lisple::MapSchema({}, {{"amount", &Lisple::Type::NUMBER}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("amount"), Lisple::RTValue::number(10)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<Lisple::sptr_rtval> amount =
    inspector.optional<Lisple::sptr_rtval>("amount");

  // Then
  EXPECT_TRUE(amount.has_value());
  EXPECT_EQ(amount.value()->i32(), 10);
}

TEST(MapSchema_Inspector, bound_inspected__get_native_object)
{
  MapSchemaFixture fixture(Lisple::MapSchema({{"point", &LispleTest::Native::POINT}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("point"),
                          LispleTest::Native::PointAdapter::make_unique(15, 30)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  LispleTest::Point& point = inspector.obj<LispleTest::Point>("point");

  // Then
  EXPECT_EQ(point.x, 15.0);
  EXPECT_EQ(point.y, 30.0);
}

TEST(MapSchema_Inspector, bound_inspected__get_optional_native_object)
{
  MapSchemaFixture fixture(Lisple::MapSchema({}, {{"point", &LispleTest::Native::POINT}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("point"),
                          LispleTest::Native::PointAdapter::make_unique(15, 30)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<LispleTest::Point> point = inspector.optional<LispleTest::Point>("point");

  // Then
  ASSERT_TRUE(point.has_value());
  EXPECT_EQ(point->x, 15.0);
  EXPECT_EQ(point->y, 30.0);
}

TEST(MapSchema_Inspector, bound_inspected__get_native_object__with_default)
{
  MapSchemaFixture fixture(Lisple::MapSchema({{"point", &LispleTest::Native::POINT}}));

  const LispleTest::Point ZERO_ZERO(0, 0);

  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("point"),
                          LispleTest::Native::PointAdapter::make_unique(15, 30)});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  const LispleTest::Point& point = inspector.obj<LispleTest::Point>("point", ZERO_ZERO);

  // Then
  EXPECT_EQ(point.x, 15.0);
  EXPECT_EQ(point.y, 30.0);
}

TEST(MapSchema_Inspector, bound_inspected__get_native_object_by_coercion)
{
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::PointNamespace>("pixils.point"));

  MapSchemaFixture fixture(std::move(namespaces),
                           Lisple::MapSchema({{"point", &LispleTest::Native::POINT}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("point"),
                          Lisple::RTValue::map({Lisple::RTValue::keyword("x"),
                                                Lisple::RTValue::number(15),
                                                Lisple::RTValue::keyword("y"),
                                                Lisple::RTValue::number(30)})});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  LispleTest::Point& point = inspector.obj<LispleTest::Point>("point");

  // Then
  EXPECT_EQ(point.x, 15.0);
  EXPECT_EQ(point.y, 30.0);
}

TEST(MapSchema_Inspector, bound_inspected__get_optional_native_object_by_coercion)
{
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::PointNamespace>("pixils.point"));

  MapSchemaFixture fixture(std::move(namespaces),
                           Lisple::MapSchema({}, {{"point", &LispleTest::Native::POINT}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("point"),
                          Lisple::RTValue::map({Lisple::RTValue::keyword("x"),
                                                Lisple::RTValue::number(15),
                                                Lisple::RTValue::keyword("y"),
                                                Lisple::RTValue::number(30)})});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  std::optional<LispleTest::Point> point = inspector.optional<LispleTest::Point>("point");

  // Then
  ASSERT_TRUE(point.has_value());
  EXPECT_EQ(point->x, 15.0);
  EXPECT_EQ(point->y, 30.0);
}

TEST(MapSchema_Inspector, bound_inspected__get_native_object_by_coercion__with_default)
{
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::PointNamespace>("pixils.point"));

  const LispleTest::Point ZERO_ZERO(0, 0);

  MapSchemaFixture fixture(std::move(namespaces),
                           Lisple::MapSchema({{"point", &LispleTest::Native::POINT}}));
  Lisple::sptr_rtval map_value =
    Lisple::RTValue::map({Lisple::RTValue::keyword("point"),
                          Lisple::RTValue::map({Lisple::RTValue::keyword("x"),
                                                Lisple::RTValue::number(15),
                                                Lisple::RTValue::keyword("y"),
                                                Lisple::RTValue::number(30)})});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  const LispleTest::Point& point = inspector.obj<LispleTest::Point>("point", ZERO_ZERO);

  // Then
  EXPECT_EQ(point.x, 15.0);
  EXPECT_EQ(point.y, 30.0);
}

TEST(MapSchema_Inspector,
     bound_inspected__get_missing_native_object_by_coercion__with_default)
{
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::PointNamespace>("pixils.point"));

  const LispleTest::Point ZERO_ZERO(0, 0);

  MapSchemaFixture fixture(std::move(namespaces),
                           Lisple::MapSchema({}, {{"point", &LispleTest::Native::POINT}}));
  Lisple::sptr_rtval map_value = Lisple::RTValue::map({});

  // When
  auto inspector = fixture.schema.bind(fixture.ctx, *map_value);
  const LispleTest::Point& point = inspector.obj<LispleTest::Point>("point", ZERO_ZERO);

  // Then
  EXPECT_EQ(point.x, 0);
  EXPECT_EQ(point.y, 0);
}
