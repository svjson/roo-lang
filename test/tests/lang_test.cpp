
#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include "test_host_objects.h"
#include <ext/alloc_traits.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
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
#include <memory>
#include <vector>

using namespace ::testing;

TEST(NsMacro, switches_namespace)
{
  // Given
  Lisple::Runtime runtime;
  std::string initial_ns = runtime.get_current_namespace().get_name();

  // When
  auto result = runtime.eval("(ns lets.switch.to.a.new.one)");

  // Then
  EXPECT_EQ(*result, *Lisple::NIL);
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
  EXPECT_EQ(*result, Lisple::Key("curry!"));

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
  EXPECT_EQ(*result, Lisple::Key("curry!"));

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

TEST(DefMacro, define_var)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  Lisple::sptr_sobject_v args;
  args.push_back(std::make_shared<Lisple::Word>("var-name"));
  args.push_back(std::make_shared<Lisple::String>("Var Value"));
  Lisple::DefMacro def;

  // When
  def.execute(fixture.ctx, args);

  // Then
  auto obj = fixture.runtime.get_current_namespace().lookup(Lisple::Word("var-name"));
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefMacro, define_var_with_gt_and_lt)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  Lisple::sptr_sobject_v args;
  args.push_back(std::make_shared<Lisple::Word>("<var-name>"));
  args.push_back(std::make_shared<Lisple::String>("Value"));
  Lisple::DefMacro def;

  // When
  def.execute(fixture.ctx, args);

  // Then
  auto obj = fixture.runtime.get_current_namespace().lookup(Lisple::Word("<var-name>"));
  EXPECT_TRUE(obj.get());
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*obj));
}

TEST(DefunMacro, define_no_arg_fun)
{
  LispleTest::RuntimeFixture fixture;

  Lisple::sptr_sobject_v args{std::make_shared<Lisple::Word>("my-fn"),
                              std::make_shared<Lisple::Array>(),
                              std::make_shared<Lisple::Number>(8)};

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
  EXPECT_EQ(*fun->as<Lisple::UserFunction>().get_body().front()->form,
            *Lisple::Number::make(8));
}

TEST(DefunMacro, define_no_arg_fun_with_docstring)
{
  LispleTest::RuntimeFixture fixture;

  Lisple::sptr_sobject_v args{
    std::make_shared<Lisple::Word>("my-fn"),
    std::make_shared<Lisple::String>(
      "This function does all the magic things you can think of..."),
    std::make_shared<Lisple::Array>(),
    std::make_shared<Lisple::Number>(8)};

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
  EXPECT_EQ(*fun->as<Lisple::UserFunction>().get_body().front()->form,
            *Lisple::Number::make(8));
}

TEST(CaseMacro, constants)
{
  // Given
  Lisple::Runtime reader;

  // When
  Lisple::sptr_sobject result =
    reader.eval(R"((case 20 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, Lisple::String("Zwanzig"));
}

TEST(CaseMacro, expressions)
{
  // Given
  Lisple::Runtime reader;

  // When
  Lisple::sptr_sobject result =
    reader.eval(R"((case (- 20 10) (- 10 10) "Zilch" (+ 5 5) "Zen" :default "Zillions"))");
  // Then
  ASSERT_EQ(*result, Lisple::String("Zen"));
}

TEST(CaseMacro, no_match_with_default)
{
  // Given
  Lisple::Runtime reader;

  // When
  Lisple::sptr_sobject result =
    reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, Lisple::String("Zillions"));
}

TEST(CaseMacro, no_match_without_default)
{
  // Given
  Lisple::Runtime reader;

  // When
  Lisple::sptr_sobject result = reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig"))");

  // Then
  ASSERT_EQ(*result, *Lisple::NIL);
}

TEST(CondMacro, match_condition)
{
  // Given
  Lisple::Runtime reader;

  // When
  Lisple::sptr_sobject result = reader.eval(
    R"((let [x 20] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions")))");

  // Then
  ASSERT_EQ(*result, Lisple::String("Zwanzig"));
}

