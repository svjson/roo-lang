
#include <lisple/lang/base.h>
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(DefForm, exec_def__string)
{
  // Given
  Lisple::Runtime runtime;

  // When
  runtime.eval("(def var-name \"Var Value\")");

  // Then
  auto obj = runtime.get_current_namespace().lookup(Lisple::Word("var-name"));
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefForm, define_var__ast_legacy_impl)
{
  // Given
  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);

  Lisple::sptr_sobject_v args;
  args.push_back(std::make_shared<Lisple::Word>("var-name"));
  args.push_back(std::make_shared<Lisple::String>("Var Value"));
  Lisple::DefForm def;

  // When
  def.execute(ctx, args);

  // Then
  auto obj = runtime.get_current_namespace().lookup(Lisple::Word("var-name"));
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefForm, define_var_with_gt_and_lt__ast_legacy_impl)
{
  // Given
  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);

  Lisple::sptr_sobject_v args;
  args.push_back(std::make_shared<Lisple::Word>("<var-name>"));
  args.push_back(std::make_shared<Lisple::String>("Value"));
  Lisple::DefForm def;

  // When
  def.execute(ctx, args);

  // Then
  auto obj = runtime.get_current_namespace().lookup(Lisple::Word("<var-name>"));
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefForm, def_returns_defined_value)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto defined = runtime.eval("(def my-var {:a 1000 :b 2000})");

  // Then
  ASSERT_EQ(defined->to_string(), "{:a 1000 :b 2000}");
}
