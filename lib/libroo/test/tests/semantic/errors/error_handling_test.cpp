#include <exception>
#include <string>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/namespace.h>
#include <roo/runtime.h>

#include "runtime_fixture.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using Semantic_ErrorHandling = RooTest::RuntimeTestFixture;

/**
 * Documents the semantic exception category, condition, facts and enrichment
 * frames exposed across libroo's main throw sites and call-into-Roo paths.
 * Exact text is pinned where it is part of the user-facing renderer contract;
 * structural assertions remain authoritative for programmatic behavior.
 */

namespace
{
  void expect_call_frame(const Roo::DiagnosticFrame& frame,
                         const std::string& operation,
                         const std::string& subject,
                         const std::string& source = "")
  {
    EXPECT_EQ(frame.kind, Roo::DiagnosticFrameKind::CALL);
    EXPECT_EQ(frame.operation, operation);
    EXPECT_EQ(frame.subject, subject);
    EXPECT_EQ(frame.source, source);
  }

  void expect_resource_frame(const Roo::DiagnosticFrame& frame,
                             const std::string& operation,
                             const std::string& subject)
  {
    EXPECT_EQ(frame.kind, Roo::DiagnosticFrameKind::RESOURCE);
    EXPECT_EQ(frame.operation, operation);
    EXPECT_EQ(frame.subject, subject);
  }
} // namespace

TEST_F(Semantic_ErrorHandling, unmatched_call_arity_reports_invocation_exception)
{
  // Given
  runtime.eval("(defun add [a b] (+ a b))");

  // When
  std::string message;
  try
  {
    runtime.eval("(add 1 2 3)");
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::INVOCATION);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NO_MATCHING_SIGNATURE);
    ASSERT_EQ(diagnostic.facts.arguments.size(), 3);
    ASSERT_EQ(diagnostic.facts.expected_signatures.size(), 1);
    EXPECT_EQ(diagnostic.facts.expected_signatures[0], "[<any>, <any>]");
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "add", "<eval>:1:1");
    ASSERT_TRUE(diagnostic.frames[0].arguments);
    EXPECT_EQ(diagnostic.frames[0].arguments->size(), 3);
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while calling add at <eval>:1:1:\n"
            "No matching signature for add: [1 2 3]\n"
            "\n"
            "Expected:\n"
            "  [<any>, <any>]\n"
            "Received:\n"
            "  [<number> <number> <number>]\n");
}

TEST_F(Semantic_ErrorHandling, large_nested_argument_is_truncated_not_dumped_in_full)
{
  // Given
  runtime.eval("(defun add [a b] (+ a b))");

  // When: 4 arguments never match add's 2-arg signature, so this reports
  // add's own "no matching signature" directly (no nested call exception).
  std::string message;
  try
  {
    runtime.eval("(add 1 2 (range 0 100000) "
                 "{:a 1 :b 2 :c 3 :d 4 :e 5 :f 6 :g 7 :h 8 :i 9 :j 10 :k 11})");
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NO_MATCHING_SIGNATURE);
    EXPECT_EQ(diagnostic.facts.callee, "#'user/add");
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "add", "<eval>:1:1");
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then: bounded (max-elements truncation, "..." marker), single line - a
  // 100000-element vector must never be walked in full just to report a
  // signature mismatch.
  EXPECT_THAT(
    message,
    ::testing::HasSubstr("No matching signature for add: "
                         "[1 2 [0 1 2 3 4 5 6 7 8 9 ...] "
                         "{:a 1 :b 2 :c 3 :d 4 :e 5 :f 6 :g 7 :h 8 :i 9 :j 10 ...}]"));
  EXPECT_THAT(message, ::testing::Not(::testing::HasSubstr("99999")));
}

TEST_F(Semantic_ErrorHandling, wrong_argument_type_reports_type_error)
{
  // When
  std::string message;
  try
  {
    runtime.eval("(assoc-in {} 5 :x)");
    FAIL() << "Expected Roo::TypeError to be thrown.";
  }
  catch (const Roo::TypeError& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::TYPE);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::MESSAGE);
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "assoc-in", "<eval>:1:1");
    ASSERT_TRUE(diagnostic.frames[0].arguments);
    EXPECT_EQ(diagnostic.frames[0].arguments->size(), 3);
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::TypeError, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while calling assoc-in with arguments [{} 5 :x] at <eval>:1:1:\n"
            "Path for assoc-in must be a sequence, got: 5");
}

