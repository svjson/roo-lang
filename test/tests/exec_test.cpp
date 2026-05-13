
#include "lisple/runtime/value.h"

#include <memory>
#include <stdexcept>

#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/host.h>
#include <lisple/lang.h>
#include <lisple/lang/base.h>
#include <lisple/lang/loop.h>
#include <lisple/lang/operator.h>
#include <lisple/lang/seq_func.h>
#include <lisple/runtime.h>
#include <lisple/type.h>

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <gtest/gtest-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using namespace ::testing;

TEST(NamedArgument, apply)
{
  // Given
  Lisple::Scope scope;
  Lisple::NamedArgumentBinding arg("muffin");
  Lisple::sptr_sobject val = std::make_unique<Lisple::Key>("blooper");

  // When
  arg.apply(scope, val);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 1);
  ASSERT_TRUE(scope.has(Lisple::Word("muffin")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("muffin")), Lisple::Key("blooper"));
}

TEST(DestructuringArgumentBinding, invalid_binding_form_throws_exception)
{
  // Given
  Lisple::Runtime runtime;

  auto arg_form = Lisple::Map::make(
    {Lisple::sptr_sobject_v{Lisple::Key::make("keys"),
                            Lisple::Map::make({// Invalid - this should be Array
                                               Lisple::Word::make("var1"),
                                               Lisple::Word::make("var2")})}});

  // When / Then
  try
  {
    Lisple::DestructuringArgumentBinding binding{*arg_form};

    FAIL() << "Expected TypeError to be thrown for destructuring binding: "
           << arg_form->to_string();
  }
  catch (Lisple::TypeError& e)
  {
    EXPECT_THAT(std::string{e.what()},
                HasSubstr("Invalid destructuring form: {:keys {var1 var2}}"));
  }
  catch (std::runtime_error& e)
  {
    FAIL() << "Expected TypeError exception, but got something else: " << e.what();
  }
}

TEST(DestructuringArgumentBinding, apply__keys_only)
{
  // Given
  Lisple::Scope scope;

  Lisple::sptr_sobject map = std::make_shared<Lisple::Map>(
    Lisple::sptr_sobject_v{std::make_shared<Lisple::Key>("source"),
                           std::make_shared<Lisple::Word>("The Thing"),
                           std::make_shared<Lisple::Key>("target"),
                           std::make_shared<Lisple::Word>("The Thang")});

  Lisple::Map arg_map{
    Lisple::sptr_sobject_v{std::make_shared<Lisple::Key>("keys"),
                           std::make_shared<Lisple::Array>(Lisple::sptr_sobject_v{
                             std::make_shared<Lisple::Word>("source"),
                             std::make_shared<Lisple::Word>("target")})}};
  Lisple::DestructuringArgumentBinding arg{arg_map};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 2);
  ASSERT_TRUE(scope.has(Lisple::Word("source")));
  ASSERT_TRUE(scope.has(Lisple::Word("target")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("source")), Lisple::Word("The Thing"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("target")), Lisple::Word("The Thang"));
}

TEST(DestructuringArgumentBinding, apply__keys_and_alias)
{
  // Given
  Lisple::Scope scope;

  Lisple::sptr_sobject map =
    Lisple::Map::make({std::make_shared<Lisple::Key>("source"),
                       std::make_shared<Lisple::Word>("The Thing"),
                       std::make_shared<Lisple::Key>("target"),
                       std::make_shared<Lisple::Word>("The Thang")});

  Lisple::Map arg_map({std::make_shared<Lisple::Key>("keys"),
                       Lisple::Array::make({std::make_shared<Lisple::Word>("source"),
                                            std::make_shared<Lisple::Word>("target")}),
                       std::make_shared<Lisple::Key>("as"),
                       std::make_shared<Lisple::Word>("context")});

  Lisple::DestructuringArgumentBinding arg{arg_map};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 3);
  ASSERT_TRUE(scope.has(Lisple::Word("source")));
  ASSERT_TRUE(scope.has(Lisple::Word("target")));
  ASSERT_TRUE(scope.has(Lisple::Word("context")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("source")), Lisple::Word("The Thing"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("target")), Lisple::Word("The Thang"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("context")), *map);
}

TEST(Executable, invocation_with_incorrect_argument_types_throws_exception)
{
  // Given
  Lisple::Runtime runtime;

  // When
  std::string msg;
  try
  {
    runtime.eval(R"((+ "not-a-number" 4))");
  }
  catch (std::exception& e)
  {
    msg = e.what();
  }

  // Then
  EXPECT_THAT(msg, HasSubstr("Could not apply args"));
}

TEST(UserFunction, invocation_of_empty_function_returns_nil)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun my-fn [arg])");

  auto fn = runtime.lookup_value("my-fn");

  Lisple::sptr_rtval_v args = {Lisple::RTValue::string("A string!")};
  Lisple::Context ctx(runtime);

  // When
  auto retval = fn->exec().execute(ctx, args);

  // Then
  ASSERT_EQ(*retval, *Lisple::Constant::NIL);
}

TEST(Macro, get_signature__sig_with_varargs__array__form)
{
  // Given
  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);
  Lisple::DoTimesForm dotimes;

  Lisple::sptr_sobject_v array__form{
    Lisple::Array::make({Lisple::Word::make("n"), Lisple::Number::make(4)}),
    Lisple::List::make(
      {Lisple::Word::make("+"), Lisple::Number::make(2), Lisple::Number::make(10)})};

  // When
  Lisple::Signature* sig = dotimes.get_signature(ctx, array__form);

  // Then
  ASSERT_NE(sig, nullptr);

  ASSERT_EQ(sig->get_arguments().size(), 2);
  EXPECT_EQ(sig->to_string(), "[Vector, <any>...]");
}

TEST(create_function__RTValue_semantics, function_should_support_rtvalue_execution)
{
  // Given
  Lisple::Runtime runtime;

  std::cout << " ---------- TEST BEGINS ---------------" << std::endl;

  Lisple::sptr_rtval_v param_array{Lisple::RTValue::symbol("n")};
  auto body_node = std::make_unique<Lisple::ExecNode>(Lisple::RTValue::number(10));
  Lisple::ptr_exec_node_v body = {body_node.get()};

  // When
  auto func = Lisple::create_function(&runtime.get_current_namespace(), param_array, body);

  // Then
  EXPECT_FALSE(func->supports_exec_tree());
  EXPECT_EQ(func->get_signatures().size(), 1);
  EXPECT_TRUE(func->get_signatures()[0]->supports_rt_value());
}
