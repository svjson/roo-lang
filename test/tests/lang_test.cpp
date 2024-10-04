
#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <ext/alloc_traits.h>
#include <memory>
#include <vector>

#include <lisple/lisp_reader.h>
#include <lisple/context.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/type.h>

#include "lisp_reader_fixture.h"

using namespace ::testing;

TEST(NsMacro, switches_namespace)
{
  // Given
  Lisple::LispReader reader;
  std::string initial_ns = reader.get_current_namespace().get_name();

  // When
  auto result = reader.eval("(ns lets.switch.to.a.new.one)");

  // Then
  EXPECT_EQ(*result, *Lisple::NIL);
  EXPECT_NE(initial_ns, reader.get_current_namespace().get_name());
  EXPECT_EQ(reader.get_current_namespace().get_name(), "lets.switch.to.a.new.one");
}

TEST(NsMacro, does_not_allow_incomplete_req_list)
{
  // Given
  Lisple::LispReader reader;

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
  Lisple::LispReader reader;

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
  Lisple::LispReader reader;
  reader.eval("(ns other)");
  reader.eval("(def what-is-hot? :curry!)");

  // When
  reader.eval("(ns my-main-ns (:require other))");
  auto result = reader.eval("what-is-hot?");

  // Then
  EXPECT_EQ(*result, Lisple::Key("curry!"));

  // And When
  reader.eval("(ns somewhere-else)");
  std::string message = "";
  try
  {
    reader.eval("what-is-hot?");
  } catch (Lisple::LispleException& e) {
    message = e.what();
  }

  EXPECT_THAT(message, HasSubstr("Unknown identifier: 'what-is-hot?'"));
}

TEST(NsMacro, import_non_existing_aliased_namespace)
{
  // Given
  Lisple::LispReader reader;

  // When
  std::string message = "";
  try
  {
    reader.eval("(ns my-main-ns (:require [other :as o]))");
  } catch (Lisple::LispleException& e) {
    message = e.what();
  }

  // Then
  EXPECT_THAT(message, HasSubstr("does not exist"));

}

TEST(NsMacro, import_existing_aliased_namespace)
{
  // Given
  Lisple::LispReader reader;
  reader.eval("(ns other)");
  reader.eval("(def what-is-hot? :curry!)");

  // When
  reader.eval("(ns my-main-ns (:require [other :as o]))");
  auto result = reader.eval("o/what-is-hot?");

  // Then
  EXPECT_EQ(*result, Lisple::Key("curry!"));

  // And When
  reader.eval("(ns somewhere-else)");
  std::string message = "";
  try
  {
    reader.eval("o/what-is-hot?");
  } catch (Lisple::LispleException& e) {
    message = e.what();
  }

  EXPECT_THAT(message, HasSubstr("Unknown identifier: 'o/what-is-hot?'"));
}


TEST(DefMacro, define_var)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  Lisple::sptr_sobject_v args;
  args.push_back(std::make_shared<Lisple::Word>("var-name"));
  args.push_back(std::make_shared<Lisple::String>("Var Value"));
  Lisple::DefMacro def;

  // When
  def.execute(fixture.ctx, args);

  // Then
  auto obj = fixture.lisp_reader.get_current_namespace().lookup(Lisple::Word("var-name"));
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefMacro, define_var_with_gt_and_lt)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  Lisple::sptr_sobject_v args;
  args.push_back(std::make_shared<Lisple::Word>("<var-name>"));
  args.push_back(std::make_shared<Lisple::String>("Value"));
  Lisple::DefMacro def;

  // When
  def.execute(fixture.ctx, args);

  // Then
  auto obj = fixture.lisp_reader.get_current_namespace().lookup(Lisple::Word("<var-name>"));
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefunMacro, define_no_arg_fun)
{
  LispleTest::LispReaderFixture fixture;

  Lisple::sptr_sobject_v args
  {
    std::make_shared<Lisple::Word>("my-fn"),
    std::make_shared<Lisple::Array>(),
    std::make_shared<Lisple::Number>(8)
  };

  Lisple::DefunMacro defun;

  // When
  defun.execute(fixture.ctx, args);

  // Then
  Lisple::Word fn_name("my-fn");
  auto fun = fixture.ctx.lookup(fn_name);
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->get_type(), Lisple::Form::FUNCTION);
  EXPECT_TRUE(fun->as<Lisple::UserFunction>().get_argument_bindings().empty());
  EXPECT_EQ(fun->as<Lisple::UserFunction>().get_body().size(), 1);
  EXPECT_EQ(*fun->as<Lisple::UserFunction>().get_body().front(), Lisple::Number(8));
}