TEST_F(Semantic_ErrorHandling,
       argument_evaluation_failure_does_not_record_partial_outer_arguments)
{
  try
  {
    runtime.eval("(replace [1 2] (assoc-in {} 5 :x) 1 [])");
    FAIL() << "Expected Roo::TypeError to be thrown.";
  }
  catch (const Roo::TypeError& e)
  {
    const auto& frames = e.get_diagnostic().frames;
    ASSERT_EQ(frames.size(), 2);

    expect_call_frame(frames[0], "calling", "assoc-in", "<eval>:1:16");
    ASSERT_TRUE(frames[0].arguments);
    EXPECT_EQ(frames[0].arguments->size(), 3);

    expect_call_frame(frames[1], "calling", "replace", "<eval>:1:1");
    EXPECT_FALSE(frames[1].arguments);
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::TypeError, got a different exception: " << e.what();
  }
}

TEST_F(Semantic_ErrorHandling, higher_order_callback_records_its_invocation_arguments)
{
  try
  {
    runtime.eval("(map [{}] assoc-in)");
    FAIL() << "Expected Roo::NoMatchingSignatureException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    const auto& frames = e.get_diagnostic().frames;
    ASSERT_EQ(frames.size(), 2);

    EXPECT_EQ(frames[0].kind, Roo::DiagnosticFrameKind::CALL);
    EXPECT_EQ(frames[0].operation, "calling");
    EXPECT_TRUE(frames[0].subject.empty());
    ASSERT_TRUE(frames[0].target);
    ASSERT_TRUE(frames[0].arguments);
    EXPECT_EQ(frames[0].arguments->size(), 1);

    expect_call_frame(frames[1], "calling", "map", "<eval>:1:1");
    ASSERT_TRUE(frames[1].arguments);
    EXPECT_EQ(frames[1].arguments->size(), 2);
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::NoMatchingSignatureException, got: " << e.what();
  }
}

TEST_F(Semantic_ErrorHandling, unbound_identifier_reports_identifier_exception)
{
  // When
  std::string message;
  try
  {
    runtime.eval("totally-unbound-identifier");
    FAIL() << "Expected Roo::IdentifierException to be thrown.";
  }
  catch (const Roo::IdentifierException& e)
  {
    EXPECT_EQ(e.get_diagnostic().category, Roo::ErrorCategory::IDENTIFIER);
    EXPECT_EQ(e.get_diagnostic().condition, Roo::ErrorCondition::MESSAGE);
    EXPECT_TRUE(e.get_diagnostic().frames.empty());
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::IdentifierException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message, "Unknown identifier: 'totally-unbound-identifier'");
}

TEST_F(Semantic_ErrorHandling, malformed_source_reports_parse_exception)
{
  // When
  std::string message;
  try
  {
    runtime.eval("(defun broken [");
    FAIL() << "Expected Roo::ParseException to be thrown.";
  }
  catch (const Roo::ParseException& e)
  {
    EXPECT_EQ(e.get_diagnostic().category, Roo::ErrorCategory::PARSE);
    EXPECT_EQ(e.get_diagnostic().condition, Roo::ErrorCondition::MESSAGE);
    EXPECT_TRUE(e.get_diagnostic().frames.empty());
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::ParseException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message, "Unexpected end of input");
}

TEST_F(Semantic_ErrorHandling, invalid_namespace_name_reports_namespace_exception)
{
  // When
  std::string message;
  try
  {
    Roo::Namespace ns("not a valid namespace name");
    FAIL() << "Expected Roo::NamespaceException to be thrown.";
  }
  catch (const Roo::NamespaceException& e)
  {
    EXPECT_EQ(e.get_diagnostic().category, Roo::ErrorCategory::NAMESPACE);
    EXPECT_EQ(e.get_diagnostic().condition, Roo::ErrorCondition::MESSAGE);
    EXPECT_TRUE(e.get_diagnostic().frames.empty());
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::NamespaceException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message, "Invalid namespace name: 'not a valid namespace name'");
}

TEST_F(Semantic_ErrorHandling,
       context_call_into_non_executable_value_reports_invocation_exception)
{
  // Given
  runtime.eval("(def not-a-function 42)");
  Roo::sptr_val_v args;

  // When
  std::string message;
  try
  {
    ctx.call("not-a-function", args);
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::InvocationException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::INVOCATION);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NOT_CALLABLE);
    ASSERT_TRUE(diagnostic.facts.target);
    EXPECT_EQ(diagnostic.facts.target->type, Roo::Value::Type::NUMBER);
    EXPECT_TRUE(diagnostic.facts.arguments.empty());
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "not-a-function");
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while calling not-a-function:\n"
            "42 is not callable. Arguments: []");
}

