
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <memory>
#include <string>

#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/namespace.h>
#include <lisple/type.h>
#include <lisple/runtime.h>
#include <lisple/dir_root_file_system.h>

#include "runtime_fixture.h"

const std::string RUNTIME_TEST_DIR = "test_resources/script/runtime_test";

using namespace ::testing;

TEST(LispReader, instantiation_vanilla)
{
  // When
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "user");
  EXPECT_FALSE(runtime.has_file_system_access());
}

TEST(Runtime, instantiation_vanilla_with_file_system)
{
  // Given
  Lisple::DirRootFileSystem fs(".");

  // When
  Lisple::Runtime runtime(&fs);

  // Then
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "user");
  EXPECT_TRUE(runtime.has_file_system_access());
}

TEST(Runtime, instantiation_with_namespace)
{
  // Given
  Lisple::Namespace ns("bonanza");

  // When
  Lisple::Runtime runtime(ns);

  // Then
  ASSERT_EQ(runtime.get_current_namespace().get_name(), "bonanza");
  EXPECT_FALSE(runtime.has_file_system_access());
}

TEST(Runtime, instantiation_with_multiple_namespaces)
{
  // Given
  std::map<const std::string, Lisple::Namespace> namespaces;
  namespaces.emplace("mustard", Lisple::Namespace("mustard"));
  namespaces.emplace("custard", Lisple::Namespace("custard"));
  namespaces.emplace("flustard", Lisple::Namespace("flustard"));

  // When
  Lisple::Runtime runtime("custard", namespaces);

    // Then
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "custard");
  EXPECT_FALSE(runtime.has_file_system_access());
}

TEST(Runtime, eval__word__lookup)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  std::shared_ptr<Lisple::Object> word = std::make_shared<Lisple::Word>("my-word");
  std::shared_ptr<Lisple::Object> my_string  = std::make_shared<Lisple::String>("my-string");
  fixture.runtime.get_current_namespace().store(Lisple::Word("my-word"), my_string);

  // When
  auto result = fixture.runtime.eval(word);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*result));
}

TEST(Runtime, eval__word__no_lookup)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  std::shared_ptr<Lisple::Object> word = std::make_shared<Lisple::Word>("my-word");
  std::shared_ptr<Lisple::Object> my_string = std::make_shared<Lisple::String>("my-string");
  fixture.runtime.get_current_namespace().store(Lisple::Word("my-word"), my_string);

  // When
  fixture.ctx.push_context(false);
  auto result = fixture.runtime.eval(word);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*result));
}

TEST(Runtime, eval__quoted_list)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  auto result = fixture.runtime.eval("'(these are bare words)");

  // Then
  ASSERT_TRUE(Lisple::Type::LIST.is_type_of(*result));

  Lisple::List& list = result->as<Lisple::List>();
  ASSERT_TRUE(list.is_quoted());
  ASSERT_EQ(list.size(), 4);

  EXPECT_EQ(*list.get_children().at(0), Lisple::Word("these"));
  EXPECT_EQ(*list.get_children().at(1), Lisple::Word("are"));
  EXPECT_EQ(*list.get_children().at(2), Lisple::Word("bare"));
  EXPECT_EQ(*list.get_children().at(3), Lisple::Word("words"));
}

TEST(Runtime, no_matching_signature_exception_bubbles_up_to_client)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  Lisple::sptr_sobject result = nullptr;
  std::string msg;
  try
  {
    result = fixture.runtime.eval("(+ \"4\" 2)");

  }
  catch (std::exception& e)
  {
    msg = e.what();
  }

  // Then
  EXPECT_FALSE(result.get());
  EXPECT_THAT(msg, HasSubstr("No matching signature"));
}