TEST(CondMacro, no_match_with_else)
{
  // Given
  Lisple::Runtime reader;

  // When
  Lisple::sptr_sobject result = reader.eval(
    R"((let [x 100] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig" :else "Zillions")))");
  // Then
  ASSERT_EQ(*result, Lisple::String("Zillions"));
}

TEST(CondMacro, no_match_without_else)
{
  // Given
  Lisple::Runtime reader;

  // When
  Lisple::sptr_sobject result =
    reader.eval(R"((let [x 100] (cond (= x 0) "Zilch" (= x 10) "Zen" (= x 20) "Zwanzig")))");
  // Then
  ASSERT_EQ(*result, *Lisple::NIL);
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

  // When
  auto result = runtime.eval("(resolve 'concat)");

  // Then
  ASSERT_EQ(result, runtime.lookup(Lisple::Word("concat")));
}

TEST(ResolveFunction, nil_resolves_to_nil)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(resolve nil)");

  // Then
  ASSERT_EQ(*result, *Lisple::NIL);
}

TEST(ResolveFunction, resolve_other_namespace)
{
  // Given
  Lisple::Runtime runtime;
  runtime.ns("some.nested.space", true)
    ->store(Lisple::Word("magic-number"), Lisple::Number::make(3));

  // When
  auto result = runtime.eval("(resolve 'some.nested.space/magic-number)");

  // Then
  EXPECT_EQ(*result, Lisple::Number(3));
  EXPECT_EQ(result->to_string(), "3");
}

TEST(NameFunction, extract_name)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(name :accept/ok)"), Lisple::String("ok"));
  EXPECT_EQ(*runtime.eval("(name :ok)"), Lisple::String("ok"));
  EXPECT_EQ(*runtime.eval("(name 'my-app/some-function)"), Lisple::String("some-function"));
  EXPECT_EQ(*runtime.eval("(name 'some-function)"), Lisple::String("some-function"));
}

TEST(NamespaceFunction, extract_namepace)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(namespace :accept/ok)"), Lisple::String("accept"));
  EXPECT_EQ(*runtime.eval("(namespace :ok)"), *Lisple::NIL);
  EXPECT_EQ(*runtime.eval("(namespace 'my-app/some-function)"), Lisple::String("my-app"));
  EXPECT_EQ(*runtime.eval("(namespace 'some-function)"), *Lisple::NIL);
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

TEST(LambdaMacro, define_and_exec_lambda)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(fn [x y] (+ (+ x 1) (- y 2)))");

  Lisple::sptr_sobject_v exec_args;
  exec_args.push_back(std::make_shared<Lisple::Number>(5));
  exec_args.push_back(std::make_shared<Lisple::Number>(5));

  // When
  auto fn = fixture.runtime.eval(code.front());
  auto retval = fn->execute(fixture.ctx, exec_args);

  // Then
  EXPECT_TRUE(Lisple::Type::NUMBER.is_type_of(*retval));
  EXPECT_EQ(retval->as<Lisple::Number>().value, 9);
}

TEST(LambdaMacro, with_bound_scope)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(defun bind-for-add [bound-num] (fn [other] (+ bound-num other)))");
  fixture.runtime.eval("(def four-adder (bind-for-add 4))");

  // When
  Lisple::sptr_sobject result = fixture.runtime.eval("(apply four-adder [3])");

  // Then
  EXPECT_EQ(*result, Lisple::Number(7));
}

TEST(LetMacro, define_and_exec_let)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  Lisple::sptr_sobject retval = fixture.runtime.eval("(let [x 10 y 20] (+ x y))");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(30));
  ASSERT_EQ(fixture.ctx.stack_size(), 1);
}

TEST(LetMacro, define_and_exec_let_with_dynamic_values)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  Lisple::sptr_sobject retval = fixture.runtime.eval("(let [x (+ 20 20)] (+ x 10))");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(50));
  ASSERT_EQ(fixture.ctx.stack_size(), 1);
}

