
#include "runtime_fixture.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using NsForm = LispleTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(NsForm, switches_namespace)
{
  // Given
  std::string initial_ns = runtime.get_current_namespace().get_name();

  // When
  auto result = runtime.eval("(ns lets.switch.to.a.new.one)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::NIL);
  EXPECT_NE(initial_ns, runtime.get_current_namespace().get_name());
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "lets.switch.to.a.new.one");
}

TEST_F(NsForm, does_not_allow_incomplete_req_list)
{
  // Given
  auto& reader = runtime;
  Lisple::LispleException* thrown = nullptr;
  std::string msg;
  try
  {
    auto result = reader.eval("(ns some.space (:require))");
  }
  catch (Lisple::LispleException& e)
  {
    thrown = &e;
    msg = e.what();
  }

  // Then
  EXPECT_THAT(thrown, NotNull());
  EXPECT_THAT(msg, HasSubstr("(ns some.space (:require))"));
}

TEST_F(NsForm, import_non_existing_namespace)
{
  // Given
  auto& reader = use_bare_runtime();
  Lisple::LispleException* thrown = nullptr;
  std::string msg;

  try
  {
    auto result = reader.eval("(ns some.space (:require muffins))");
  }
  catch (Lisple::LispleException& e)
  {
    thrown = &e;
    msg = e.what();
  }

  // Then
  EXPECT_THAT(thrown, NotNull());
  EXPECT_THAT(msg, HasSubstr("not exist"));
}

TEST_F(NsForm, import_existing_namespace)
{
  // Given
  auto& reader = runtime;
  reader.eval("(ns other)");
  reader.eval("(def what-is-hot? :curry!)");

  // When
  reader.eval("(ns my-main-ns (:require other))");
  auto result = reader.eval("what-is-hot?");

  // Then
  EXPECT_EQ(*result, *Lisple::Value::keyword("curry!"));

  // And When
  reader.eval("(ns somewhere-else)");
  std::string message = "";
  try
  {
    reader.eval("what-is-hot?");
  }
  catch (Lisple::LispleException& e)
  {
    message = e.what();
  }

  EXPECT_THAT(message, HasSubstr("Unknown identifier: 'what-is-hot?'"));
}

TEST_F(NsForm, import_non_existing_aliased_namespace)
{
  // Given
  auto& reader = use_bare_runtime();
  std::string message = "";
  try
  {
    reader.eval("(ns my-main-ns (:require [other :as o]))");
  }
  catch (Lisple::LispleException& e)
  {
    message = e.what();
  }

  // Then
  EXPECT_THAT(message, HasSubstr("does not exist"));
}

TEST_F(NsForm, import_existing_aliased_namespace)
{
  // Given
  auto& reader = runtime;
  reader.eval("(ns other)");
  reader.eval("(def what-is-hot? :curry!)");

  // When
  reader.eval("(ns my-main-ns (:require [other :as o]))");
  auto result = reader.eval("o/what-is-hot?");

  // Then
  EXPECT_EQ(*result, *Lisple::Value::keyword("curry!"));

  // And When
  reader.eval("(ns somewhere-else)");
  std::string message = "";
  try
  {
    reader.eval("o/what-is-hot?");
  }
  catch (Lisple::LispleException& e)
  {
    message = e.what();
  }

  EXPECT_THAT(message, HasSubstr("Unknown identifier: 'o/what-is-hot?'"));
}
