
#include "lisple/runtime/value.h"

#include <memory>
#include <stdexcept>

#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
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


using Executable = LispleTest::RuntimeTestFixture;
using UserFunction = LispleTest::RuntimeTestFixture;
using Macro = LispleTest::RuntimeTestFixture;
using create_function__RTValue_semantics = LispleTest::RuntimeTestFixture;
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

TEST_F(UserFunction, invocation_of_empty_function_returns_nil)
{
  // Given
  runtime.eval("(defun my-fn [arg])");

  auto fn = runtime.lookup_value("my-fn");

  Lisple::sptr_rtval_v args = {Lisple::RTValue::string("A string!")};

  // When
  auto retval = fn->exec().execute(ctx, args);

  // Then
  ASSERT_EQ(*retval, *Lisple::Constant::NIL);
}

TEST_F(Macro, get_signature__sig_with_varargs__array__form)
{
  // Given
  Lisple::DoTimesForm dotimes;

  Lisple::uptr_exec_node_v array__form;
  array__form.push_back(std::make_unique<Lisple::ExecNode>(Lisple::RTValue::vector(
    {Lisple::RTValue::symbol("n"), Lisple::RTValue::number(4)})));
  array__form.push_back(std::make_unique<Lisple::ExecNode>(Lisple::RTValue::number(12)));

  // When
  Lisple::Signature* sig = dotimes.get_signature(ctx, array__form);

  // Then
  ASSERT_NE(sig, nullptr);

  ASSERT_EQ(sig->get_arguments().size(), 2);
  EXPECT_EQ(sig->to_string(), "[Vector, <any>...]");
}

TEST_F(create_function__RTValue_semantics, function_should_support_rtvalue_execution)
{
  // Given
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