TEST(LetMacro, destructure_array)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(let [[a b] [10 25]] (+ a b))");

  // Then
  ASSERT_EQ(*result, *Lisple::Number::make(35));
}

TEST(LetMacro, destructure_map)
{
  // Given
  Lisple::Runtime runtime;

  // When
  Lisple::sptr_sobject result = runtime.eval("(let [{:keys [a b]} {:a 10 :b 25}] (+ a b))");

  // Then
  ASSERT_EQ(*result, *Lisple::Number::make(35));
}

TEST(IfLetMacro, if_let)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a 10})] value \"no value\")")->to_string(),
            "10");
  EXPECT_EQ(runtime.eval("(if-let [value (:b {:a 10})] value \"no value\")")->to_string(),
            "\"no value\"");
}

TEST(IfLetMacro, if_check_must_happen_only_at_current_scope_level)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def value 1234)");

  // When
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a 10})] value \"no value\")")->to_string(),
            "10");
  EXPECT_EQ(runtime.eval("(if-let [value (:b {:a 10})] value \"no value\")")->to_string(),
            "\"no value\"");
}

TEST(IfLetMacro, branching_should_happen_according_to_truthiness_not_just_ifdef)
{
  // Given
  Lisple::Runtime runtime;

  // When
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a true})] value \"no value\")")->to_string(),
            "true");
  EXPECT_EQ(runtime.eval("(if-let [value (:a {:a false})] value \"no value\")")->to_string(),
            "\"no value\"");
}

TEST(ThreadFirstMacro, deep_map_traversal)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def deep_map {:file {:metadata {:size {:mb 1200}}}})");

  // When
  auto retval = fixture.runtime.eval("(-> deep_map :file :metadata :size :mb)");

  // Then
  ASSERT_EQ(*retval, Lisple::Number(1200));
}

TEST(ThreadFirstMacro, functions)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  auto retval = fixture.runtime.eval("(-> 1 (* 2) (* 16) (/ 4))");

  // Then
  ASSERT_EQ(retval->to_string(), Lisple::Number(8).to_string());
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
            "[1 \"2\" :foo 'BAR]");
  EXPECT_EQ(fixture.runtime.eval("(vector :bork)")->to_string(), "[:bork]");
  EXPECT_EQ(fixture.runtime.eval("(vector 1 [2 3])")->to_string(), "[1 [2 3]]");
}

TEST(StrFunction, concat_strings)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  EXPECT_EQ(fixture.runtime.eval(R"((str "a" "b" "c"))")->to_string(), "\"abc\"");
  EXPECT_EQ(fixture.runtime.eval(R"((str 'a' 'b' 'c'))")->to_string(), "\"abc\"");
  EXPECT_EQ(fixture.runtime.eval(R"((str "a" 1 :rust))")->to_string(), "\"a1:rust\"");
  EXPECT_EQ(fixture.runtime.eval(R"((str [1 2 3]))")->to_string(), "\"[1 2 3]\"");
  EXPECT_EQ(fixture.runtime.eval(R"((str 'a' false {:a 1}))")->to_string(),
            R"("afalse{:a 1}")");
  EXPECT_EQ(fixture.runtime.eval(R"((str :key " " nil))")->to_string(), R"(":key nil")");
}

TEST(UpperCaseFunction, uppercase)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(upper-case \"mIxEd-CaSe!\")"), Lisple::String("MIXED-CASE!"));
  EXPECT_EQ(*runtime.eval("(upper-case \"The King is dead.\")"),
            Lisple::String("THE KING IS DEAD."));
  EXPECT_EQ(*runtime.eval("(upper-case :regular-key)"), Lisple::String(":REGULAR-KEY"));
  EXPECT_EQ(*runtime.eval("(upper-case {:x 10 :y 8})"), Lisple::String("{:X 10 :Y 8}"));
  EXPECT_EQ(*runtime.eval("(upper-case nil)"), Lisple::String("NIL"));
}

