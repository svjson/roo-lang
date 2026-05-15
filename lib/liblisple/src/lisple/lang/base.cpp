#include "lisple/exec.h"
#include "lisple/form.h"
#include "lisple/runtime/lower.h"
#include "lisple/runtime/node.h"

#include <algorithm>
#include <cctype>
#include <iostream>

#include <lisple/lang/base.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
  /* DefForm */
  SPECIAL_FORM_IMPL(DefForm,
                    MULTI_SIG((FN_ARGS((&Type::SYMBOL, DATA), (&Type::ANY)),
                               EXEC_DISPATCH(&DefForm::execnode_def)),
                              (FN_ARGS((&Type::SYMBOL, DATA), (&Type::STRING), (&Type::ANY)),
                               EXEC_DISPATCH(&DefForm::execnode_def_docstring))))

  SFORM_LOWER_IMPL(DefForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements.size() < 3 || elements.size() > 4)
    {
      throw LispleException("Invalid def form: " + ast_node->to_string());
    }
    if (elements[1]->get_type() != Lisple::Form::SYMBOL)
    {
      throw TypeError("Invalid symbol name: " + elements[0]->to_string());
    }
    if (elements.size() == 4 && elements[2]->get_type() != Lisple::Form::STRING)
    {
      throw LispleException("Invalid def form: " + ast_node->to_string());
    }

    Lisple::AST::Symbol& symbol = elements[1]->as<Lisple::AST::Symbol>();
    sptr_ast_node doc_string = elements.size() == 4 ? elements[2] : nullptr;
    sptr_ast_node value = !doc_string ? elements[2] : elements[3];

    uptr_exec_node_v value_node;
    value_node.push_back(lower_expr(ctx, value));

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, {Value::symbol(symbol.value)}, std::move(value_node)));
  }

  EXECNODE_BODY(DefForm, execnode_def)
  {
    std::string symbol = snode.values.front()->str();
    sptr_val value = exec(ctx, *snode.exec_nodes.front());
    ctx.store_namespace(symbol, value);
    return value;
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
                         EXEC_DISPATCH(&DoForm::execnode_do))))

  SFORM_LOWER_IMPL(DoForm)
  {
    uptr_exec_node_v exec_nodes;

    sptr_ast_node_v& elements = ast_node->get_children();

    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_val_v{}, std::move(exec_nodes)));
  }
  EXECNODE_BODY(DoForm, execnode_do)
  {
    Lisple::sptr_val ret;
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
                         EXEC_DISPATCH(&AndForm::execnode_and))))

  SFORM_LOWER_IMPL(AndForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    uptr_exec_node_v exec_nodes;
    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_val_v{}, std::move(exec_nodes)));
  }

  EXECNODE_BODY(AndForm, execnode_and)
  {
    Lisple::sptr_val val;
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

  /** KeywordFunction - keyword */
  FUNC_IMPL(KeywordFunction,
            SIG((FN_ARGS((&Type::STRING_OR_SYMBOL)),
                 EXEC_DISPATCH(&KeywordFunction::exec_keyword))))

  EXEC_BODY(KeywordFunction, exec_keyword)
  {
    return Value::keyword(args[0]->str());
  }

  /** KeywordPFunction - keyword? */
  FUNC_IMPL(KeywordPFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&KeywordPFunction::exec_keyword))))

  EXEC_BODY(KeywordPFunction, exec_keyword)
  {
    return args[0]->type == Value::Type::KEYWORD ? Constant::BOOL_TRUE
                                                 : Constant::BOOL_FALSE;
  }

  /* NsMacro */
  SPECIAL_FORM_IMPL(NsForm,
                    MULTI_SIG((FN_ARGS((&Type::SYMBOL, DATA)),
                               EXEC_DISPATCH(&NsForm::execnode_ns)),
                              (FN_ARGS((&Type::SYMBOL, DATA), (&Type::LIST, DATA)),
                               EXEC_DISPATCH(&NsForm::execnode_ns))))

  AST::Keyword KEY_REQUIRE("require");
  AST::Keyword KEY_AS("as");

  void throw_ns_exception(AST::Symbol& ns, AST::List& req_list, std::string msg = "")
  {
    std::string ns_decl = "(ns " + ns.value;
    ns_decl += " " + req_list.to_string();
    ns_decl += ")";

    throw NamespaceException("Invalid ns form: " + ns_decl + msg);
  }

  SFORM_LOWER_IMPL(NsForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    AST::Symbol& ns_symbol = elements[1]->as<AST::Symbol>();

    Lisple::sptr_ast_node_v imports;
    if (elements.size() == 3)
    {
      Lisple::AST::List& list = elements.back()->as<AST::List>();
      if (list.size() < 2 || (list.size() > 0 && *list.get_children()[0] != KEY_REQUIRE))
      {
        throw_ns_exception(ns_symbol, list);
      }
      imports = list.tail();

      // Verify the require forms the brute-ish and tedious way...
      // FIXME: Maybe implement some kind of Matcher system for forms?
      for (auto& imp : imports)
      {
        if (Type::SYMBOL.is_type_of(*imp))
        {
          if (imp->as<AST::Symbol>().is_qualified())
          {
            throw_ns_exception(ns_symbol,
                               list,
                               ". Invalid require-entry: " + imp->to_string());
          }
        }
        else if (Type::VECTOR.is_type_of(*imp))
        {
          if (imp->get_children().size() != 3)
          {
            throw_ns_exception(ns_symbol,
                               list,
                               ". Invalid require-entry: " + imp->to_string());
          }

          if (*imp->get_children()[1] == KEY_AS)
          {
            if (!Type::SYMBOL.is_type_of(*imp->get_children().back()) ||
                imp->get_children().back()->as<AST::Symbol>().is_qualified())
            {
              throw_ns_exception(ns_symbol,
                                 list,
                                 ". Invalid require-entry: " + imp->to_string());
            }
          }
          else
          {
            throw_ns_exception(ns_symbol,
                               list,
                               ". Invalid require-entry: " + imp->to_string());
          }
        }
        else
        {
          throw_ns_exception(ns_symbol,
                             list,
                             ". Invalid require-entry: " + imp->to_string());
        }
      }
    }

    ctx.ctx->switch_namespace(ns_symbol.value);
    for (auto& imp : imports)
    {
      if (Type::SYMBOL.is_type_of(*imp))
      {
        // Full import
        AST::Symbol& imp_symbol = imp->as<AST::Symbol>();
        ctx.ctx->import_namespace(imp_symbol.value);
      }
      else if (Type::VECTOR.is_type_of(*imp))
      {
        // Aliased import
        AST::Vector& imp_vector = imp->as<AST::Vector>();
        AST::Symbol& imp_symbol = imp_vector.head()->as<AST::Symbol>();
        AST::Symbol& alias_symbol = imp_vector.get_children().back()->as<AST::Symbol>();
        ctx.ctx->define_namespace_alias(imp_symbol.value, alias_symbol.value);
      }
    }

    return std::make_unique<ExecNode>(Constant::NIL);
  }
  EXECNODE_BODY(NsForm, execnode_ns)
  {
    throw LispleException("Invocation of namespace");
  }

  /** NameFunction - name */
  FUNC_IMPL(NameFunction,
            SIG((FN_ARGS((&Type::QUALIFIABLE)), EXEC_DISPATCH(&NameFunction::exec_name))))

  EXEC_BODY(NameFunction, exec_name)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::NIL;
    return Value::string(args[0]->qual().second);
  }

  /** OrForm - or */
  SPECIAL_FORM_IMPL(OrForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&OrForm::execnode_or))))

  SFORM_LOWER_IMPL(OrForm)
  {
    uptr_exec_node_v exec_nodes;

    sptr_ast_node_v& elements = ast_node->get_children();

    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_val_v{}, std::move(exec_nodes)));
  }
  EXECNODE_BODY(OrForm, execnode_or)
  {
    Lisple::sptr_val val = Constant::NIL;
    for (auto& form : snode.exec_nodes)
    {
      val = exec(ctx, *form);
      if (Lisple::is_truthy(*val))
      {
        return val;
      }
    }

    return val;
  }

  /** PrnFunction - prn */
  FUNC_IMPL(PrnFunction,
            SIG((FN_ARGS((&VARARG, &Type::ANY)), EXEC_DISPATCH(&PrnFunction::exec_prn))))

  EXEC_BODY(PrnFunction, exec_prn)
  {
    for (size_t i = 0; i < args.size(); i++)
    {
      if (i > 0) std::cout << " ";
      std::cout << (args[i]->type == Value::Type::STRING ? args[i]->str()
                                                         : args[i]->to_string());
    }
    std::cout << std::endl;
    return Constant::NIL;
  }

  /** TypeOfFunction - type-of */
  FUNC_IMPL(TypeOfFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&TypeOfFunction::exec_type_of))))

  EXEC_BODY(TypeOfFunction, exec_type_of)
  {
    return Value::string(std::string(type_string(*args[0])));
  }

  /** ResolveFunction - resolve */
  FUNC_IMPL(ResolveFunction,
            MULTI_SIG((FN_ARGS((&Lisple::Type::QUOTED_SYMBOL)),
                       EXEC_DISPATCH(&ResolveFunction::exec_resolve)),
                      (FN_ARGS((&Lisple::Type::SYMBOL)),
                       EXEC_DISPATCH(&ResolveFunction::exec_resolve))))

  EXEC_BODY(ResolveFunction, exec_resolve)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::NIL;
    return ctx.lookup(args[0]->str());
  }

  /** RndFunction - rnd */
  FUNC_IMPL(RndFunction,
            MULTI_SIG((FN_ARGS((&Lisple::Type::NUMBER)),
                       EXEC_DISPATCH(&RndFunction::exec_rnd)),
                      (FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                       EXEC_DISPATCH(&RndFunction::exec_rnd))))

  EXEC_BODY(RndFunction, exec_rnd)
  {
    if (args[0]->type != Value::Type::NUMBER &&
        (args.size() == 2 && args[1]->type != Value::Type::NUMBER))
    {
      return Constant::NIL;
    }
    int min = args.size() == 1 ? 0 : std::get<const Value::Number>(args[0]->value).get_int();
    int max = std::get<const Value::Number>(args[args.size() == 1 ? 0 : 1]->value).get_int();

    if (min == max) return Value::number(min);

    return Value::number((std::rand() % (max - min)) + min);
  }

  /** SetBangForm - set! */
  SPECIAL_FORM_IMPL(SetBangForm,
                    SIG((FN_ARGS((&Type::VECTOR, DATA), (&Lisple::Type::ANY)),
                         EXEC_DISPATCH(&SetBangForm::execnode_set))))

  SFORM_LOWER_IMPL(SetBangForm)
  {
    auto& elements = ast_node->get_children();
    if (elements.size() != 3 || elements[1]->get_type() != Form::VECTOR)
    {
      throw TypeError("Invalid set! form: " + ast_node->to_string());
    }

    uptr_exec_node_v value_node;
    value_node.push_back(lower_expr(ctx, elements.back()));

    sptr_val_v bind_path = to_rt_value(*elements[1])->elements();

    return std::make_unique<ExecNode>(
      ast_node,
      SpecialFormNode(this, bind_path, std::move(value_node)));
  }
  EXECNODE_BODY(SetBangForm, execnode_set)
  {
    sptr_val_v member_refs = snode.values;

    auto value = exec(ctx, *snode.exec_nodes.front());

    if (member_refs.size() == 1)
    {
      const std::string& identifier = member_refs[0]->str();
      Scope& scope = ctx.get_scope_of(identifier);
      scope.mutate(identifier, value);
    }
    else if (member_refs.size() == 2)
    {
      auto& prop = member_refs[0];
      sptr_val owner = ctx.lookup(member_refs.back()->to_string());

      Lisple::Dict::set_property(owner, prop, value);
    }
    else
    {
      throw Lisple::InvocationException("Incorrect member reference: " +
                                        Value::vector(member_refs)->to_string());
    }

    return value;
  }

  /** QualifierFunction - namespace */
  FUNC_IMPL(QualifierFunction,
            SIG((FN_ARGS((&Type::QUALIFIABLE)),
                 EXEC_DISPATCH(&QualifierFunction::exec_qualifier))))

  EXEC_BODY(QualifierFunction, exec_qualifier)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::NIL;
    const std::string qualifier = args[0]->qual().first;
    return qualifier.empty() ? Constant::NIL : Value::string(qualifier);
  }

  /** StringPFunction - keyword? */
  FUNC_IMPL(StringPFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&StringPFunction::exec_stringp))))

  EXEC_BODY(StringPFunction, exec_stringp)
  {
    return args[0]->type == Value::Type::STRING ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

} // namespace Lisple
