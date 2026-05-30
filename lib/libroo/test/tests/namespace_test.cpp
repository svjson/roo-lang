
#include <memory>

#include <roo/form.h>
#include <roo/namespace.h>

#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using Namespace = RooTest::RuntimeTestFixture;
TEST_F(Namespace, store_and_lookup)
{
  // Given
  Roo::Namespace ns("test");
  Roo::sptr_val obj = Roo::Value::string("my string");

  // When
  ns.store("myvar", obj);
  Roo::sptr_val mystring = ns.lookup(*Roo::Value::symbol("myvar"));

  // Then
  ASSERT_NE(mystring, nullptr);
  ASSERT_EQ(mystring->str(), "my string");
}

TEST_F(Namespace, access_identifier_from_other_ns)
{
  // Given
  runtime.switch_namespace("test-ns.number-one");
  runtime.get_current_namespace().store("my-value", Roo::Value::number(123));

  // When
  runtime.switch_namespace("test-ns.number-two");
  Roo::sptr_val result = runtime.lookup("test-ns.number-one/my-value");

  // Then
  ASSERT_EQ(result->to_string(), "123");
}
