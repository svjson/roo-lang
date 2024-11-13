
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <memory>

#include <lisple/namespace.h>
#include <lisple/form.h>
#include <lisple/runtime.h>

TEST(Namespace, store_and_lookup)
{
  // Given
  Lisple::Namespace ns("test");
  Lisple::Word identifier("myvar");
  std::shared_ptr<Lisple::Object> obj = std::make_shared<Lisple::String>("my string");

  // When
  ns.store(identifier.value, obj);
  std::shared_ptr<Lisple::Object> mystring = ns.lookup(identifier);

  // Then
  ASSERT_NE(mystring, nullptr);
  ASSERT_EQ(mystring->as<Lisple::String>().value, "my string");
}


TEST(Namespace, access_identifier_from_other_ns)
{
  // Given
  Lisple::Runtime runtime;
  runtime.switch_namespace("test-ns.number-one");
  runtime.get_current_namespace().store(Lisple::Word("my-value"), std::make_shared<Lisple::Number>(123));

  // When
  runtime.switch_namespace("test-ns.number-two");
  Lisple::sptr_sobject result = runtime.lookup(Lisple::Word("test-ns.number-one/my-value"));

  // Then
  ASSERT_EQ(result->to_string(), "123");
}