TEST_F(Semantic_ErrorHandling,
       runtime_invoke_into_non_executable_value_reports_invocation_exception)
{
  // Given
  runtime.eval("(def not-a-function 42)");
  Roo::sptr_val_v args;

  // When
  std::string message;
  try
  {
    runtime.invoke("not-a-function", args);
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::InvocationException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::INVOCATION);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NOT_CALLABLE);
    ASSERT_TRUE(diagnostic.facts.target);
    EXPECT_EQ(diagnostic.facts.target->type, Roo::Value::Type::NUMBER);
    EXPECT_TRUE(diagnostic.facts.arguments.empty());
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "invoking", "not-a-function");
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while invoking not-a-function:\n"
            "42 is not callable. Arguments: []");
}

TEST_F(Semantic_ErrorHandling,
       context_call_user_function_arity_mismatch_reports_invocation_exception)
{
  // Given
  runtime.eval("(defun add [a b] (+ a b))");
  Roo::sptr_val_v args{Roo::Value::number(1), Roo::Value::number(2), Roo::Value::number(3)};

  // When
  std::string message;
  try
  {
    ctx.call("add", args);
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::INVOCATION);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NO_MATCHING_SIGNATURE);
    EXPECT_EQ(diagnostic.facts.callee, "#'user/add");
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "add");
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while calling add:\n"
            "No matching signature for add: [1 2 3]\n"
            "\n"
            "Expected:\n"
            "  [<any>, <any>]\n"
            "Received:\n"
            "  [<number> <number> <number>]\n");
}

TEST_F(Semantic_ErrorHandling,
       context_call_native_function_arity_mismatch_reports_invocation_exception)
{
  // Given
  Roo::sptr_val_v args{Roo::Value::number(1), Roo::Value::number(2)};

  // When
  std::string message;
  try
  {
    ctx.call("count", args);
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NO_MATCHING_SIGNATURE);
    EXPECT_EQ(diagnostic.facts.callee, "<fn>");
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "count");
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while calling count:\n"
            "No matching signature for count: [1 2]\n"
            "\n"
            "Expected:\n"
            "  [<any>]\n"
            "Received:\n"
            "  [<number> <number>]\n");
}

TEST_F(Semantic_ErrorHandling,
       context_call_captured_lambda_arity_mismatch_reports_invocation_exception)
{
  // Given
  runtime.eval("(def my-lambda (fn [a b] (+ a b)))");
  Roo::sptr_val_v args{Roo::Value::number(1)};

  // When
  std::string message;
  try
  {
    ctx.call("my-lambda", args);
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::NoMatchingSignatureException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NO_MATCHING_SIGNATURE);
    EXPECT_EQ(diagnostic.facts.callee, "#'user/<lambda>");
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "my-lambda");
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::NoMatchingSignatureException, got: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while calling my-lambda:\n"
            "No matching signature for my-lambda: [1]\n"
            "\n"
            "Expected:\n"
            "  [<any>, <any>]\n"
            "Received:\n"
            "  [<number>]\n");
}

TEST_F(Semantic_ErrorHandling, context_call_preserves_type_error_and_adds_call_frame)
{
  // Given
  Roo::sptr_val_v args{Roo::Value::map({}), Roo::Value::number(5), Roo::Value::keyword("x")};

  // When
  std::string message;
  try
  {
    ctx.call("assoc-in", args);
    FAIL() << "Expected an exception to be thrown.";
  }
  catch (const Roo::TypeError& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::TYPE);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::MESSAGE);
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "assoc-in");
    ASSERT_TRUE(diagnostic.frames[0].arguments);
    EXPECT_EQ(diagnostic.frames[0].arguments->size(), 3);
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::TypeError, got: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while calling assoc-in with arguments [{} 5 :x]:\n"
            "Path for assoc-in must be a sequence, got: 5");
}

TEST_F(Semantic_ErrorHandling, runtime_invoke_preserves_type_error_and_adds_call_frame)
{
  // Given
  Roo::sptr_val_v args{Roo::Value::map({}), Roo::Value::number(5), Roo::Value::keyword("x")};

  // When
  std::string message;
  try
  {
    runtime.invoke("assoc-in", args);
    FAIL() << "Expected an exception to be thrown.";
  }
  catch (const Roo::TypeError& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::TYPE);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::MESSAGE);
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "invoking", "assoc-in");
    ASSERT_TRUE(diagnostic.frames[0].arguments);
    EXPECT_EQ(diagnostic.frames[0].arguments->size(), 3);
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error while invoking assoc-in with arguments [{} 5 :x]:\n"
            "Path for assoc-in must be a sequence, got: 5");
}

