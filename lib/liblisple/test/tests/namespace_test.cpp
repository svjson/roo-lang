
#include <memory>

#include <lisple/form.h>
#include <lisple/namespace.h>

#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using Namespace = LispleTest::RuntimeTestFixture;
TEST_F(Namespace, store_and_lookup)
{
  // Given
  Lisple::Namespace ns("test");
  Lisple::sptr_val obj = Lisple::Value::string("my string");

  // When
  ns.store("myvar", obj);
  Lisple::sptr_val mystring = ns.lookup(*Lisple::Value::symbol("myvar"));

  // Then
  ASSERT_NE(mystring, nullptr);
  ASSERT_EQ(mystring->str(), "my string");
}

TEST_F(Namespace, access_identifier_from_other_ns)
{
  // Given
  runtime.switch_namespace("test-ns.number-one");
  runtime.get_current_namespace().store("my-value", Lisple::Value::number(123));

  // When
  runtime.switch_namespace("test-ns.number-two");
  Lisple::sptr_val result = runtime.lookup("test-ns.number-one/my-value");

  // Then
  ASSERT_EQ(result->to_string(), "123");
}
