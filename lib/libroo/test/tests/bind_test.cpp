
#include "roo/form.h"
#include "roo/runtime/value.h"

#include <roo/bind.h>
#include <roo/runtime.h>

#include "gmock/gmock.h"
#include <gtest/gtest.h>

using namespace ::testing;

TEST(SymbolBinding, apply)
{
  // Given
  Roo::Scope scope;
  Roo::SymbolBinding arg("muffin");
  Roo::sptr_val val = Roo::Value::keyword("blooper");

  // When
  arg.apply(scope, val);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 1);
  ASSERT_TRUE(scope.has("muffin"));
  ASSERT_EQ(*scope.lookup("muffin"), *Roo::Value::keyword("blooper"));
}

TEST(RestBinding, apply_collects_values_into_vector)
{
  // Given
  Roo::Scope scope;
  Roo::RestBinding binding("rest");
  Roo::sptr_val_v values = {Roo::Value::number(1),
                            Roo::Value::number(2),
                            Roo::Value::number(3)};

  // When
  binding.apply(scope, values);

  // Then
  auto bound = scope.lookup("rest");
  ASSERT_EQ(*bound,
            *Roo::Value::vector(
              {Roo::Value::number(1), Roo::Value::number(2), Roo::Value::number(3)}));
}

TEST(RestBinding, apply_empty_values_binds_empty_vector)
{
  // Given
  Roo::Scope scope;
  Roo::RestBinding binding("rest");

  // When
  binding.apply(scope, {});

  // Then
  auto bound = scope.lookup("rest");
  ASSERT_EQ(*bound, *Roo::Value::vector({}));
}

TEST(MapDestructureBinding, invalid_binding_form_throws_exception)
{
  // Given
  Roo::sptr_val bind_form = Roo::Value::map({Roo::Value::keyword("keys"),
                                             Roo::Value::map({// This should be vector
                                                              Roo::Value::symbol("var1"),
                                                              Roo::Value::symbol("var2")})});

  // When / Then
  try
  {
    Roo::MapDestructureBinding binding{std::get<Roo::sptr_val_v>(bind_form->value)};

    FAIL() << "Expected TypeError to be thrown for destructuring binding: "
           << bind_form->to_string();
  }
  catch (Roo::TypeError& e)
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
  Roo::Scope scope;

  Roo::sptr_val map = Roo::Value::map({Roo::Value::keyword("source"),
                                       Roo::Value::symbol("the-thing"),
                                       Roo::Value::keyword("target"),
                                       Roo::Value::string("The Thang")});

  Roo::MapDestructureBinding arg{
    {Roo::Value::keyword("keys"),
     Roo::Value::vector({Roo::Value::symbol("source"), Roo::Value::symbol("target")})}};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 2);
  ASSERT_TRUE(scope.has("source"));
  ASSERT_TRUE(scope.has("target"));
  ASSERT_EQ(*scope.lookup(*Roo::Value::symbol("source")), *Roo::Value::symbol("the-thing"));
  ASSERT_EQ(*scope.lookup("target"), *Roo::Value::string("The Thang"));
}

TEST(MapDestructureBinding, apply__keys_and_alias)
{
  // Given
  Roo::Scope scope;

  Roo::sptr_val map = Roo::Value::map({Roo::Value::keyword("source"),
                                       Roo::Value::symbol("the-thing"),
                                       Roo::Value::keyword("target"),
                                       Roo::Value::string("The Thang")});

  Roo::MapDestructureBinding arg{
    {Roo::Value::keyword("keys"),
     Roo::Value::vector({Roo::Value::symbol("source"), Roo::Value::symbol("target")}),
     Roo::Value::keyword("as"),
     Roo::Value::symbol("context")}};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 3);
  ASSERT_TRUE(scope.has("source"));
  ASSERT_TRUE(scope.has("target"));
  ASSERT_TRUE(scope.has("context"));
  ASSERT_EQ(*scope.lookup("source"), *Roo::Value::symbol("the-thing"));
  ASSERT_EQ(*scope.lookup("target"), *Roo::Value::string("The Thang"));
  ASSERT_EQ(*scope.lookup("context"), *map);
}
