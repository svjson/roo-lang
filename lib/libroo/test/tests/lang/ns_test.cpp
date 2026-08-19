
#include "runtime_fixture.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using NsForm = RooTest::RuntimeTestFixture;
using namespace ::testing;

TEST_F(NsForm, switches_namespace)
{
  // Given
  std::string initial_ns = runtime.get_current_namespace().get_name();

  // When
  auto result = runtime.eval("(ns lets.switch.to.a.new.one)");

  // Then
  EXPECT_EQ(*result, *Roo::Constant::NIL);
  EXPECT_NE(initial_ns, runtime.get_current_namespace().get_name());
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "lets.switch.to.a.new.one");
}

TEST_F(NsForm, accepts_namespace_docstring)
{
  auto result = runtime.eval(R"((ns documented.core "A documented namespace."))");

  EXPECT_EQ(*result, *Roo::Constant::NIL);
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "documented.core");
}

TEST_F(NsForm, accepts_docstring_before_require_clause)
{
  runtime.eval("(ns documented.util)");
  runtime.eval("(def answer 42)");

  runtime.eval(R"((ns documented.core
                    "A documented namespace."
                    (:require [documented.util :as util])))");

  EXPECT_EQ(*runtime.eval("util/answer"), *Roo::Value::number(42));
}

TEST_F(NsForm, exposes_all_four_source_signatures)
{
  const Roo::sptr_val ns = runtime.eval("ns");

  EXPECT_EQ(ns->exec().get_signatures().size(), 4);
}

TEST_F(NsForm, rejects_require_before_docstring)
{
  EXPECT_THROW(runtime.eval(R"((ns invalid.core
                                (:require invalid.util)
                                "Too late."))"),
               Roo::NamespaceException);
}

TEST_F(NsForm, does_not_allow_incomplete_req_list)
{
  // Given
  auto& reader = runtime;
  Roo::RooException* thrown = nullptr;
  std::string msg;
  try
  {
    auto result = reader.eval("(ns some.space (:require))");
  }
  catch (Roo::RooException& e)
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
  Roo::RooException* thrown = nullptr;
  std::string msg;

  try
  {
    auto result = reader.eval("(ns some.space (:require muffins))");
  }
  catch (Roo::RooException& e)
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
  EXPECT_EQ(*result, *Roo::Value::keyword("curry!"));

  // And When
  reader.eval("(ns somewhere-else)");
  std::string message = "";
  try
  {
    reader.eval("what-is-hot?");
  }
  catch (Roo::RooException& e)
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
  catch (Roo::RooException& e)
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
  EXPECT_EQ(*result, *Roo::Value::keyword("curry!"));

  // And When
  reader.eval("(ns somewhere-else)");
  std::string message = "";
  try
  {
    reader.eval("o/what-is-hot?");
  }
  catch (Roo::RooException& e)
  {
    message = e.what();
  }

  EXPECT_THAT(message, HasSubstr("Unknown identifier: 'o/what-is-hot?'"));
}
