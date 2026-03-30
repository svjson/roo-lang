
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>
#include <lisple/context.h>
#include <lisple/host/schema.h>
#include <lisple/runtime.h>

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