TEST(DefunMacro, define_no_arg_fun_with_docstring)
{
  LispleTest::LispReaderFixture fixture;

  Lisple::sptr_sobject_v args
  {
    std::make_shared<Lisple::Word>("my-fn"),
    std::make_shared<Lisple::String>("This function does all the magic things you can think of..."),
    std::make_shared<Lisple::Array>(),
    std::make_shared<Lisple::Number>(8)
  };

  Lisple::DefunMacro defun;

  // When
  defun.execute(fixture.ctx, args);

  // Then
  Lisple::Word fn_name("my-fn");
  auto fun = fixture.ctx.lookup(fn_name);
  ASSERT_TRUE(fun.get());
  ASSERT_EQ(fun->get_type(), Lisple::Form::FUNCTION);
  EXPECT_TRUE(fun->as<Lisple::UserFunction>().get_argument_bindings().empty());
  EXPECT_EQ(fun->as<Lisple::UserFunction>().get_body().size(), 1);
  EXPECT_EQ(*fun->as<Lisple::UserFunction>().get_body().front(), Lisple::Number(8));
}

TEST(CaseMacro, constants)
{
  // Given
  Lisple::LispReader reader;

  // When
  Lisple::sptr_sobject result = reader.eval(R"((case 20 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, Lisple::String("Zwanzig"));
}

TEST(CaseMacro, expressions)
{
  // Given
  Lisple::LispReader reader;

  // When
  Lisple::sptr_sobject result = reader.eval(R"((case (- 20 10) (- 10 10) "Zilch" (+ 5 5) "Zen" :default "Zillions"))");
  // Then
  ASSERT_EQ(*result, Lisple::String("Zen"));
}

TEST(CaseMacro, no_match_with_default)
{
  // Given
  Lisple::LispReader reader;

  // When
  Lisple::sptr_sobject result = reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, Lisple::String("Zillions"));
}

TEST(CaseMacro, no_match_without_default)
{
  // Given
  Lisple::LispReader reader;

  // When
  Lisple::sptr_sobject result = reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig"))");

  // Then
  ASSERT_EQ(*result, *Lisple::NIL);
}


TEST(CondMacro, match_condition)
{
  // Given
  Lisple::LispReader reader;

  // When
  Lisple::sptr_sobject result = reader.eval(R"((let [x 20] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions")))");

  // Then
  ASSERT_EQ(*result, Lisple::String("Zwanzig"));
}

TEST(CondMacro, no_match_with_else)
{
  // Given
  Lisple::LispReader reader;

  // When
  Lisple::sptr_sobject result = reader.eval(R"((let [x 100] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions")))");
  // Then
  ASSERT_EQ(*result, Lisple::String("Zillions"));
}

TEST(CondMacro, no_match_without_else)
{
  // Given
  Lisple::LispReader reader;

  // When
  Lisple::sptr_sobject result = reader.eval(R"((let [x 100] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig")))");
  // Then
  ASSERT_EQ(*result, *Lisple::NIL);
}


TEST(FilterFunction, filter_array)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto sexps = fixture.parser.read_sexps("(filter [1 2 3 4 5 6] even?)");
  auto retval = fixture.lisp_reader.eval(sexps.front());

  // Then
  ASSERT_TRUE(Lisple::Type::ARRAY.is_type_of(*retval));

  Lisple::Object& result = retval->as<Lisple::Array>();
  ASSERT_EQ(result.get_children().size(), 3);

  EXPECT_EQ(*result.get_children().at(0), Lisple::Number(2));
  EXPECT_EQ(*result.get_children().at(1), Lisple::Number(4));
  EXPECT_EQ(*result.get_children().at(2), Lisple::Number(6));
}

TEST(MapFunction, map_single_vec_of_numbers)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto retval = fixture.lisp_reader.eval("(map [1 2 3] (fn [num] (* num 2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6]");
}


TEST(MapFunction, map_two_vecs_of_numbers)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto retval = fixture.lisp_reader.eval("(map [1 2 3] [30 20 10] (fn [n1 n2] (+ n1 n2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[31 22 13]");
}

TEST(MapFunction, map_two_vecs_of_difference_sizes)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto retval = fixture.lisp_reader.eval("(map [1 2 3 4] [2 1] (fn [n1 n2] (* n1 n2)))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 2 nil nil]");
}

TEST(MapFunction, map_using_keyword)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto result = fixture.lisp_reader.eval("(map [{:a 10} {:a 9} {:a \"How rare! A string!\"}] :a)");

  // Then
  ASSERT_EQ(result->to_string(), "[10 9 \"How rare! A string!\"]");
}

TEST(SelectKeysFunction, all_keys_present)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = fixture.lisp_reader.eval("(select-keys my-map [:b :d])");

  // Then
  ASSERT_EQ(result->to_string(), "{:b 2 :d 4}");
}

TEST(SelectKeysFunction, no_keys_present)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-map {:a 1 :b 2 :c 3 :d 4})");

  // When
  auto result = fixture.lisp_reader.eval("(select-keys my-map [:f :g])");

  // Then
  ASSERT_EQ(result->to_string(), "{}");
}


TEST(ApplyFunction, apply_concat)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto retval = fixture.lisp_reader.eval("(apply concat [[1 2 3] [4 5 6]])");

  // Then
  ASSERT_EQ(retval->to_string(), "[1 2 3 4 5 6]");
}

