
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
  Lisple::sptr_rtval val = Lisple::RTValue::keyword("blooper");

  // When
  arg.apply(scope, val);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 1);
  ASSERT_TRUE(scope.has(Lisple::Word("muffin")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("muffin")), Lisple::Key("blooper"));
}

TEST(MapDestructureBinding, invalid_binding_form_throws_exception)
{
  // Given
  Lisple::sptr_rtval bind_form =
    Lisple::RTValue::map({Lisple::RTValue::keyword("keys"),
                          Lisple::RTValue::map({// This should be array
                                                Lisple::RTValue::symbol("var1"),
                                                Lisple::RTValue::symbol("var2")})});

  // When / Then
  try
  {
    Lisple::MapDestructureBinding binding{std::get<Lisple::sptr_rtval_v>(bind_form->value)};

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

  Lisple::sptr_rtval map = Lisple::RTValue::map({Lisple::RTValue::keyword("source"),
                                                 Lisple::RTValue::symbol("the-thing"),
                                                 Lisple::RTValue::keyword("target"),
                                                 Lisple::RTValue::string("The Thang")});

  Lisple::MapDestructureBinding arg{
    {Lisple::RTValue::keyword("keys"),
     Lisple::RTValue::vector(
       {Lisple::RTValue::symbol("source"), Lisple::RTValue::symbol("target")})}};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 2);
  ASSERT_TRUE(scope.has(Lisple::Word("source")));
  ASSERT_TRUE(scope.has(Lisple::Word("target")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("source")), Lisple::Word("the-thing"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("target")), Lisple::String("The Thang"));
}

TEST(MapDestructureBinding, apply__keys_and_alias)
{
  // Given
  Lisple::Scope scope;

  Lisple::sptr_rtval map = Lisple::RTValue::map({Lisple::RTValue::keyword("source"),
                                                 Lisple::RTValue::symbol("the-thing"),
                                                 Lisple::RTValue::keyword("target"),
                                                 Lisple::RTValue::string("The Thang")});

  Lisple::MapDestructureBinding arg{
    {Lisple::RTValue::keyword("keys"),
     Lisple::RTValue::vector(
       {Lisple::RTValue::symbol("source"), Lisple::RTValue::symbol("target")}),
     Lisple::RTValue::keyword("as"),
     Lisple::RTValue::symbol("context")}};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 3);
  ASSERT_TRUE(scope.has(Lisple::Word("source")));
  ASSERT_TRUE(scope.has(Lisple::Word("target")));
  ASSERT_TRUE(scope.has(Lisple::Word("context")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("source")), Lisple::Word("the-thing"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("target")), Lisple::String("The Thang"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("context")), Lisple::RuntimeValueWrapper(map));
}
