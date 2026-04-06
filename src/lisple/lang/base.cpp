#include "lisple/exec.h"
#include "lisple/form.h"
#include "lisple/runtime/lower.h"
#include "lisple/runtime/node.h"

#include <iostream>

#include <lisple/lang/base.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
  /* DefForm */
  SPECIAL_FORM_IMPL(DefForm,
                    MULTI_SIG((FN_ARGS((&Type::WORD, DATA), (&Type::ANY)),
                               EXEC_DISPATCH(&DefForm::inv_def, &DefForm::execnode_def)),
                              (FN_ARGS((&Type::WORD, DATA), (&Type::STRING), (&Type::ANY)),
                               EXEC_DISPATCH(&DefForm::inv_def_docstring,
                                             &DefForm::execnode_def_docstring))))

  SFORM_LOWER_IMPL(DefForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();
    if (elements.size() < 3 || elements.size() > 4)
    {
      throw LispleException("Invalid def form: " + ast_node->to_string());
    }
    if (elements[1]->get_type() != Lisple::Form::WORD)
    {
      throw TypeError("Invalid symbol name: " + elements[0]->to_string());
    }
    if (elements.size() == 4 && elements[2]->get_type() != Lisple::Form::STRING)
    {
      throw LispleException("Invalid def form: " + ast_node->to_string());
    }

    Lisple::Word& symbol = elements[1]->as<Lisple::Word>();
    sptr_sobject doc_string = elements.size() == 4 ? elements[2] : nullptr;
    sptr_sobject value = !doc_string ? elements[2] : elements[3];

    uptr_exec_node_v value_node;
    value_node.push_back(lower_expr(ctx, value));

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, {RTValue::symbol(symbol.value)}, std::move(value_node)));
  }

  /**
   * Legacy AST-based implementation.
   */
  MACRO_BODY(DefForm, inv_def)
  {
    deprecated_special_form_invocations++;
    Lisple::Word& symbol = args[0]->as<Lisple::Word>();
    ctx.store_namespace(symbol, args[1]);
    return ctx.get_current_namespace()->lookup(symbol);
  }

  EXECNODE_BODY(DefForm, execnode_def)
  {
    std::string symbol = snode.values.front()->str();
    sptr_rtval value = exec(ctx, *snode.exec_nodes.front());
    ctx.store_namespace(symbol, value);
    return value;
  }

  /**
   * Legacy AST-based implementation.
   */
  MACRO_BODY(DefForm, inv_def_docstring)
  {
    deprecated_special_form_invocations++;
    ctx.store_namespace(args[0]->as<Lisple::Word>(), args[2]);
    return args[2];
  }

  EXECNODE_BODY(DefForm, execnode_def_docstring)
  {
    deprecated_special_form_invocations++;

    return Constant::NIL;
  }

  /**
   * DoForm - do
   */
  SPECIAL_FORM_IMPL(DoForm,
                    SIG((FN_ARGS((&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&DoForm::inv_do, &DoForm::execnode_do))))

  SFORM_LOWER_IMPL(DoForm)
  {
    uptr_exec_node_v exec_nodes;

    sptr_sobject_v& elements = ast_node->get_children();

    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_rtval_v{}, std::move(exec_nodes)));
  }

  MACRO_BODY(DoForm, inv_do)
  {
    deprecated_special_form_invocations++;
    Lisple::sptr_sobject ret;
    ctx.push_context(true);
    for (auto& arg : args)
    {
      ret = ctx.eval_ast(arg);
    }
    ctx.pop_context();
    return ret;
  }

  EXECNODE_BODY(DoForm, execnode_do)
  {
    Lisple::sptr_rtval ret;
    for (auto& form : snode.exec_nodes)
    {
      ret = exec(ctx, *form);
    }
    return ret;
  }

  /** EvalFunction - eval */
  FUNC_IMPL(EvalFunction,
            MULTI_SIG((FN_ARGS((&Lisple::Type::STRING)),
                       EXEC_DISPATCH(&EvalFunction::exec_eval_string)),
                      (FN_ARGS((&Lisple::Type::ANY)),
                       EXEC_DISPATCH(&EvalFunction::exec_eval_form))))

  EXEC_BODY(EvalFunction, exec_eval_string)
  {
    return ctx.eval(args[0]->str());
  }

  EXEC_BODY(EvalFunction, exec_eval_form)
  {
    return ctx.eval(to_AST(*args[0]));
  }

  /** IncludeFunction - include */
  FUNC_IMPL(IncludeFunction,
            SIG((FN_ARGS((&Lisple::Type::STRING)),
                 EXEC_DISPATCH(&IncludeFunction::exec_include))))

  EXEC_BODY(IncludeFunction, exec_include)
  {
    ctx.read_file(args[0]->str());
    return args[0];
  }

  /** AndForm - and */
  SPECIAL_FORM_IMPL(AndForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&AndForm::inv_and, &AndForm::execnode_and))))

  SFORM_LOWER_IMPL(AndForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    uptr_exec_node_v exec_nodes;
    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_rtval_v{}, std::move(exec_nodes)));
  }

  /**
   * Legacy AST-based implementation.
   */
  MACRO_BODY(AndForm, inv_and)
  {
    deprecated_special_form_invocations++;
    ctx.push_context(true);
    for (auto& arg : args)
    {
      sptr_sobject lmnt = ctx.eval_ast(arg);
      if (!lmnt->is_truthy())
      {
        ctx.pop_context();
        return B_FALSE;
      }
    }
    ctx.pop_context();
    return B_TRUE;
  }

  EXECNODE_BODY(AndForm, execnode_and)
  {
    Lisple::sptr_rtval val;
    for (auto& form : snode.exec_nodes)
    {
      val = exec(ctx, *form);
      if (!Lisple::is_truthy(*val))
      {
        return Constant::BOOL_FALSE;
      }
    }
    return val;
  }

  /* NsMacro */
  SPECIAL_FORM_IMPL(NsForm,
                    MULTI_SIG((FN_ARGS((&Type::WORD, DATA)),
                               EXEC_DISPATCH(&NsForm::inv_ns, &NsForm::execnode_ns)),
                              (FN_ARGS((&Type::WORD, DATA), (&Type::LIST, DATA)),
                               EXEC_DISPATCH(&NsForm::inv_ns, &NsForm::execnode_ns))))

  Key KEY_REQUIRE("require");
  Key KEY_AS("as");

  void throw_ns_exception(Word& ns, List& req_list, std::string msg = "")
  {
    std::string ns_decl = "(ns " + ns.value;
    ns_decl += " " + req_list.to_string();
    ns_decl += ")";

    throw NamespaceException("Invalid ns form: " + ns_decl + msg);
  }

  SFORM_LOWER_IMPL(NsForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    Word& ns_word = elements[1]->as<Word>();

    Lisple::sptr_sobject_v imports;
    if (elements.size() == 3)
    {
      Lisple::List& list = elements.back()->as<List>();
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

    ctx.ctx->switch_namespace(ns_word.value);
    for (auto& imp : imports)
    {
      if (Type::WORD.is_type_of(*imp))
      {
        // Full import
        Word& imp_word = imp->as<Word>();
        ctx.ctx->import_namespace(imp_word.value);
      }
      else if (Type::ARRAY.is_type_of(*imp))
      {
        // Aliased import
        Array& imp_array = imp->as<Array>();
        Word& imp_word = imp_array.head()->as<Word>();
        Word& alias_word = imp_array.get_children().back()->as<Word>();
        ctx.ctx->define_namespace_alias(imp_word.value, alias_word.value);
      }
    }

    return std::make_unique<ExecNode>(Constant::NIL);
  }

  MACRO_BODY(NsForm, inv_ns)
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

  /** NameFunction - name */
  FUNC_IMPL(NameFunction,
            SIG((FN_ARGS((&Type::QUALIFIABLE)), EXEC_DISPATCH(&NameFunction::exec_name))))

  EXEC_BODY(NameFunction, exec_name)
  {
    if (args[0]->type == RTValue::Type::NIL) return Constant::NIL;
    return RTValue::string(args[0]->qual().second);
  }

  EXECNODE_BODY(NsForm, execnode_ns)
  {
    throw LispleException("Invocation of namespace");
  }

  /** OrForm - or */
  SPECIAL_FORM_IMPL(OrForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&OrForm::inv_or, &OrForm::execnode_or))))

  SFORM_LOWER_IMPL(OrForm)
  {
    uptr_exec_node_v exec_nodes;

    sptr_sobject_v& elements = ast_node->get_children();

    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_rtval_v{}, std::move(exec_nodes)));
  }

  MACRO_BODY(OrForm, inv_or)
  {
    deprecated_special_form_invocations++;
    ctx.push_context(true);
    for (auto& arg : args)
    {
      sptr_sobject lmnt = ctx.eval_ast(arg);
      if (lmnt->is_truthy())
      {
        ctx.pop_context();
        return lmnt;
      }
    }
    ctx.pop_context();
    return B_FALSE;
  }

  EXECNODE_BODY(OrForm, execnode_or)
  {
    Lisple::sptr_rtval val;
    for (auto& form : snode.exec_nodes)
    {
      val = exec(ctx, *form);
      if (Lisple::is_truthy(*val))
      {
        return val;
      }
    }

    return Constant::BOOL_FALSE;
  }

  /** PrnFunction - prn */
  FUNC_IMPL(PrnFunction,
            SIG((FN_ARGS((&VARARG, &Type::ANY)), EXEC_DISPATCH(&PrnFunction::exec_prn))))

  EXEC_BODY(PrnFunction, exec_prn)
  {
    for (size_t i = 0; i < args.size(); i++)
    {
      if (i > 0) std::cout << " ";
      std::cout << (args[i]->type == RTValue::Type::STRING ? args[i]->str()
                                                           : args[i]->to_string());
    }
    std::cout << std::endl;
    return Constant::NIL;
  }

  /** ResolveFunction - resolve */
  FUNC_IMPL(ResolveFunction,
            MULTI_SIG((FN_ARGS((&Lisple::Type::SYMBOL)),
                       EXEC_DISPATCH(&ResolveFunction::exec_resolve)),
                      (FN_ARGS((&Lisple::Type::WORD)),
                       EXEC_DISPATCH(&ResolveFunction::exec_resolve))))

  EXEC_BODY(ResolveFunction, exec_resolve)
  {
    if (args[0]->type == RTValue::Type::NIL) return Constant::NIL;
    return ctx.lookup_value(Word(args[0]->str()));
  }

  /** RndFunction - rnd */
  FUNC_IMPL(RndFunction,
            MULTI_SIG((FN_ARGS((&Lisple::Type::NUMBER)),
                       EXEC_DISPATCH(&RndFunction::exec_rnd)),
                      (FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                       EXEC_DISPATCH(&RndFunction::exec_rnd))))

  EXEC_BODY(RndFunction, exec_rnd)
  {
    if (args[0]->type != RTValue::Type::NUMBER &&
        (args.size() == 2 && args[1]->type != RTValue::Type::NUMBER))
    {
      return Constant::NIL;
    }
    int min =
      args.size() == 1 ? 0 : std::get<const RTValue::Number>(args[0]->value).get_int();
    int max =
      std::get<const RTValue::Number>(args[args.size() == 1 ? 0 : 1]->value).get_int();

    if (min == max) return RTValue::number(min);

    return RTValue::number((std::rand() % (max - min)) + min);
  }

  /** SetBangForm - set! */
  SPECIAL_FORM_IMPL(SetBangForm,
                    SIG((FN_ARGS((&Type::ARRAY, DATA), (&Lisple::Type::ANY)),
                         EXEC_DISPATCH(&SetBangForm::inv_set, &SetBangForm::execnode_set))))

  SFORM_LOWER_IMPL(SetBangForm)
  {
    auto& elements = ast_node->get_children();
    if (elements.size() != 3 || elements[1]->get_type() != Form::ARRAY)
    {
      throw TypeError("Invalid set! form: " + ast_node->to_string());
    }

    uptr_exec_node_v value_node;
    value_node.push_back(lower_expr(ctx, elements.back()));

    sptr_rtval_v bind_path = to_rt_value(*elements[1])->elements();

    return std::make_unique<ExecNode>(
      ast_node,
      SpecialFormNode(this, bind_path, std::move(value_node)));
  }

  MACRO_BODY(SetBangForm, inv_set)
  {
    deprecated_special_form_invocations++;
    Lisple::Array& member_ref = args[0]->as<Lisple::Array>();

    if (member_ref.size() == 1)
    {
      auto identifier = member_ref.get_children()[0]->as<Lisple::Word>();
      Scope& scope = ctx.get_scope_of(identifier);
      scope.mutate(identifier, args.back());
    }
    else if (member_ref.size() == 2)
    {
      auto actual_mem_ref = ctx.eval_ast(args[0]);
      Lisple::Object& prop = *actual_mem_ref->get_children()[0];
      Lisple::Object& owner = *actual_mem_ref->get_children().back();

      if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(&owner))
      {
        auto val = to_rt_value(args.back());
        Lisple::Dict::set_property(wrapper->val, to_rt_value(prop), val);
      }
      else
      {
        owner.set_property(&ctx, prop, args.back());
      }
    }
    else
    {
      throw Lisple::InvocationException("Incorrect member reference: " +
                                        member_ref.to_string());
    }

    return args.back();
  }

  EXECNODE_BODY(SetBangForm, execnode_set)
  {
    sptr_rtval_v member_refs = snode.values;

    auto value = exec(ctx, *snode.exec_nodes.front());

    if (member_refs.size() == 1)
    {
      auto identifier = Lisple::Word::make(member_refs[0]->str());
      Scope& scope = ctx.get_scope_of(*identifier);
      scope.mutate(identifier->value, value);
    }
    else if (member_refs.size() == 2)
    {
      auto& prop = member_refs[0];
      sptr_rtval owner = ctx.lookup_value(member_refs.back()->to_string());

      Lisple::Dict::set_property(owner, prop, value);
    }
    else
    {
      throw Lisple::InvocationException("Incorrect member reference: " +
                                        RTValue::vector(member_refs)->to_string());
    }

    return value;
  }

  /** QualifierFunction - namespace */
  FUNC_IMPL(QualifierFunction,
            SIG((FN_ARGS((&Type::QUALIFIABLE)),
                 EXEC_DISPATCH(&QualifierFunction::exec_qualifier))))

  EXEC_BODY(QualifierFunction, exec_qualifier)
  {
    if (args[0]->type == RTValue::Type::NIL) return Constant::NIL;
    const std::string qualifier = args[0]->qual().first;
    return qualifier.empty() ? Constant::NIL : RTValue::string(qualifier);
  }

} // namespace Lisple