TEST(LowerCaseFunction, lowercase)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(lower-case \"mIxEd-CaSe!\")"), Lisple::String("mixed-case!"));
  EXPECT_EQ(*runtime.eval("(lower-case \"The King is dead.\")"),
            Lisple::String("the king is dead."));
  EXPECT_EQ(*runtime.eval("(lower-case :UPCASE-KEY)"), Lisple::String(":upcase-key"));
  EXPECT_EQ(*runtime.eval("(lower-case {:X 10 :Y 8})"), Lisple::String("{:x 10 :y 8}"));
  EXPECT_EQ(*runtime.eval("(lower-case nil)"), Lisple::String("nil"));
}

TEST(JoinFunction, join_strs)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // Then
  EXPECT_EQ(*fixture.runtime.eval(R"((join " " "This" "is" "bat" "country"))"),
            Lisple::String("This is bat country"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-" "hyphenate" "all" "the" "things"))"),
            Lisple::String("hyphenate-all-the-things"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join ", " "CSV" "to" "the" "rescue"))"),
            Lisple::String("CSV, to, the, rescue"));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-"))"), Lisple::String(""));
  EXPECT_EQ(*fixture.runtime.eval(R"((join "-" "foreveralone"))"),
            Lisple::String("foreveralone"));
}

TEST(IntFunction, char_to_int)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // Then
  EXPECT_EQ(*fixture.runtime.eval("(int 'c')"), Lisple::Number(99));
  EXPECT_EQ(*fixture.runtime.eval("(int ' ')"), Lisple::Number(32));
}

TEST(IntFunction, float_to_int)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // Then
  EXPECT_EQ(*fixture.runtime.eval("(int 12.0)"), Lisple::Number(12));
  EXPECT_EQ(*fixture.runtime.eval("(int 15.4)"), Lisple::Number(15));
  EXPECT_EQ(*fixture.runtime.eval("(int 15.5)"), Lisple::Number(16));
}

TEST(CeilFunction, ceil)
{
  // Given
  Lisple::Runtime runtime;

  // Then
  EXPECT_EQ(*runtime.eval("(ceil 10.2)"), Lisple::Number(11));
  EXPECT_EQ(*runtime.eval("(ceil 10.0)"), Lisple::Number(10));
  EXPECT_EQ(*runtime.eval("(ceil 5)"), Lisple::Number(5));
  EXPECT_EQ(*runtime.eval("(ceil 19.1)"), Lisple::Number(20));
}

TEST(PlusFunction, simple_addition)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(+ 10 5)");

  // When
  auto result = fixture.runtime.eval(code.at(0));

  // Then
  EXPECT_EQ(*result, Lisple::Number(15));
}

TEST(PlusFunction, multiple_arg_addition)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(+ 10 5 2 5)");

  // When
  auto result = fixture.runtime.eval(code.at(0));

  // Then
  EXPECT_EQ(*result, Lisple::Number(22));
}

TEST(MinusFunction, simple_subtraction)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(- 10 5)");

  // When
  auto result = fixture.runtime.eval(code.at(0));

  // Then
  EXPECT_EQ(*result, Lisple::Number(5));
}

TEST(MinusFunction, on_evaluated_numbers)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v code = fixture.parser.read_sexps("(- (+ 10 10) (+ 3 5))");

  // When
  auto result = fixture.runtime.eval(code.at(0));

  // Then
  EXPECT_EQ(*result, Lisple::Number(12));
}

TEST(MinusFunction, single_positive_flips_sign)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(- 25)");

  // Then
  ASSERT_EQ(*result, Lisple::Number(-25));
}

TEST(MinusFunction, single_negative_flips_sign)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto result = runtime.eval("(- -25)");

  // Then
  ASSERT_EQ(*result, Lisple::Number(25));
}

