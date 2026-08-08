#include <exception>
#include <string>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/namespace.h>
#include <roo/runtime.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Semantic_ErrorHandling = RooTest::RuntimeTestFixture;

/**
 * Documents, by assertion, the exception behavior libroo exposes across its
 * main throw sites and call-into-Roo paths: which exception type is thrown,
 * and the exact message text. Message content is pinned deliberately - if a
 * message string changes, the corresponding test here must fail.
 */

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
  catch (const Roo::InvocationException& e)
  {
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message,
            "Could not apply args: [1 2 3] to signature expecting: [<any>, <any>].");
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
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::TypeError, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message, "Path for assoc-in must be a sequence, got: 5");
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
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message, "42 is not executable.");
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
    message = e.what();
  }
  catch (const std::exception& e)
  {
    FAIL() << "Expected Roo::InvocationException, got a different exception: " << e.what();
  }

  // Then
  EXPECT_EQ(message, "42 is not executable.");
}
