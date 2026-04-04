
#include <memory>

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

TEST(ThreadFirstMacro, deep_map_traversal)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def deep_map {:file {:metadata {:size {:mb 1200}}}})");

  // When
  auto retval = fixture.runtime.eval("(-> deep_map :file :metadata :size :mb)");

  // Then
  ASSERT_EQ(*retval, *Lisple::RTValue::number(1200));
}

TEST(ThreadFirstMacro, functions)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  auto retval = fixture.runtime.eval("(-> 1 (* 2) (* 16) (/ 4))");

  // Then
  ASSERT_EQ(retval->i64(), 8);
}

TEST(ThreadFirstMacro, retrieved_map_is_same_instance)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def deep_map {:file {:metadata {:size {:mb 1200}}}})");

  // When
  auto tf_size = fixture.runtime.eval("(-> deep_map :file :metadata :size)");
  auto tf_fn_size = fixture.runtime.eval("(-> deep_map (get :file) :metadata (get :size))");
  auto size = fixture.runtime.eval("(:size (:metadata (:file deep_map)))");

  // Then
  EXPECT_EQ(*tf_size, *size);
  EXPECT_EQ(*tf_fn_size, *size);

  // Temporarily cater to RuntimeWrapper
  EXPECT_EQ(tf_size.get(), size.get());

  EXPECT_EQ(tf_size->to_string(), size->to_string());

  EXPECT_EQ(tf_fn_size.get(), size.get());
  EXPECT_EQ(tf_fn_size->to_string(), size->to_string());
}

TEST(EmptyPredicateFunction, emptyp_seqs)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  auto list = fixture.parser.read_sexps("'(\"value1\" \"value2\")");
  auto empty_list = fixture.parser.read_sexps("'()");
  auto array = fixture.parser.read_sexps("[\"value1\" \"value2\"]");
  auto empty_array = fixture.parser.read_sexps("[]");
  auto map = fixture.parser.read_sexps("{:key1 \"value1\" :key2 \"value2\"}");
  auto empty_map = fixture.parser.read_sexps("{}");

  Lisple::EmptyPredicateFunction fn;

  // Then
  EXPECT_FALSE(fn.execute(fixture.ctx, list)->as<Lisple::Boolean>().value);
  EXPECT_FALSE(fn.execute(fixture.ctx, array)->as<Lisple::Boolean>().value);
  EXPECT_FALSE(fn.execute(fixture.ctx, map)->as<Lisple::Boolean>().value);

  EXPECT_TRUE(fn.execute(fixture.ctx, empty_list)->as<Lisple::Boolean>().value);
  EXPECT_TRUE(fn.execute(fixture.ctx, empty_array)->as<Lisple::Boolean>().value);
  EXPECT_TRUE(fn.execute(fixture.ctx, empty_map)->as<Lisple::Boolean>().value);
}

TEST(EmptyPredicateFunction, emptyp_strings)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  auto str1 = fixture.parser.read_sexps("\"a string\"");
  auto str2 = fixture.parser.read_sexps("\"a\"");
  auto str3 = fixture.parser.read_sexps("\"\"");
  auto str4 = fixture.parser.read_sexps("\" \"");

  Lisple::EmptyPredicateFunction fn;

  // Then
  EXPECT_FALSE(fn.execute(fixture.ctx, str1)->as<Lisple::Boolean>().value);
  EXPECT_FALSE(fn.execute(fixture.ctx, str2)->as<Lisple::Boolean>().value);
  EXPECT_TRUE(fn.execute(fixture.ctx, str3)->as<Lisple::Boolean>().value);
  EXPECT_FALSE(fn.execute(fixture.ctx, str4)->as<Lisple::Boolean>().value);
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

TEST(IntFunction, char_to_int)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // Then
  EXPECT_EQ(*fixture.runtime.eval("(int 'c')"), *Lisple::RTValue::number(99));
  EXPECT_EQ(*fixture.runtime.eval("(int ' ')"), *Lisple::RTValue::number(32));
}

