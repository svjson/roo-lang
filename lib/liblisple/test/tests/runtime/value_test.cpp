
#include <lisple/runtime/value.h>

#include <gtest/gtest.h>

TEST(Value_keyword, equals)
{
  auto kind = Lisple::Value::keyword("kind");
  auto kind_ = Lisple::Value::keyword("kind");
  auto value = Lisple::Value::keyword("value");

  EXPECT_TRUE(*kind == *kind);
  EXPECT_TRUE(*kind == *kind_);
  EXPECT_FALSE(*kind == *value);
  EXPECT_FALSE(*value == *kind);
}

TEST(ValueFactory, root_namespace_forwards_to_value_factories)
{
  EXPECT_EQ(*Lisple::boolean(true), *Lisple::Value::boolean(true));
  EXPECT_EQ(*Lisple::number(42), *Lisple::Value::number(42));
  EXPECT_EQ(*Lisple::string("thing"), *Lisple::Value::string("thing"));
  EXPECT_EQ(*Lisple::character('x'), *Lisple::Value::character('x'));
  EXPECT_EQ(*Lisple::keyword("kind"), *Lisple::Value::keyword("kind"));
  EXPECT_EQ(*Lisple::symbol("name"), *Lisple::Value::symbol("name"));

  Lisple::sptr_val_v values{Lisple::number(1), Lisple::number(2)};
  EXPECT_EQ(*Lisple::list(values), *Lisple::Value::list(values));
  EXPECT_EQ(*Lisple::vector(values), *Lisple::Value::vector(values));

  Lisple::sptr_val_v entries{Lisple::keyword("name"), Lisple::string("Gonzo")};
  EXPECT_EQ(*Lisple::map(entries), *Lisple::Value::map(entries));
}
