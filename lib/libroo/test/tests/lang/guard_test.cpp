#include <stdexcept>

#include <roo/exception.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Guard = RooTest::RuntimeTestFixture;

namespace
{
  Roo::sptr_val property(const Roo::sptr_val& map, const std::string& key)
  {
    return Roo::Dict::get_property(map, *Roo::Value::keyword(key));
  }

  class ApplicationIOException : public Roo::IOException
  {
   public:
    ApplicationIOException()
      : IOException("That file is not yours")
    {
    }

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override
    {
      IOException::append_roo_error_types(types);
      types.push_back("my-app.io/aint-your-file-bubba");
    }

    void append_error_fields(Roo::ErrorMapBuilder& builder) const override
    {
      builder.add("path", Roo::Value::string("private.roo"));
      builder.add("type", Roo::Value::keyword("my-app/incorrect-override"));
    }
  };
} // namespace

TEST(ErrorMap, materializes_the_common_envelope_as_an_ordinary_map)
{
  Roo::TypeError error("Expected a number");
  error.set_form_site("plus", "app.roo:4");
  error.add_call_context("calling", "calculate", "app.roo:8");

  const auto value = error.to_error_map();

  ASSERT_EQ(value->type, Roo::Value::Type::MAP);
  EXPECT_EQ(property(value, "type")->str(), "roo/type-error");
  ASSERT_EQ(property(value, "parent-types")->elements().size(), 1);
  EXPECT_EQ(property(value, "parent-types")->elements()[0]->str(), "roo/error");
  EXPECT_EQ(property(value, "message")->str(),
            "Expected a number\n  in calculate at app.roo:8");
  EXPECT_EQ(property(value, "detail")->str(), "Expected a number");
  EXPECT_EQ(property(property(value, "site"), "subject")->str(), "plus");
  EXPECT_EQ(property(property(value, "site"), "source")->str(), "app.roo:4");
  ASSERT_EQ(property(value, "frames")->elements().size(), 1);
  const auto frame = property(value, "frames")->elements()[0];
  EXPECT_EQ(property(frame, "kind")->str(), "call");
  EXPECT_EQ(property(frame, "operation")->str(), "calling");
  EXPECT_EQ(property(frame, "subject")->str(), "calculate");
  EXPECT_EQ(property(frame, "source")->str(), "app.roo:8");
  EXPECT_EQ(property(value, "cause")->type, Roo::Value::Type::NIL);
}

TEST(ErrorMap, built_in_exception_classes_publish_the_documented_roo_types)
{
  std::vector<std::pair<std::unique_ptr<Roo::RooException>, std::string>> cases;
  cases.emplace_back(std::make_unique<Roo::RooException>("error"), "roo/error");
  cases.emplace_back(std::make_unique<Roo::ParseException>("error"), "roo/parse-error");
  cases.emplace_back(std::make_unique<Roo::InvalidFormException>("error"),
                     "roo/invalid-form-error");
  cases.emplace_back(std::make_unique<Roo::IdentifierException>("error"),
                     "roo/identifier-error");
  cases.emplace_back(std::make_unique<Roo::InvocationException>("error"),
                     "roo/invocation-error");
  cases.emplace_back(std::make_unique<Roo::NamespaceException>("error"),
                     "roo/namespace-error");
  cases.emplace_back(std::make_unique<Roo::CyclicNamespaceException>("error"),
                     "roo/require-cycle-error");
  cases.emplace_back(std::make_unique<Roo::TypeError>("error"), "roo/type-error");
  cases.emplace_back(std::make_unique<Roo::IOException>("error"), "roo.io/error");

  for (const auto& [error, expected_type] : cases)
  {
    EXPECT_EQ(property(error->to_error_map(), "type")->str(), expected_type);
  }

  EXPECT_TRUE(property(cases[0].first->to_error_map(), "parent-types")->elements().empty());
  const auto cycle_parents =
    property(cases[6].first->to_error_map(), "parent-types")->elements();
  ASSERT_EQ(cycle_parents.size(), 2);
  EXPECT_EQ(cycle_parents[0]->str(), "roo/namespace-error");
  EXPECT_EQ(cycle_parents[1]->str(), "roo/error");
}

TEST(ErrorMap, invocation_conditions_have_granular_types_and_complete_parents)
{
  const auto target = Roo::Value::keyword("name");
  const auto mismatch = Roo::ArgumentMismatchException(target, {}, 1).to_error_map();

  EXPECT_EQ(property(mismatch, "type")->str(), "roo/argument-mismatch-error");
  ASSERT_EQ(property(mismatch, "parent-types")->elements().size(), 2);
  EXPECT_EQ(property(mismatch, "parent-types")->elements()[0]->str(),
            "roo/invocation-error");
  EXPECT_EQ(property(mismatch, "parent-types")->elements()[1]->str(), "roo/error");
  EXPECT_EQ(property(mismatch, "expected-arity")->i64(), 1);
  EXPECT_EQ(property(mismatch, "arguments")->elements().size(), 0);
}

