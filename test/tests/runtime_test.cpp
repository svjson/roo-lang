
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

TEST_F(Runtime, eval__word__lookup)
{
  // Given
  std::shared_ptr<Lisple::Object> word = std::make_shared<Lisple::Word>("my-word");
  runtime.get_current_namespace().store("my-word",
                                        Lisple::RTValue::string("my-string"));

  // When
  auto result = runtime.eval(word);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*result));
}

TEST_F(Runtime, eval__word__no_lookup)
{
  // Given
  std::shared_ptr<Lisple::Object> word = std::make_shared<Lisple::Word>("my-word");
  runtime.get_current_namespace().store("my-word",
                                        Lisple::RTValue::string("my-string"));

  // When
  ctx.push_context(false);
  auto result = runtime.eval(word);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*result));
}

TEST_F(Runtime, eval__quoted_list)
{
  // When
  auto result = runtime.eval("'(these are bare words)");

  // Then
  ASSERT_TRUE(Lisple::Type::LIST.is_type_of(*result));

  ASSERT_EQ(Lisple::count(*result), 4);

  EXPECT_EQ(*result->elements().at(0), *Lisple::RTValue::symbol("these"));
  EXPECT_EQ(*result->elements().at(1), *Lisple::RTValue::symbol("are"));
  EXPECT_EQ(*result->elements().at(2), *Lisple::RTValue::symbol("bare"));
  EXPECT_EQ(*result->elements().at(3), *Lisple::RTValue::symbol("words"));
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
