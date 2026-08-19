#include <roo/exception.h>
#include <roo/runtime/value.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Semantic_KeywordCallability = RooTest::RuntimeTestFixture;

TEST_F(Semantic_KeywordCallability, keyword_literal_is_callable)
{
  runtime.eval("(def my-map {:my-key 42})");

  EXPECT_EQ(*runtime.eval("(:my-key my-map)"), *Roo::Value::number(42));
}

TEST_F(Semantic_KeywordCallability, keyword_value_bound_by_def_is_callable)
{
  auto result = runtime.eval(R"(
    (def my-map {:my-key 42})
    (def my-key :my-key)
    (my-key my-map)
  )");

  EXPECT_EQ(*result, *Roo::Value::number(42));
}

TEST_F(Semantic_KeywordCallability, keyword_value_bound_by_let_is_callable)
{
  auto result = runtime.eval(R"(
    (let [my-map {:my-key 42}
          my-key :my-key]
      (my-key my-map))
  )");

  EXPECT_EQ(*result, *Roo::Value::number(42));
}

TEST_F(Semantic_KeywordCallability, apply_invokes_keyword_literal)
{
  runtime.eval("(def my-map {:my-key 42})");

  EXPECT_EQ(*runtime.eval("(apply :my-key [my-map])"), *Roo::Value::number(42));
}

TEST_F(Semantic_KeywordCallability, apply_invokes_keyword_value_bound_by_def)
{
  runtime.eval("(def my-map {:my-key 42})");
  runtime.eval("(def my-key :my-key)");

  EXPECT_EQ(*runtime.eval("(apply my-key [my-map])"), *Roo::Value::number(42));
}

TEST_F(Semantic_KeywordCallability, apply_invokes_keyword_value_bound_by_let)
{
  auto result = runtime.eval(R"(
    (let [my-map {:my-key 42}
          my-key :my-key]
      (apply my-key [my-map]))
  )");

  EXPECT_EQ(*result, *Roo::Value::number(42));
}

TEST_F(Semantic_KeywordCallability, unary_higher_order_functions_invoke_keywords)
{
  EXPECT_EQ(*runtime.eval("(any? [{:active false} {:active true}] :active)"),
            *Roo::Constant::BOOL_TRUE);
  EXPECT_EQ(runtime.eval("(filter [{:keep true} {:keep false} {}] :keep)")->to_string(),
            "[{:keep true}]");
  EXPECT_EQ(runtime.eval("(keep [{:id 1} {} {:id 3}] :id)")->to_string(), "[1 3]");
  EXPECT_EQ(runtime.eval("(remove [{:drop true} {:drop false} {}] :drop)")->to_string(),
            "[{:drop false} {}]");
  EXPECT_EQ(runtime.eval("(remove-first [{:drop true :id 1} {:drop true :id 2}] :drop)")
              ->to_string(),
            "[{:drop true :id 2}]");
}

TEST_F(Semantic_KeywordCallability, mutating_remove_invokes_keyword)
{
  EXPECT_EQ(runtime.eval("(remove! [{:drop true} {:drop false}] :drop)")->to_string(),
            "[{:drop false}]");
}

TEST_F(Semantic_KeywordCallability, update_functions_invoke_keywords)
{
  EXPECT_EQ(runtime.eval("(update {:item {:value 42}} :item :value)")->to_string(),
            "{:item 42}");
  EXPECT_EQ(runtime.eval("(update! {:item {:value 42}} :item :value)")->to_string(),
            "{:item 42}");
  EXPECT_EQ(runtime.eval("(update-in {:outer {:item {:value 42}}} [:outer :item] :value)")
              ->to_string(),
            "{:outer {:item 42}}");
  EXPECT_EQ(runtime.eval("(update-in! {:outer {:item {:value 42}}} [:outer :item] :value)")
              ->to_string(),
            "{:outer {:item 42}}");
}

TEST_F(Semantic_KeywordCallability, conditional_threading_invokes_keyword)
{
  EXPECT_EQ(runtime.eval("(cond-> {:item {:value 42}} true :item)")->to_string(),
            "{:value 42}");
}

TEST_F(Semantic_KeywordCallability, keyword_invocation_rejects_non_unary_arity)
{
  EXPECT_THROW(runtime.eval("(let [my-key :my-key] (my-key))"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(let [my-key :my-key] (my-key {} {}))"),
               Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(apply :my-key [])"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(apply :my-key [{} {}])"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(map :my-key [{}] [{}])"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(sort [{:rank 2} {:rank 1}] :rank)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(reduce-kv :value {:a 1} {})"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(update {:item {:value 42}} :item [:value :extra])"),
               Roo::InvalidFormException);
}
