
#include "lang.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <vector>

#include "context.h"
#include "exec.h"
#include "form.h"
#include "impl.h"
#include "scope.h"
#include "type.h"
#include "host.h"
#include "lisple_exception.h"

namespace Lisple
{
  using Lisple::UserFunction;

  Namespace make_language_namespace()
  {
    std::map<std::string, Lisple::sptr_sobject> lang;

    lang.emplace("+", std::make_shared<PlusFunction>());
    lang.emplace("-", std::make_shared<MinusFunction>());
    lang.emplace("/", std::make_shared<DivideFunction>());
    lang.emplace("*", std::make_shared<MultiplyFunction>());
    lang.emplace("=", std::make_shared<EqualsPredicateFunction>());
    lang.emplace("<", std::make_shared<LessThanFunction>());
    lang.emplace("<=", std::make_shared<LessThanOrEqualsFunction>());
    lang.emplace(">", std::make_shared<GreaterThanFunction>());
    lang.emplace(">=", std::make_shared<GreaterThanOrEqualsFunction>());
    lang.emplace("->", std::make_shared<ThreadFirstMacro>());
    lang.emplace("and", std::make_shared<AndFunction>());
    lang.emplace("apply", std::make_shared<ApplyFunction>());
    lang.emplace("assoc", std::make_shared<AssocFunction>());
    lang.emplace("assoc!", std::make_shared<AssocBangFunction>());
    lang.emplace("assoc-in!", std::make_shared<AssocInBangFunction>());
    lang.emplace("case", std::make_shared<CaseMacro>());
    lang.emplace("concat", std::make_shared<ConcatFunction>());
    lang.emplace("cond", std::make_shared<CondMacro>());
    lang.emplace("contains?", std::make_shared<ContainsPredicateFunction>());
    lang.emplace("count", std::make_shared<CountFunction>());
    lang.emplace("def", std::make_shared<DefMacro>());
    lang.emplace("defun", std::make_shared<DefunMacro>());
    lang.emplace("do", std::make_shared<DoMacro>());
    lang.emplace("empty?", std::make_shared<EmptyPredicateFunction>());
    lang.emplace("eval", std::make_shared<EvalFunction>());
    lang.emplace("even?", std::make_shared<OddEvenPredicateFunction>(0));
    lang.emplace("if", std::make_shared<IfMacro>());
    lang.emplace("false", Lisple::B_FALSE);
    lang.emplace("flatten", std::make_shared<FlattenFunction>());
    lang.emplace("filter", std::make_shared<FilterFunction>());
    lang.emplace("find-first", std::make_shared<FindFirstFunction>());
    lang.emplace("fn", std::make_shared<LambdaMacro>());
    lang.emplace("for", std::make_shared<ForMacro>());
    lang.emplace("for-indexed", std::make_shared<ForIndexedMacro>());
    lang.emplace("get", std::make_shared<GetFunction>());
    lang.emplace("head", std::make_shared<HeadFunction>());
    lang.emplace("int", std::make_shared<IntFunction>());
    lang.emplace("join", std::make_shared<JoinFunction>());
    lang.emplace("include", std::make_shared<IncludeFunction>());
    lang.emplace("last", std::make_shared<LastFunction>());
    lang.emplace("let", std::make_shared<LetMacro>());
    lang.emplace("keys", std::make_shared<KeysFunction>());
    lang.emplace("map", std::make_shared<MapFunction>());
    lang.emplace("max", std::make_shared<MinMaxFunction>(false));
    lang.emplace("merge", std::make_shared<MergeFunction>());
    lang.emplace("min", std::make_shared<MinMaxFunction>(true));
    lang.emplace("nil", Lisple::NIL);
    lang.emplace("nil?", std::make_shared<NilPredicateFunction>());
    lang.emplace("not", std::make_shared<NotFunction>());
    lang.emplace("not=", std::make_shared<NotEqualsFunction>());
    lang.emplace("not-empty?", std::make_shared<NotEmptyPredicateFunction>());
    lang.emplace("ns", std::make_shared<NsMacro>());
    lang.emplace("nth", std::make_shared<NthFunction>());
    lang.emplace("odd?", std::make_shared<OddEvenPredicateFunction>(1));
    lang.emplace("or", std::make_shared<OrMacro>());
    lang.emplace("partition", std::make_shared<PartitionFunction>());
    lang.emplace("prn", std::make_shared<PrintFunction>());
    lang.emplace("rand-nth", std::make_shared<RandNthFunction>());
    lang.emplace("range", std::make_shared<RangeFunction>());
    lang.emplace("reduce", std::make_shared<ReduceFunction>());
    lang.emplace("reduce-kv", std::make_shared<ReduceKeyValueFunction>());
    lang.emplace("remove", std::make_shared<RemoveFunction>());
    lang.emplace("rnd", std::make_shared<RndFunction>());
    lang.emplace("select-keys", std::make_shared<SelectKeysFunction>());
    lang.emplace("seq-match", std::make_shared<SeqMatchFunction>());
    lang.emplace("set!", std::make_shared<SetBangMacro>());
    lang.emplace("some?", std::make_shared<SomeFunction>());
    lang.emplace("sort", std::make_shared<SortFunction>());
    lang.emplace("str", std::make_shared<StrFunction>());
    lang.emplace("tail", std::make_shared<TailFunction>());
    lang.emplace("take", std::make_shared<TakeFunction>());
    lang.emplace("threshold", std::make_shared<ThresholdFunction>());
    lang.emplace("true", Lisple::B_TRUE);
    lang.emplace("vector", std::make_shared<VectorFunction>());
    lang.emplace("when", std::make_shared<WhenMacro>());
    lang.emplace("when-let", std::make_shared<WhenLetMacro>());
    lang.emplace("while", std::make_shared<WhileMacro>());

    return Namespace::make_lang(lang);
  }

  /* NsMacro */
  MACRO_IMPL(NsMacro, MULTI_SIG((FN_ARGS((&Type::WORD, false)),
                                 EXEC_DISPATCH(&NsMacro::switch_ns)),
                                (FN_ARGS((&Type::WORD, false), (&Type::LIST, false)),
                                 EXEC_DISPATCH(&NsMacro::switch_ns))))

  Key KEY_REQUIRE("require");
  Key KEY_AS("as");

  void throw_ns_exception(Word& ns, List& req_list, std::string msg="")
  {
    std::string ns_decl = "(ns " + ns.value;;
    for (size_t i=0; i< req_list.get_children().size(); i++)
    {
      ns_decl += " " + req_list.get_children().at(i)->to_string();
    }
    ns_decl += ")";

    throw NamespaceException("Invalid ns form: " + ns_decl + msg);
  }

