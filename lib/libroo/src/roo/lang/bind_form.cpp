
#include "roo/runtime/lower.h"
#include "roo/runtime/value.h"

#include <utility>

#include <roo/bind.h>
#include <roo/exception.h>
#include <roo/lang/bind_form.h>
#include <roo/runtime/eval_plan.h>
#include <roo/runtime/exec_node.h>

namespace Roo
{
  /** LetForm - roo/let */
  SPECIAL_FORM_IMPL(LetForm,
                    SIG((FN_ARGS((&Type::VECTOR, &Eval::REPEAT_LAZY_BIND_SYM_VAL),
                                 (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&LetForm::execnode_let))))

  SFORM_LOWER_IMPL(LetForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements[1]->get_type() != Form::VECTOR)
    {
      throw TypeError("let: Invalid bind form: " + elements[1]->to_string());
    }

    sptr_ast_node_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    bindings.reserve(bind_forms.size() / 2);

    ctx.push({});
    for (size_t i = 0; i < bind_forms.size(); i += 2)
    {
      auto sym_node = lower_literal(bind_forms[i]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       lower_expr(ctx, bind_forms[i + 1])));
      ctx.add_lexical_binding(*bindings.back().first);
    }

    uptr_exec_node_v body;
    body.reserve(elements.size() - 2);
    for (size_t i = 2; i < elements.size(); i++)
    {
      body.push_back(lower_expr(ctx, elements[i]));
    }
    ctx.pop();

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(body)));
  }

  EXECNODE_BODY(LetForm, execnode_let)
  {
    sptr_val result;

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

  /** IfLetForm - roo/if-let */
  SPECIAL_FORM_IMPL(IfLetForm,
                    SIG((FN_ARGS((&Type::VECTOR, &Eval::REPEAT_LAZY_BIND_SYM_VAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&IfLetForm::execnode_if_let))))

  SFORM_LOWER_IMPL(IfLetForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements[1]->get_type() != Form::VECTOR)
    {
      throw TypeError("if-let: Invalid bind form: " + elements[1]->to_string());
    }
    if (elements.size() < 3 || elements.size() > 4)
    {
      throw RooException("if-let: Invalid number of branches: " +
                         std::to_string(elements.size() - 2));
    }

    sptr_ast_node_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    bindings.reserve(bind_forms.size() / 2);

    ctx.push({});
    for (size_t i = 0; i < bind_forms.size(); i += 2)
    {
      auto sym_node = lower_literal(bind_forms[i]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       lower_expr(ctx, bind_forms[i + 1])));
      ctx.add_lexical_binding(*bindings.back().first);
    }

    uptr_exec_node_v body;
    body.reserve(elements.size() - 2);
    for (size_t i = 2; i < elements.size(); i++)
    {
      body.push_back(lower_expr(ctx, elements[i]));
    }
    ctx.pop();

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(body)));
  }
  EXECNODE_BODY(IfLetForm, execnode_if_let)
  {
    sptr_val result = Constant::NIL;

    bool truthy = true;
    Scope bind_scope;
    ctx.push_context(true, bind_scope);
    for (auto& [b, v] : snode.bind_forms)
    {
      sptr_val val = exec(ctx, *v);
      if (!Roo::is_truthy(*val))
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

  /** WhenLetForm - roo/when-let */
  SPECIAL_FORM_IMPL(WhenLetForm,
                    SIG((FN_ARGS((&Type::VECTOR, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&WhenLetForm::execnode_when_let))))

  SFORM_LOWER_IMPL(WhenLetForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements[1]->get_type() != Form::VECTOR)
    {
      throw TypeError("when-let: Invalid bind form: " + elements[1]->to_string());
    }
    if (elements.size() < 2)
    {
      throw RooException("when-let: Invalid form: " + std::to_string(elements.size() - 2));
    }

    sptr_ast_node_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    bindings.reserve(bind_forms.size() / 2);

    ctx.push({});
    for (size_t i = 0; i < bind_forms.size(); i += 2)
    {
      auto sym_node = lower_literal(bind_forms[i]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       lower_expr(ctx, bind_forms[i + 1])));
      ctx.add_lexical_binding(*bindings.back().first);
    }

    uptr_exec_node_v body;
    body.reserve(elements.size() - 2);
    for (size_t i = 2; i < elements.size(); i++)
    {
      body.push_back(lower_expr(ctx, elements[i]));
    }
    ctx.pop();

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(body)));
  }
  EXECNODE_BODY(WhenLetForm, execnode_when_let)
  {
    sptr_val result = Constant::NIL;

    bool truthy = true;
    Scope bind_scope;
    ctx.push_context(true, bind_scope);
    for (auto& [b, v] : snode.bind_forms)
    {
      sptr_val val = exec(ctx, *v);
      if (!Roo::is_truthy(*val))
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

} // namespace Roo
