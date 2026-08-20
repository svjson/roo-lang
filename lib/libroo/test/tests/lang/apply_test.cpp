
#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ApplyFunction = RooTest::RuntimeTestFixture;
TEST_F(ApplyFunction, apply_concat)
{
  // Given
  auto retval = runtime.eval("(apply concat [[1 2 3] [4 5 6]])");

  // Then
  ASSERT_EQ(retval->to_string(), "[1 2 3 4 5 6]");
}

TEST_F(ApplyFunction, apply_dynamic)
{
  // Given
  runtime.eval("(def function-name 'max)");
  auto result = runtime.eval("(apply (resolve function-name) [10 8 17 4 0])");

  // Then
  ASSERT_EQ(result->to_string(), "17");
}

TEST_F(ApplyFunction, applies_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((apply str "ab"))")->to_string(), R"("ab")");
}

TEST_F(ApplyFunction, applies_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(apply vector {:a 1})")->to_string(), "[:a 1]");
}

TEST_F(ApplyFunction, applies_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(apply + values)")->to_string(), "6");
}

TEST_F(ApplyFunction, nil_callee_is_rejected_by_apply)
{
  runtime.set_call_stack_diagnostics(true);

  try
  {
    runtime.eval("(apply nil [1 2])");
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::InvocationException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NOT_CALLABLE);
    ASSERT_TRUE(diagnostic.facts.target);
    EXPECT_EQ(diagnostic.facts.target->type, Roo::Value::Type::NIL);
    ASSERT_EQ(diagnostic.facts.arguments.size(), 2);

    ASSERT_EQ(diagnostic.frames.size(), 2);
    const auto& indirect = diagnostic.frames[0];
    EXPECT_EQ(indirect.kind, Roo::DiagnosticFrameKind::CALL);
    EXPECT_EQ(indirect.operation, "applying");
    EXPECT_TRUE(indirect.subject.empty());
    EXPECT_TRUE(indirect.source.empty());
    ASSERT_TRUE(indirect.target);
    EXPECT_EQ(indirect.target->type, Roo::Value::Type::NIL);
    ASSERT_TRUE(indirect.arguments);
    EXPECT_EQ(indirect.arguments->size(), 2);

    const auto& outer = diagnostic.frames[1];
    EXPECT_EQ(outer.operation, "calling");
    EXPECT_EQ(outer.subject, "apply");
    EXPECT_EQ(outer.source, "<eval>:1:1");
    ASSERT_TRUE(outer.arguments);
    EXPECT_EQ(outer.arguments->size(), 2);

    EXPECT_EQ(e.what(),
              std::string("Error while calling apply at <eval>:1:1:\n"
                          "Error while applying nil:\n"
                          "Cannot invoke nil with arguments: [1 2]"));
  }
}

TEST_F(ApplyFunction, callee_signature_failure_names_the_indirect_invocation)
{
  runtime.set_call_stack_diagnostics(true);

  try
  {
    runtime.eval(R"((apply + [1 "x"]))");
    FAIL() << "Expected Roo::NoMatchingSignatureException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NO_MATCHING_SIGNATURE);
    EXPECT_EQ(diagnostic.facts.callee, "<fn>");
    ASSERT_EQ(diagnostic.frames.size(), 2);

    const auto& indirect = diagnostic.frames[0];
    EXPECT_EQ(indirect.operation, "applying");
    EXPECT_TRUE(indirect.subject.empty());
    ASSERT_TRUE(indirect.target);
    EXPECT_EQ(indirect.target->type, Roo::Value::Type::FUNCTION);
    ASSERT_TRUE(indirect.arguments);
    EXPECT_EQ(indirect.arguments->size(), 2);

    const std::string message = e.what();
    EXPECT_NE(message.find("Error while applying <fn>:"), std::string::npos);
    EXPECT_EQ(message.find("Error while applying <fn> with arguments"), std::string::npos);
    EXPECT_NE(message.find("No matching signature for <fn>"), std::string::npos);
    EXPECT_EQ(message.find("No matching signature for apply"), std::string::npos);
  }
}

TEST_F(ApplyFunction, callee_body_failure_retains_the_applied_invocation)
{
  runtime.set_call_stack_diagnostics(true);

  try
  {
    runtime.eval("(defun broken [value] (+ value \"x\"))\n"
                 "(apply broken [1])");
    FAIL() << "Expected Roo::NoMatchingSignatureException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    const auto& frames = e.get_diagnostic().frames;
    ASSERT_EQ(frames.size(), 3);
    EXPECT_EQ(frames[0].subject, "+");
    EXPECT_EQ(frames[1].operation, "applying");
    EXPECT_TRUE(frames[1].subject.empty());
    ASSERT_TRUE(frames[1].target);
    EXPECT_EQ(frames[1].target->to_string(), "#'user/broken");
    ASSERT_TRUE(frames[1].arguments);
    ASSERT_EQ(frames[1].arguments->size(), 1);
    EXPECT_EQ(frames[2].subject, "apply");

    const std::string message = e.what();
    EXPECT_NE(message.find("Error while applying #'user/broken:"), std::string::npos);
    EXPECT_EQ(message.find("Error while applying #'user/broken with arguments"),
              std::string::npos);
    EXPECT_NE(message.find("Error while calling +"), std::string::npos);
  }
}

TEST_F(ApplyFunction, source_only_diagnostics_prefer_the_enclosing_source_frame)
{
  runtime.set_source_diagnostics(true);
  runtime.set_call_stack_diagnostics(false);

  try
  {
    runtime.eval(R"((apply + [1 "x"]))");
    FAIL() << "Expected Roo::NoMatchingSignatureException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    ASSERT_EQ(e.get_diagnostic().frames.size(), 2);
    const std::string message = e.what();
    EXPECT_NE(message.find("Error while calling apply at <eval>:1:1"), std::string::npos);
    EXPECT_EQ(message.find("Error while applying"), std::string::npos);
    EXPECT_NE(message.find("No matching signature for <fn>"), std::string::npos);
    EXPECT_EQ(message.find("No matching signature for apply"), std::string::npos);
  }
}

TEST_F(ApplyFunction, non_callable_callee_is_rejected_by_signature)
{
  EXPECT_THROW(runtime.eval("(apply 42 [1 2])"), Roo::InvocationException);
}
