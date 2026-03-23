
#include <gtest/gtest.h>
#include <lisple/lang/base.h>
#include <lisple/runtime.h>

TEST(DefForm, exec_def__string)
{
  // Given
  Lisple::DefForm def;

  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);

  Lisple::ptr_exec_node_v args;
  Lisple::ExecNode var_name(Lisple::RTValue::symbol("var-name"));
  Lisple::ExecNode str_value(Lisple::RTValue::string("Var Value"));
  args.push_back(&var_name);
  args.push_back(&str_value);

  // When
  def.execnode_def(ctx, args);

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
  std::cout << "------------ DEFINE my-var ------------------" << std::endl;
  Lisple::sptr_sobject defined = runtime.eval("(def my-var {:a 1000 :b 2000})");
  std::cout << "------------ COMPLETE - DEFINE my-var--------" << std::endl;

  // Then
  ASSERT_EQ(defined->to_string(), "{:a 1000 :b 2000}");
  if (auto* wrapped = dynamic_cast<Lisple::RuntimeValueWrapper*>(defined.get()))
  {
    std::cout << "Defined Value is RuntimeValueWrapper, RTValue(" << wrapped->val.get()
              << ")" << std::endl;
  }
  else
  {
    std::cout << "Defined Value is NOT RuntimeValueWrapper" << std::endl;
  }
}
