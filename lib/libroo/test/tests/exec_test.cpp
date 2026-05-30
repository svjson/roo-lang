
#include "roo/runtime/value.h"

#include <memory>
#include <stdexcept>

#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/lang.h>
#include <roo/lang/base.h>
#include <roo/lang/loop.h>
#include <roo/lang/operator.h>
#include <roo/lang/seq_func.h>
#include <roo/runtime.h>
#include <roo/type.h>

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <gtest/gtest-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using Executable = RooTest::RuntimeTestFixture;
using UserFunction = RooTest::RuntimeTestFixture;
using Macro = RooTest::RuntimeTestFixture;
using create_function__Value_semantics = RooTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(Executable, invocation_with_incorrect_argument_types_throws_exception)
{
  // Given
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

TEST_F(Executable, source_diagnostics_are_disabled_by_default)
{
  // Given
  std::string msg;

  // When
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
  EXPECT_THAT(msg, Not(HasSubstr("Error while calling +")));
  EXPECT_THAT(msg, Not(HasSubstr("<eval>:1:1")));
}

TEST_F(Executable, source_diagnostics_include_callee_and_location)
{
  // Given
  runtime.set_source_diagnostics(true);
  std::string msg;

  // When
  try
  {
    runtime.eval(R"((+ "not-a-number" 4))");
  }
  catch (std::exception& e)
  {
    msg = e.what();
  }

  // Then
  EXPECT_THAT(msg, HasSubstr("Error while calling + at <eval>:1:1"));
  EXPECT_THAT(msg, HasSubstr("Could not apply args"));
}

TEST_F(Executable, call_stack_diagnostics_include_nested_call_context)
{
  // Given
  runtime.set_call_stack_diagnostics(true);
  std::string msg;

  // When
  try
  {
    runtime.eval("(defun broken [] (+ \"bad\" 1))\n(broken)");
  }
  catch (std::exception& e)
  {
    msg = e.what();
  }

  // Then
  EXPECT_THAT(msg, HasSubstr("Error while calling broken at <eval>:2:1"));
  EXPECT_THAT(msg, HasSubstr("Error while calling + at <eval>:1:18"));
  EXPECT_THAT(msg, HasSubstr("Could not apply args"));
}

TEST_F(UserFunction, invocation_of_empty_function_returns_nil)
{
  // Given
  runtime.eval("(defun my-fn [arg])");

  auto fn = runtime.lookup("my-fn");

  Roo::sptr_val_v args = {Roo::Value::string("A string!")};

  // When
  auto retval = fn->exec().execute(ctx, args);

  // Then
  ASSERT_EQ(*retval, *Roo::Constant::NIL);
}

TEST_F(Macro, get_signature__sig_with_varargs__vector__form)
{
  // Given
  Roo::DoTimesForm dotimes;

  Roo::uptr_exec_node_v vector__form;
  vector__form.push_back(std::make_unique<Roo::ExecNode>(
    Roo::Value::vector({Roo::Value::symbol("n"), Roo::Value::number(4)})));
  vector__form.push_back(std::make_unique<Roo::ExecNode>(Roo::Value::number(12)));

  // When
  Roo::Signature* sig = dotimes.get_signature(ctx, vector__form);

  // Then
  ASSERT_NE(sig, nullptr);

  ASSERT_EQ(sig->get_arguments().size(), 2);
  EXPECT_EQ(sig->to_string(), "[Vector, <any>...]");
}

TEST_F(create_function__Value_semantics, function_should_support_value_execution)
{
  // Given
  std::cout << " ---------- TEST BEGINS ---------------" << std::endl;

  Roo::sptr_val_v param_vector{Roo::Value::symbol("n")};
  auto body_node = std::make_unique<Roo::ExecNode>(Roo::Value::number(10));
  Roo::ptr_exec_node_v body = {body_node.get()};

  // When
  auto func = Roo::create_function(&runtime.get_current_namespace(), param_vector, body);

  // Then
  EXPECT_FALSE(func->supports_exec_tree());
  EXPECT_EQ(func->get_signatures().size(), 1);
  EXPECT_TRUE(func->get_signatures()[0]->supports_rt_value());
}