TEST(ErrorMap, native_subclass_owns_its_type_fields_and_inherits_io_parents)
{
  const auto value = ApplicationIOException().to_error_map();

  EXPECT_EQ(property(value, "type")->str(), "my-app.io/aint-your-file-bubba");
  ASSERT_EQ(property(value, "parent-types")->elements().size(), 2);
  EXPECT_EQ(property(value, "parent-types")->elements()[0]->str(), "roo.io/error");
  EXPECT_EQ(property(value, "parent-types")->elements()[1]->str(), "roo/error");
  EXPECT_EQ(property(value, "path")->str(), "private.roo");
}

TEST(ErrorMap, attached_foreign_cause_is_materialized_as_a_roo_error_map)
{
  Roo::RooException error("Native operation failed");
  error.set_cause(std::make_exception_ptr(std::runtime_error("disk offline")));

  const auto cause = property(error.to_error_map(), "cause");

  ASSERT_EQ(cause->type, Roo::Value::Type::MAP);
  EXPECT_EQ(property(cause, "type")->str(), "roo/foreign-error");
  EXPECT_EQ(property(cause, "message")->str(), "disk offline");
  ASSERT_EQ(property(cause, "parent-types")->elements().size(), 1);
  EXPECT_EQ(property(cause, "parent-types")->elements()[0]->str(), "roo/error");
}

TEST_F(Guard, returns_the_guarded_body_result_when_no_error_is_thrown)
{
  EXPECT_EQ(runtime.eval("(guard (([e] :handled)) 1 42)")->i64(), 42);
  EXPECT_EQ(runtime.eval("(guard ())")->type, Roo::Value::Type::NIL);
}

TEST_F(Guard, catches_specific_and_parent_invocation_types)
{
  auto exact =
    runtime.eval("(guard ((:roo/no-matching-signature-error [e] (:type e))) (count))");
  auto parent = runtime.eval("(guard ((:roo/invocation-error [e] (:type e))) (count))");

  EXPECT_EQ(exact->str(), "roo/no-matching-signature-error");
  EXPECT_EQ(parent->str(), "roo/no-matching-signature-error");
}

TEST_F(Guard, unqualified_selector_matches_concrete_and_parent_qualifiers)
{
  auto result = runtime.eval("(guard ((:roo [e] (:type e))) (count))");

  EXPECT_EQ(result->str(), "roo/no-matching-signature-error");
}

TEST_F(Guard, io_qualifier_matches_io_exception)
{
  auto result = runtime.eval("(guard ((:roo.io [e] [(map? e) (:type e)]))"
                             "  (roo.io/slurp! \"missing.roo\"))");

  ASSERT_EQ(result->elements().size(), 2);
  EXPECT_TRUE(*result->elements()[0] == *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(result->elements()[1]->str(), "roo.io/error");
}

TEST_F(Guard, evaluates_conditions_in_order_and_shares_one_error_map)
{
  auto result = runtime.eval("(guard"
                             "  ((:roo/no-matching-signature-error"
                             "     [e (do (assoc! e :type :my-app/mutated) false)]"
                             "     :not-selected)"
                             "   (:roo/invocation-error [e] (:type e)))"
                             "  (count))");

  EXPECT_EQ(result->str(), "my-app/mutated");
}

TEST_F(Guard, handler_has_an_implicit_multi_form_body)
{
  runtime.eval("(def handled-count 0)");

  auto result =
    runtime.eval("(guard"
                 "  (([e] (set! [handled-count] (inc handled-count)) handled-count))"
                 "  (count))");

  EXPECT_EQ(result->i64(), 1);
  EXPECT_EQ(runtime.eval("handled-count")->i64(), 1);
}

TEST_F(Guard, empty_matching_handler_returns_nil)
{
  EXPECT_EQ(runtime.eval("(guard (([e])) (count))")->type, Roo::Value::Type::NIL);
}

TEST_F(Guard, unmatched_error_is_rethrown_with_its_dynamic_type)
{
  EXPECT_THROW(runtime.eval("(guard ((:roo/type-error [e] :wrong)) (count))"),
               Roo::NoMatchingSignatureException);
}

TEST_F(Guard, errors_from_handlers_escape_to_an_outer_guard)
{
  auto result = runtime.eval("(guard"
                             "  (([outer-error] (:type outer-error)))"
                             "  (guard (([inner-error] (nil))) (count)))");

  EXPECT_EQ(result->str(), "roo/not-callable-error");
}

TEST_F(Guard, binding_scope_is_restored_when_a_condition_throws)
{
  auto result = runtime.eval("(let [e :outer]"
                             "  (guard"
                             "    (([outer-error] e))"
                             "    (guard (([e (nil)] :never)) (count))))");

  EXPECT_EQ(result->str(), "outer");
}

TEST_F(Guard, rejects_malformed_handler_clauses)
{
  EXPECT_THROW(runtime.eval("(guard (:not-a-clause-list) 1)"), Roo::InvalidFormException);
  EXPECT_THROW(runtime.eval("(guard ((:roo/error [e condition extra] :bad)) 1)"),
               Roo::InvalidFormException);
}
