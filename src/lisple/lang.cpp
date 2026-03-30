
#include "lisple/lang.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

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
#include <ctype.h>

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
    lang_symbols.emplace("=", EqualsPredicateFunction::make());
    lang_symbols.emplace("<", LessThanFunction::make());
    lang_symbols.emplace("<=", LessThanOrEqualsFunction::make());
    lang_symbols.emplace(">", GreaterThanFunction::make());
    lang_symbols.emplace(">=", GreaterThanOrEqualsFunction::make());
    lang_symbols.emplace("->", ThreadFirstForm::make());
    lang_symbols.emplace("abs", AbsFunction::make());
    lang_symbols.emplace("and", AndForm::make());
    lang.emplace("append!", std::make_shared<AppendBangFunction>());
    lang.emplace("apply", std::make_shared<ApplyFunction>());
    lang_symbols.emplace("assoc", AssocFunction::make());
    lang_symbols.emplace("assoc!", AssocBangFunction::make());
    lang.emplace("assoc-in!", std::make_shared<AssocInBangFunction>());
    lang.emplace("between?", std::make_shared<BetweenPredicateFunction>());
    lang.emplace("case", std::make_shared<CaseMacro>());
    lang.emplace("ceil", std::make_shared<CeilFunction>());
    lang.emplace("comment", std::make_shared<CommentMacro>());
    lang_symbols.emplace("concat", ConcatFunction::make());
    lang.emplace("concat!", std::make_shared<ConcatBangFunction>());
    lang_symbols.emplace("cond", CondForm::make());
    lang.emplace("contains?", std::make_shared<ContainsPredicateFunction>());
    lang_symbols.emplace("cos", CosFunction::make());
    lang_symbols.emplace("count", CountFunction::make());
    lang_symbols.emplace("def", DefForm::make());
    lang_symbols.emplace("defun", DefunForm::make());
    lang.emplace("dissoc!", std::make_shared<DissocBangFunction>());
    lang.emplace("do", std::make_shared<DoMacro>());
    lang_symbols.emplace("dotimes", DoTimesForm::make());
    lang.emplace("empty?", std::make_shared<EmptyPredicateFunction>());
    lang.emplace("eval", std::make_shared<EvalFunction>());
    lang.emplace("even?", std::make_shared<OddEvenPredicateFunction>(0));
    lang_symbols.emplace("false", Constant::BOOL_FALSE);
    lang.emplace("flatten", std::make_shared<FlattenFunction>());
    lang.emplace("filter", std::make_shared<FilterFunction>());
    lang.emplace("find-first", std::make_shared<FindFirstFunction>());
    lang_symbols.emplace("find-index", FindIndexFunction::make());
    lang_symbols.emplace("fn", FnForm::make());
    lang_symbols.emplace("for", ForForm::make());
    lang.emplace("for-indexed", std::make_shared<ForIndexedMacro>());
    lang_symbols.emplace("get", GetFunction::make());
    lang.emplace("head", std::make_shared<HeadFunction>());
    lang_symbols.emplace("if", IfForm::make());
    lang_symbols.emplace("if-let", IfLetForm::make());
    lang.emplace("include", std::make_shared<IncludeFunction>());
    lang.emplace("int", std::make_shared<IntFunction>());
    lang.emplace("join", std::make_shared<JoinFunction>());
    lang.emplace("keep", std::make_shared<KeepFunction>());
    lang.emplace("keys", std::make_shared<KeysFunction>());
    lang.emplace("last", std::make_shared<LastFunction>());
    lang_symbols.emplace("let", LetForm::make());
    lang.emplace("lower-case", std::make_shared<LowerCaseFunction>());
    lang_symbols.emplace("map", MapFunction::make());
    lang.emplace("max", std::make_shared<MinMaxFunction>(false));
    lang.emplace("merge", std::make_shared<MergeFunction>());
    lang.emplace("min", std::make_shared<MinMaxFunction>(true));
    lang.emplace("name", std::make_shared<NameFunction>());
    lang.emplace("namespace", std::make_shared<NamespaceFunction>());
    lang_symbols.emplace("nil", Constant::NIL);
    lang.emplace("nil?", std::make_shared<NilPredicateFunction>());
    lang_symbols.emplace("not", NotFunction::make());
    lang.emplace("not=", std::make_shared<NotEqualsFunction>());
    lang.emplace("not-empty?", std::make_shared<NotEmptyPredicateFunction>());
    lang.emplace("ns", std::make_shared<NsMacro>());
    lang_symbols.emplace("nth", NthFunction::make());
    lang.emplace("odd?", std::make_shared<OddEvenPredicateFunction>(1));
    lang_symbols.emplace("or", OrForm::make());
    lang.emplace("partition", std::make_shared<PartitionFunction>());
    lang.emplace("prn", std::make_shared<PrintFunction>());
    lang_symbols.emplace("rand-nth", RandNthFunction::make());
    lang_symbols.emplace("range", RangeFunction::make());
    lang_symbols.emplace("reduce", ReduceFunction::make());
    lang.emplace("reduce-kv", std::make_shared<ReduceKeyValueFunction>());
    lang.emplace("remove", std::make_shared<RemoveFunction>());
    lang.emplace("remove-first", std::make_shared<RemoveFirstFunction>());
    lang.emplace("remove!", std::make_shared<RemoveBangFunction>());
    lang_symbols.emplace("remove-nth", RemoveNthFunction::make());
    lang.emplace("remove-nth!", std::make_shared<RemoveNthBangFunction>());
    lang.emplace("repeat", std::make_shared<RepeatFunction>());
    lang.emplace("resolve", std::make_shared<ResolveFunction>());
    lang_symbols.emplace("rnd", RndFunction::make());
    lang.emplace("select-keys", std::make_shared<SelectKeysFunction>());
    lang.emplace("seq-match", std::make_shared<SeqMatchFunction>());
    lang.emplace("set!", std::make_shared<SetBangMacro>());
    lang.emplace("sin", std::make_shared<SinFunction>());
    lang.emplace("some?", std::make_shared<SomeFunction>());
    lang.emplace("sort", std::make_shared<SortFunction>());
    lang_symbols.emplace("sqrt", SqrtFunction::make());
    lang_symbols.emplace("str", StrFunction::make());
    lang.emplace("tail", std::make_shared<TailFunction>());
    lang.emplace("take", std::make_shared<TakeFunction>());
    lang.emplace("threshold", std::make_shared<ThresholdFunction>());
    lang_symbols.emplace("true", Constant::BOOL_TRUE);
    lang.emplace("upper-case", std::make_shared<UpperCaseFunction>());
    lang.emplace("vector", std::make_shared<VectorFunction>());
    lang.emplace("when", std::make_shared<WhenMacro>());
    lang.emplace("when-let", std::make_shared<WhenLetMacro>());
    lang.emplace("while", std::make_shared<WhileMacro>());

    return Namespace::make_lang(lang, lang_symbols);
  }

  /* NsMacro */
  MACRO_IMPL(NsMacro,
             MULTI_SIG((FN_ARGS((&Type::WORD, DATA)), EXEC_DISPATCH(&NsMacro::switch_ns)),
                       (FN_ARGS((&Type::WORD, DATA), (&Type::LIST, DATA)),
                        EXEC_DISPATCH(&NsMacro::switch_ns))))

  Key KEY_REQUIRE("require");
  Key KEY_AS("as");

  void throw_ns_exception(Word& ns, List& req_list, std::string msg = "")
  {
    std::string ns_decl = "(ns " + ns.value;

    for (size_t i = 0; i < req_list.get_children().size(); i++)
    {
      ns_decl += " " + req_list.get_children()[i]->to_string();
    }
    ns_decl += ")";

    throw NamespaceException("Invalid ns form: " + ns_decl + msg);
  }

  MACRO_BODY(NsMacro, switch_ns)
  {
    Word& ns_word = args[0]->as<Word>();
    Lisple::sptr_sobject_v imports;
    if (args.size() == 2)
    {
      Lisple::List& list = args.back()->as<List>();
      if (list.size() < 2 || (list.size() > 0 && *list.get_children()[0] != KEY_REQUIRE))
      {
        throw_ns_exception(ns_word, list);
      }
      imports = list.tail();

      // Verify the require forms the brute-ish and tedious way...
      // FIXME: Maybe implement some kind of Matcher system for forms?
      for (auto& imp : imports)
      {
        if (Type::WORD.is_type_of(*imp))
        {
          if (imp->as<Word>().is_qualified())
          {
            throw_ns_exception(ns_word,
                               list,
                               ". Invalid require-entry: " + imp->to_string());
          }
        }
        else if (Type::ARRAY.is_type_of(*imp))
        {
          if (imp->get_children().size() != 3)
          {
            throw_ns_exception(ns_word,
                               list,
                               ". Invalid require-entry: " + imp->to_string());
          }

          if (*imp->get_children()[1] == KEY_AS)
          {
            if (!Type::WORD.is_type_of(*imp->get_children().back()) ||
                imp->get_children().back()->as<Word>().is_qualified())
            {
              throw_ns_exception(ns_word,
                                 list,
                                 ". Invalid require-entry: " + imp->to_string());
            }
          }
          else
          {
            throw_ns_exception(ns_word,
                               list,
                               ". Invalid require-entry: " + imp->to_string());
          }
        }
        else
        {
          throw_ns_exception(ns_word, list, ". Invalid require-entry: " + imp->to_string());
        }
      }
    }

    ctx.switch_namespace(ns_word.value);
    for (auto& imp : imports)
    {
      if (Type::WORD.is_type_of(*imp))
      {
        // Full import
        Word& imp_word = imp->as<Word>();
        ctx.import_namespace(imp_word.value);
      }
      else if (Type::ARRAY.is_type_of(*imp))
      {
        // Aliased import
        Array& imp_array = imp->as<Array>();
        Word& imp_word = imp_array.head()->as<Word>();
        Word& alias_word = imp_array.get_children().back()->as<Word>();
        ctx.define_namespace_alias(imp_word.value, alias_word.value);
      }
    }

    return NIL;
  }

  /* CommentMacro */
  MACRO_IMPL(CommentMacro,
             SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&CommentMacro::comment))))

  MACRO_BODY(CommentMacro, comment)
  {
    return NIL;
  }

  /* WhenLetMacro */
  MACRO_IMPL(WhenLetMacro,
             SIG((FN_ARGS((&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&WhenLetMacro::make_when_let))))

  MACRO_BODY(WhenLetMacro, make_when_let)
  {
    Object& var_def_array = *args[0];

    if (var_def_array.get_children().size() % 2 != 0)
    {
      throw LispleException(
        "Wrong number of parameters in binding form of when-let expression: " +
        var_def_array.to_string());
    }

    bool contains_nil = false;

    size_t scopes = 0;
    for (size_t i = 0; i < var_def_array.size(); i += 2)
    {
      auto& var_name_obj = *var_def_array.get_children()[i];
      auto var_val_obj = ctx.eval(var_def_array.get_children()[i + 1]);

      if (*var_val_obj == *NIL)
      {
        contains_nil = true;
        break;
      }

      if (!Type::WORD.is_type_of(var_name_obj))
      {
        throw TypeError(
          "Invalid variable identifier in binding form of when-let expression: " +
          var_name_obj.to_string() + " in " + var_def_array.to_string());
      }

      Scope var_scope;
      var_scope.store(var_name_obj.as<Word>(), var_val_obj);
      ctx.push_context(true, var_scope);
      scopes++;
    }

    sptr_sobject result = NIL;

    if (!contains_nil)
    {
      for (size_t i = 1; i < args.size(); i++)
      {
        result = ctx.eval(args[i]);
      }
    }

    for (size_t i = 0; i < scopes; i++)
    {
      ctx.pop_context();
    }

    return result;
  }

  MACRO_IMPL(DoMacro,
             SIG((FN_ARGS((&VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&DoMacro::make_do))))

  MACRO_BODY(DoMacro, make_do)
  {
    Lisple::sptr_sobject ret;
    ctx.push_context(true);
    for (auto& arg : args)
    {
      ret = ctx.eval(arg);
    }
    ctx.pop_context();
    return ret;
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

  /* NilPredicateFunction */
  FUNC_IMPL(NilPredicateFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&NilPredicateFunction::is_nil))))

  FUNC_BODY(NilPredicateFunction, is_nil)
  {
    return *args[0] == *NIL ? B_TRUE : B_FALSE;
  }

  SetBangMacro::SetBangMacro()
    : Macro(SIG((FN_ARGS((&Type::ARRAY, DATA), (&Lisple::Type::ANY)),
                 EXEC_DISPATCH(&SetBangMacro::do_set_member))))
  {
  }

  MACRO_BODY(SetBangMacro, do_set_member)
  {
    Lisple::Array& member_ref = args[0]->as<Lisple::Array>();

    if (member_ref.size() == 1)
    {
      auto identifier = member_ref.get_children()[0]->as<Lisple::Word>();
      Scope& scope = ctx.get_scope_of(identifier);
      scope.mutate(identifier, args.back());
    }
    else if (member_ref.size() == 2)
    {
      auto actual_mem_ref = ctx.eval(args[0]);
      Lisple::Object& prop = *actual_mem_ref->get_children()[0];
      Lisple::Object& owner = *actual_mem_ref->get_children().back();

      owner.set_property(&ctx, prop, args.back());
    }
    else
    {
      throw Lisple::InvocationException("Incorrect member reference: " +
                                        member_ref.to_string());
    }

    return args.back();
  }

  MACRO_IMPL(WhileMacro,
             SIG((FN_ARGS((&Type::ANY, NO_EVAL), (VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&WhileMacro::make_while))))

  MACRO_BODY(WhileMacro, make_while)
  {
    Lisple::sptr_sobject retval = NIL;

    ctx.push_context(true);
    while (ctx.eval(args[0])->is_truthy())
    {
      for (size_t i = 1; i < args.size(); i++)
      {
        retval = ctx.eval(args[i]);
      }
    }
    ctx.pop_context();

    return retval;
  }

  /* when */
  MACRO_IMPL(WhenMacro,
             SIG((FN_ARGS((&Type::ANY, NO_EVAL), (VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&WhenMacro::make_when))))

  MACRO_BODY(WhenMacro, make_when)
  {
    sptr_sobject retval = NIL;

    ctx.push_context(true);
    auto condition = ctx.eval(args[0]);
    if (condition->is_truthy())
    {
      for (size_t i = 1; i < args.size(); i++)
      {
        retval = ctx.eval(args[i]);
      }
    }
    ctx.pop_context();
    return retval;
  }

  /* case */
  MACRO_IMPL(CaseMacro,
             SIG((FN_ARGS((&Type::ANY, NO_EVAL), (VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&CaseMacro::make_case))))

  Key DEFAULT = Key("default");

  MACRO_BODY(CaseMacro, make_case)
  {
    if ((args.size() - 1) % 2 != 0)
    {
      throw InvocationException("Incomplete condition-expression pair passed to case");
    }
    else if (args.size() == 1)
    {
      throw InvocationException("Empty case-form");
    }

    sptr_sobject retval = NIL;

    ctx.push_context(true);
    sptr_sobject value = ctx.eval(args[0]);

    for (size_t i = 1; i < args.size(); i += 2)
    {
      if (*ctx.eval(args[i]) == *value || *args[i] == DEFAULT)
      {
        retval = ctx.eval(args[i + 1]);
        break;
      }
    }

    ctx.pop_context();
    return retval;
  }

  /* ForIndexedMacro - for-indexed */
  MACRO_IMPL(ForIndexedMacro,
             SIG((FN_ARGS((&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&ForIndexedMacro::make_for))))

  MACRO_BODY(ForIndexedMacro, make_for)
  {
    sptr_sobject_v& bind_form = args[0]->get_children();

    if (bind_form.size() != 3)
    {
      throw InvocationException(
        "Invalid number of elements in for-indexed bind form, expected 3: " +
        args[0]->to_string());
    }

    if (!Type::WORD.is_type_of(*bind_form[0]))
    {
      throw TypeError("for-indexed macro requires the index variable to be a word");
    }

    if (!Type::WORD.is_type_of(*bind_form[1]))
    {
      throw TypeError("for-indexed macro requires the iteration variable to be a word");
    }

    auto seq = ctx.eval(bind_form[2]);
    if (!Type::SEQ.is_type_of(*seq))
    {
      throw TypeError("for-indexed macro requires an iterable. Wrong type: " +
                      bind_form[2]->to_string());
    }

    auto index_binding = ArgumentBinding::create(*bind_form[0]);
    auto seq_binding = ArgumentBinding::create(*bind_form[1]);

    sptr_sobject_v result;
    result.reserve(seq->size());

    ctx.push_context(true);
    Scope& iter_scope = ctx.current_scope();
    for (size_t i = 0; i < seq->size(); i++)
    {
      auto& lmnt = seq->get_children()[i];
      sptr_sobject index = Number::make(static_cast<int>(i));
      index_binding->apply(iter_scope, index);
      seq_binding->apply(iter_scope, lmnt);
      sptr_sobject iter_result;
      for (size_t e = 1; e < args.size(); e++)
      {
        iter_result = ctx.eval(args[e]);
      }
      result.push_back(std::move(iter_result));
      iter_scope.clear();
    }
    ctx.pop_context();

    return std::make_shared<Array>(std::move(result));
  }

  FUNC_IMPL(NotEqualsFunction,
            SIG((FN_ARGS((&Type::ANY), (&Type::ANY)),
                 EXEC_DISPATCH(&NotEqualsFunction::not_equals_any))))

  FUNC_BODY(NotEqualsFunction, not_equals_any)
  {
    return *args[0] == *args[1] ? B_FALSE : B_TRUE;
  }

  /* CeilFunction - ceil */
  FUNC_IMPL(CeilFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&CeilFunction::ceil))))

  FUNC_BODY(CeilFunction, ceil)
  {
    Number& num = args[0]->as<Number>();
    return Number::make(static_cast<int>(std::ceil(num.float_value())));
  }

  /* IntFunction - int */
  FUNC_IMPL(IntFunction, SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&IntFunction::to_int))))

  FUNC_BODY(IntFunction, to_int)
  {
    sptr_sobject& obj = args[0];

    if (Type::NUMBER.is_type_of(*obj))
    {
      return Number::make(obj->as<Number>().int_value());
    }
    else if (Type::CHAR.is_type_of(*obj))
    {
      return Number::make(static_cast<int>(obj->as<Char>().value));
    }

    throw LispleException("Cannot convert " + obj->to_string() + " to integer.");
  }

  /* BetweenPredicateFunction - between? */
  FUNC_IMPL(BetweenPredicateFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&BetweenPredicateFunction::between))))

  FUNC_BODY(BetweenPredicateFunction, between)
  {
    return Lisple::float_val(*args[0]) > Lisple::float_val(*args[1]) &&
               Lisple::float_val(*args[0]) < Lisple::float_val(*args[2])
             ? B_TRUE
             : B_FALSE;
  }

  /* ThresholdFunction */
  FUNC_IMPL(ThresholdFunction,
            SIG((FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                 EXEC_DISPATCH(&ThresholdFunction::cap_value))))

  FUNC_BODY(ThresholdFunction, cap_value)
  {
    int a = args[0]->as<Lisple::Number>().value;
    int b = args[1]->as<Lisple::Number>().value;

    return b > a ? args[0] : args[1];
  }

  MinMaxFunction::MinMaxFunction(bool min)
    : Function(SIG((FN_ARGS((&Lisple::Type::NUMBER), (VARARG, &Lisple::Type::NUMBER)),
                    EXEC_DISPATCH(&MinMaxFunction::select_min_or_max))))
    , min(min)
  {
  }

  FUNC_BODY(MinMaxFunction, select_min_or_max)
  {
    float result_val = args[0]->as<Lisple::Number>().value;
    size_t result_index = 0;

    for (size_t i = 1; i < args.size(); i++)
    {
      float num = args[i]->as<Lisple::Number>().value;
      if (min == (num < result_val))
      {
        result_val = num;
        result_index = i;
      }
    }

    return args[result_index];
  }

  /* DissocBangFunction */
  FUNC_IMPL(DissocBangFunction,
            SIG((FN_ARGS((&Type::MAP), (&Type::ANY)),
                 EXEC_DISPATCH(&DissocBangFunction::dissoc_bang))))

  FUNC_BODY(DissocBangFunction, dissoc_bang)
  {
    if (*NIL == *args[0])
    {
      return NIL;
    }

    sptr_sobject removed = args[0]->as<Lisple::Map>().remove_key(*args[1]);
    if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(args[0].get()))
    {
      Dict::remove_property(wrapper->val, to_rt_value(args[1]));
    }

    return removed;
  }

  /* MergeFunction - merge */
  FUNC_IMPL(MergeFunction,
            SIG((FN_ARGS((&Type::MAP), (&VARARG, &Type::MAP)),
                 EXEC_DISPATCH(&MergeFunction::merge_maps))))

  FUNC_BODY(MergeFunction, merge_maps)
  {
    std::shared_ptr<Map> result = std::make_shared<Map>(args[0]->as<Map>());

    for (size_t i = 1; i < args.size(); i++)
    {
      if (*args[i] == *NIL) continue;
      for (auto& key : args[i]->as<Map>().key_ptrs())
      {
        result->set_property(key, args[i]->get_sptr_property(*key));
      }
    }

    return result;
  }

  /* AppendBangFunction - append_bang */
  FUNC_IMPL(AppendBangFunction,
            SIG((FN_ARGS((&Type::SEQ), (&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&AppendBangFunction::append_bang))))

  FUNC_BODY(AppendBangFunction, append_bang)
  {
    Lisple::Seq& seq = args[0]->as<Lisple::Seq>();

    for (size_t i = 1; i < args.size(); i++)
    {
      seq.append(args[i]);
    }

    return args[0];
  }

  /* ConcatBangFunction - concat */
  FUNC_IMPL(ConcatBangFunction,
            SIG((FN_ARGS((&Type::SEQ), (&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&ConcatBangFunction::concat_array))))

  FUNC_BODY(ConcatBangFunction, concat_array)
  {
    auto result = args[0];

    for (size_t i = 1; i < args.size(); i++)
    {
      auto& vec = args[i];
      if ((Type::ARRAY.is_type_of(*vec) || Type::LIST.is_type_of(*vec)) && *vec != *NIL)
      {
        for (auto& element : vec->get_children())
        {
          result->append(element);
        }
      }
      else
      {
        result->append(vec);
      }
    }

    return result;
  }

  /* FlattenFunction - flatten */
  FUNC_IMPL(FlattenFunction,
            SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&FlattenFunction::flatten_array))))

  FUNC_BODY(FlattenFunction, flatten_array)
  {
    sptr_sobject_v result;

    for (auto obj : args[0]->get_children())
    {
      if (Type::ARRAY.is_type_of(*obj) || Type::LIST.is_type_of(*obj) ||
          Type::HOST_SEQ.is_type_of(*obj))
      {
        auto flat_args = sptr_sobject_v{obj};
        auto flattened = flatten_array(ctx, flat_args);
        for (auto fl_obj : flattened->get_children())
        {
          result.push_back(fl_obj);
        }
      }
      else
      {
        result.push_back(obj);
      }
    }

    return std::make_shared<Array>(std::move(result));
  }

  /* HeadFunction */
  FUNC_IMPL(HeadFunction, SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&HeadFunction::head))))

  FUNC_BODY(HeadFunction, head)
  {
    if (*NIL == *args[0]) return NIL;
    return args[0]->as<Lisple::Seq>().head();
  }

  /* TailFunction */
  FUNC_IMPL(TailFunction, SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&TailFunction::tail))))

  FUNC_BODY(TailFunction, tail)
  {
    return std::make_shared<Lisple::Array>(args[0]->as<Lisple::Seq>().tail());
  }

  /* LastFunction */
  FUNC_IMPL(LastFunction, SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&LastFunction::last))))

  FUNC_BODY(LastFunction, last)
  {
    return args[0]->as<Lisple::Seq>().get_children().back();
  }

  /* FilterFunction */
  FUNC_IMPL(FilterFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                 EXEC_DISPATCH(&FilterFunction::filter_seq))))

  FUNC_BODY(FilterFunction, filter_seq)
  {
    auto original = args[0];
    if (NIL == original) return Array::make({});

    sptr_sobject result = Seq::new_sequence(original->get_type(), original->size());

    auto& filter_fn = args.back()->as<Executable>();

    for (auto val : original->get_children())
    {
      sptr_sobject_v val_args{val};
      sptr_sobject pred_result = filter_fn.execute(ctx, val_args);
      if (*pred_result != *NIL && *pred_result != *B_FALSE)
      {
        result->append(val);
      }
    }

    return result;
  }

  /* SortFunction - sort */
  FUNC_IMPL(SortFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)), EXEC_DISPATCH(&SortFunction::sort))))

  FUNC_BODY(SortFunction, sort)
  {
    Lisple::sptr_sobject_v elements = args[0]->get_children();
    if (elements.size() > 1)
    {
      Executable& comparator = args.back()->as<Executable>();
      Lisple::sptr_sobject tmp;
      bool modified = false;
      do
      {
        modified = false;
        for (size_t i = 0; i < elements.size() - 1; i++)
        {
          Lisple::sptr_sobject_v args = {elements[i], elements[i + 1]};
          Lisple::sptr_sobject_v args_reverse = {elements[i + 1], elements[i]};

          if (comparator.execute(ctx, args)->is_truthy() &&
              !comparator.execute(ctx, args_reverse)->is_truthy())
          {
            tmp = elements[i];
            elements[i] = elements[i + 1];
            elements[i + 1] = tmp;
            modified = true;
          }
        }
      } while (modified);
    }

    return std::make_shared<Array>(std::move(elements));
  }

  /* SomeFunction */
  FUNC_IMPL(SomeFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)), EXEC_DISPATCH(&SomeFunction::some))))

  FUNC_BODY(SomeFunction, some)
  {
    for (auto& element : args[0]->get_children())
    {
      sptr_sobject_v args = {element};
      sptr_sobject result = args.back()->execute(ctx, args);
      if (*result != *NIL && *result != *B_FALSE)
      {
        return B_TRUE;
      }
    }
    return B_FALSE;
  }

  /* RemoveFunction */
  FUNC_IMPL(RemoveFunction,
            MULTI_SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ)),
                       EXEC_DISPATCH(&RemoveFunction::remove_seq)),
                      (FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                       EXEC_DISPATCH(&RemoveFunction::remove_seq))))

  FUNC_BODY(RemoveFunction, remove_seq)
  {
    Object* original;
    Executable* remove_fn;

    if (Type::SEQ.is_type_of(*args[0]))
    {
      original = args[0].get();
      remove_fn = &args[1]->as<Executable>();
    }
    else
    {
      original = args[1].get();
      remove_fn = &args[0]->as<Executable>();
    }

    Lisple::sptr_sobject result =
      Lisple::Seq::new_sequence(original->get_type(), original->size());

    for (auto val : original->get_children())
    {
      Lisple::sptr_sobject_v val_args{val};
      Lisple::sptr_sobject test_result = remove_fn->execute(ctx, val_args);
      if (*test_result == *Lisple::B_FALSE || *test_result == *Lisple::NIL)
      {
        result->append(val);
      }
    }

    return result;
  }

  /* RemoveFirstFunction - find-first */
  FUNC_IMPL(RemoveFirstFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&RemoveFirstFunction::remove_first))))

  FUNC_BODY(RemoveFirstFunction, remove_first)
  {
    auto original = args.back();
    Lisple::sptr_sobject result =
      Lisple::Seq::new_sequence(original->get_type(), original->size());

    auto& remove_fn = args[0]->as<Lisple::Executable>();

    bool removed = false;
    for (auto val : original->get_children())
    {
      Lisple::sptr_sobject_v val_args{val};
      Lisple::sptr_sobject test_result = remove_fn.execute(ctx, val_args);
      if (removed || *test_result == *Lisple::B_FALSE || *test_result == *Lisple::NIL)
      {
        result->append(val);
      }
      else
      {
        removed = true;
      }
    }

    return result;
  }

  /* RemoveBangFunction - remove! */
  FUNC_IMPL(RemoveBangFunction,
            SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ)),
                 EXEC_DISPATCH(&RemoveBangFunction::remove_seq))))

  FUNC_BODY(RemoveBangFunction, remove_seq)
  {
    auto& remove_fn = args[0]->as<Lisple::Executable>();

    sptr_sobject_v& children = args[1]->get_children();

    auto it = std::remove_if(children.begin(),
                             children.end(),
                             [&](const Lisple::sptr_sobject& element)
                             {
                               Lisple::sptr_sobject_v val_args{element};
                               return remove_fn.execute(ctx, val_args)->is_truthy();
                             });

    children.erase(it, children.end());

    if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(args[1].get()))
    {
      if (Type::HOST_SEQ.is_type_of(*wrapper->delegate))
      {
        args[1]->as<Seq>().replace_children(children);
      }
      else
      {
        sptr_rtval_v& vec = std::get<sptr_rtval_v>(wrapper->val->value);
        vec.clear();
        for (auto& c : children)
        {
          vec.push_back(to_rt_value(c));
        }
      }
    }
    else if (Type::HOST_SEQ.is_type_of(*args[1]))
    {
      args[1]->as<Seq>().replace_children(children);
    }
    return args.back();
  }

  /* RemoveNthBangFunction - remove-nth! */
  FUNC_IMPL(RemoveNthBangFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::NUMBER)),
                 EXEC_DISPATCH(&RemoveNthBangFunction::remove_nth))))

  FUNC_BODY(RemoveNthBangFunction, remove_nth)
  {
    auto& seq = args[0]->as<Lisple::Seq>();
    int n = args[1]->as<Lisple::Number>().int_value();

    sptr_sobject_v& children = seq.get_children();

    if (n < 0 || n >= static_cast<int>(children.size()))
    {
      return Lisple::NIL;
    }

    sptr_sobject to_delete = children[n];

    children.erase(children.begin() + n);
    if (Type::HOST_SEQ.is_type_of(seq))
    {
      seq.replace_children(children);
    }
    return to_delete;
  }

  /* ReduceKeyValueFunction - reduce-kv */
  FUNC_IMPL(ReduceKeyValueFunction,
            SIG((FN_ARGS((&Type::MAP), (&Type::ANY), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&ReduceKeyValueFunction::reduce_kv))))

  FUNC_BODY(ReduceKeyValueFunction, reduce_kv)
  {
    sptr_sobject result = args[1];
    Function& reducer = args.back()->as<Function>();

    for (auto key : args[0]->as<Map>().key_ptrs())
    {
      sptr_sobject_v reducer_args{result, key, args[0]->get_sptr_property(*key)};

      sptr_sobject new_result = reducer.execute(ctx, reducer_args);
      if (new_result.get() != result.get())
      {
        result.swap(new_result);
      }
    }

    return result;
  }

  /* FindFirstFunction - find-first */
  FUNC_IMPL(FindFirstFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&FindFirstFunction::find_first_in_seq))))

  FUNC_BODY(FindFirstFunction, find_first_in_seq)
  {
    auto original = args[0];

    auto& filter_fn = args.back()->as<Executable>();

    for (auto val : args[0]->get_children())
    {
      sptr_sobject_v val_args{val};
      sptr_sobject pred_result = filter_fn.execute(ctx, val_args);
      if (*pred_result != *B_FALSE && *pred_result != *NIL)
      {
        return val;
      }
    }

    return NIL;
  }

  FUNC_IMPL(SeqMatchFunction,
            SIG((FN_ARGS((&Lisple::Type::SEQ), (&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&SeqMatchFunction::match))))

  bool match_map_like(sptr_sobject& obj, sptr_sobject& pattern)
  {
    for (Object* key : pattern->as<Lisple::Map>().keys())
    {
      sptr_sobject prop = pattern->get_sptr_property(*key);
      sptr_sobject value = obj->get_sptr_property(*key);

      if (*NIL != *prop && Type::MAP.is_type_of(*prop))
      {
        if (!match_map_like(value, prop))
        {
          return false;
        }
      }
      else if (*prop != *value)
      {
        return false;
      }
    }

    return true;
  }

  FUNC_BODY(SeqMatchFunction, match)
  {
    sptr_sobject& seq = args[0];
    sptr_sobject& pattern = args.back();

    for (auto& obj : seq->get_children())
    {
      if (match_map_like(obj, pattern))
      {
        return obj;
      }
    }

    return NIL;
  }

  /* KeepFunction - keep */
  FUNC_IMPL(KeepFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)), EXEC_DISPATCH(&KeepFunction::keep))))

  FUNC_BODY(KeepFunction, keep)
  {
    sptr_sobject_v values = args[0]->get_children();
    sptr_sobject exec = args[1];

    sptr_sobject_v result;

    for (auto& v : values)
    {
      sptr_sobject_v arg{v};
      sptr_sobject r = exec->execute(ctx, arg);
      if (*r != *NIL) result.push_back(r);
    }

    return Array::make(result);
  }

  /* KeysFunction - keys */
  FUNC_IMPL(KeysFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&KeysFunction::keys_fn))))

  FUNC_BODY(KeysFunction, keys_fn)
  {
    sptr_sobject_v result;

    if (args[0]->get_type() == Form::HOST_OBJECT)
    {
      for (auto& k : args[0]->as<Lisple::AbstractHostObject>().keys())
      {
        result.push_back(k);
      }
    }
    else if (args[0]->get_type() == Form::MAP)
    {
      result = args[0]->as<Map>().key_ptrs();
    }

    return std::make_shared<Array>(result);
  }

  /* SelectKeysFunction */
  FUNC_IMPL(SelectKeysFunction,
            SIG((FN_ARGS((&Lisple::Type::ANY), (&Lisple::Type::SEQ)),
                 EXEC_DISPATCH(&SelectKeysFunction::select_keys_fn))));

  FUNC_BODY(SelectKeysFunction, select_keys_fn)
  {
    auto& obj = *args[0];

    sptr_sobject_v new_content;
    for (auto& key : args.back()->get_children())
    {
      sptr_sobject value = obj.get_sptr_property(*key);
      if (value->get_type() != Form::NIL)
      {
        new_content.push_back(key);
        new_content.push_back(value);
      }
    }

    return std::make_shared<Lisple::Map>(new_content);
  }

  /* PartitionFunction */
  FUNC_IMPL(PartitionFunction,
            SIG((FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::SEQ)),
                 EXEC_DISPATCH(&PartitionFunction::partition))))

  FUNC_BODY(PartitionFunction, partition)
  {
    std::shared_ptr<Lisple::Array> result = std::make_shared<Lisple::Array>();
    unsigned int part_size = Lisple::uint_val(*args[0]);

    std::shared_ptr<Lisple::Array> partition = std::make_shared<Lisple::Array>();
    for (auto& child : args.back()->get_children())
    {
      partition->append(child);
      if (partition->size() == part_size)
      {
        result->append(partition);
        partition = std::make_shared<Lisple::Array>();
      }
    }
    if (partition->size())
    {
      result->append(partition);
    }

    return result;
  }

  /* OddEvenPredicateFunction */
  OddEvenPredicateFunction::OddEvenPredicateFunction(uint8_t modulus)
    : Function(SIG((FN_ARGS((&Lisple::Type::NUMBER)),
                    EXEC_DISPATCH(&OddEvenPredicateFunction::exec_oddevenp))))
    , modulus(modulus)
  {
  }

  FUNC_BODY(OddEvenPredicateFunction, exec_oddevenp)
  {
    return args[0]->as<Lisple::Number>().int_value() % 2 == modulus ? Lisple::B_TRUE
                                                                    : Lisple::B_FALSE;
  }

  /* EmptyPredicateFunction */
  FUNC_IMPL(EmptyPredicateFunction,
            MULTI_SIG((FN_ARGS((&Lisple::Type::SEQ)),
                       EXEC_DISPATCH(&EmptyPredicateFunction::exec_emptyp_seq)),
                      (FN_ARGS((&Lisple::Type::STRING)),
                       EXEC_DISPATCH(&EmptyPredicateFunction::exec_emptyp_string))))

  FUNC_BODY(EmptyPredicateFunction, exec_emptyp_seq)
  {
    return Lisple::Boolean::wrap(args[0]->get_children().empty());
  }

  FUNC_BODY(EmptyPredicateFunction, exec_emptyp_string)
  {
    return Lisple::Boolean::wrap(args[0]->as<Lisple::String>().value.empty());
  }

  /* NotEmptyPredicateFunction */
  FUNC_IMPL(NotEmptyPredicateFunction,
            MULTI_SIG((FN_ARGS((&Lisple::Type::SEQ)),
                       EXEC_DISPATCH(&NotEmptyPredicateFunction::exec_not_emptyp_seq)),
                      (FN_ARGS((&Lisple::Type::STRING)),
                       EXEC_DISPATCH(&NotEmptyPredicateFunction::exec_not_emptyp_string))))

  FUNC_BODY(NotEmptyPredicateFunction, exec_not_emptyp_seq)
  {
    return Lisple::Boolean::wrap(!args[0]->get_children().empty());
  }

  FUNC_BODY(NotEmptyPredicateFunction, exec_not_emptyp_string)
  {
    return Lisple::Boolean::wrap(!args[0]->as<Lisple::String>().value.empty());
  }

  FUNC_IMPL(IncludeFunction,
            SIG((FN_ARGS((&Lisple::Type::STRING)),
                 EXEC_DISPATCH(&IncludeFunction::include_file))))

  FUNC_BODY(IncludeFunction, include_file)
  {
    ctx.read_file(Lisple::Value<std::string>::value_of(*args[0]));
    return args[0];
  }

  /* EvalFunction - eval */
  FUNC_IMPL(
    EvalFunction,
    MULTI_SIG((FN_ARGS((&Lisple::Type::STRING)), EXEC_DISPATCH(&EvalFunction::eval_string)),
              (FN_ARGS((&Lisple::Type::ANY)), EXEC_DISPATCH(&EvalFunction::eval_form))))

  FUNC_BODY(EvalFunction, eval_string)
  {
    const std::string& str = args[0]->as<Lisple::String>().value;
    return ctx.eval(str);
  }

  FUNC_BODY(EvalFunction, eval_form)
  {
    return ctx.eval(args[0]);
  }

  /* ResolveFunction - resolve */
  FUNC_IMPL(
    ResolveFunction,
    MULTI_SIG((FN_ARGS((&Lisple::Type::SYMBOL)), EXEC_DISPATCH(&ResolveFunction::resolve)),
              (FN_ARGS((&Lisple::Type::WORD)), EXEC_DISPATCH(&ResolveFunction::resolve))));

  FUNC_BODY(ResolveFunction, resolve)
  {
    if (*args[0] == *NIL) return NIL;
    return ctx.lookup(args[0]->as<Value<std::string>>().value);
  }

  /* ApplyFunction - apply */
  FUNC_IMPL(ApplyFunction,
            SIG((FN_ARGS((&Lisple::Type::EXEC), (&Lisple::Type::SEQ)),
                 EXEC_DISPATCH(&ApplyFunction::apply_fn))))

  FUNC_BODY(ApplyFunction, apply_fn)
  {
    auto& fn = *args[0];
    auto& fn_args = args.back()->get_children();

    return fn.execute(ctx, fn_args);
  }

  FUNC_IMPL(VectorFunction,
            SIG((FN_ARGS((&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&VectorFunction::make_vector))))

  FUNC_BODY(VectorFunction, make_vector)
  {
    sptr_sobject_v vector;

    for (auto obj : args)
    {
      vector.push_back(obj);
    }

    return std::make_shared<Array>(vector);
  }

  /*
   * ContainsPredicateFunction - contains?
   */
  FUNC_IMPL(ContainsPredicateFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::ANY)),
                 EXEC_DISPATCH(&ContainsPredicateFunction::contains))))

  FUNC_BODY(ContainsPredicateFunction, contains)
  {
    if (*NIL == *args[0]) return B_FALSE;

    sptr_sobject_v& vector = args[0]->get_children();
    return std::find_if(vector.begin(),
                        vector.end(),
                        [&args](sptr_sobject lmnt)
                        { return *lmnt == *args.back(); }) != vector.end()
             ? B_TRUE
             : B_FALSE;
  }

  /*
   * RepeatFunction - repeat
   */
  FUNC_IMPL(RepeatFunction,
            SIG((FN_ARGS((&Type::NUMBER), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&RepeatFunction::repeat))))

  FUNC_BODY(RepeatFunction, repeat)
  {
    int n = Lisple::int_val(*args[0]);
    Lisple::sptr_sobject_v array;
    array.reserve(n * (args.size() - 1));
    for (int ni = 0; ni < n; ni++)
    {
      for (size_t i = 1; i < args.size(); i++)
      {
        array.push_back(args[i]);
      }
    }

    return std::make_shared<Lisple::Array>(std::move(array));
  }

  /*
   * TakeFunction - take
   */
  FUNC_IMPL(TakeFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::SEQ)),
                 EXEC_DISPATCH(&TakeFunction::take_fn))))

  FUNC_BODY(TakeFunction, take_fn)
  {
    int amount = args[0]->as<Number>().int_value();
    sptr_sobject_v vector = args.back()->get_children();

    sptr_sobject_v result;
    result.reserve(vector.size());
    for (int i = 0; i < std::min(amount, static_cast<int>(vector.size())); i++)
    {
      result.push_back(vector[i]);
    }

    return std::make_shared<Array>(std::move(result));
  }

  /*
   * UpperCaseFunction - upper-case
   */
  FUNC_IMPL(UpperCaseFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&UpperCaseFunction::uppercase))));

  FUNC_BODY(UpperCaseFunction, uppercase)
  {
    Object& arg = *args[0];
    std::string str =
      (arg != *NIL && Type::STRING.is_type_of(arg)) ? str_val(*args[0]) : arg.to_string();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return String::make(str);
  }

  /*
   * LowerCaseFunction - lower-case
   */
  FUNC_IMPL(LowerCaseFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&LowerCaseFunction::lowercase))));

  FUNC_BODY(LowerCaseFunction, lowercase)
  {
    Object& arg = *args[0];
    std::string str =
      (arg != *NIL && Type::STRING.is_type_of(arg)) ? str_val(*args[0]) : arg.to_string();
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return String::make(str);
  }

  /*
   * NameFunction - name
   */
  FUNC_IMPL(NameFunction,
            SIG((FN_ARGS((&Type::QUALIFIABLE)),
                 EXEC_DISPATCH(&NameFunction::extract_name))));

  FUNC_BODY(NameFunction, extract_name)
  {
    sptr_sobject& arg = args[0];
    if (*NIL == *arg) return NIL;
    return String::make(arg->as<QualifiableStringValue>().get_identifier());
  }

  /*
   * NamespaceFunction - namespace
   */
  FUNC_IMPL(NamespaceFunction,
            SIG((FN_ARGS((&Type::QUALIFIABLE)),
                 EXEC_DISPATCH(&NamespaceFunction::extract_namespace))));

  FUNC_BODY(NamespaceFunction, extract_namespace)
  {
    sptr_sobject& arg = args[0];
    if (*NIL == *arg) return NIL;

    if (const std::string& val = arg->as<QualifiableStringValue>().get_qualifier();
        !val.empty())
    {
      return String::make(val);
    }
    return NIL;
  }

  /*
   * JoinFunction - join
   */
  FUNC_IMPL(JoinFunction,
            SIG((FN_ARGS((VARARG, &Type::STRING)), EXEC_DISPATCH(&JoinFunction::join_str))))

  FUNC_BODY(JoinFunction, join_str)
  {
    if (args.size() < 2)
    {
      return std::make_shared<Lisple::String>("");
    }

    std::string joiner = args[0]->as<Lisple::String>().value;
    std::string result = args[1]->as<Lisple::String>().value;

    for (size_t i = 2; i < args.size(); i++)
    {
      result += joiner + args[i]->as<Lisple::String>().value;
    }
    return std::make_shared<Lisple::String>(result);
  }

} // namespace Lisple
