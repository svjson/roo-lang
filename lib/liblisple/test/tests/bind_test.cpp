
#include "lisple/form.h"
#include "lisple/runtime/value.h"

#include <lisple/bind.h>
#include <lisple/runtime.h>

#include "gmock/gmock.h"
#include <gtest/gtest.h>

using namespace ::testing;

TEST(SymbolBinding, apply)
{
  // Given
  Lisple::Scope scope;
  Lisple::SymbolBinding arg("muffin");
  Lisple::sptr_val val = Lisple::Value::keyword("blooper");

  // When
  arg.apply(scope, val);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 1);
  ASSERT_TRUE(scope.has("muffin"));
  ASSERT_EQ(*scope.lookup("muffin"), *Lisple::Value::keyword("blooper"));
}

TEST(RestBinding, apply_collects_values_into_vector)
{
  // Given
  Lisple::Scope scope;
  Lisple::RestBinding binding("rest");
  Lisple::sptr_val_v values = {Lisple::Value::number(1),
                               Lisple::Value::number(2),
                               Lisple::Value::number(3)};

  // When
  binding.apply(scope, values);

  // Then
  auto bound = scope.lookup("rest");
  ASSERT_EQ(
    *bound,
    *Lisple::Value::vector(
      {Lisple::Value::number(1), Lisple::Value::number(2), Lisple::Value::number(3)}));
}

TEST(RestBinding, apply_empty_values_binds_empty_vector)
{
  // Given
  Lisple::Scope scope;
  Lisple::RestBinding binding("rest");

  // When
  binding.apply(scope, {});

  // Then
  auto bound = scope.lookup("rest");
  ASSERT_EQ(*bound, *Lisple::Value::vector({}));
}

TEST(MapDestructureBinding, invalid_binding_form_throws_exception)
{
  // Given
  Lisple::sptr_val bind_form =
    Lisple::Value::map({Lisple::Value::keyword("keys"),
                        Lisple::Value::map({// This should be vector
                                            Lisple::Value::symbol("var1"),
                                            Lisple::Value::symbol("var2")})});

  // When / Then
  try
  {
    Lisple::MapDestructureBinding binding{std::get<Lisple::sptr_val_v>(bind_form->value)};

    FAIL() << "Expected TypeError to be thrown for destructuring binding: "
           << bind_form->to_string();
  }
  catch (Lisple::TypeError& e)
  {
    EXPECT_THAT(std::string{e.what()},
                HasSubstr("Invalid map destructure form: {:keys {var1 var2}}"));
  }
  catch (std::runtime_error& e)
  {
    FAIL() << "Expected TypeError exception, but got something else: " << e.what();
  }
}

TEST(MapDestructureBinding, apply__keys_only)
{
  // Given
  Lisple::Scope scope;

  Lisple::sptr_val map = Lisple::Value::map({Lisple::Value::keyword("source"),
                                             Lisple::Value::symbol("the-thing"),
                                             Lisple::Value::keyword("target"),
                                             Lisple::Value::string("The Thang")});

  Lisple::MapDestructureBinding arg{
    {Lisple::Value::keyword("keys"),
     Lisple::Value::vector(
       {Lisple::Value::symbol("source"), Lisple::Value::symbol("target")})}};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 2);
  ASSERT_TRUE(scope.has("source"));
  ASSERT_TRUE(scope.has("target"));
  ASSERT_EQ(*scope.lookup(*Lisple::Value::symbol("source")),
            *Lisple::Value::symbol("the-thing"));
  ASSERT_EQ(*scope.lookup("target"), *Lisple::Value::string("The Thang"));
}

TEST(MapDestructureBinding, apply__keys_and_alias)
{
  // Given
  Lisple::Scope scope;

  Lisple::sptr_val map = Lisple::Value::map({Lisple::Value::keyword("source"),
                                             Lisple::Value::symbol("the-thing"),
                                             Lisple::Value::keyword("target"),
                                             Lisple::Value::string("The Thang")});

  Lisple::MapDestructureBinding arg{
    {Lisple::Value::keyword("keys"),
     Lisple::Value::vector(
       {Lisple::Value::symbol("source"), Lisple::Value::symbol("target")}),
     Lisple::Value::keyword("as"),
     Lisple::Value::symbol("context")}};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 3);
  ASSERT_TRUE(scope.has("source"));
  ASSERT_TRUE(scope.has("target"));
  ASSERT_TRUE(scope.has("context"));
  ASSERT_EQ(*scope.lookup("source"), *Lisple::Value::symbol("the-thing"));
  ASSERT_EQ(*scope.lookup("target"), *Lisple::Value::string("The Thang"));
  ASSERT_EQ(*scope.lookup("context"), *map);
}