TEST(ResolveFunction, resolve)
{
  // Given
  Lisple::LispReader runtime;

  // When
  auto result = runtime.eval("(resolve 'concat)");

  // Then
  ASSERT_EQ(result, runtime.lookup(Lisple::Word("concat")));
}

TEST(ApplyFunction, apply_dynamic)
{
  // Given
  Lisple::LispReader runtime;

  // When
  runtime.eval("(def function-name 'max)");
  auto result = runtime.eval("(apply (resolve function-name) [10 8 17 4 0])");

  // Then
  ASSERT_EQ(result->to_string(), "17");
}

TEST(FindFirstFunction, find_first_array)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval(R"((def my-array ["AA" "BB" "CCC" "DDDD" "EEE" "FF"]))");

  // When
  auto three_letter = fixture.lisp_reader.eval("(find-first my-array (fn [lmnt] (= 3 (count lmnt))))");
  auto four_letter = fixture.lisp_reader.eval("(find-first my-array (fn [lmnt] (= 4 (count lmnt))))");
  auto five_letter = fixture.lisp_reader.eval("(find-first my-array (fn [lmnt] (= 5 (count lmnt))))");

  // Then
  EXPECT_EQ(*three_letter, Lisple::String("CCC"));
  EXPECT_EQ(*four_letter, Lisple::String("DDDD"));
  EXPECT_EQ(*five_letter, *Lisple::NIL);
}

TEST(SeqMatchFunction, seq_match_single_field)
{
  // Given
  Lisple::LispReader reader;
  reader.eval(R"((def my-seq [{:id 1 :name "Air Man"} {:id 2 :name "Bubble Man"}]))");

  // Then
  EXPECT_EQ(reader.eval("(seq-match my-seq {:id 1})")->to_string(), R"({:id 1 :name "Air Man"})");
  EXPECT_EQ(reader.eval("(seq-match my-seq {:id 2})")->to_string(), R"({:id 2 :name "Bubble Man"})");
  EXPECT_EQ(reader.eval("(seq-match my-seq {:id 3})")->to_string(), R"(nil)");
}

TEST(SeqMatchFunction, seq_match_nested)
{
  // Given
  Lisple::LispReader reader;
  const std::string air_man = R"({:id 1 :name "Air Man" :stats {:weapon "Air Shooter" :weakness "Leaf Shield"}})";
  const std::string bubble_man = R"({:id 2 :name "Bubble Man" :stats {:weapon "Bubble Lead" :weakness "Metal Blade"}})";
  const std::string wood_man = R"({:id 3 :name "Wood Man" :stats {:weapon "Leaf Shield" :weakness "Metal Blade"}})";
  reader.eval("(def my-seq [" + air_man + bubble_man + wood_man + "])");

  // Then
  EXPECT_EQ(reader.eval(R"((seq-match my-seq {:stats {:weakness "Metal Blade"}}))")->to_string(), bubble_man);
  EXPECT_EQ(reader.eval(R"((seq-match my-seq {:stats {:weapon "Leaf Shield"}}))")->to_string(), wood_man);
  EXPECT_EQ(reader.eval(R"((seq-match my-seq {:id 3 :stats {:weapon "Leaf Shield"}}))")->to_string(), wood_man);
  EXPECT_EQ(reader.eval(R"((seq-match my-seq {:stats {:weakness "Bubble Lead"}}))")->to_string(), "nil");
}

TEST(LambdaMacro, define_and_exec_lambda)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(fn [x y] (+ (+ x 1) (- y 2)))");

  Lisple::sptr_sobject_v exec_args;
  exec_args.push_back(std::make_shared<Lisple::Number>(5));
  exec_args.push_back(std::make_shared<Lisple::Number>(5));

  // When
  auto fn = fixture.lisp_reader.eval(code.front());
  auto retval = fn->execute(fixture.ctx, exec_args);

  // Then
  EXPECT_TRUE(Lisple::Type::NUMBER.is_type_of(*retval));
  EXPECT_EQ(retval->as<Lisple::Number>().value, 9);
}

TEST(LambdaMacro, with_bound_scope)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(defun bind-for-add [bound-num] (fn [other] (+ bound-num other)))");
  fixture.lisp_reader.eval("(def four-adder (bind-for-add 4))");

  // When
  Lisple::sptr_sobject result = fixture.lisp_reader.eval("(apply four-adder [3])");

  // Then
  EXPECT_EQ(*result, Lisple::Number(7));
}

TEST(LetMacro, define_and_exec_let)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  Lisple::sptr_sobject retval = fixture.lisp_reader.eval("(let [x 10 y 20] (+ x y))");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(30));
  ASSERT_EQ(fixture.ctx.stack_size(), 1);
}

TEST(LetMacro, define_and_exec_let_with_dynamic_values)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  Lisple::sptr_sobject retval = fixture.lisp_reader.eval("(let [x (+ 20 20)] (+ x 10))");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(50));
  ASSERT_EQ(fixture.ctx.stack_size(), 1);
}

