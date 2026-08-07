
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

    return result ? result : Roo::Constant::NIL;
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

  /** CondLetForm - roo/cond-let */
  SPECIAL_FORM_IMPL(CondLetForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&CondLetForm::execnode_cond_let))))

  SFORM_LOWER_IMPL(CondLetForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    if (elements.size() % 2 != 1)
    {
      throw RooException("cond-let: Unmatched test-body pairs: " + ast_node->to_string());
    }

    uptr_exec_node_v exec_nodes;
    exec_nodes.reserve(elements.size() - 1);

    sptr_val_v values;
    values.reserve((elements.size() - 1) / 2);

    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bind_forms;

    for (size_t i = 1; i < elements.size(); i += 2)
    {
      sptr_ast_node& test_form = elements[i];
      sptr_ast_node& body_form = elements[i + 1];

      if (test_form->get_type() == Form::VECTOR && test_form->size() == 2)
      {
        sptr_ast_node_v& vec_elems = test_form->get_children();

        exec_nodes.push_back(lower_expr(ctx, vec_elems[1]));

        auto sym_node = lower_literal(vec_elems[0]);
        auto binding  = LexicalBinding::create(std::get<LiteralNode>(sym_node->data));

        ctx.push({});
        ctx.add_lexical_binding(*binding);
        exec_nodes.push_back(lower_expr(ctx, body_form));
        ctx.pop();

        bind_forms.push_back(
          std::make_pair(std::move(binding), std::make_unique<ExecNode>(Constant::NIL)));
        values.push_back(Value::number(1));
      }
      else
      {
        exec_nodes.push_back(lower_expr(ctx, test_form));
        exec_nodes.push_back(lower_expr(ctx, body_form));
        values.push_back(Value::number(0));
      }
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, values, std::move(bind_forms), std::move(exec_nodes)));
  }

  EXECNODE_BODY(CondLetForm, execnode_cond_let)
  {
    const size_t n_clauses = snode.values.size();
    size_t binding_idx     = 0;

    for (size_t i = 0; i < n_clauses; i++)
    {
      const bool is_binding = snode.values[i]->i64() == 1;
      sptr_val test_val     = exec(ctx, *snode.exec_nodes[2 * i]);

      if (Roo::is_truthy(*test_val))
      {
        if (is_binding)
        {
          Scope bind_scope;
          ctx.push_context(true, bind_scope);
          snode.bind_forms[binding_idx].first->apply(ctx.current_scope(), test_val);
          sptr_val result = exec(ctx, *snode.exec_nodes[2 * i + 1]);
          ctx.pop_context();
          return result;
        }
        return exec(ctx, *snode.exec_nodes[2 * i + 1]);
      }

      if (is_binding)
      {
        binding_idx++;
      }
    }

    return Constant::NIL;
  }

} // namespace Roo