TEST(RangeFunction, rising_numbers)
{
  LispleTest::RuntimeFixture fixture;

  EXPECT_EQ(fixture.runtime.eval("(range 5 10)")->to_string(), "[5 6 7 8 9 10]");
  EXPECT_EQ(fixture.runtime.eval("(range -5 2)")->to_string(), "[-5 -4 -3 -2 -1 0 1 2]");
}

TEST(RangeFunction, descending_numbers)
{
  LispleTest::RuntimeFixture fixture;

  EXPECT_EQ(fixture.runtime.eval("(range 10 7)")->to_string(), "[10 9 8 7]");
  EXPECT_EQ(fixture.runtime.eval("(range 2 -5)")->to_string(), "[2 1 0 -1 -2 -3 -4 -5]");
}

TEST(EqualsFunction, ints)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(= 1 1)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= 50 (+ 25 25))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= 999 999)"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(= 1 2)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(= 50 (+ 25 250))"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(= 999 -999)"), *Lisple::B_FALSE);
}

TEST(EqualsFunction, string)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(= \"test\" \"test\")"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= \"a whole sentence\" \"a whole sentence\")"),
            *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= \" test\" \" test\")"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(= \"test\" \" test\")"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(= \"a whole sentence\" \"a_whole_sentence\")"),
            *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(= \" test\" \" test \")"), *Lisple::B_FALSE);
}

TEST(NotEqualsFunction, ints)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not= 1 2)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= 50 (+ 25 250))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= 999 -999)"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(not= 1 1)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= 50 (+ 25 25))"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= 999 999)"), *Lisple::B_FALSE);
}

TEST(NotEqualsFunction, chars)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not= 'a' 'b')"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= '-' ':')"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= '.' ',')"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(not= 'a' 'a')"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= '-' '-')"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= '.' '.')"), *Lisple::B_FALSE);
}

TEST(NotEqualsFunction, string)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not= \"test\" \" test\")"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \"a whole sentence\" \"a_whole_sentence\")"),
            *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \" test\" \" test \")"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(not= \"test\" \"test\")"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \"a whole sentence\" \"a whole sentence\")"),
            *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not= \" test\" \" test\")"), *Lisple::B_FALSE);
}

TEST(AndMacro, logical_and)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(and (odd? 1) (odd? 3))"), *Lisple::B_TRUE);

  EXPECT_EQ(*fixture.ctx.eval("(and (odd? 2) (odd? 3))"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(and (odd? 1) (odd? 2))"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(and (odd? 2) (odd? 4))"), *Lisple::B_FALSE);
}

TEST(OrFunction, logical_or)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(or (odd? 1) (odd? 3))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(or (odd? 2) (odd? 3))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(or (odd? 1) (odd? 2))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(or (odd? 2) (odd? 4))"), *Lisple::B_FALSE);
}

TEST(NilPredicateFunction, nil)
{
  LispleTest::RuntimeFixture fixture;
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
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(not true)"), *Lisple::B_FALSE);
  EXPECT_EQ(*fixture.ctx.eval("(not false)"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not (odd? 2))"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(not (odd? 1))"), *Lisple::B_FALSE);
}

TEST(NotFunction, values)
{
  // Given
  Lisple::Runtime reader;
  reader.eval("(def my-val 15)");
  reader.eval("(def other-val nil)");

  // Then
  EXPECT_EQ(*reader.eval("(not my-val)"), *Lisple::B_FALSE);
  EXPECT_EQ(*reader.eval("(not other-val)"), *Lisple::B_TRUE);
  EXPECT_EQ(*reader.eval("(not 0)"), *Lisple::B_FALSE);
}

TEST(EqualsFunction, mixed_types)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_NE(*fixture.ctx.eval("(= \"test\" 'test)"), *Lisple::B_TRUE);
  EXPECT_NE(*fixture.ctx.eval("(= :test 'test)"), *Lisple::B_TRUE);
  EXPECT_NE(*fixture.ctx.eval("(= :test \"test\")"), *Lisple::B_TRUE);

  EXPECT_NE(*fixture.ctx.eval("(= [1 2] {1 2})"), *Lisple::B_TRUE);
  EXPECT_EQ(*fixture.ctx.eval("(= [1 2] [1 2])"), *Lisple::B_TRUE);
}

