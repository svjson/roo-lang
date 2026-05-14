
#include <memory>
#include <string>

#include <lisple/context.h>
#include <lisple/dir_root_file_system.h>
#include <lisple/form.h>
#include <lisple/namespace.h>
#include <lisple/runtime.h>
#include <lisple/runtime/seq.h>
#include <lisple/type.h>

#include "runtime_fixture.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using Runtime = LispleTest::RuntimeTestFixture;
const std::string RUNTIME_TEST_DIR = "test_resources/script/runtime_test";

using namespace ::testing;

TEST_F(Runtime, instantiation_vanilla)
{
  // When
  auto& runtime = use_bare_runtime();

  // Then
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "user");
  EXPECT_FALSE(runtime.has_file_system_access());
}

TEST_F(Runtime, instantiation_vanilla_with_file_system)
{
  // Given
  Lisple::DirRootFileSystem fs(".");

  // When
  auto& runtime = use_runtime_with(fs);

  // Then
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "user");
  EXPECT_TRUE(runtime.has_file_system_access());
}

TEST_F(Runtime, instantiation_with_namespace)
{
  // Given
  Lisple::Namespace ns("bonanza");

  // When
  auto& runtime = use_runtime_with(ns);

  // Then
  ASSERT_EQ(runtime.get_current_namespace().get_name(), "bonanza");
  EXPECT_FALSE(runtime.has_file_system_access());
}

TEST_F(Runtime, instantiation_with_multiple_namespaces)
{
  // Given
  std::map<const std::string, Lisple::Namespace> namespaces;
  namespaces.emplace("mustard", Lisple::Namespace("mustard"));
  namespaces.emplace("custard", Lisple::Namespace("custard"));
  namespaces.emplace("flustard", Lisple::Namespace("flustard"));

  // When
  auto& runtime = use_runtime_with("custard", namespaces);

  // Then
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "custard");
  EXPECT_FALSE(runtime.has_file_system_access());
}

TEST_F(Runtime, eval__symbol__lookup)
{
  // Given
  std::shared_ptr<Lisple::AST::ASTNode> symbol =
    std::make_shared<Lisple::AST::Symbol>("my-symbol");
  runtime.get_current_namespace().store("my-symbol", Lisple::RTValue::string("my-string"));

  // When
  auto result = runtime.eval(symbol);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*result));
}

TEST_F(Runtime, eval__symbol__no_lookup)
{
  // Given
  std::shared_ptr<Lisple::AST::ASTNode> symbol =
    std::make_shared<Lisple::AST::Symbol>("my-symbol");
  runtime.get_current_namespace().store("my-symbol", Lisple::RTValue::string("my-string"));

  // When
  ctx.push_context(false);
  auto result = runtime.eval(symbol);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*result));
}

TEST_F(Runtime, eval__quoted_list)
{
  // When
  auto result = runtime.eval("'(these are bare symbols)");

  // Then
  ASSERT_TRUE(Lisple::Type::LIST.is_type_of(*result));

  ASSERT_EQ(Lisple::count(*result), 4);

  EXPECT_EQ(*result->elements().at(0), *Lisple::RTValue::symbol("these"));
  EXPECT_EQ(*result->elements().at(1), *Lisple::RTValue::symbol("are"));
  EXPECT_EQ(*result->elements().at(2), *Lisple::RTValue::symbol("bare"));
  EXPECT_EQ(*result->elements().at(3), *Lisple::RTValue::symbol("symbols"));
}

TEST_F(Runtime, lookup__string_with_default__returns_value_when_found)
{
  // Given
  runtime.get_current_namespace().store("my-value", Lisple::RTValue::string("found"));

  // When
  auto result = runtime.lookup("my-value", Lisple::RTValue::string("default"));

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::string("found"));
}

TEST_F(Runtime, lookup__string_with_default__returns_default_when_missing)
{
  // When
  auto result = runtime.lookup("missing-value", Lisple::RTValue::string("default"));

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::string("default"));
}

TEST_F(Runtime, lookup__string_with_default__keeps_existing_nil_value)
{
  // Given
  runtime.get_current_namespace().store("nil-value", Lisple::Constant::NIL);

  // When
  auto result = runtime.lookup("nil-value", Lisple::RTValue::string("default"));

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::NIL);
}

TEST_F(Runtime, lookup__rtvalue_symbol__returns_value)
{
  // Given
  runtime.get_current_namespace().store("my-value", Lisple::RTValue::string("found"));

  // When
  auto result = runtime.lookup(*Lisple::RTValue::symbol("my-value"));

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::string("found"));
}

TEST_F(Runtime, lookup__rtvalue_non_symbol__throws)
{
  EXPECT_THROW(runtime.lookup(*Lisple::RTValue::keyword("my-value")), Lisple::TypeError);
}

TEST_F(Runtime, eval__dynamic_local_callee_does_not_reuse_first_resolved_function)
{
  // Given
  runtime.eval(R"(
    (defun increment [n] (+ n 1))
    (defun multiply-by-ten [n] (* n 10))
    (def handlers {:increment increment :multiply multiply-by-ten})
    (defun dispatch [handler-key n]
      (let [handler (get handlers handler-key)]
        (handler n)))
  )");

  // When
  auto result = runtime.eval("[(dispatch :increment 4) (dispatch :multiply 4)]");

  // Then
  ASSERT_EQ(result->to_string(), "[5 40]");
}

TEST_F(Runtime, no_matching_signature_exception_bubbles_up_to_client)
{
  // When
  Lisple::sptr_rtval result = nullptr;
  std::string msg;
  try
  {
    result = runtime.eval("(+ \"4\" 2)");
  }
  catch (std::exception& e)
  {
    msg = e.what();
  }

  // Then
  EXPECT_FALSE(result.get());
  EXPECT_THAT(msg, HasSubstr("Could not apply"));
}
