
#include "lisple/runtime/lower.h"
#include "lisple/runtime/value.h"

#include <utility>

#include <lisple/bind.h>
#include <lisple/exception.h>
#include <lisple/lang/bind_form.h>
#include <lisple/runtime/eval_plan.h>
#include <lisple/runtime/exec_node.h>

namespace Lisple
{
  /** LetForm - let */
  SPECIAL_FORM_IMPL(LetForm,
                    SIG((FN_ARGS((&Type::ARRAY, &Eval::REPEAT_LAZY_BIND_SYM_VAL),
                                 (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&LetForm::inv_let, &LetForm::execnode_let))))

  SFORM_LOWER_IMPL(LetForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();
    if (elements[1]->get_type() != Form::ARRAY)
    {
      throw TypeError("let: Invalid bind form: " + elements[1]->to_string());
    }

    sptr_sobject_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;

    for (size_t i = 0; i < bind_forms.size(); i += 2)
    {
      auto sym_node = lower_literal(bind_forms[i]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       lower_expr(ctx, bind_forms[i + 1])));
    }

    uptr_exec_node_v body;
    body.reserve(elements.size() - 2);
    for (size_t i = 2; i < elements.size(); i++)
    {
      body.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(body)));
  }

  /**
   * Legacy AST-based implementation
   */
  MACRO_BODY(LetForm, inv_let)
  {
    deprecated_special_form_invocations++;
    Object& bindings = *args[0];

    if (bindings.get_children().size() % 2 != 0)
    {
      throw LispleException(
        "Wrong number of parameters in binding form of let expression: " +
        bindings.to_string());
    }

    for (size_t i = 0; i < bindings.size(); i += 2)
    {
      Scope var_scope;
      auto binding = ArgumentBinding::create(*bindings.get_children()[i]);
      auto init_expr = ctx.eval_ast(bindings.get_children()[i + 1]);
      binding->apply(var_scope, init_expr);
      ctx.push_context(true, var_scope);
    }

    sptr_sobject result;

    for (size_t i = 1; i < args.size(); i++)
    {
      result = ctx.eval_ast(args[i]);
    }

    for (size_t i = 0; i < bindings.size() / 2; i++)
    {
      ctx.pop_context();
    }

    return result;
  }

  EXECNODE_BODY(LetForm, execnode_let)
  {
    sptr_rtval result;

    for (auto& [b, v] : snode.bind_forms)
    {
      Scope bind_scope;
      b->apply(bind_scope, exec(ctx, *v));
      ctx.push_context(true, bind_scope);
    }

    for (auto& body_node : snode.exec_nodes)
    {
      result = exec(ctx, *body_node);
    }

    for (size_t i = 0; i < snode.bind_forms.size(); i++)
    {
      ctx.pop_context();
    }

    return result;
  }

  /* IfLetForm - if-let */
  SPECIAL_FORM_IMPL(IfLetForm,
                    SIG((FN_ARGS((&Type::ARRAY, &Eval::REPEAT_LAZY_BIND_SYM_VAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&IfLetForm::inv_if_let,
                                       &IfLetForm::execnode_if_let))))

  SFORM_LOWER_IMPL(IfLetForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();
    if (elements[1]->get_type() != Form::ARRAY)
    {
      throw TypeError("if-let: Invalid bind form: " + elements[1]->to_string());
    }
    if (elements.size() < 3 || elements.size() > 4)
    {
      throw LispleException("if-let: Invalid number of branches: " +
                            std::to_string(elements.size() - 2));
    }

    sptr_sobject_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;

    for (size_t i = 0; i < bind_forms.size(); i += 2)
    {
      auto sym_node = lower_literal(bind_forms[i]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       lower_expr(ctx, bind_forms[i + 1])));
    }

    uptr_exec_node_v body;
    body.reserve(elements.size() - 2);
    for (size_t i = 2; i < elements.size(); i++)
    {
      body.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(body)));
  }

  FUNC_BODY(IfLetForm, inv_if_let)
  {
    Object& binding_form = *args[0];

    if (binding_form.get_children().size() % 2 != 0)
    {
      throw LispleException(
        "Wrong number of parameters in binding form of if-let expression: " +
        binding_form.to_string());
    }

    bool contains_non_truthy = false;

    size_t scopes = 0;
    for (size_t i = 0; i < binding_form.size(); i += 2)
    {
      auto& var_name_obj = *binding_form.get_children()[i];
      auto var_val_obj = ctx.eval_ast(binding_form.get_children()[i + 1]);

      if (!var_val_obj->is_truthy())
      {
        contains_non_truthy = true;
        break;
      }

      if (!Type::WORD.is_type_of(var_name_obj))
      {
        throw TypeError(
          "Invalid variable identifier in binding form of if-let expression: " +
          var_name_obj.to_string() + " in " + binding_form.to_string());
      }

      Scope var_scope;
      var_scope.store(var_name_obj.as<Word>(), var_val_obj);
      ctx.push_context(true, var_scope);
      scopes++;
    }

    sptr_sobject result = NIL;

    if (!contains_non_truthy)
    {
      result = ctx.eval_ast(args[1]);
    }
    else
    {
      result = ctx.eval_ast(args[2]);
    }
    for (size_t i = 0; i < scopes; i++)
    {
      ctx.pop_context();
    }

    return result;
  }

  EXECNODE_BODY(IfLetForm, execnode_if_let)
  {
    sptr_rtval result = Constant::NIL;

    bool truthy = true;
    Scope bind_scope;
    ctx.push_context(true, bind_scope);
    for (auto& [b, v] : snode.bind_forms)
    {
      sptr_rtval val = exec(ctx, *v);
      if (!Lisple::is_truthy(*val))
      {
        truthy = false;
        ctx.pop_context();
        break;
      }
      b->apply(ctx.current_scope(), val);
    }

    if (truthy)
    {
      result = exec(ctx, *snode.exec_nodes[0]);
      ctx.pop_context();
    }
    else if (snode.exec_nodes.size() == 2)
    {
      result = exec(ctx, *snode.exec_nodes[1]);
    }

    return result;
  }

  /** WhenLetForm - when-let */
  SPECIAL_FORM_IMPL(WhenLetForm,
                    SIG((FN_ARGS((&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&WhenLetForm::inv_when_let,
                                       &WhenLetForm::execnode_when_let))))

  SFORM_LOWER_IMPL(WhenLetForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();
    if (elements[1]->get_type() != Form::ARRAY)
    {
      throw TypeError("when-let: Invalid bind form: " + elements[1]->to_string());
    }
    if (elements.size() < 2 || elements.size() > 3)
    {
      throw LispleException("when-let: Invalid form: " +
                            std::to_string(elements.size() - 2));
    }

    sptr_sobject_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;

    for (size_t i = 0; i < bind_forms.size(); i += 2)
    {
      auto sym_node = lower_literal(bind_forms[i]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       lower_expr(ctx, bind_forms[i + 1])));
    }

    uptr_exec_node_v body;
    body.reserve(elements.size() - 2);
    for (size_t i = 2; i < elements.size(); i++)
    {
      body.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(body)));
  }

  MACRO_BODY(WhenLetForm, inv_when_let)
  {
    deprecated_special_form_invocations++;
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
      sptr_sobject var_expr = var_def_array.get_children()[i + 1];
      auto var_val_obj = RuntimeValueWrapper::make(ctx.eval(var_expr));

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
        result = ctx.eval_ast(args[i]);
      }
    }

    for (size_t i = 0; i < scopes; i++)
    {
      ctx.pop_context();
    }

    return result;
  }

  EXECNODE_BODY(WhenLetForm, execnode_when_let)
  {
    sptr_rtval result = Constant::NIL;

    bool truthy = true;
    Scope bind_scope;
    ctx.push_context(true, bind_scope);
    for (auto& [b, v] : snode.bind_forms)
    {
      sptr_rtval val = exec(ctx, *v);
      if (!Lisple::is_truthy(*val))
      {
        truthy = false;
        ctx.pop_context();
        break;
      }
      b->apply(ctx.current_scope(), val);
    }

    if (truthy)
    {
      for (auto& node : snode.exec_nodes)
      {
        result = exec(ctx, *node);
      }
      ctx.pop_context();
    }

    return result;
  }

} // namespace Lisple
