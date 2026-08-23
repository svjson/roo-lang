#include <roo/exception.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Raise = RooTest::RuntimeTestFixture;

namespace
{
  Roo::sptr_val property(const Roo::sptr_val& map, const std::string& key)
  {
    return Roo::Dict::get_property(map, *Roo::Value::keyword(key));
  }
} // namespace

TEST_F(Raise, string_creates_a_generic_error)
{
  const auto error = runtime.eval("(guard (([error] error)) (raise \"Broken\"))");

  EXPECT_EQ(property(error, "type")->str(), "roo/error");
  EXPECT_TRUE(property(error, "parent-types")->elements().empty());
  EXPECT_EQ(property(error, "detail")->str(), "Broken");
}

TEST_F(Raise, qualified_keyword_creates_a_typed_error_with_a_default_message)
{
  const auto error = runtime.eval("(guard (([error] error)) (raise :my-app/not-ready))");

  EXPECT_EQ(property(error, "type")->str(), "my-app/not-ready");
  ASSERT_EQ(property(error, "parent-types")->elements().size(), 1);
  EXPECT_EQ(property(error, "parent-types")->elements()[0]->str(), "roo/error");
  EXPECT_EQ(property(error, "detail")->str(), "Raised :my-app/not-ready");
}

TEST_F(Raise, type_message_and_metadata_are_materialized_flat)
{
  const auto error = runtime.eval(
    "(guard (([error] error))"
    "  (raise :my-app/not-ready \"Not ready\" {:account-id 42 :retryable true}))");

  EXPECT_EQ(property(error, "type")->str(), "my-app/not-ready");
  EXPECT_EQ(property(error, "detail")->str(), "Not ready");
  EXPECT_EQ(property(error, "account-id")->i64(), 42);
  EXPECT_EQ(property(error, "retryable")->type, Roo::Value::Type::BOOL);
  EXPECT_TRUE(*property(error, "retryable") == *Roo::Constant::BOOL_TRUE);
}

TEST_F(Raise, message_or_type_can_be_combined_with_metadata)
{
  const auto generic =
    runtime.eval("(guard (([error] error)) (raise \"Broken\" {:operation :save}))");
  const auto typed =
    runtime.eval("(guard (([error] error)) (raise :my-app/broken {:operation :save}))");

  EXPECT_EQ(property(generic, "operation")->str(), "save");
  EXPECT_EQ(property(typed, "type")->str(), "my-app/broken");
  EXPECT_EQ(property(typed, "operation")->str(), "save");
}

TEST_F(Raise, full_map_defaults_missing_common_fields)
{
  const auto error = runtime.eval("(guard (([error] error)) (raise {}))");

  EXPECT_EQ(property(error, "type")->str(), "roo/error");
  EXPECT_TRUE(property(error, "parent-types")->elements().empty());
  EXPECT_EQ(property(error, "detail")->str(), "Raised :roo/error");
  EXPECT_EQ(property(error, "site")->type, Roo::Value::Type::NIL);
  EXPECT_EQ(property(error, "frames")->type, Roo::Value::Type::VECTOR);
  EXPECT_EQ(property(error, "cause")->type, Roo::Value::Type::NIL);
}

TEST_F(Raise, full_map_preserves_diagnostic_history_and_cause)
{
  const auto error =
    runtime.eval("(guard (([error] error))"
                 "  (raise {:type :my-app/wrapped"
                 "          :message \"Wrapping failed\""
                 "          :detail \"Original detail\""
                 "          :site {:subject \"load\" :source \"app.roo:4\"}"
                 "          :frames [{:kind :call :operation \"calling\""
                 "                    :subject \"load\" :source \"app.roo:4\"}]"
                 "          :cause {:type :my-app/cause}"
                 "          :resource-id 7}))");

  EXPECT_EQ(property(error, "detail")->str(), "Original detail");
  EXPECT_EQ(property(property(error, "site"), "subject")->str(), "load");
  EXPECT_FALSE(property(error, "frames")->elements().empty());
  EXPECT_EQ(property(property(error, "cause"), "type")->str(), "my-app/cause");
  EXPECT_EQ(property(error, "resource-id")->i64(), 7);
}

TEST_F(Raise, one_parent_type_expands_to_the_parent_and_root)
{
  const auto error = runtime.eval("(guard (([error] error))"
                                  "  (raise {:type :my-app/file-vanished"
                                  "          :parent-type :roo.io/error"
                                  "          :parent-types [:ignored/parent]"
                                  "          :message \"Gone\"}))");

  const auto parents = property(error, "parent-types")->elements();
  ASSERT_EQ(parents.size(), 2);
  EXPECT_EQ(parents[0]->str(), "roo.io/error");
  EXPECT_EQ(parents[1]->str(), "roo/error");
}

TEST_F(Raise, supplied_parent_chain_is_ignored_without_a_parent_type)
{
  const auto error = runtime.eval("(guard (([error] error))"
                                  "  (raise {:type :my-app/error"
                                  "          :parent-types [:made-up/one :made-up/two]}))");

  const auto parents = property(error, "parent-types")->elements();
  ASSERT_EQ(parents.size(), 1);
  EXPECT_EQ(parents[0]->str(), "roo/error");
}

TEST_F(Raise, unchanged_guard_error_rethrows_the_original_native_exception)
{
  EXPECT_THROW(runtime.eval("(guard (([error] (raise error))) (count))"),
               Roo::NoMatchingSignatureException);
}

TEST_F(Raise, unchanged_guard_error_can_be_rethrown_from_a_condition)
{
  EXPECT_THROW(runtime.eval("(guard (([error (raise error)] :never)) (count))"),
               Roo::NoMatchingSignatureException);
}

TEST_F(Raise, changed_guard_error_is_a_new_raised_error)
{
  EXPECT_THROW(runtime.eval("(guard"
                            "  (([error]"
                            "     (assoc! error :message \"Rewritten\")"
                            "     (raise error)))"
                            "  (count))"),
               Roo::RaisedError);
}

TEST_F(Raise, stored_guard_error_is_a_new_raised_error)
{
  runtime.eval("(def saved-error (guard (([error] error)) (count)))");

  EXPECT_THROW(runtime.eval("(raise saved-error)"), Roo::RaisedError);
}

TEST_F(Raise, known_roo_type_does_not_synthesize_a_native_exception_class)
{
  try
  {
    runtime.eval("(raise :roo.io/error)");
    FAIL() << "raise returned normally";
  }
  catch (const Roo::IOException&)
  {
    FAIL() << "synthetic Roo IO error was thrown as IOException";
  }
  catch (const Roo::RaisedError& error)
  {
    EXPECT_EQ(error.roo_error_type(), "roo.io/error");
  }
}

TEST_F(Raise, rejects_invalid_types_and_reserved_metadata)
{
  EXPECT_THROW(runtime.eval("(raise :unqualified)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(raise \"Broken\" {:type :my-app/error})"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(raise {:type :my-app/error :parent-type :my-app/error})"),
               Roo::TypeError);
}