TEST(LetMacro, destructure_array)
{
  // Given
  Lisple::LispReader runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(let [[a b] [10 25]] (+ a b))");

  // Then
  ASSERT_EQ(*result, *Lisple::Number::make(35));
}

TEST(IfLetMacro, if_let)
{
  // Given
  Lisple::LispReader runtime;

  // Then
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a 10})] value \"no value\")")->to_string(), "10");
  EXPECT_EQ(runtime.eval("(if-let [value (:b {:a 10})] value \"no value\")")->to_string(), "\"no value\"");
}

TEST(ThreadFirstMacro, deep_map_traversal)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def deep_map {:file {:metadata {:size {:mb 1200}}}})");

  // When
  auto retval = fixture.lisp_reader.eval("(-> deep_map :file :metadata :size :mb)");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(1200));
}

TEST(ThreadFirstMacro, functions)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto retval = fixture.lisp_reader.eval("(-> 1 (* 2) (* 16) (/ 4))");

  // Then
  ASSERT_EQ(retval->to_string(), Lisple::Number(8).to_string());
}

TEST(EmptyPredicateFunction, emptyp_seqs)
{
  // Given
  LispleTest::LispReaderFixture fixture;
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
  LispleTest::LispReaderFixture fixture;
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
  LispleTest::LispReaderFixture fixture;

  // Then
  EXPECT_EQ(fixture.lisp_reader.eval("(vector 1 2 3 4)")->to_string(), "[1 2 3 4]");
  EXPECT_EQ(fixture.lisp_reader.eval(R"((vector 1 "2" :foo 'BAR))")->to_string(), "[1 \"2\" :foo 'BAR]");
  EXPECT_EQ(fixture.lisp_reader.eval("(vector :bork)")->to_string(), "[:bork]");
  EXPECT_EQ(fixture.lisp_reader.eval("(vector 1 [2 3])")->to_string(), "[1 [2 3]]");
}

TEST(StrFunction, concat_strings)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  EXPECT_EQ(fixture.lisp_reader.eval(R"((str "a" "b" "c"))")->to_string(), "\"abc\"");
  EXPECT_EQ(fixture.lisp_reader.eval(R"((str 'a' 'b' 'c'))")->to_string(), "\"abc\"");
  EXPECT_EQ(fixture.lisp_reader.eval(R"((str "a" 1 :rust))")->to_string(), "\"a1:rust\"");
  EXPECT_EQ(fixture.lisp_reader.eval(R"((str [1 2 3]))")->to_string(), "\"[1 2 3]\"");
  EXPECT_EQ(fixture.lisp_reader.eval(R"((str 'a' false {:a 1}))")->to_string(), R"("afalse{:a 1}")");
  EXPECT_EQ(fixture.lisp_reader.eval(R"((str :key " " nil))")->to_string(), R"(":key nil")");
}

TEST(JoinFunction, join_strs)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // Then
  EXPECT_EQ(*fixture.lisp_reader.eval(R"((join " " "This" "is" "bat" "country"))"), Lisple::String("This is bat country"));
  EXPECT_EQ(*fixture.lisp_reader.eval(R"((join "-" "hyphenate" "all" "the" "things"))"), Lisple::String("hyphenate-all-the-things"));
  EXPECT_EQ(*fixture.lisp_reader.eval(R"((join ", " "CSV" "to" "the" "rescue"))"), Lisple::String("CSV, to, the, rescue"));
  EXPECT_EQ(*fixture.lisp_reader.eval(R"((join "-"))"), Lisple::String(""));
  EXPECT_EQ(*fixture.lisp_reader.eval(R"((join "-" "foreveralone"))"), Lisple::String("foreveralone"));
}

TEST(CountFunction, count_str_length)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // Then
  EXPECT_EQ(*fixture.lisp_reader.eval("(count \"\")"), Lisple::Number(0));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count \" \")"), Lisple::Number(1));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count \"..\")"), Lisple::Number(2));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count \" !\")"), Lisple::Number(2));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count \"abc\")"), Lisple::Number(3));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count \"Where is my garmonbozia?\")"), Lisple::Number(24));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count \"0123456789\")"), Lisple::Number(10));
}

TEST(CountFunction, count_array_length)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // Then
  EXPECT_EQ(*fixture.lisp_reader.eval("(count [])"), Lisple::Number(0));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count ['a' 'b'])"), Lisple::Number(2));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count [1 2 3])"), Lisple::Number(3));
  EXPECT_EQ(*fixture.lisp_reader.eval("(count [0 0 0 0 0])"), Lisple::Number(5));
}

TEST(HeadFunction, head_of_array)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-array ['a' 'b' 'c'])");

  // When
  auto result_ref = fixture.lisp_reader.eval("(head my-array)");
  auto result_lit = fixture.lisp_reader.eval("(head ['a' 'b' 'c'])");

  // Then
  EXPECT_EQ(*result_ref, Lisple::Char('a'));
  EXPECT_EQ(*result_lit, Lisple::Char('a'));
}

