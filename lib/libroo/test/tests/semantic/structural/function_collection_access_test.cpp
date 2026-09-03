#include <roo/exception.h>
#include <roo/runtime/value.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using Semantic_FunctionCollectionAccess = RooTest::RuntimeTestFixture;

namespace
{
  void define_subject(Roo::Runtime& runtime)
  {
    runtime.eval("(defun subject [] nil)");
  }
} // namespace

TEST_F(Semantic_FunctionCollectionAccess, keys_returns_nil_for_function)
{
  define_subject(runtime);

  EXPECT_EQ(runtime.eval("(keys subject)"), Roo::Constant::NIL);
}

TEST_F(Semantic_FunctionCollectionAccess, vals_returns_nil_for_function)
{
  define_subject(runtime);

  EXPECT_EQ(runtime.eval("(vals subject)"), Roo::Constant::NIL);
}

TEST_F(Semantic_FunctionCollectionAccess, append_rejects_function_at_signature_dispatch)
{
  define_subject(runtime);

  EXPECT_THROW(runtime.eval("(append subject :value)"),
               Roo::NoMatchingSignatureException);
}

TEST_F(Semantic_FunctionCollectionAccess, keys_and_vals_return_nil_for_non_map_collection)
{
  EXPECT_EQ(runtime.eval("(keys [:a :b])"), Roo::Constant::NIL);
  EXPECT_EQ(runtime.eval("(vals [:a :b])"), Roo::Constant::NIL);
}

TEST_F(Semantic_FunctionCollectionAccess, property_lookup_returns_nil_or_default)
{
  define_subject(runtime);

  EXPECT_EQ(runtime.eval("(:missing subject)"), Roo::Constant::NIL);
  EXPECT_EQ(runtime.eval("(get subject :missing)"), Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(get subject :missing :fallback)"),
            *Roo::Value::keyword("fallback"));
}

TEST_F(Semantic_FunctionCollectionAccess, property_path_lookup_returns_nil_or_default)
{
  define_subject(runtime);

  EXPECT_EQ(runtime.eval("(get-in subject [:missing])"), Roo::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(get-in subject [:missing] :fallback)"),
            *Roo::Value::keyword("fallback"));
}

TEST_F(Semantic_FunctionCollectionAccess, select_keys_returns_an_empty_map)
{
  define_subject(runtime);

  EXPECT_EQ(*runtime.eval("(select-keys subject [:missing])"), *Roo::Value::map({}));
}

TEST_F(Semantic_FunctionCollectionAccess, generic_count_treats_function_as_one_value)
{
  define_subject(runtime);

  EXPECT_EQ(runtime.eval("(count subject)")->i64(), 1);
}

TEST_F(Semantic_FunctionCollectionAccess, generic_concat_treats_function_as_one_value)
{
  define_subject(runtime);

  auto result = runtime.eval("(concat subject)");

  ASSERT_EQ(result->elements().size(), 1);
  EXPECT_EQ(result->elements()[0], runtime.lookup("subject"));
}