TEST(MinMaxFunction, min)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(min 1 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 0 1)"), Lisple::Number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 0)"), Lisple::Number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 2 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 2)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 10 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 1 10)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(min 10 0)"), Lisple::Number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 0 10)"), Lisple::Number(0));
  EXPECT_EQ(*fixture.ctx.eval("(min 4 10 2 8 200)"), Lisple::Number(2));
}

TEST(MinMaxFunction, max)
{
  LispleTest::RuntimeFixture fixture;
  EXPECT_EQ(*fixture.ctx.eval("(max 1 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 0 1)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 0)"), Lisple::Number(1));
  EXPECT_EQ(*fixture.ctx.eval("(max 2 1)"), Lisple::Number(2));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 2)"), Lisple::Number(2));
  EXPECT_EQ(*fixture.ctx.eval("(max 10 1)"), Lisple::Number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 1 10)"), Lisple::Number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 10 0)"), Lisple::Number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 0 10)"), Lisple::Number(10));
  EXPECT_EQ(*fixture.ctx.eval("(max 4 10 9 199 200 8 2)"), Lisple::Number(200));
}

TEST(ThresholdFunction, threshold)
{
  LispleTest::RuntimeFixture fixture;
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
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v num1{std::make_shared<Lisple::Number>(1)};
  Lisple::sptr_sobject_v num2{std::make_shared<Lisple::Number>(2)};
  Lisple::sptr_sobject_v num3{std::make_shared<Lisple::Number>(3)};
  Lisple::sptr_sobject_v num4{std::make_shared<Lisple::Number>(4)};
  Lisple::sptr_sobject_v num5{std::make_shared<Lisple::Number>(5)};
  Lisple::sptr_sobject_v num6{std::make_shared<Lisple::Number>(6)};

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
  LispleTest::RuntimeFixture fixture;
  Lisple::sptr_sobject_v num1{std::make_shared<Lisple::Number>(1)};
  Lisple::sptr_sobject_v num2{std::make_shared<Lisple::Number>(2)};
  Lisple::sptr_sobject_v num3{std::make_shared<Lisple::Number>(3)};
  Lisple::sptr_sobject_v num4{std::make_shared<Lisple::Number>(4)};
  Lisple::sptr_sobject_v num5{std::make_shared<Lisple::Number>(5)};
  Lisple::sptr_sobject_v num6{std::make_shared<Lisple::Number>(6)};

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
  LispleTest::RuntimeFixture fixture;
  Lisple::EvalFunction eval_fn;

  Lisple::sptr_sobject_v args{std::make_shared<Lisple::String>("(+ 10 100)")};

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

TEST(RndFunction, max)
{
  LispleTest::RuntimeFixture fixture;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = fixture.ctx.eval("(rnd 5)")->as<Lisple::Number>().value;
    ASSERT_TRUE(rndval >= 0 && rndval <= 4);
  }
}

TEST(RndFunction, min_max)
{
  LispleTest::RuntimeFixture fixture;
  for (int i = 0; i < 1000; i++)
  {
    int rndval = fixture.ctx.eval("(rnd 50 55)")->as<Lisple::Number>().value;
    ASSERT_TRUE(rndval >= 50 && rndval <= 54);
  }
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

TEST(GetFunction, get_from_map)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = fixture.runtime.eval("(get my-map :b)");

  // Then
  ASSERT_EQ(*result, Lisple::Number(2));
}

TEST(AssocFunction, add_key_to_map)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = fixture.runtime.eval("(assoc my-map :c 3)");

  // Then
  EXPECT_EQ(*result, *fixture.runtime.eval("{:a 1 :b 2 :c 3}"));
  EXPECT_EQ(*fixture.runtime.lookup(Lisple::Word("my-map")),
            *fixture.runtime.eval("{:a 1 :b 2}"));
}