TEST(HeadFunction, single_element)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto result = fixture.lisp_reader.eval("(head ['a'])");

  // Then
  EXPECT_EQ(*result, Lisple::Char('a'));
}

TEST(TailFunction, tail_of_array)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-array ['a' 'b' 'c'])");

  // When
  auto result_ref = fixture.lisp_reader.eval("(tail my-array)");
  auto result_lit = fixture.lisp_reader.eval("(tail ['a' 'b' 'c'])");

  // Then
  EXPECT_EQ(*result_ref, *fixture.lisp_reader.eval("['b' 'c']"));
  EXPECT_EQ(*result_lit, *fixture.lisp_reader.eval("['b' 'c']"));
}

TEST(TailFunction, single_element)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto result = fixture.lisp_reader.eval("(tail ['a'])");

  // Then
  EXPECT_EQ(*result, *fixture.lisp_reader.eval("[]"));
}

TEST(LastFunction, last)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // Then
  EXPECT_EQ(*fixture.lisp_reader.eval("(last [1 2 3])"), Lisple::Number(3));
}

TEST(IntFunction, char_to_int)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // Then
  EXPECT_EQ(*fixture.lisp_reader.eval("(int 'c')"), Lisple::Number(99));
  EXPECT_EQ(*fixture.lisp_reader.eval("(int ' ')"), Lisple::Number(32));
}

TEST(IntFunction, float_to_int)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // Then
  EXPECT_EQ(*fixture.lisp_reader.eval("(int 12.0)"), Lisple::Number(12));
  EXPECT_EQ(*fixture.lisp_reader.eval("(int 15.4)"), Lisple::Number(15));
  EXPECT_EQ(*fixture.lisp_reader.eval("(int 15.5)"), Lisple::Number(16));
}

TEST(PlusFunction, simple_addition)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(+ 10 5)");

  // When
  auto result = fixture.lisp_reader.eval(code.at(0));

  // Then
  EXPECT_EQ(*result, Lisple::Number(15));
}

TEST(PlusFunction, multiple_arg_addition)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(+ 10 5 2 5)");

  // When
  auto result = fixture.lisp_reader.eval(code.at(0));

  // Then
  EXPECT_EQ(*result, Lisple::Number(22));
}


TEST(MinusFunction, simple_subtraction)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(- 10 5)");

  // When
  auto result = fixture.lisp_reader.eval(code.at(0));

  // Then
  EXPECT_EQ(*result, Lisple::Number(5));
}

TEST(MinusFunction, on_evaluated_numbers)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(- (+ 10 10) (+ 3 5))");

  // When
  auto result = fixture.lisp_reader.eval(code.at(0));

  // Then
  EXPECT_EQ(*result, Lisple::Number(12));
}

TEST(MinusFunction, single_positive_flips_sign)
{
  // Given
  Lisple::LispReader runtime;

  // When
  auto result = runtime.eval("(- 25)");

  // Then
  ASSERT_EQ(*result, Lisple::Number(-25));
}

TEST(MinusFunction, single_negative_flips_sign)
{
  // Given
  Lisple::LispReader runtime;

  // When
  auto result = runtime.eval("(- -25)");

  // Then
  ASSERT_EQ(*result, Lisple::Number(25));
}

TEST(RangeFunction, rising_numbers)
{
  LispleTest::LispReaderFixture fixture;

  EXPECT_EQ(fixture.lisp_reader.eval("(range 5 10)")->to_string(), "[5 6 7 8 9 10]");
  EXPECT_EQ(fixture.lisp_reader.eval("(range -5 2)")->to_string(), "[-5 -4 -3 -2 -1 0 1 2]");
}

TEST(RangeFunction, descending_numbers)
{
  LispleTest::LispReaderFixture fixture;

  EXPECT_EQ(fixture.lisp_reader.eval("(range 10 7)")->to_string(), "[10 9 8 7]");
  EXPECT_EQ(fixture.lisp_reader.eval("(range 2 -5)")->to_string(), "[2 1 0 -1 -2 -3 -4 -5]");
}

TEST(EqualsFunction, ints)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(= 1 1)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= 50 (+ 25 25))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= 999 999)"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(= 1 2)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(= 50 (+ 25 250))"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(= 999 -999)"), *Lisple::B_FALSE);
}

TEST(EqualsFunction, string)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(= \"test\" \"test\")"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= \"a whole sentence\" \"a whole sentence\")"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= \" test\" \" test\")"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(= \"test\" \" test\")"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(= \"a whole sentence\" \"a_whole_sentence\")"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(= \" test\" \" test \")"), *Lisple::B_FALSE);
}

TEST(NotEqualsFunction, ints)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not= 1 2)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= 50 (+ 25 250))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= 999 -999)"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(not= 1 1)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= 50 (+ 25 25))"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= 999 999)"), *Lisple::B_FALSE);
}