  MACRO_BODY(NsMacro, switch_ns)
  {
    Word& ns_word = args.front()->as<Word>();
    Lisple::sptr_sobject_v imports;
    if (args.size() == 2)
    {
      Lisple::List& list = args.back()->as<List>();
      if (list.size() < 2 || (list.size() > 0 && *list.get_children().front() != KEY_REQUIRE))
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
            throw_ns_exception(ns_word, list, ". Invalid require-entry: " + imp->to_string());
          }
        }
        else if (Type::ARRAY.is_type_of(*imp))
        {
          if (imp->get_children().size() != 3)
          {
            throw_ns_exception(ns_word, list, ". Invalid require-entry: " + imp->to_string());
          }

          if (*imp->get_children().at(1) == KEY_AS)
          {
            if (!Type::WORD.is_type_of(*imp->get_children().back()) ||
                imp->get_children().back()->as<Word>().is_qualified())
            {
              throw_ns_exception(ns_word, list, ". Invalid require-entry: " + imp->to_string());
            }
          }
          else
          {
            throw_ns_exception(ns_word, list, ". Invalid require-entry: " + imp->to_string());
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

  /* DefMacro */
  MACRO_IMPL(DefMacro, SIG((FN_ARGS((&Type::WORD, false), (&Lisple::Type::ANY, true)),
                            EXEC_DISPATCH(&DefMacro::define_obj))))

  std::shared_ptr<UserFunction> create_function(const Namespace* home_ns, Object& arg_array, sptr_sobject_v& body)
  {
    std::vector<Argument> arg_types;
    std::vector<std::unique_ptr<ArgumentBinding>> arg_bindings;
    for (auto& arg : arg_array.as<Array>().get_children())
    {
      if (arg->get_type() != Form::WORD &&
          arg->get_type() != Form::MAP)
      {
        throw LispleException("Illegal fn argument declaration: " + arg_array.to_string());
      }
      arg_types.push_back(Lisple::arg(&Type::ANY));
      arg_bindings.push_back(ArgumentBinding::create(*arg));
    }
    return std::make_shared<UserFunction>(home_ns, arg_types, arg_bindings, body);
  }

  std::shared_ptr<DetachedFunction> create_detached_function(Lisple::Context& ctx, Lisple::Object& arg_array, Lisple::sptr_sobject_v& body)
  {
    std::shared_ptr<Function> fn = create_function(ctx.get_current_namespace(), arg_array, body);
    return std::make_shared<Lisple::DetachedFunction>(ctx.detach(), fn);
  }

  MACRO_BODY(DefMacro, define_obj)
  {
    ctx.store_namespace(args.at(0)->as<Lisple::Word>(), args.at(1));
    return args.at(1);
  }

  MACRO_IMPL(DefunMacro, SIG((FN_ARGS((&Type::WORD, false),
                                      (&Type::ARRAY, false),
                                      (VARARG, &Type::ANY, false)),
                              EXEC_DISPATCH(&DefunMacro::define_fun))))

  MACRO_BODY(DefunMacro, define_fun)
  {
    std::string fun_name = Lisple::Value<std::string>::value_of(*args.at(0));
    sptr_sobject_v body;
    for (size_t i=2; i<args.size(); i++)
    {
      body.push_back(args.at(i));
    }
    auto fn = create_function(ctx.get_current_namespace(), *args.at(1), body);
    ctx.store_namespace(fun_name, fn);

    return fn;
  }

  MACRO_IMPL(LambdaMacro, SIG((FN_ARGS((&Type::ARRAY, false), (VARARG, &Type::ANY, false)),
                               EXEC_DISPATCH(&LambdaMacro::make_lambda))))

  MACRO_BODY(LambdaMacro, make_lambda)
  {
    sptr_sobject_v body;
    for (size_t i=1; i < args.size(); i++)
    {
      body.push_back(args.at(i));
    }
    return create_detached_function(ctx, *args.at(0), body);
  }

  /* LetMacro */
  MACRO_IMPL(LetMacro, SIG((FN_ARGS((&Type::ARRAY, false), (VARARG, &Type::ANY, false)),
                            EXEC_DISPATCH(&LetMacro::make_let))))

  MACRO_BODY(LetMacro, make_let)
  {
    Array var_def_array = args.front()->as<Array>();

    if (var_def_array.get_children().size() % 2 != 0)
    {
      throw LispleException("Wrong number of parameters to let-expression: " + var_def_array.to_string());
    }

    for (size_t i=0; i < var_def_array.size(); i+=2)
    {
      auto& var_name_obj = *var_def_array.get_children().at(i);
      auto var_val_obj = ctx.eval(var_def_array.get_children().at(i+1));

      if (!Type::WORD.is_type_of(var_name_obj))
      {
        throw TypeError("Invalid variable identifier in let-expression: " + var_name_obj.to_string() + " in " + var_def_array.to_string());
      }

      Scope var_scope;
      var_scope.store(var_name_obj.as<Word>(), var_val_obj);
      ctx.push_context(true, var_scope);
    }

    sptr_sobject result;

    for (size_t i=1; i < args.size(); i++)
    {
      result = ctx.eval(args.at(i));
    }

    for (size_t i=0; i < var_def_array.size() / 2; i++)
    {
      ctx.pop_context();
    }

    return result;
  }

  /* WhenLetMacro */
  MACRO_IMPL(WhenLetMacro, SIG((FN_ARGS((&Type::ARRAY, false), (VARARG, &Type::ANY, false)),
                                EXEC_DISPATCH(&WhenLetMacro::make_when_let))))

  MACRO_BODY(WhenLetMacro, make_when_let)
  {
    Array var_def_array = args.front()->as<Array>();

    if (var_def_array.get_children().size() % 2 != 0)
    {
      throw LispleException("Wrong number of parameters to let-expression: " + var_def_array.to_string());
    }

    bool contains_nil = false;

    size_t scopes = 0;
    for (size_t i=0; i < var_def_array.size(); i+=2)
    {
      auto& var_name_obj = *var_def_array.get_children().at(i);
      auto var_val_obj = ctx.eval(var_def_array.get_children().at(i+1));

      if (*var_val_obj == *NIL)
      {
        contains_nil = true;
        break;
      }

      if (!Type::WORD.is_type_of(var_name_obj))
      {
        throw TypeError("Invalid variable identifier in let-expression: " + var_name_obj.to_string() + " in " + var_def_array.to_string());
      }

      Scope var_scope;
      var_scope.store(var_name_obj.as<Word>(), var_val_obj);
      ctx.push_context(true, var_scope);
      scopes = i;
    }

    sptr_sobject result = NIL;

    if (!contains_nil)
    {
      for (size_t i=1; i < args.size(); i++)
      {
        result = ctx.eval(args.at(i));
      }
    }

    for (size_t i=0; i < scopes; i++)
    {
      ctx.pop_context();
    }

    return result;
  }

  MACRO_IMPL(DoMacro, SIG((FN_ARGS((&VARARG, &Type::ANY, false)),
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

  FUNC_IMPL(PrintFunction, SIG((FN_ARGS((&VARARG, &Type::ANY, true)),
                                EXEC_DISPATCH(&PrintFunction::do_print))))

  FUNC_BODY(PrintFunction, do_print)
  {
    for (size_t i = 0; i < args.size(); i++)
    {
      auto obj = args.at(i);
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

  MACRO_IMPL(ThreadFirstMacro, SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY, false)),
                                    EXEC_DISPATCH(&ThreadFirstMacro::make_thread_first))))

  MACRO_BODY(ThreadFirstMacro, make_thread_first)
  {
    sptr_sobject value = args.at(0);
    for (size_t i=1; i<args.size(); i++)
    {
      sptr_sobject ifn = args.at(i);
      if (ifn->get_type() == Form::LIST)
      {
        std::shared_ptr<List> realized = ifn->as<List>().insert(1, value);
        value = ctx.eval(realized);
      }
      else
      {
        sptr_sobject_v elements;
        elements.push_back(ifn);
        elements.push_back(value);
        std::shared_ptr<List> realized = std::make_shared<List>(elements);
        value = ctx.eval(realized);
      }
    }

    return value;
  }

  /**
   * NilPredicateFunction
   */
  FUNC_IMPL(NilPredicateFunction, SIG((FN_ARGS((&Type::ANY)),
                                       EXEC_DISPATCH(&NilPredicateFunction::is_nil))))

  FUNC_BODY(NilPredicateFunction, is_nil)
  {
    return *args.front() == *NIL ? B_TRUE : B_FALSE;
  }


  FUNC_IMPL(NotFunction, MULTI_SIG((FN_ARGS((&Type::BOOL)),
                                    EXEC_DISPATCH(&NotFunction::invert_boolean)),
                                   (FN_ARGS((&Type::ANY)),
                                    EXEC_DISPATCH(&NotFunction::not_any))))

  FUNC_BODY(NotFunction, invert_boolean)
  {
    return *args.front() == *B_TRUE ? B_FALSE : B_TRUE;
  }

  FUNC_BODY(NotFunction, not_any)
  {
    return *args.front() == *B_FALSE || *args.front() == *NIL ? B_TRUE : B_FALSE;
  }

  SetBangMacro::SetBangMacro()
    : Macro(SIG((FN_ARGS((&Lisple::Type::ARRAY, false), (&Lisple::Type::ANY)),
                 EXEC_DISPATCH(&SetBangMacro::do_set_member))))
  {
  }

  MACRO_BODY(SetBangMacro, do_set_member)
  {
    Lisple::Array& member_ref = args.front()->as<Lisple::Array>();

    if (member_ref.size() == 1)
    {
      auto identifier = member_ref.get_children().front()->as<Lisple::Word>();
      Scope& scope = ctx.get_scope_of(identifier);
      scope.mutate(identifier, args.back());
    }
    else if (member_ref.size() == 2)
    {
      auto actual_mem_ref = ctx.eval(args.at(0));
      Lisple::Object& prop = *actual_mem_ref->get_children().front();
      Lisple::Object& owner = *actual_mem_ref->get_children().back();

      owner.set_property(&ctx, prop, args.back());
    }
    else
    {
      throw Lisple::InvocationException("Incorrect member reference: " + member_ref.to_string());
    }

    return args.back();
  }


  MACRO_IMPL(WhileMacro, SIG((FN_ARGS((&Lisple::Type::ANY, false), (&Lisple::Type::ANY, false)),
                              EXEC_DISPATCH(&WhileMacro::make_while))))

  MACRO_BODY(WhileMacro, make_while)
  {
    Lisple::sptr_sobject retval = NIL;

    ctx.push_context(true);
    while (ctx.eval(args.front())->is_truthy())
    {
      retval = ctx.eval(args.back());
    }
    ctx.pop_context();

    return retval;
  }

  MACRO_IMPL(IfMacro, MULTI_SIG((FN_ARGS((&Lisple::Type::ANY, false), (&Lisple::Type::ANY, false)),
                                 EXEC_DISPATCH(&IfMacro::make_if)),
                                (FN_ARGS((&Lisple::Type::ANY, false), (&Lisple::Type::ANY, false), (&Lisple::Type::ANY, false)),
                                 EXEC_DISPATCH(&IfMacro::make_if))))

  MACRO_BODY(IfMacro, make_if)
  {
    Lisple::sptr_sobject retval = Lisple::NIL;

    ctx.push_context(true);
    auto condition = ctx.eval(args.front());
    if (*condition != *Lisple::B_FALSE && *condition != *Lisple::NIL)
    {
      retval = ctx.eval(args.at(1));
    }
    else if (args.size() == 3)
    {
      retval = ctx.eval(args.at(2));
    }
    ctx.pop_context();
    return retval;
  }

  /* when */
  MACRO_IMPL(WhenMacro, SIG((FN_ARGS((&Type::ANY, false), (VARARG, &Type::ANY, false)),
                             EXEC_DISPATCH(&WhenMacro::make_when))))

  MACRO_BODY(WhenMacro, make_when)
  {
    sptr_sobject retval = NIL;

    ctx.push_context(true);
    auto condition = ctx.eval(args.front());
    if (condition->is_truthy())
    {
      for (size_t i=1; i<args.size(); i++)
      {
        retval = ctx.eval(args.at(i));
      }
    }
    ctx.pop_context();
    return retval;
  }

  /* case */
  MACRO_IMPL(CaseMacro, SIG((FN_ARGS((&Type::ANY, false), (VARARG, &Type::ANY, false)),
                             EXEC_DISPATCH(&CaseMacro::make_case))))

  Key DEFAULT = Key("default");

  MACRO_BODY(CaseMacro, make_case)
  {
    if ((args.size()-1) % 2 != 0)
    {
      throw InvocationException("Incomplete condition-expression pair passed to case");
    }
    else if (args.size() == 1)
    {
      throw InvocationException("Empty case-form");
    }

    sptr_sobject retval = NIL;

    ctx.push_context(true);
    sptr_sobject value = ctx.eval(args.front());

    for (size_t i=1; i<args.size(); i+=2)
    {
      if (*ctx.eval(args.at(i)) == *value ||
          *args.at(i) == DEFAULT)
      {
        retval = ctx.eval(args.at(i+1));
        break;
      }
    }

    ctx.pop_context();
    return retval;
  }

  /* cond */
  MACRO_IMPL(CondMacro, SIG((FN_ARGS((VARARG, &Type::ANY, false)),
                             EXEC_DISPATCH(&CondMacro::make_cond))))

  MACRO_BODY(CondMacro, make_cond)
  {
    if (args.size() % 2 != 0)
    {
      throw InvocationException("Uneven number of forms passed to cond");
    }
    else if (args.size() == 0)
    {
      throw InvocationException("Empty cond-form");
    }

    sptr_sobject retval = Lisple::NIL;

    ctx.push_context(true);
    for (size_t i=0; i<args.size(); i+=2)
    {
      sptr_sobject condition = ctx.eval(args.at(i));
      if (*condition != *B_FALSE && *condition != *NIL)
      {
        retval = ctx.eval(args.at(i+1));
        break;
      }
    }

    ctx.pop_context();
    return retval;
  }

  /* ForMacro - for */
  MACRO_IMPL(ForMacro, SIG((FN_ARGS((&Type::ARRAY, false), (VARARG, &Type::ANY, false)),
                            EXEC_DISPATCH(&ForMacro::make_for))))

  MACRO_BODY(ForMacro, make_for)
  {
    sptr_sobject_v result;
    sptr_sobject_v& seq_expr = args.front()->as<Array>().get_children();

    sptr_sobject obj_iterable = ctx.eval(seq_expr.back());
    if (*Lisple::NIL != *obj_iterable)
    {
      if (!Type::SEQ.is_type_of(*obj_iterable))
      {
        throw TypeError("For macro requires an iterable. Wrong type: " + obj_iterable->to_string());
      }

      auto seq_binding = ArgumentBinding::create(*seq_expr.front());

      for (auto& lmnt : obj_iterable->get_children())
      {
        Scope iter_scope;
        seq_binding->apply(iter_scope, lmnt);
        ctx.push_context(true, iter_scope);
        sptr_sobject iter_result;
        for (size_t i=1; i<args.size(); i++)
        {
          iter_result = ctx.eval(args.at(i));
        }
        result.push_back(iter_result);
        ctx.pop_context();
      }
    }
    return std::make_shared<Array>(result);
  }

  /* ForIndexedMacro - for-indexed */
  MACRO_IMPL(ForIndexedMacro, SIG((FN_ARGS((&Type::ARRAY, false), (VARARG, &Type::ANY, false)),
                                   EXEC_DISPATCH(&ForIndexedMacro::make_for))))

  MACRO_BODY(ForIndexedMacro, make_for)
  {
    sptr_sobject_v& bind_form = args.front()->as<Array>().get_children();

    if (bind_form.size() != 3)
    {
      throw InvocationException("Invalid number of elements in for-indexed bind form, expected 3: " + args.front()->to_string());
    }

    if (!Type::WORD.is_type_of(*bind_form.at(0)))
    {
      throw TypeError("for-indexed macro requires the index variable to be a word");
    }

    if (!Type::WORD.is_type_of(*bind_form.at(1)))
    {
      throw TypeError("for-indexed macro requires the iteration variable to be a word");
    }

    if (!Type::SEQ.is_type_of(*bind_form.at(2)))
    {
      throw TypeError("for-indexed macro requires an iterable. Wrong type: " + bind_form.at(2)->to_string());
    }

    auto index_binding = ArgumentBinding::create(*bind_form.at(0));
    auto seq_binding = ArgumentBinding::create(*bind_form.at(1));

    auto& seq = bind_form.at(2);
    sptr_sobject_v result;

    for (size_t i=0; i<seq->size(); i++)
    {
      auto& lmnt = seq->get_children().at(i);
      Scope iter_scope;
      sptr_sobject index = Number::make(static_cast<int>(i));
      index_binding->apply(iter_scope, index);
      seq_binding->apply(iter_scope, lmnt);
      ctx.push_context(true, iter_scope);
      sptr_sobject iter_result;
      for (size_t e=1; e<args.size(); e++)
      {
        iter_result = ctx.eval(args.at(e));
      }
      result.push_back(iter_result);
    }

    return std::make_shared<Array>(result);
  }


  FUNC_IMPL(EqualsPredicateFunction, SIG((FN_ARGS((&Lisple::Type::ANY), (&Lisple::Type::ANY)),
                                          EXEC_DISPATCH(&EqualsPredicateFunction::equals_any))))

  FUNC_BODY(EqualsPredicateFunction, equals_any)
  {
    return *args.at(0) == *args.at(1) ? Lisple::B_TRUE : Lisple::B_FALSE;
  }

  FUNC_IMPL(NotEqualsFunction, SIG((FN_ARGS((&Type::ANY), (&Type::ANY)),
                                    EXEC_DISPATCH(&NotEqualsFunction::not_equals_any))))


  FUNC_BODY(NotEqualsFunction, not_equals_any)
  {
    return *args.front() != *args.at(1) ? B_TRUE : B_FALSE;
  }

  std::shared_ptr<Number> box_number(float num)
  {
    if (floorf(num) || num == 0.0)
    {
      return std::make_shared<Number>(static_cast<int>(num));
    }
    return std::make_shared<Number>(num);
  }

  FUNC_IMPL(IntFunction, SIG((FN_ARGS((&Type::ANY)),
                              EXEC_DISPATCH(&IntFunction::to_int))))

  FUNC_BODY(IntFunction, to_int)
  {
    sptr_sobject& obj = args.front();

    if (Type::NUMBER.is_type_of(*obj))
    {
      return std::make_shared<Number>(obj->as<Number>().int_value());
    }
    else if (Type::CHAR.is_type_of(*obj))
    {
      return std::make_shared<Number>(static_cast<int>(obj->as<Char>().value));
    }

    throw LispleException("Cannot convert " + obj->to_string() + " to integer.");
  }

  FUNC_IMPL(PlusFunction, SIG((FN_ARGS((VARARG, &Lisple::Type::NUMBER)),
                               EXEC_DISPATCH(&PlusFunction::do_addition))))

  FUNC_BODY(PlusFunction, do_addition)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to +");
    }

    float result = Lisple::Number::value_of(*args.at(0));

    for (size_t i = 1; i < args.size(); i++)
    {
      result += Lisple::Number::value_of(*args.at(i));
    }

    return box_number(result);
  }

  FUNC_IMPL(MinusFunction, SIG((FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                                EXEC_DISPATCH(&MinusFunction::do_subtraction))))



  FUNC_BODY(MinusFunction, do_subtraction)
  {
    return box_number(Number::value_of(*args.at(0)) - Number::value_of(*args.at(1)));
  }

  FUNC_IMPL(DivideFunction, SIG((FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                                 EXEC_DISPATCH(&DivideFunction::do_division))))

  FUNC_BODY(DivideFunction, do_division)
  {
    return box_number(Number::value_of(*args.at(0)) / Number::value_of(*args.at(1)));
  }

  FUNC_IMPL(MultiplyFunction, SIG((FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                                   EXEC_DISPATCH(&MultiplyFunction::do_multiplication))));

  FUNC_BODY(MultiplyFunction, do_multiplication)
  {
    return box_number(Number::value_of(*args.at(0)) * Number::value_of(*args.at(1)));
  }

  /* LessThanFunction */
  FUNC_IMPL(LessThanFunction, SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                                   EXEC_DISPATCH(&LessThanFunction::lt_fn))))

  FUNC_BODY(LessThanFunction, lt_fn)
  {
    return Number::value_of(*args.at(0)) < Number::value_of(*args.at(1)) ? B_TRUE : B_FALSE;
  }

  /* LessThanOrEqualsFunction */
  FUNC_IMPL(LessThanOrEqualsFunction, SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                                           EXEC_DISPATCH(&LessThanOrEqualsFunction::lte_fn))))

  FUNC_BODY(LessThanOrEqualsFunction, lte_fn)
  {
    return Number::value_of(*args.at(0)) <= Number::value_of(*args.at(1)) ? B_TRUE : B_FALSE;
  }

  /* GreaterThanFunction */
  FUNC_IMPL(GreaterThanFunction, SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                                   EXEC_DISPATCH(&GreaterThanFunction::gt_fn))))

  FUNC_BODY(GreaterThanFunction, gt_fn)
  {
    return Number::value_of(*args.at(0)) > Number::value_of(*args.at(1)) ? B_TRUE : B_FALSE;
  }

  /* GreaterThanOrEqualsFunction */
  FUNC_IMPL(GreaterThanOrEqualsFunction, SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                                   EXEC_DISPATCH(&GreaterThanOrEqualsFunction::gte_fn))))

  FUNC_BODY(GreaterThanOrEqualsFunction, gte_fn)
  {
    return Number::value_of(*args.at(0)) >= Number::value_of(*args.at(1)) ? B_TRUE : B_FALSE;
  }


  FUNC_IMPL(ThresholdFunction, SIG((FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                                    EXEC_DISPATCH(&ThresholdFunction::cap_value))))

  FUNC_BODY(ThresholdFunction, cap_value)
  {
    int a = args.at(0)->as<Lisple::Number>().value;
    int b = args.at(1)->as<Lisple::Number>().value;

    return b > a ? args.at(0) : args.at(1);
  }

  FUNC_IMPL(RangeFunction, SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                                EXEC_DISPATCH(&RangeFunction::make_range))))

  FUNC_BODY(RangeFunction, make_range)
  {
    float begin = args.front()->as<Number>().float_value();
    float end = args.back()->as<Number>().float_value();

    sptr_sobject_v result;

    for (float i = begin;
         begin < end ? i <= end : i >= end;
         begin < end ? i++ : i--)
    {
      result.push_back(box_number(i));
    }

    return std::make_shared<Array>(result);
  }


  MinMaxFunction::MinMaxFunction(bool min)
    : Function(SIG((FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                    EXEC_DISPATCH(&MinMaxFunction::select_min_or_max))))
    , min(min)
  {
  }

  FUNC_BODY(MinMaxFunction, select_min_or_max)
  {
    int a = args.at(0)->as<Lisple::Number>().value;
    int b = args.at(1)->as<Lisple::Number>().value;

    return (a < b) == min ? args.at(0) : args.at(1);
  }

  FUNC_IMPL(AndFunction, SIG((FN_ARGS((Lisple::VARARG, &Lisple::Type::BOOL)),
                              EXEC_DISPATCH(&AndFunction::logical_and))))

  FUNC_BODY(AndFunction, logical_and)
  {
    for (auto& arg : args)
    {
      if (*arg == *Lisple::B_FALSE)
      {
        return Lisple::B_FALSE;
      }
    }
    return Lisple::B_TRUE;
  }

  MACRO_IMPL(OrMacro, SIG((FN_ARGS((Lisple::VARARG, &Lisple::Type::ANY, false)),
                           EXEC_DISPATCH(&OrMacro::logical_or))))


  MACRO_BODY(OrMacro, logical_or)
  {
    ctx.push_context(true);
    for (auto& arg : args)
    {
      if (*ctx.eval(arg) == *Lisple::B_TRUE)
      {
        ctx.pop_context();
        return Lisple::B_TRUE;
      }
    }
    ctx.pop_context();
    return Lisple::B_FALSE;
  }

  FUNC_IMPL(GetFunction, SIG((FN_ARGS((&Type::ANY), (&Type::ANY)),
                              EXEC_DISPATCH(&GetFunction::get))))

  FUNC_BODY(GetFunction, get)
  {
    return args.front()->get_sptr_property(*args.back());
  }

  FUNC_IMPL(NthFunction, SIG((FN_ARGS((&Type::SEQ), (&Type::NUMBER)),
                              EXEC_DISPATCH(&NthFunction::get_nth))))

  FUNC_BODY(NthFunction, get_nth)
  {
    int n = args.back()->as<Number>().int_value();
    if (n >= static_cast<int>(args.front()->get_children().size()) || n < 0) return NIL;
    return args.front()->get_children().at(n);
  }

  /* AssocFunction - assoc */
  FUNC_IMPL(AssocFunction, SIG((FN_ARGS((&Type::MAP), (&Type::ANY), (&Type::ANY)),
                                EXEC_DISPATCH(&AssocFunction::assoc))))

  FUNC_BODY(AssocFunction, assoc)
  {
    Map& map = args.front()->as<Map>();
    sptr_sobject_v new_content;
    sptr_sobject assoc_key = args.at(1);
    sptr_sobject value = args.back();

    for (auto key : map.key_ptrs())
    {
      new_content.push_back(key);
      if (*key != *assoc_key)
      {
        new_content.push_back(map.get_sptr_property(*key));
      }
      else
      {
        new_content.push_back(value);
        value.reset();
      }
    }

    if (value.get())
    {
      new_content.push_back(assoc_key);
      new_content.push_back(value);
    }

    return std::make_shared<Map>(new_content);
  }

  /* AssocBangFunction - assoc! */
  FUNC_IMPL(AssocBangFunction, SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&Type::ANY)),
                                    EXEC_DISPATCH(&AssocBangFunction::assoc_bang))))

  FUNC_BODY(AssocBangFunction, assoc_bang)
  {
    sptr_sobject assoc_key = args.at(1);
    sptr_sobject value = args.back();

    if (Lisple::Type::MAP.is_type_of(*args.front()))
    {
      Map& map = args.front()->as<Map>();
      map.set_property(assoc_key, value);
    }
    else if (Lisple::Type::HOST_OBJECT.is_type_of(*args.front()))
    {
      AbstractHostObject& ho = args.front()->as<AbstractHostObject>();
      ho.set_property(&ctx, *assoc_key, value);
    }
    else
    {
      throw Lisple::TypeError("Cannot set key " + assoc_key->to_string() + " of " + args.front()->to_string());
    }

    return args.front();
  }

  /* AssocInBangFunction - assoc-in! */
  FUNC_IMPL(AssocInBangFunction, SIG((FN_ARGS((&Type::COMPLEX), (&Type::ARRAY), (&Type::ANY)),
                                      EXEC_DISPATCH(&AssocInBangFunction::assoc_in_bang))))

  FUNC_BODY(AssocInBangFunction, assoc_in_bang)
  {
    sptr_sobject assoc_path = args.at(1);
    if (assoc_path->get_children().empty())
    {
      throw InvocationException("Path for assoc-in! cannot be empty.");
    }
    sptr_sobject value = args.back();
    sptr_sobject assoc_key = assoc_path->get_children().back();

    sptr_sobject& target = args.front();
    for (size_t i=0; i<assoc_path->get_children().size()-1; i++)
    {
      target = target->get_sptr_property(*assoc_path->get_children().at(i));
    }

    if (Lisple::Type::MAP.is_type_of(*args.front()))
    {
      Map& map = args.front()->as<Map>();
      map.set_property(assoc_key, value);
    }
    else if (Lisple::Type::HOST_OBJECT.is_type_of(*args.front()))
    {
      AbstractHostObject& ho = args.front()->as<AbstractHostObject>();
      ho.set_property(&ctx, *assoc_key, value);
    }
    else
    {
      throw Lisple::TypeError("Cannot set key " + assoc_key->to_string() + " of " + args.front()->to_string());
    }

    return args.front();
  }

  /* MergeFunction - merge */
  FUNC_IMPL(MergeFunction, SIG((FN_ARGS((&Type::MAP), (&VARARG, &Type::MAP)),
                                EXEC_DISPATCH(&MergeFunction::merge_maps))))

  FUNC_BODY(MergeFunction, merge_maps)
  {
    std::shared_ptr<Map> result = std::make_shared<Map>(args.front()->as<Map>());

    for (size_t i=1; i<args.size(); i++)
    {
      for (auto& key : args.at(i)->as<Map>().key_ptrs())
      {
        result->set_property(key, args.at(i)->get_sptr_property(*key));
      }
    }

    return result;
  }

  /* ConcatFunction - concat */
  FUNC_IMPL(ConcatFunction, SIG((FN_ARGS((&VARARG, &Type::ANY)),
                                 EXEC_DISPATCH(&ConcatFunction::concat_array))))

  FUNC_BODY(ConcatFunction, concat_array)
  {
    auto result = std::make_shared<Lisple::Array>();

    for (auto& vec : args)
    {
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

  FUNC_IMPL(FlattenFunction, SIG((FN_ARGS((&Type::SEQ)),
                                  EXEC_DISPATCH(&FlattenFunction::flatten_array))))

  /* FlattenFunction - flatten */
  FUNC_BODY(FlattenFunction, flatten_array)
  {
    sptr_sobject_v result;

    for (auto obj : args.front()->get_children())
    {
      if (Type::ARRAY.is_type_of(*obj) ||
          Type::LIST.is_type_of(*obj))
      {
        auto flat_args = sptr_sobject_v { obj };
        auto flattened = flatten_array(ctx, flat_args);
        for (auto fl_obj : flattened->get_children())
        {
          result.push_back(fl_obj);
        }
      } else
      {
        result.push_back(obj);
      }
    }

    return std::make_shared<Array>(result);
  }

  FUNC_IMPL(HeadFunction, SIG((FN_ARGS((&Type::SEQ)),
                               EXEC_DISPATCH(&HeadFunction::head))))

  FUNC_BODY(HeadFunction, head)
  {
    return args.front()->as<Lisple::Sexpression>().head();
  }

  FUNC_IMPL(TailFunction, SIG((FN_ARGS((&Type::SEQ)),
                               EXEC_DISPATCH(&TailFunction::tail))))

  FUNC_BODY(TailFunction, tail)
  {
    return std::make_shared<Lisple::Array>(args.front()->as<Lisple::Sexpression>().tail());
  }

  FUNC_IMPL(LastFunction, SIG((FN_ARGS((&Type::SEQ)),
                               EXEC_DISPATCH(&LastFunction::last))))

  FUNC_BODY(LastFunction, last)
  {
    return args.front()->as<Lisple::Sexpression>().get_children().back();
  }

  FUNC_IMPL(CountFunction, MULTI_SIG((FN_ARGS((&Type::SEQ)),
                                       EXEC_DISPATCH(&CountFunction::count)),
                                     (FN_ARGS((&Type::STRING)),
                                       EXEC_DISPATCH(&CountFunction::count))));

  FUNC_BODY(CountFunction, count)
  {
    if (Lisple::Type::STRING.is_type_of(*args.front()))
    {
      return std::make_shared<Lisple::Number>((int) args.front()->as<Lisple::String>().value.length());
    }
    else if (Lisple::Type::SEQ.is_type_of(*args.front()))
    {
      return std::make_shared<Lisple::Number>((int) args.front()->get_children().size());
    }
    throw LispleException(args.front()->to_string() + " is not something that can be counted");
  }

  /* FilterFunction */
  FUNC_IMPL(FilterFunction, SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                                 EXEC_DISPATCH(&FilterFunction::filter_seq))))

  FUNC_BODY(FilterFunction, filter_seq)
  {
    auto original = args.front();
    sptr_sobject result = Sexpression::new_sequence(original->get_type());

    auto& filter_fn = args.back()->as<Executable>();

    for (auto val : original->get_children())
    {
      sptr_sobject_v val_args { val };
      sptr_sobject pred_result = filter_fn.execute(ctx, val_args);
      if (*pred_result != *NIL && *pred_result != *B_FALSE)
      {
        result->append(val);
      }
    }

    return result;
  }

  /* SortFunction - sort */
  FUNC_IMPL(SortFunction, SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                               EXEC_DISPATCH(&SortFunction::sort))))

  FUNC_BODY(SortFunction, sort)
  {
    Lisple::sptr_sobject_v elements = args.front()->get_children();
    if (elements.size() > 1)
    {
      Executable& comparator = args.back()->as<Executable>();
      Lisple::sptr_sobject tmp;
      bool modified = false;
      do
      {
        modified = false;
        for (size_t i=0; i<elements.size()-1; i++)
        {
          Lisple::sptr_sobject_v args = { elements.at(i), elements.at(i+1)};
          Lisple::sptr_sobject_v args_reverse = { elements.at(i+1), elements.at(i) };

          if (comparator.execute(ctx, args)->is_truthy() &&
              !comparator.execute(ctx, args_reverse)->is_truthy())
          {
            tmp = elements.at(i);
            elements.at(i) = elements.at(i+1);
            elements.at(i+1) = tmp;
            modified = true;
          }
        }
      }
      while (modified);
    }

    return std::make_shared<Array>(elements);
  }

  /* SomeFunction */
  FUNC_IMPL(SomeFunction, SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                               EXEC_DISPATCH(&SomeFunction::some))))

  FUNC_BODY(SomeFunction, some)
  {
    for (auto& element : args.front()->get_children())
    {
      sptr_sobject_v args = { element };
      sptr_sobject result = args.back()->execute(ctx, args);
      if (*result != *NIL && *result != *B_FALSE)
      {
        return B_TRUE;
      }
    }
    return B_FALSE;
  }

  /* RemoveFunction */
  FUNC_IMPL(RemoveFunction, SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ)),
                                 EXEC_DISPATCH(&RemoveFunction::remove_seq))))

  FUNC_BODY(RemoveFunction, remove_seq)
  {
    auto original = args.back();
    Lisple::sptr_sobject result = Lisple::Sexpression::new_sequence(original->get_type());

    auto& remove_fn = args.front()->as<Lisple::Executable>();

    for (auto val : original->get_children())
    {
      Lisple::sptr_sobject_v val_args { val };
      Lisple::sptr_sobject test_result = remove_fn.execute(ctx, val_args);
      if (*test_result == *Lisple::B_FALSE ||
          *test_result == *Lisple::NIL)
      {
        result->append(val);
      }
    }

    return result;
  }


  /* MapFunction - map */
  FUNC_IMPL(MapFunction, SIG((FN_ARGS((&VARARG, &Type::SEQ), (&Type::EXEC)),
                              EXEC_DISPATCH(&MapFunction::map_seq))))

  FUNC_BODY(MapFunction, map_seq)
  {
    auto& map_fn = args.back()->as<Object>();
    sptr_sobject_v result;
    sptr_sobject_v seqs;

    for (size_t i=0; i<args.size()-1; i++)
    {
      seqs.push_back(args.at(i));
    }

    auto max_lmnts_it = std::max_element(seqs.begin(), seqs.end(),
                                         [](const auto& a, const auto& b) {
                                           return a->size() < b->size();
                                         });
    for (size_t i = 0; i < (*max_lmnts_it)->size(); i++)
    {
      sptr_sobject_v iter_args;
      for (size_t seq_i = 0; seq_i < seqs.size(); seq_i++)
      {
        if (i < seqs.at(seq_i)->size())
        {
          iter_args.push_back(seqs.at(seq_i)->get_children().at(i));
        }
      }

      if (iter_args.size() == seqs.size())
      {
        sptr_sobject iter_result = map_fn.execute(ctx, iter_args);
        result.push_back(iter_result);
      }
      else
      {
        result.push_back(NIL);
      }
    }

    return std::make_shared<Array>(result);
  }

  /* ReduceFunction - reduce */
  FUNC_IMPL(ReduceFunction, SIG((FN_ARGS((&Type::SEQ), (&Type::ANY), (&Type::FUNCTION)),
                                 EXEC_DISPATCH(&ReduceFunction::reduce))))

  FUNC_BODY(ReduceFunction, reduce)
  {
    sptr_sobject result = args.at(1);
    Function& reducer = args.back()->as<Function>();

    for (auto& lmnt : args.front()->get_children())
    {
      sptr_sobject_v reducer_args { result, lmnt };
      sptr_sobject iter_result = reducer.execute(ctx, reducer_args);
      if (iter_result.get() != result.get())
      {
        result.swap(iter_result);
      }
    }

    return result;
  }

  /* ReduceKeyValueFunction - reduce-kv */
  FUNC_IMPL(ReduceKeyValueFunction, SIG((FN_ARGS((&Type::MAP), (&Type::ANY), (&Type::FUNCTION)),
                                         EXEC_DISPATCH(&ReduceKeyValueFunction::reduce_kv))))

  FUNC_BODY(ReduceKeyValueFunction, reduce_kv)
  {
    sptr_sobject result = args.at(1);
    Function& reducer = args.back()->as<Function>();

    for (auto key : args.front()->as<Map>().key_ptrs())
    {
      sptr_sobject_v reducer_args { result,
                                    key,
                                    args.front()->get_sptr_property(*key) };

      sptr_sobject new_result = reducer.execute(ctx, reducer_args);
      if (new_result.get() != result.get())
      {
        result.swap(new_result);
      }
    }

    return result;
  }

  /* FindFirstFunction - find-first */
  FUNC_IMPL(FindFirstFunction, SIG((FN_ARGS((&Type::SEQ), (&Type::FUNCTION)),
                                    EXEC_DISPATCH(&FindFirstFunction::find_first_in_seq))))

  FUNC_BODY(FindFirstFunction, find_first_in_seq)
  {
    auto original = args.front();
    sptr_sobject result = Lisple::Sexpression::new_sequence(original->get_type());

    auto& filter_fn = args.back()->as<Executable>();

    for (auto val : args.front()->get_children())
    {
      sptr_sobject_v val_args{ val };
      sptr_sobject pred_result = filter_fn.execute(ctx, val_args);
      if (*pred_result != *B_FALSE && *pred_result != *NIL)
      {
        return val;
      }
    }

    return NIL;
  }

  /* SeqMatchFunction - seq-match */
  FUNC_IMPL(SeqMatchFunction, SIG((FN_ARGS((&Lisple::Type::SEQ), (&Lisple::Type::MAP)),
                                   EXEC_DISPATCH(&SeqMatchFunction::match))))

  bool match_map_like(sptr_sobject& obj, sptr_sobject& pattern)
  {
    for (Object* key : pattern->as<Lisple::Map>().keys())
    {
      sptr_sobject prop = pattern->get_sptr_property(*key);
      sptr_sobject value = obj->get_sptr_property(*key);

      if (Type::MAP.is_type_of(*prop))
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
    sptr_sobject& seq = args.front();
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

  FUNC_IMPL(KeysFunction, SIG((FN_ARGS((&Lisple::Type::ANY)),
                               EXEC_DISPATCH(&KeysFunction::keys_fn))))

  FUNC_BODY(KeysFunction, keys_fn)
  {
    sptr_sobject_v result;

    if (args.front()->get_type() == Form::HOST_OBJECT)
    {
      for (auto& k : args.front()->as<Lisple::AbstractHostObject>().keys())
      {
        result.push_back(k);
      }
    }
    else if (args.front()->get_type() == Form::MAP)
    {
      result = args.front()->as<Map>().key_ptrs();
    }

    return std::make_shared<Array>(result);
  }

  /* SelectKeysFunction */
  FUNC_IMPL(SelectKeysFunction, SIG((FN_ARGS((&Lisple::Type::ANY), (&Lisple::Type::SEQ)),
                                     EXEC_DISPATCH(&SelectKeysFunction::select_keys_fn))));

  FUNC_BODY(SelectKeysFunction, select_keys_fn)
  {
    auto& obj = args.front()->as<Lisple::Object>();

    sptr_sobject_v new_content;
    for (auto& key : args.back()->as<Lisple::Sexpression>().get_children())
    {
      auto value = obj.get_sptr_property(*key);
      if (value->get_type() != Form::NIL)
      {
        new_content.push_back(key);
        new_content.push_back(value);
      }
    }

    return std::make_shared<Lisple::Map>(new_content);
  }

  /* PartitionFunction */
  FUNC_IMPL(PartitionFunction, SIG((FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::SEQ)),
                                    EXEC_DISPATCH(&PartitionFunction::partition))))

  FUNC_BODY(PartitionFunction, partition)
  {
    std::shared_ptr<Lisple::Array> result = std::make_shared<Lisple::Array>();
    unsigned int part_size = Lisple::uint_val(*args.front());

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
    return args.front()->as<Lisple::Number>().int_value() % 2 == modulus ? Lisple::B_TRUE : Lisple::B_FALSE;
  }

  /* EmptyPredicateFunction */
  FUNC_IMPL(EmptyPredicateFunction, MULTI_SIG((FN_ARGS((&Lisple::Type::SEQ)),
                                               EXEC_DISPATCH(&EmptyPredicateFunction::exec_emptyp_seq)),
                                              (FN_ARGS((&Lisple::Type::STRING)),
                                               EXEC_DISPATCH(&EmptyPredicateFunction::exec_emptyp_string))))

  FUNC_BODY(EmptyPredicateFunction, exec_emptyp_seq)
  {
    return Lisple::Boolean::wrap(args.front()->get_children().empty());
  }

  FUNC_BODY(EmptyPredicateFunction, exec_emptyp_string)
  {
    return Lisple::Boolean::wrap(args.front()->as<Lisple::String>().value.empty());
  }

  /* NotEmptyPredicateFunction */
  FUNC_IMPL(NotEmptyPredicateFunction, MULTI_SIG((FN_ARGS((&Lisple::Type::SEQ)),
                                                  EXEC_DISPATCH(&NotEmptyPredicateFunction::exec_not_emptyp_seq)),
                                                 (FN_ARGS((&Lisple::Type::STRING)),
                                                  EXEC_DISPATCH(&NotEmptyPredicateFunction::exec_not_emptyp_string))))

  FUNC_BODY(NotEmptyPredicateFunction, exec_not_emptyp_seq)
  {
    return Lisple::Boolean::wrap(!args.front()->get_children().empty());
  }

  FUNC_BODY(NotEmptyPredicateFunction, exec_not_emptyp_string)
  {
    return Lisple::Boolean::wrap(!args.front()->as<Lisple::String>().value.empty());
  }

  FUNC_IMPL(IncludeFunction, SIG((FN_ARGS((&Lisple::Type::STRING)),
                                  EXEC_DISPATCH(&IncludeFunction::include_file))))

  FUNC_BODY(IncludeFunction, include_file)
  {
    ctx.read_file(Lisple::Value<std::string>::value_of(*args.at(0)));
    return args.at(0);
  }

  FUNC_IMPL(EvalFunction, MULTI_SIG((FN_ARGS((&Lisple::Type::STRING)),
                                     EXEC_DISPATCH(&EvalFunction::eval_string)),
                                    (FN_ARGS((&Lisple::Type::LIST)),
                                     EXEC_DISPATCH(&EvalFunction::eval_seq))))

  FUNC_IMPL(ApplyFunction, SIG((FN_ARGS((&Lisple::Type::FUNCTION), (&Lisple::Type::ARRAY)),
                                EXEC_DISPATCH(&ApplyFunction::apply_fn))))

  FUNC_BODY(ApplyFunction, apply_fn)
  {
    auto& fn = *args.front();
    auto& fn_args = args.back()->get_children();

    return fn.execute(ctx, fn_args);
  }

  FUNC_BODY(EvalFunction, eval_string)
  {
    const std::string& str = args.at(0)->as<Lisple::String>().value;
    return ctx.eval(str);
  }

  FUNC_BODY(EvalFunction, eval_seq)
  {
    return ctx.eval(args.at(0));
  }

  FUNC_IMPL(RandNthFunction, SIG((FN_ARGS((&Lisple::Type::SEQ)),
                                  EXEC_DISPATCH(&RandNthFunction::rand_nth))))

  FUNC_BODY(RandNthFunction, rand_nth)
  {
    auto& seq = args.front();
    if (seq->get_children().size() == 0)
    {
      return NIL;
    }
    return seq->get_children().at(std::rand() % (seq->size()-1));
  }

  FUNC_IMPL(RndFunction, MULTI_SIG((FN_ARGS((&Lisple::Type::NUMBER)),
                                    EXEC_DISPATCH(&RndFunction::random_number)),
                                   (FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                                    EXEC_DISPATCH(&RndFunction::random_number))))

  FUNC_BODY(RndFunction, random_number)
  {
    int min = args.size() == 1 ? 0 : args.at(0)->as<Lisple::Number>().value;
    int max = args.at(args.size() == 1 ? 0 : 1)->as<Lisple::Number>().value;

    return std::make_shared<Lisple::Number>((std::rand() % (max - min)) + min);
  }

  FUNC_IMPL(VectorFunction, SIG((FN_ARGS((&VARARG, &Type::ANY)),
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

  /**
   * ContainsPredicateFunction
   */
  FUNC_IMPL(ContainsPredicateFunction, SIG((FN_ARGS((&Type::ARRAY), (&Type::ANY)),
                                            EXEC_DISPATCH(&ContainsPredicateFunction::contains))))

  FUNC_BODY(ContainsPredicateFunction, contains)
  {
    sptr_sobject_v vector = args.front()->as<Array>().get_children();
    return std::find_if(vector.begin(),
                        vector.end(),
                        [&args](sptr_sobject lmnt)
                        { return *lmnt == *args.back(); }) != vector.end() ? B_TRUE : B_FALSE;
  }

  FUNC_IMPL(TakeFunction, SIG((FN_ARGS((&Type::NUMBER), (&Type::SEQ)),
                               EXEC_DISPATCH(&TakeFunction::take_fn))))

  FUNC_BODY(TakeFunction, take_fn)
  {
    int amount = args.front()->as<Number>().int_value();
    sptr_sobject_v vector = args.back()->as<Sexpression>().get_children();

    sptr_sobject_v result;
    for (int i=0; i < std::min(amount, static_cast<int>(vector.size())); i++)
    {
      result.push_back(vector.at(i));
    }

    return std::make_shared<Array>(result);
  }

  FUNC_IMPL(StrFunction, SIG((FN_ARGS((VARARG, &Type::ANY)),
                              EXEC_DISPATCH(&StrFunction::concat_str))))

  FUNC_BODY(StrFunction, concat_str)
  {
    std::string result = "";

    for (auto& obj : args)
    {
      switch (obj->get_type())
      {
       case Form::STRING:
        result += obj->as<String>().value;
        break;
       case Form::CHAR:
        result += obj->as<Char>().value;
        break;
       default:
        result += obj->to_string();
        break;
      }
    }

    return std::make_shared<String>(result);
  }

  FUNC_IMPL(JoinFunction, SIG((FN_ARGS((VARARG, &Type::STRING)),
                               EXEC_DISPATCH(&JoinFunction::join_str))))

  FUNC_BODY(JoinFunction, join_str)
  {
    if (args.size() < 2)
    {
      return std::make_shared<Lisple::String>("");
    }

    std::string joiner = args.at(0)->as<Lisple::String>().value;
    std::string result = args.at(1)->as<Lisple::String>().value;

    for (size_t i=2; i < args.size(); i++)
    {
      result += joiner + args.at(i)->as<Lisple::String>().value;
    }
    return std::make_shared<Lisple::String>(result);
  }

}// namespace Lisple
