

#include <lisple/adapter.h>
#include <lisple/context.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/runtime.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/type.h>

#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include <ext/alloc_traits.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using namespace ::testing;

TEST(SelectKeysFunction, all_keys_present)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = fixture.runtime.eval("(select-keys my-map [:b :d])");

  // Then
  ASSERT_EQ(result->to_string(), "{:b 2 :d 4}");
}

TEST(SelectKeysFunction, no_keys_present)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = fixture.runtime.eval("(select-keys my-map [:f :g])");

  // Then
  ASSERT_EQ(result->to_string(), "{}");
}

TEST(ResolveFunction, resolve)
{
  // Given
  Lisple::Runtime runtime;
  auto rt_lookup_result = runtime.lookup_value(Lisple::Word("concat"));

  // When
  auto result = runtime.eval("(resolve 'concat)");

  // Then
  ASSERT_EQ(result->type, Lisple::RTValue::Type::FUNCTION);
  ASSERT_EQ(result->exec(), rt_lookup_result->exec());
}

TEST(ResolveFunction, nil_resolves_to_nil)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(resolve nil)");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST(ResolveFunction, resolve_other_namespace)
{
  // Given
  Lisple::Runtime runtime;
  runtime.ns("some.nested.space", true)->store("magic-number", Lisple::RTValue::number(3));

  // When
  auto result = runtime.eval("(resolve 'some.nested.space/magic-number)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(3));
  EXPECT_EQ(result->to_string(), "3");
}

TEST(VectorFunction, make_vector)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // Then
  EXPECT_EQ(fixture.runtime.eval("(vector 1 2 3 4)")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(fixture.runtime.eval(R"((vector 1 "2" :foo 'BAR))")->to_string(),
            "[1 \"2\" :foo BAR]");
  EXPECT_EQ(fixture.runtime.eval("(vector :bork)")->to_string(), "[:bork]");
  EXPECT_EQ(fixture.runtime.eval("(vector 1 [2 3])")->to_string(), "[1 [2 3]]");
}

TEST(EvalFunction, eval_string)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::EvalFunction eval_fn;

  Lisple::sptr_sobject_v args{Lisple::String::make("(+ 10 100)")};

  // When
  auto result = eval_fn.eval_string(fixture.ctx, args);

  // Then
  EXPECT_EQ(*result, Lisple::Number(110));
}

TEST(EvalFunction, eval_list)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::EvalFunction eval_fn;

  Lisple::sptr_sobject_v args = fixture.parser.read_sexps("(+ 8 90)");

  // When
  auto result = eval_fn.eval_form(fixture.ctx, args);

  // Then
  EXPECT_EQ(*result, Lisple::Number(98));
}

TEST(RepeatFunction, repeat)
{
  Lisple::Runtime runtime;

  EXPECT_EQ(runtime.eval("(repeat 0 :key)")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(repeat 1 :key)")->to_string(), "[:key]");
  EXPECT_EQ(runtime.eval("(repeat 2 :key)")->to_string(), "[:key :key]");
  EXPECT_EQ(runtime.eval("(repeat 2 :a :b)")->to_string(), "[:a :b :a :b]");
  EXPECT_EQ(runtime.eval("(repeat 3 :a :b)")->to_string(), "[:a :b :a :b :a :b]");
}
