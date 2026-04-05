

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

TEST(NsMacro, switches_namespace)
{
  // Given
  Lisple::Runtime runtime;
  std::string initial_ns = runtime.get_current_namespace().get_name();

  // When
  auto result = runtime.eval("(ns lets.switch.to.a.new.one)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::NIL);
  EXPECT_NE(initial_ns, runtime.get_current_namespace().get_name());
  EXPECT_EQ(runtime.get_current_namespace().get_name(), "lets.switch.to.a.new.one");
}

TEST(NsMacro, does_not_allow_incomplete_req_list)
{
  // Given
  Lisple::Runtime reader;

  // When
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

TEST(NsMacro, import_non_existing_namespace)
{
  // Given
  Lisple::Runtime reader;

  // When
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

TEST(NsMacro, import_existing_namespace)
{
  // Given
  Lisple::Runtime reader;
  reader.eval("(ns other)");
  reader.eval("(def what-is-hot? :curry!)");

  // When
  reader.eval("(ns my-main-ns (:require other))");
  auto result = reader.eval("what-is-hot?");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::keyword("curry!"));

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

TEST(NsMacro, import_non_existing_aliased_namespace)
{
  // Given
  Lisple::Runtime reader;

  // When
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

TEST(NsMacro, import_existing_aliased_namespace)
{
  // Given
  Lisple::Runtime reader;
  reader.eval("(ns other)");
  reader.eval("(def what-is-hot? :curry!)");

  // When
  reader.eval("(ns my-main-ns (:require [other :as o]))");
  auto result = reader.eval("o/what-is-hot?");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::keyword("curry!"));

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

TEST(CaseMacro, constants)
{
  // Given
  Lisple::Runtime reader;

  // When
  auto result =
    reader.eval(R"((case 20 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zwanzig"));
}

TEST(CaseMacro, expressions)
{
  // Given
  Lisple::Runtime reader;

  // When
  auto result =
    reader.eval(R"((case (- 20 10) (- 10 10) "Zilch" (+ 5 5) "Zen" :default "Zillions"))");
  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zen"));
}

TEST(CaseMacro, no_match_with_default)
{
  // Given
  Lisple::Runtime reader;

  // When
  auto result =
    reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zillions"));
}

TEST(CaseMacro, no_match_without_default)
{
  // Given
  Lisple::Runtime reader;

  // When
  auto result = reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}

TEST(ApplyFunction, apply_concat)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  auto retval = fixture.runtime.eval("(apply concat [[1 2 3] [4 5 6]])");

  // Then
  ASSERT_EQ(retval->to_string(), "[1 2 3 4 5 6]");
}

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

TEST(NameFunction, extract_name)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(name :accept/ok)"), *Lisple::RTValue::string("ok"));
  EXPECT_EQ(*runtime.eval("(name :ok)"), *Lisple::RTValue::string("ok"));
  EXPECT_EQ(*runtime.eval("(name 'my-app/some-function)"),
            *Lisple::RTValue::string("some-function"));
  EXPECT_EQ(*runtime.eval("(name 'some-function)"),
            *Lisple::RTValue::string("some-function"));
}

TEST(NamespaceFunction, extract_namepace)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(namespace :accept/ok)"), *Lisple::RTValue::string("accept"));
  EXPECT_EQ(*runtime.eval("(namespace :ok)"), *Lisple::Constant::NIL);
  EXPECT_EQ(*runtime.eval("(namespace 'my-app/some-function)"),
            *Lisple::RTValue::string("my-app"));
  EXPECT_EQ(*runtime.eval("(namespace 'some-function)"), *Lisple::Constant::NIL);
}

TEST(ApplyFunction, apply_dynamic)
{
  // Given
  Lisple::Runtime runtime;

  // When
  runtime.eval("(def function-name 'max)");
  auto result = runtime.eval("(apply (resolve function-name) [10 8 17 4 0])");

  // Then
  ASSERT_EQ(result->to_string(), "17");
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

TEST(UpperCaseFunction, uppercase)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(upper-case \"mIxEd-CaSe!\")"),
            *Lisple::RTValue::string("MIXED-CASE!"));
  EXPECT_EQ(*runtime.eval("(upper-case \"The King is dead.\")"),
            *Lisple::RTValue::string("THE KING IS DEAD."));
  EXPECT_EQ(*runtime.eval("(upper-case :regular-key)"),
            *Lisple::RTValue::string(":REGULAR-KEY"));
  EXPECT_EQ(*runtime.eval("(upper-case {:x 10 :y 8})"),
            *Lisple::RTValue::string("{:X 10 :Y 8}"));
  EXPECT_EQ(*runtime.eval("(upper-case nil)"), *Lisple::RTValue::string("NIL"));
}

TEST(LowerCaseFunction, lowercase)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(lower-case \"mIxEd-CaSe!\")"),
            *Lisple::RTValue::string("mixed-case!"));
  EXPECT_EQ(*runtime.eval("(lower-case \"The King is dead.\")"),
            *Lisple::RTValue::string("the king is dead."));
  EXPECT_EQ(*runtime.eval("(lower-case :UPCASE-KEY)"),
            *Lisple::RTValue::string(":upcase-key"));
  EXPECT_EQ(*runtime.eval("(lower-case {:X 10 :Y 8})"),
            *Lisple::RTValue::string("{:x 10 :y 8}"));
  EXPECT_EQ(*runtime.eval("(lower-case nil)"), *Lisple::RTValue::string("nil"));
}

TEST(JoinFunction, join_strs)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // Then
  EXPECT_EQ(*fixture.runtime.eval(R"((join " " "This" "is" "bat" "country"))"),
            *Lisple::RTValue::string("This is bat country"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-" "hyphenate" "all" "the" "things"))"),
            *Lisple::RTValue::string("hyphenate-all-the-things"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join ", " "CSV" "to" "the" "rescue"))"),
            *Lisple::RTValue::string("CSV, to, the, rescue"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-"))"), *Lisple::RTValue::string(""));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-" "foreveralone"))"),
            *Lisple::RTValue::string("foreveralone"));
}

TEST(CeilFunction, ceil)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(ceil 10.2)"), *Lisple::RTValue::number(11));
  EXPECT_EQ(*runtime.eval("(ceil 10.0)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*runtime.eval("(ceil 5)"), *Lisple::RTValue::number(5));
  EXPECT_EQ(*runtime.eval("(ceil 19.1)"), *Lisple::RTValue::number(20));
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