TEST(NotEqualsFunction, string)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not= \"test\" \" test\")"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \"a whole sentence\" \"a_whole_sentence\")"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \" test\" \" test \")"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(not= \"test\" \"test\")"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \"a whole sentence\" \"a whole sentence\")"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \" test\" \" test\")"), *Lisple::B_FALSE);
}


TEST(AndMacro, logical_and)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(and (odd? 1) (odd? 3))"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(and (odd? 2) (odd? 3))"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(and (odd? 1) (odd? 2))"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(and (odd? 2) (odd? 4))"), *Lisple::B_FALSE);
}

TEST(OrFunction, logical_or)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(or (odd? 1) (odd? 3))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(or (odd? 2) (odd? 3))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(or (odd? 1) (odd? 2))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(or (odd? 2) (odd? 4))"), *Lisple::B_FALSE);
}

TEST(NilPredicateFunction, nil)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(nil? nil)"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(nil? [nil])"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? 0)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? \"\")"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? :nil)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? 'nil)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(nil? false)"), *Lisple::B_FALSE);
}

TEST(NotFunction, booleans)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not true)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not false)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not (odd? 2))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not (odd? 1))"), *Lisple::B_FALSE);
}

TEST(NotFunction, values)
{
  // Given
  Lisple::LispReader reader;
  reader.eval("(def my-val 15)");
  reader.eval("(def other-val nil)");

  // Then
  EXPECT_EQ(*reader.eval("(not my-val)"), *Lisple::B_FALSE);
  EXPECT_EQ(*reader.eval("(not other-val)"), *Lisple::B_TRUE);
  EXPECT_EQ(*reader.eval("(not 0)"), *Lisple::B_FALSE);
}

TEST(EqualsFunction, mixed_types)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_NE(*fixture.ctx.eval("(= \"test\" 'test)"), *Lisple::B_TRUE);
  EXPECT_NE(*fixture.ctx.eval("(= :test 'test)"), *Lisple::B_TRUE);
  EXPECT_NE(*fixture.ctx.eval("(= :test \"test\")"), *Lisple::B_TRUE);

  EXPECT_NE(*fixture.ctx.eval("(= [1 2] {1 2})"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= [1 2] [1 2])"), *Lisple::B_TRUE);
}

TEST(MinMaxFunction, min)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(min 1 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 0 1)"), Lisple::Number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 0)"), Lisple::Number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 2 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 2)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 10 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 10)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 10 0)"), Lisple::Number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 0 10)"), Lisple::Number(0));
}

TEST(MinMaxFunction, max)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(max 1 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 0 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 0)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 2 1)"), Lisple::Number(2));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 2)"), Lisple::Number(2));
  EXPECT_EQ(*fixture.ctx.eval("(max 10 1)"), Lisple::Number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 10)"), Lisple::Number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 10 0)"), Lisple::Number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 0 10)"), Lisple::Number(10));
}

TEST(ThresholdFunction, threshold)
{
  LispleTest::LispReaderFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(threshold 1 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 1 2)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 200 150)"), Lisple::Number(150));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 200 215)"), Lisple::Number(200));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 10 9)"), Lisple::Number(9));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 10 11)"), Lisple::Number(10));
  EXPECT_EQ(*fixture.ctx.eval("(threshold 0 1)"), Lisple::Number(0));
}

TEST(OddEvenPredicateFunction, odd_test)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v num1{ std::make_shared<Lisple::Number>(1) };
  Lisple::sptr_sobject_v num2{ std::make_shared<Lisple::Number>(2) };
  Lisple::sptr_sobject_v num3{ std::make_shared<Lisple::Number>(3) };
  Lisple::sptr_sobject_v num4{ std::make_shared<Lisple::Number>(4) };
  Lisple::sptr_sobject_v num5{ std::make_shared<Lisple::Number>(5) };
  Lisple::sptr_sobject_v num6{ std::make_shared<Lisple::Number>(6) };

  Lisple::OddEvenPredicateFunction fn(1);

  // Then
  EXPECT_EQ(*fn.execute(fixture.ctx, num1), *Lisple::B_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num2), *Lisple::B_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num3), *Lisple::B_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num4), *Lisple::B_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num5), *Lisple::B_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num6), *Lisple::B_FALSE);
}

TEST(OddEvenPredicateFunction, even_test)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::sptr_sobject_v num1{ std::make_shared<Lisple::Number>(1) };
  Lisple::sptr_sobject_v num2{ std::make_shared<Lisple::Number>(2) };
  Lisple::sptr_sobject_v num3{ std::make_shared<Lisple::Number>(3) };
  Lisple::sptr_sobject_v num4{ std::make_shared<Lisple::Number>(4) };
  Lisple::sptr_sobject_v num5{ std::make_shared<Lisple::Number>(5) };
  Lisple::sptr_sobject_v num6{ std::make_shared<Lisple::Number>(6) };

  Lisple::OddEvenPredicateFunction fn(0);

  // Then
  EXPECT_EQ(*fn.execute(fixture.ctx, num1), *Lisple::B_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num2), *Lisple::B_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num3), *Lisple::B_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num4), *Lisple::B_TRUE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num5), *Lisple::B_FALSE);
  EXPECT_EQ(*fn.execute(fixture.ctx, num6), *Lisple::B_TRUE);
}