TEST(IntFunction, float_to_int)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // Then
  EXPECT_EQ(*fixture.runtime.eval("(int 12.0)"), *Lisple::RTValue::number(12));
  EXPECT_EQ(*fixture.runtime.eval("(int 15.4)"), *Lisple::RTValue::number(15));
  EXPECT_EQ(*fixture.runtime.eval("(int 15.5)"), *Lisple::RTValue::number(15));
  EXPECT_EQ(*fixture.runtime.eval("(int 15.9)"), *Lisple::RTValue::number(15));
  EXPECT_EQ(*fixture.runtime.eval("(int 16)"), *Lisple::RTValue::number(16));
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

TEST(NotEqualsFunction, ints)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_TRUE(Lisple::is_truthy(*fixture.ctx.eval("(not= 1 2)")));
  EXPECT_TRUE(Lisple::is_truthy(*fixture.ctx.eval("(not= 50 (+ 25 250))")));
  EXPECT_TRUE(Lisple::is_truthy(*fixture.ctx.eval("(not= 999 -999)")));

  EXPECT_FALSE(Lisple::is_truthy(*fixture.ctx.eval("(not= 1 1)")));
  EXPECT_FALSE(Lisple::is_truthy(*fixture.ctx.eval("(not= 50 (+ 25 25))")));
  EXPECT_FALSE(Lisple::is_truthy(*fixture.ctx.eval("(not= 999 999)")));
}

TEST(NotEqualsFunction, chars)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not= 'a' 'b')"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= '-' ':')"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= '.' ',')"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(not= 'a' 'a')"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= '-' '-')"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= '.' '.')"), *Lisple::Constant::BOOL_FALSE);
}

TEST(NotEqualsFunction, string)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not= \"test\" \" test\")"), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \"a whole sentence\" \"a_whole_sentence\")"),
            *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \" test\" \" test \")"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(not= \"test\" \"test\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \"a whole sentence\" \"a whole sentence\")"),
            *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \" test\" \" test\")"), *Lisple::Constant::BOOL_FALSE);
}

TEST(NilPredicateFunction, nil)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(nil? nil)"), *Lisple::Constant::BOOL_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(nil? [nil])"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? 0)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? \"\")"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? :nil)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? 'nil)"), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? false)"), *Lisple::Constant::BOOL_FALSE);
}

TEST(MinMaxFunction, min)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(min 1 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 0 1)"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 0)"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 2 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 2)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 10 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 10)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 10 0)"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 0 10)"), *Lisple::RTValue::number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 4 10 2 8 200)"), *Lisple::RTValue::number(2));
}

TEST(MinMaxFunction, max)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(max 1 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 0 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 0)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 2 1)"), *Lisple::RTValue::number(2));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 2)"), *Lisple::RTValue::number(2));
  EXPECT_EQ(*fixture.ctx.eval("(max 10 1)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 10)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 10 0)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 0 10)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 4 10 9 199 200 8 2)"), *Lisple::RTValue::number(200));
}

TEST(ThresholdFunction, threshold)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(threshold 1 1)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 1 2)"), *Lisple::RTValue::number(1));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 200 150)"), *Lisple::RTValue::number(150));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 200 215)"), *Lisple::RTValue::number(200));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 10 9)"), *Lisple::RTValue::number(9));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 10 11)"), *Lisple::RTValue::number(10));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 0 1)"), *Lisple::RTValue::number(0));
}