TEST(AssocFunction, replace_key_in_map)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = fixture.runtime.eval("(assoc my-map :a 10)");

  // Then
  EXPECT_EQ(*result, *fixture.runtime.eval("{:a 10 :b 2}"));
  EXPECT_EQ(*fixture.runtime.lookup(Lisple::Word("my-map")),
            *fixture.runtime.eval("{:a 1 :b 2}"));
}

TEST(AssocBangFunction, add_key_to_map)
{
  // Given
  Lisple::Runtime runtime;
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
  Lisple::Runtime runtime;
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
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2})");

  // When
  auto result = runtime.eval("(assoc! my-map :b 10 :c 3 :d \"some string\")");

  // Then
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}"));
  EXPECT_EQ(*runtime.lookup(Lisple::Word("my-map")),
            *runtime.eval("{:a 1 :b 10 :c 3 :d \"some string\"}"));
}

TEST(DissocBangFunction, removal_of_non_existing_key_returns_nil)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def my-map {:a 1 :b 2 :c 3})");

  // When
  auto result = runtime.eval("(dissoc! my-map :d)");

  // Then
  EXPECT_EQ(*result, *Lisple::NIL);
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
  EXPECT_EQ(*result, *Lisple::Number::make(2));
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
  auto& global_var =
    *fixture.runtime.get_current_namespace().lookup(Lisple::Word("my-global-var"));
  ASSERT_EQ(global_var.as<Lisple::Number>().value, Lisple::Number(50).value);
}

TEST(SetBangMacro, set_parent_scope_value)
{
  // Given
  LispleTest::RuntimeFixture fixture;

  // When
  auto my_var = fixture.runtime.eval("(let [my-var 10] (do (set! [my-var] 20) my-var))");

  // Then
  EXPECT_EQ(my_var->as<Lisple::Number>().value, 20);
}

TEST(SetBangMacro, set_global_map_value)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def global-map {:key1 10 :key2 20})");

  // When
  fixture.runtime.eval("(set! [:key2 global-map] \"Number 9\")");

  // Then
  auto& global_map =
    *fixture.runtime.get_current_namespace().lookup(Lisple::Word("global-map"));
  auto expected_map = fixture.runtime.eval("{:key1 10 :key2 \"Number 9\"}");
  ASSERT_EQ(global_map, *expected_map);
}

TEST(ForMacro, transform_vector_of_int)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def my-vector [1 2 3 4 5 6])");

  // When
  auto retval = fixture.runtime.eval("(for [num my-vector] (* num 2))");

  // Then
  ASSERT_EQ(retval->to_string(), "[2 4 6 8 10 12]");
}

TEST(ForMacro, with_map_destructuring)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def my-vector [{:a 1 :b 10} {:a 5 :b 4}])");

  // When
  auto retval = fixture.runtime.eval("(for [{:keys [a b]} my-vector] (+ a b))");

  // Then
  ASSERT_EQ(retval->to_string(), "[11 9]");
}

TEST(WhileMacro, loop_with_counter)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def x 0)");

  // When
  auto retval = fixture.runtime.eval("(while (not= x 10) (set! [x] (+ x 1)))");

  // Then
  EXPECT_EQ(*retval, Lisple::Number(10));
  EXPECT_EQ(*fixture.runtime.get_current_namespace().lookup(Lisple::Word("x")),
            Lisple::Number(10));
}

TEST(WhileMacro, multi_form_loop)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  fixture.runtime.eval("(def x 0)");

  // When
  auto retval = fixture.runtime.eval("(while (not= x 10) (set! [x] (+ x 1)) (* x 2))");

  // Then
  EXPECT_EQ(*retval, Lisple::Number(20));
  EXPECT_EQ(*fixture.runtime.get_current_namespace().lookup(Lisple::Word("x")),
            Lisple::Number(10));
}
