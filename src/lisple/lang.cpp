
#include "lisple/lang.h"

#include "lisple/lang/num.h"
#include "lisple/runtime/value.h"

#include <map>

#include <lisple/bind.h>
#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/host.h>
#include <lisple/impl.h>
#include <lisple/lang/base.h>
#include <lisple/lang/bind_form.h>
#include <lisple/lang/branch.h>
#include <lisple/lang/func.h>
#include <lisple/lang/loop.h>
#include <lisple/lang/math.h>
#include <lisple/lang/operator.h>
#include <lisple/lang/predicate.h>
#include <lisple/lang/rewrite.h>
#include <lisple/lang/seq.h>
#include <lisple/lang/seq_func.h>
#include <lisple/lang/string.h>
#include <lisple/lang/struct.h>
#include <lisple/lang/time.h>
#include <lisple/namespace.h>
#include <lisple/runtime/dict.h>
#include <lisple/scope.h>
#include <lisple/type.h>

#include <bits/std_abs.h>

namespace Lisple
{
  Namespace make_language_namespace()
  {
    std::map<std::string, Lisple::sptr_sobject> lang;
    std::map<std::string, sptr_rtval> lang_symbols;

    lang_symbols.emplace("+", PlusFunction::make());
    lang_symbols.emplace("-", MinusFunction::make());
    lang_symbols.emplace("/", DivideFunction::make());
    lang_symbols.emplace("*", MultiplyFunction::make());
    lang_symbols.emplace("=", EqualsPFunction::make());
    lang_symbols.emplace("<", LessThanFunction::make());
    lang_symbols.emplace("<=", LessThanOrEqualsFunction::make());
    lang_symbols.emplace(">", GreaterThanFunction::make());
    lang_symbols.emplace(">=", GreaterThanOrEqualsFunction::make());
    lang_symbols.emplace("->", ThreadFirstForm::make());
    lang_symbols.emplace("abs", AbsFunction::make());
    lang_symbols.emplace("and", AndForm::make());
    lang_symbols.emplace("append!", AppendBangFunction::make());
    lang_symbols.emplace("apply", ApplyFunction::make());
    lang_symbols.emplace("assoc", AssocFunction::make());
    lang_symbols.emplace("assoc!", AssocBangFunction::make());
    lang_symbols.emplace("assoc-in", AssocInFunction::make());
    lang_symbols.emplace("assoc-in!", AssocInBangFunction::make());
    lang_symbols.emplace("update", UpdateFunction::make());
    lang_symbols.emplace("update-in", UpdateInFunction::make());
    lang_symbols.emplace("between?", BetweenPredicateFunction::make());
    lang_symbols.emplace("blank?", BlankPFunction::make());
    lang_symbols.emplace("case", CaseForm::make());
    lang_symbols.emplace("ceil", CeilFunction::make());
    lang_symbols.emplace("comment", CommentForm::make());
    lang_symbols.emplace("concat", ConcatFunction::make());
    lang_symbols.emplace("concat!", ConcatBangFunction::make());
    lang_symbols.emplace("cond", CondForm::make());
    lang_symbols.emplace("contains?", ContainsPFunction::make());
    lang_symbols.emplace("cos", CosFunction::make());
    lang_symbols.emplace("count", CountFunction::make());
    lang_symbols.emplace("dec", DecFunction::make());
    lang_symbols.emplace("def", DefForm::make());
    lang_symbols.emplace("defun", DefunForm::make());
    lang_symbols.emplace("dissoc", DissocFunction::make());
    lang_symbols.emplace("dissoc!", DissocBangFunction::make());
    lang_symbols.emplace("do", DoForm::make());
    lang_symbols.emplace("dotimes", DoTimesForm::make());
    lang_symbols.emplace("empty?", EmptyPFunction::make());
    lang_symbols.emplace("epoch-ms", EpochMsFunction::make());
    lang_symbols.emplace("eval", EvalFunction::make());
    lang_symbols.emplace("even?", EvenPFunction::make());
    lang_symbols.emplace("false", Constant::BOOL_FALSE);
    lang_symbols.emplace("flatten", FlattenFunction::make());
    lang_symbols.emplace("filter", FilterFunction::make());
    lang_symbols.emplace("find-first", FindFirstFunction::make());
    lang_symbols.emplace("find-index", FindIndexFunction::make());
    lang_symbols.emplace("fn", FnForm::make());
    lang_symbols.emplace("for", ForForm::make());
    lang_symbols.emplace("for-indexed", ForIndexedForm::make());
    lang_symbols.emplace("get", GetFunction::make());
    lang_symbols.emplace("get-in", GetInFunction::make());
    lang_symbols.emplace("head", HeadFunction::make());
    lang_symbols.emplace("if", IfForm::make());
    lang_symbols.emplace("if-let", IfLetForm::make());
    lang_symbols.emplace("inc", IncFunction::make());
    lang_symbols.emplace("include", IncludeFunction::make());
    lang_symbols.emplace("int", IntFunction::make());
    lang_symbols.emplace("join", JoinFunction::make());
    lang_symbols.emplace("keep", KeepFunction::make());
    lang_symbols.emplace("keys", KeysFunction::make());
    lang_symbols.emplace("keyword", KeywordFunction::make());
    lang_symbols.emplace("keyword?", KeywordPFunction::make());
    lang_symbols.emplace("last", LastFunction::make());
    lang_symbols.emplace("let", LetForm::make());
    lang_symbols.emplace("lower-case", LowerCaseFunction::make());
    lang_symbols.emplace("map", MapFunction::make());
    lang_symbols.emplace("max", MaxFunction::make());
    lang_symbols.emplace("merge", MergeFunction::make());
    lang_symbols.emplace("min", MinFunction::make());
    lang_symbols.emplace("mod", ModulusFunction::make());
    lang_symbols.emplace("name", NameFunction::make());
    lang_symbols.emplace("nil", Constant::NIL);
    lang_symbols.emplace("nil?", NilPFunction::make());
    lang_symbols.emplace("not", NotFunction::make());
    lang_symbols.emplace("not=", NotEqualsFunction::make());
    lang_symbols.emplace("not-empty?", NotEmptyPFunction::make());
    lang_symbols.emplace("ns", NsForm::make());
    lang_symbols.emplace("nth", NthFunction::make());
    lang_symbols.emplace("number?", NumberPFunction::make());
    lang_symbols.emplace("odd?", OddPFunction::make());
    lang_symbols.emplace("or", OrForm::make());
    lang_symbols.emplace("partition", PartitionFunction::make());
    lang_symbols.emplace("prn", PrnFunction::make());
    lang_symbols.emplace("qualifier", QualifierFunction::make());
    lang_symbols.emplace("rand-nth", RandNthFunction::make());
    lang_symbols.emplace("range", RangeFunction::make());
    lang_symbols.emplace("reduce", ReduceFunction::make());
    lang_symbols.emplace("reduce-kv", ReduceKeyValueFunction::make());
    lang_symbols.emplace("remove", RemoveFunction::make());
    lang_symbols.emplace("remove-first", RemoveFirstFunction::make());
    lang_symbols.emplace("remove!", RemoveBangFunction::make());
    lang_symbols.emplace("remove-nth", RemoveNthFunction::make());
    lang_symbols.emplace("remove-nth!", RemoveNthBangFunction::make());
    lang_symbols.emplace("repeat", RepeatFunction::make());
    lang_symbols.emplace("resolve", ResolveFunction::make());
    lang_symbols.emplace("rnd", RndFunction::make());
    lang_symbols.emplace("select-keys", SelectKeysFunction::make());
    lang_symbols.emplace("seq-match", SeqMatchFunction::make());
    lang_symbols.emplace("set!", SetBangForm::make());
    lang_symbols.emplace("sin", SinFunction::make());
    lang_symbols.emplace("some?", SomeFunction::make());
    lang_symbols.emplace("sort", SortFunction::make());
    lang_symbols.emplace("sqrt", SqrtFunction::make());
    lang_symbols.emplace("str", StrFunction::make());
    lang_symbols.emplace("string?", StringPFunction::make());
    lang_symbols.emplace("tail", TailFunction::make());
    lang_symbols.emplace("take", TakeFunction::make());
    lang_symbols.emplace("true", Constant::BOOL_TRUE);
    lang_symbols.emplace("upper-case", UpperCaseFunction::make());
    lang_symbols.emplace("vector", VectorFunction::make());
    lang_symbols.emplace("when", WhenForm::make());
    lang_symbols.emplace("when-let", WhenLetForm::make());
    lang_symbols.emplace("while", WhileForm::make());
    lang_symbols.emplace("zero?", ZeroPFunction::make());

    return Namespace::make_lang(lang, lang_symbols);
  }

} // namespace Lisple