TEST_F(Semantic_ErrorHandling,
       invalid_form_reports_its_source_without_synthetic_call_frames)
{
  // When
  std::string message;
  try
  {
    runtime.eval("(defun broken []\n"
                 "  (let [value {}]\n"
                 "    (update value :missing)))",
                 "broken.roo");
    FAIL() << "Expected an exception to be thrown.";
  }
  catch (const Roo::InvalidFormException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::FORM);
    ASSERT_TRUE(diagnostic.site);
    EXPECT_EQ(diagnostic.site->subject, "update");
    EXPECT_EQ(diagnostic.site->source, "broken.roo:3:5");
    EXPECT_TRUE(diagnostic.frames.empty());
    message = e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Invalid update form at broken.roo:3:5:\n"
            "Missing updater for key/path ':missing'.");
}

TEST_F(Semantic_ErrorHandling,
       runtime_read_file_preserves_type_error_and_adds_call_and_resource_frames)
{
  // Given
  fs.add_file("broken.roo", "(assoc-in {} 5 :x)");

  // When
  std::string message;
  try
  {
    runtime.read_file("broken.roo");
    FAIL() << "Expected an exception to be thrown.";
  }
  catch (const Roo::TypeError& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::TYPE);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::MESSAGE);
    ASSERT_EQ(diagnostic.frames.size(), 2);
    expect_call_frame(diagnostic.frames[0], "calling", "assoc-in", "broken.roo:1:1");
    ASSERT_TRUE(diagnostic.frames[0].arguments);
    EXPECT_EQ(diagnostic.frames[0].arguments->size(), 3);
    expect_resource_frame(diagnostic.frames[1], "reading", "broken.roo");
    message = e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Error reading 'broken.roo': Error while calling assoc-in with arguments "
            "[{} 5 :x] at broken.roo:1:1:\n"
            "Path for assoc-in must be a sequence, got: 5");
}

TEST_F(Semantic_ErrorHandling, nil_invocation_records_target_and_arguments)
{
  try
  {
    runtime.eval("(let [callable nil] (callable 1 2))");
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::InvocationException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::INVOCATION);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::NOT_CALLABLE);
    ASSERT_TRUE(diagnostic.facts.target);
    EXPECT_EQ(diagnostic.facts.target->type, Roo::Value::Type::NIL);
    ASSERT_EQ(diagnostic.facts.arguments.size(), 2);
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "callable", "<eval>:1:21");
    ASSERT_TRUE(diagnostic.frames[0].arguments);
    EXPECT_EQ(diagnostic.frames[0].arguments->size(), 2);
    EXPECT_EQ(e.what(),
              std::string("Error while calling callable at <eval>:1:21:\n"
                          "Cannot invoke nil with arguments: [1 2]"));
  }
}

TEST_F(Semantic_ErrorHandling, keyword_arity_records_target_arguments_and_expectation)
{
  try
  {
    runtime.eval("(let [key :value] (key {} {}))");
    FAIL() << "Expected Roo::InvocationException to be thrown.";
  }
  catch (const Roo::InvocationException& e)
  {
    const auto& diagnostic = e.get_diagnostic();
    EXPECT_EQ(diagnostic.category, Roo::ErrorCategory::INVOCATION);
    EXPECT_EQ(diagnostic.condition, Roo::ErrorCondition::ARGUMENT_MISMATCH);
    ASSERT_TRUE(diagnostic.facts.target);
    EXPECT_EQ(diagnostic.facts.target->type, Roo::Value::Type::KEYWORD);
    ASSERT_TRUE(diagnostic.facts.expected_arity);
    EXPECT_EQ(*diagnostic.facts.expected_arity, 1);
    ASSERT_EQ(diagnostic.facts.arguments.size(), 2);
    ASSERT_EQ(diagnostic.frames.size(), 1);
    expect_call_frame(diagnostic.frames[0], "calling", "key", "<eval>:1:19");
    ASSERT_TRUE(diagnostic.frames[0].arguments);
    EXPECT_EQ(diagnostic.frames[0].arguments->size(), 2);
    EXPECT_EQ(e.what(),
              std::string("Error while calling key at <eval>:1:19:\n"
                          "Keyword :value expects exactly 1 argument, got 2: [{} {}]"));
  }
}