TEST(EvalFunction, eval_string)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::EvalFunction eval_fn;

  Lisple::sptr_sobject_v args
  {
    std::make_shared<Lisple::String>("(+ 10 100)")
  };

  // When
  auto result = eval_fn.eval_string(fixture.ctx, args);

  // Then
  EXPECT_EQ(*result, Lisple::Number(110));
}

TEST(EvalFunction, eval_list)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::EvalFunction eval_fn;

  Lisple::sptr_sobject_v args = fixture.parser.read_sexps("(+ 8 90)");

  // When
  auto result = eval_fn.eval_form(fixture.ctx, args);

  // Then
  EXPECT_EQ(*result, Lisple::Number(98));
}

TEST(RndFunction, max)
{
  LispleTest::LispReaderFixture fixture;
  for (int i=0; i < 1000; i++)
  {
    int rndval = fixture.ctx.eval("(rnd 5)")->as<Lisple::Number>().value;
    ASSERT_TRUE(rndval >= 0 && rndval <= 4);
  }
}

TEST(RndFunction, min_max)
{
  LispleTest::LispReaderFixture fixture;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = fixture.ctx.eval("(rnd 50 55)")->as<Lisple::Number>().value;
    ASSERT_TRUE(rndval >= 50 && rndval <= 54);
  }
}

TEST(RandNth, all_elements_possible)
{
  Lisple::LispReader reader;

  std::vector freq { 0, 0, 0, 0, 0 };

  for (int i=0; i < 5000; i++)
  {
    int num = reader.eval("(rand-nth [0 1 2 3 4])")->as<Lisple::Number>().value;
    freq[num]++;;
  }

  for (int num_freq : freq)
  {
    std::cout << "Freq: " << num_freq << std::endl;
    EXPECT_NE(num_freq, 0);
  }
}

TEST(RandNth, single_element)
{
  Lisple::LispReader reader;

  for (int i=0; i<1000;i++)
  {
    int num = reader.eval("(rand-nth [8])")->as<Lisple::Number>().value;
    EXPECT_EQ(num, 8);
  }
}

TEST(RepeatFunction, repeat)
{
  Lisple::LispReader runtime;

  EXPECT_EQ(runtime.eval("(repeat 0 :key)")->to_string(), "[]");
  EXPECT_EQ(runtime.eval("(repeat 1 :key)")->to_string(), "[:key]");
  EXPECT_EQ(runtime.eval("(repeat 2 :key)")->to_string(), "[:key :key]");
  EXPECT_EQ(runtime.eval("(repeat 2 :a :b)")->to_string(), "[:a :b :a :b]");
  EXPECT_EQ(runtime.eval("(repeat 3 :a :b)")->to_string(), "[:a :b :a :b :a :b]");
}

TEST(GetFunction, get_from_map)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = fixture.lisp_reader.eval("(get my-map :b)");

  // Then
  ASSERT_EQ(*result, Lisple::Number(2));
}

TEST(AssocFunction, add_key_to_map)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = fixture.lisp_reader.eval("(assoc my-map :c 3)");

  // Then
  EXPECT_EQ(*result, *fixture.lisp_reader.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(*fixture.lisp_reader.lookup(Lisple::Word("my-map")),
            *fixture.lisp_reader.eval("{:a 1 :b 2}"));
}

TEST(AssocFunction, replace_key_in_map)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = fixture.lisp_reader.eval("(assoc my-map :a 10)");

  // Then
  EXPECT_EQ(*result, *fixture.lisp_reader.eval("{:a 10 :b 2}"));
  EXPECT_EQ(*fixture.lisp_reader.lookup(Lisple::Word("my-map")), *fixture.lisp_reader.eval("{:a 1 :b 2}"));
}

TEST(AssocBangFunction, add_key_to_map)
{
  // Given
  Lisple::LispReader runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :c 3)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(*runtime.lookup(Lisple::Word("my-map")), *runtime.eval("{:a 1 :b 2 :c 3}"));
}

TEST(AssocBangFunction, replace_key_in_map)
{
  // Given
  Lisple::LispReader runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :b 10)");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10}"));
  EXPECT_EQ(*runtime.lookup(Lisple::Word("my-map")), *runtime.eval("{:a 1 :b 10}"));
}

TEST(AssocBangFunction, add_and_replace_multiple)
{
  // Given
  Lisple::LispReader runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :b 10 :c 3 :d \"some string\")");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}"));
  EXPECT_EQ(*runtime.lookup(Lisple::Word("my-map")), *runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}"));
}

TEST(ContainsPredicateFunction, contains)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-vec [1 3 5 6 7 8])");

  // Then
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 1)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 2)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 3)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 4)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 5)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 6)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 7)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 8)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 9)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.lisp_reader.eval("(contains? my-vec 10)"), *Lisple::B_FALSE);
}