TEST(OddEvenPredicateFunction, odd_test)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_rtval_v num1{Lisple::RTValue::number(1)};
  Lisple::sptr_rtval_v num2{Lisple::RTValue::number(2)};
  Lisple::sptr_rtval_v num3{Lisple::RTValue::number(3)};
  Lisple::sptr_rtval_v num4{Lisple::RTValue::number(4)};
  Lisple::sptr_rtval_v num5{Lisple::RTValue::number(5)};
  Lisple::sptr_rtval_v num6{Lisple::RTValue::number(6)};

  Lisple::OddEvenPredicateFunction fn(1);

  // Then
  EXPECT_EQ(*fn.execute(fixture.ctx, num1), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num2), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num3), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num4), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num5), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num6), *Lisple::Constant::BOOL_FALSE);
}

TEST(OddEvenPredicateFunction, even_test)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_rtval_v num1{Lisple::RTValue::number(1)};
  Lisple::sptr_rtval_v num2{Lisple::RTValue::number(2)};
  Lisple::sptr_rtval_v num3{Lisple::RTValue::number(3)};
  Lisple::sptr_rtval_v num4{Lisple::RTValue::number(4)};
  Lisple::sptr_rtval_v num5{Lisple::RTValue::number(5)};
  Lisple::sptr_rtval_v num6{Lisple::RTValue::number(6)};

  Lisple::OddEvenPredicateFunction fn(0);

  // Then
  EXPECT_EQ(*fn.execute(fixture.ctx, num1), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num2), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num3), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num4), *Lisple::Constant::BOOL_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num5), *Lisple::Constant::BOOL_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num6), *Lisple::Constant::BOOL_TRUE);
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

TEST(DissocBangFunction, removal_of_non_existing_key_returns_nil)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc! my-map :d)");

  // Then
  EXPECT_EQ(*result, *Lisple::Constant::NIL);
  EXPECT_EQ(runtime.eval("my-map")->to_string(), "{:a 1 :b 2 :c 3}");
}

TEST(DissocBangFunction, removal_of_key_returns_value)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc! my-map :b)");

  // Then
  EXPECT_EQ(*result, *Lisple::RTValue::number(2));
  EXPECT_EQ(runtime.eval("my-map")->to_string(), "{:a 1 :c 3}");
}

TEST(SetBangMacro, set_global_value)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def my-global-var 10)");

  // When
  fixture.runtime.eval("(set! [my-global-var] 50)");

  // Then
  auto global_var = fixture.runtime.get_current_namespace().lookup_value("my-global-var");
  ASSERT_EQ(global_var->i64(), 50);
}

TEST(SetBangMacro, set_parent_scope_value)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  auto my_var = fixture.runtime.eval("(let [my-var 10] (do (set! [my-var] 20) my-var))");

  // Then
  EXPECT_EQ(my_var->i64(), 20);
}

TEST(SetBangMacro, set_global_map_value)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def global-map {:key1 10 :key2 20})");

  // When
  fixture.runtime.eval("(set! [:key2 global-map] \"Number 9\")");

  // Then
  auto global_map = fixture.runtime.get_current_namespace().lookup_value("global-map");
  std::cout << &global_map << std::endl;
  auto expected_map = fixture.runtime.eval("{:key1 10 :key2 \"Number 9\"}");
  std::cout << global_map->to_string() << std::endl;
  ASSERT_EQ(*global_map, *expected_map);
}

TEST(WhileMacro, loop_with_counter)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def x 0)");

  // When
  auto retval = fixture.runtime.eval("(while (not= x 10) (set! [x] (+ x 1)))");

  // Then
  EXPECT_EQ(*retval, *Lisple::RTValue::number(10));
  EXPECT_EQ(*fixture.runtime.get_current_namespace().lookup_value("x"),
            *Lisple::RTValue::number(10));
}

TEST(WhileMacro, multi_form_loop)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def x 0)");

  // When
  auto retval = fixture.runtime.eval("(while (not= x 10) (set! [x] (+ x 1)) (* x 2))");

  // Then
  EXPECT_EQ(*retval, *Lisple::RTValue::number(20));
  EXPECT_EQ(*fixture.runtime.get_current_namespace().lookup_value("x"),
            *Lisple::RTValue::number(10));
}
