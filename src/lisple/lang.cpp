
#include "lisple/lang.h"

#include "lisple/lang/num.h"
#include "lisple/runtime/value.h"

#include <algorithm>
#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

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
    lang_symbols.emplace("assoc-in!", AssocInBangFunction::make());
    lang_symbols.emplace("between?", BetweenPredicateFunction::make());
    lang_symbols.emplace("case", CaseForm::make());
    lang_symbols.emplace("ceil", CeilFunction::make());
    lang_symbols.emplace("comment", CommentForm::make());
    lang_symbols.emplace("concat", ConcatFunction::make());
    lang_symbols.emplace("concat!", ConcatBangFunction::make());
    lang_symbols.emplace("cond", CondForm::make());
    lang_symbols.emplace("contains?", ContainsPFunction::make());
    lang_symbols.emplace("cos", CosFunction::make());
    lang_symbols.emplace("count", CountFunction::make());
    lang_symbols.emplace("def", DefForm::make());
    lang_symbols.emplace("defun", DefunForm::make());
    lang_symbols.emplace("dissoc", DissocFunction::make());
    lang_symbols.emplace("dissoc!", DissocBangFunction::make());
    lang_symbols.emplace("do", DoForm::make());
    lang_symbols.emplace("dotimes", DoTimesForm::make());
    lang_symbols.emplace("empty?", EmptyPFunction::make());
    lang.emplace("eval", std::make_shared<EvalFunction>());
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
    lang_symbols.emplace("head", HeadFunction::make());
    lang_symbols.emplace("if", IfForm::make());
    lang_symbols.emplace("if-let", IfLetForm::make());
    lang.emplace("include", std::make_shared<IncludeFunction>());
    lang_symbols.emplace("int", IntFunction::make());
    lang_symbols.emplace("join", JoinFunction::make());
    lang_symbols.emplace("keep", KeepFunction::make());
    lang_symbols.emplace("keys", KeysFunction::make());
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
    lang_symbols.emplace("odd?", OddPFunction::make());
    lang_symbols.emplace("or", OrForm::make());
    lang_symbols.emplace("partition", PartitionFunction::make());
    lang.emplace("prn", std::make_shared<PrintFunction>());
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
    lang.emplace("repeat", std::make_shared<RepeatFunction>());
    lang.emplace("resolve", std::make_shared<ResolveFunction>());
    lang_symbols.emplace("rnd", RndFunction::make());
    lang_symbols.emplace("select-keys", SelectKeysFunction::make());
    lang_symbols.emplace("seq-match", SeqMatchFunction::make());
    lang_symbols.emplace("set!", SetBangForm::make());
    lang_symbols.emplace("sin", SinFunction::make());
    lang_symbols.emplace("some?", SomeFunction::make());
    lang_symbols.emplace("sort", SortFunction::make());
    lang_symbols.emplace("sqrt", SqrtFunction::make());
    lang_symbols.emplace("str", StrFunction::make());
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

  /* PrintFunction - prn */
  FUNC_IMPL(PrintFunction,
            SIG((FN_ARGS((&VARARG, &Type::ANY)), EXEC_DISPATCH(&PrintFunction::do_print))))

  FUNC_BODY(PrintFunction, do_print)
  {
    for (size_t i = 0; i < args.size(); i++)
    {
      auto obj = args[i];
      if (i > 0) std::cout << " ";
      if (obj->get_type() == Form::STRING)
      {
        std::cout << obj->as<String>().value;
      }
      else
      {
        std::cout << obj->to_string();
      }
    }

    std::cout << std::endl;
    return NIL;
  }

  FUNC_IMPL(IncludeFunction,
            SIG((FN_ARGS((&Lisple::Type::STRING)),
                 EXEC_DISPATCH(&IncludeFunction::include_file))))

  FUNC_BODY(IncludeFunction, include_file)
  {
    ctx.read_file(Lisple::Value<std::string>::value_of(*args[0]));
    return args[0];
  }

  /** EvalFunction - eval */
  FUNC_IMPL(
    EvalFunction,
    MULTI_SIG((FN_ARGS((&Lisple::Type::STRING)), EXEC_DISPATCH(&EvalFunction::eval_string)),
              (FN_ARGS((&Lisple::Type::ANY)), EXEC_DISPATCH(&EvalFunction::eval_form))))

  FUNC_BODY(EvalFunction, eval_string)
  {
    const std::string& str = args[0]->as<Lisple::String>().value;
    return RuntimeValueWrapper::make(ctx.eval(str));
  }

  FUNC_BODY(EvalFunction, eval_form)
  {
    return ctx.eval_ast(args[0]);
  }

  /** ResolveFunction - resolve */
  FUNC_IMPL(
    ResolveFunction,
    MULTI_SIG((FN_ARGS((&Lisple::Type::SYMBOL)), EXEC_DISPATCH(&ResolveFunction::resolve)),
              (FN_ARGS((&Lisple::Type::WORD)), EXEC_DISPATCH(&ResolveFunction::resolve))));

  FUNC_BODY(ResolveFunction, resolve)
  {
    if (*args[0] == *NIL) return NIL;
    return ctx.lookup(args[0]->as<Value<std::string>>().value);
  }

} // namespace Lisple