TEST(ConcatFunction, numbers)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto retval = fixture.lisp_reader.eval("(concat [1 2 3] [4 5 6])");

  // Then
  EXPECT_EQ(*retval, *fixture.lisp_reader.eval("[1 2 3 4 5 6]"));
}

TEST(FlattenFunction, array_of_array)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto retval = fixture.lisp_reader.eval("(flatten [[1 2 3] [4 5 6] [7 8 9]])");

  // Then
  ASSERT_EQ(*retval, *fixture.lisp_reader.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST(FlattenFunction, nested_arrays)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto retval = fixture.lisp_reader.eval("(flatten [1 [2 3] [4 [5 [6]] [7]] 8 9])");

  // Then
  ASSERT_EQ(*retval, *fixture.lisp_reader.eval("[1 2 3 4 5 6 7 8 9]"));
}

TEST(SetBangMacro, set_global_value)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-global-var 10)");

  // When
  fixture.lisp_reader.eval("(set! [my-global-var] 50)");

  // Then
  auto& global_var = *fixture.lisp_reader.get_current_namespace().lookup(Lisple::Word("my-global-var"));
  ASSERT_EQ(global_var.as<Lisple::Number>().value, Lisple::Number(50).value);
}

TEST(SetBangMacro, set_parent_scope_value)
{
  // Given
  LispleTest::LispReaderFixture fixture;

  // When
  auto my_var = fixture.lisp_reader.eval("(let [my-var 10] (do (set! [my-var] 20) my-var))");

  // Then
  EXPECT_EQ(my_var->as<Lisple::Number>().value, 20);
}

TEST(SetBangMacro, set_global_map_value)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def global-map {:key1 10 :key2 20})");

  // When
  fixture.lisp_reader.eval("(set! [:key2 global-map] \"Number 9\")");

  // Then
  auto& global_map = *fixture.lisp_reader.get_current_namespace().lookup(Lisple::Word("global-map"));
  auto expected_map = fixture.lisp_reader.eval("{:key1 10 :key2 \"Number 9\"}");
  ASSERT_EQ(global_map, *expected_map);
}

TEST(ForMacro, transform_vector_of_int)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-vector [1 2 3 4 5 6])");

  // When
  auto retval = fixture.lisp_reader.eval("(for [num my-vector] (* num 2))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6 8 10 12]");
}

TEST(ForMacro, with_map_destructuring)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def my-vector [{:a 1 :b 10} {:a 5 :b 4}])");

  // When
  auto retval = fixture.lisp_reader.eval("(for [{:keys [a b]} my-vector] (+ a b))");

  // Then
  ASSERT_EQ(retval->to_string(), "[11 9]");
}

TEST(WhileMacro, loop_with_counter)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def x 0)");

  // When
  auto retval = fixture.lisp_reader.eval("(while (not= x 10) (set! [x] (+ x 1)))");

  // Then
  EXPECT_EQ(*retval, Lisple::Number(10));
  EXPECT_EQ(*fixture.lisp_reader.get_current_namespace().lookup(Lisple::Word("x")), Lisple::Number(10));
}

TEST(WhileMacro, multi_form_loop)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def x 0)");

  // When
  auto retval = fixture.lisp_reader.eval("(while (not= x 10) (set! [x] (+ x 1)) (* x 2))");

  // Then
  EXPECT_EQ(*retval, Lisple::Number(20));
  EXPECT_EQ(*fixture.lisp_reader.get_current_namespace().lookup(Lisple::Word("x")), Lisple::Number(10));
}

TEST(RemoveBangFunction, remove_even)
{
  // Given
  Lisple::LispReader reader;
  reader.eval("(def my-seq [1 2 3 4])");

  // When
  reader.eval("(remove! (fn [n] (prn \"(even?  \" n (even? n) \")\") (even? n)) my-seq)");

  // Then
  Lisple::Array& my_seq = reader.lookup(Lisple::Word("my-seq"))->as<Lisple::Array>();
  EXPECT_EQ(my_seq.get_children().size(), 2);
  EXPECT_EQ(my_seq.get_children().at(0)->as<Lisple::Number>().int_value(), 1);
  EXPECT_EQ(my_seq.get_children().at(1)->as<Lisple::Number>().int_value(), 3);
}

TEST(ReduceFunction, reduce_simple)
{
  // Given
  Lisple::LispReader reader;

  // When
  auto retval = reader.eval("(reduce [128 64 32 16 8 4 2 1] 0 +)");

  // Then
  EXPECT_EQ(*retval, Lisple::Number(255));
}

TEST(ReduceKeyValueFunction, recreate_map)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  fixture.lisp_reader.eval("(def original-map {:a [1 2 3 4 5] :b [1 2 3]})");

  // when
  auto retval = fixture.lisp_reader.eval("(reduce-kv original-map {} (fn [r k v] (assoc r k (count v))))");

  // Then
  EXPECT_EQ(*retval, *fixture.lisp_reader.eval("{:a 5 :b 3}"));
}
