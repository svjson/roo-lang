
#include <roo/runtime/value.h>

#include <gtest/gtest.h>

TEST(Value_keyword, equals)
{
  auto kind = Roo::Value::keyword("kind");
  auto kind_ = Roo::Value::keyword("kind");
  auto value = Roo::Value::keyword("value");

  EXPECT_TRUE(*kind == *kind);
  EXPECT_TRUE(*kind == *kind_);
  EXPECT_FALSE(*kind == *value);
  EXPECT_FALSE(*value == *kind);
}

TEST(Value_string, to_string_escapes_edn_string_content)
{
  auto map = Roo::Value::map({Roo::Value::keyword("example"),
                              Roo::Value::string("=> \"source text\"\nnext")});

  EXPECT_EQ(map->to_string(), R"({:example "=> \"source text\"\nnext"})");
}

TEST(ValueFactory, root_namespace_forwards_to_value_factories)
{
  EXPECT_EQ(*Roo::boolean(true), *Roo::Value::boolean(true));
  EXPECT_EQ(*Roo::number(42), *Roo::Value::number(42));
  EXPECT_EQ(*Roo::string("thing"), *Roo::Value::string("thing"));
  EXPECT_EQ(*Roo::character('x'), *Roo::Value::character('x'));
  EXPECT_EQ(*Roo::keyword("kind"), *Roo::Value::keyword("kind"));
  EXPECT_EQ(*Roo::symbol("name"), *Roo::Value::symbol("name"));

  Roo::sptr_val_v values{Roo::number(1), Roo::number(2)};
  EXPECT_EQ(*Roo::list(values), *Roo::Value::list(values));
  EXPECT_EQ(*Roo::vector(values), *Roo::Value::vector(values));

  Roo::sptr_val_v entries{Roo::keyword("name"), Roo::string("Gonzo")};
  EXPECT_EQ(*Roo::map(entries), *Roo::Value::map(entries));
}
