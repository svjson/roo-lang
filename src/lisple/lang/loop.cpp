

#include "lisple/exec.h"
#include "lisple/runtime/value.h"
#include "lisple/type.h"

#include <iostream>

#include <lisple/bind.h>
#include <lisple/exception.h>
#include <lisple/lang/loop.h>
#include <lisple/runtime/eval_plan.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>
#include <lisple/runtime/seq.h>

namespace Lisple
{
  /** DoTimes - dotimes */
  SPECIAL_FORM_IMPL(DoTimesForm,
                    SIG((FN_ARGS((&Type::VECTOR, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&DoTimesForm::execnode_dotimes))))

  SFORM_LOWER_IMPL(DoTimesForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw LispleException("dotimes: No bind form - " + ast_node->to_string());
    }

    if (elements[1]->get_type() != Form::VECTOR || elements[1]->size() < 1 ||
        elements[1]->size() > 2)
    {
      throw LispleException("dotimes: Invalid bind form - " + ast_node->to_string());
    }

    sptr_sobject_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;

    if (bind_forms.size() == 2)
    {
      auto sym_node = lower_literal(bind_forms[0]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       std::make_unique<ExecNode>(Constant::NIL)));
    }

    uptr_exec_node_v exec_nodes;
    exec_nodes.push_back(lower_expr(ctx, bind_forms.back()));

    for (size_t i = 2; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(exec_nodes)));
  }

  EXECNODE_BODY(DoTimesForm, execnode_dotimes)
  {
    sptr_rtval_v result;

    auto* binding =
      snode.bind_forms.empty() ? nullptr : snode.bind_forms.front().first.get();

    sptr_rtval iterations_val = exec(ctx, *snode.exec_nodes[0]);

    if (iterations_val->type == RTValue::Type::NUMBER)
    {
      long iterations = std::get<const RTValue::Number>(iterations_val->value).get_long();
      if (iterations > 0)
      {
        result.reserve(iterations);

        ctx.push_context(true);
        Scope& iter_scope = ctx.current_scope();
        size_t n_args = snode.exec_nodes.size();

        for (long i = 0; i < iterations; i++)
        {
          if (binding)
          {
            binding->apply(iter_scope, RTValue::number(i));
          }

          sptr_rtval iter_result;

          for (size_t j = 1; j < n_args; j++)
          {
            iter_result = exec(ctx, *snode.exec_nodes[j]);
          }

          result.push_back(iter_result);
          iter_scope.clear();
        }

        ctx.pop_context();
      }
    }

    return RTValue::vector(std::move(result));
  }

  /* ForForm - for */
  SPECIAL_FORM_IMPL(ForForm,
                    SIG((FN_ARGS((&Type::SEQ, &Eval::BIND_SYM_VAL),
                                 (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ForForm::execnode_for))))

  SFORM_LOWER_IMPL(ForForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw LispleException("for: No loop expression - " + ast_node->to_string());
    }

    if (elements[1]->get_type() != Form::VECTOR || elements[1]->size() != 2)
    {
      throw LispleException("for: Invalid loop expression - " + ast_node->to_string());
    }

    sptr_sobject_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;

    auto sym_node = lower_literal(bind_forms[0]);
    bindings.push_back(
      std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                     std::make_unique<ExecNode>(Constant::NIL)));

    uptr_exec_node_v exec_nodes;
    exec_nodes.push_back(lower_expr(ctx, bind_forms.back()));

    for (size_t i = 2; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(exec_nodes)));
  }
  EXECNODE_BODY(ForForm, execnode_for)
  {
    sptr_rtval_v result;

    auto* binding = snode.bind_forms.front().first.get();

    sptr_rtval seq = exec(ctx, *snode.exec_nodes[0]);

    if (seq->type != RTValue::Type::NIL &&
        (Type::STRICT_SEQ_OR_STRING.is_type_of(*seq) ||
         seq->type == RTValue::Type::NATIVE_OBJECT))
    {
      sptr_rtval_v elements = Lisple::get_children(*seq);
      if (elements.size() > 0)
      {
        result.reserve(elements.size());

        ctx.push_context(true);
        Scope& iter_scope = ctx.current_scope();
        size_t n_args = snode.exec_nodes.size();

        for (auto& item : elements)
        {
          binding->apply(iter_scope, item);

          sptr_rtval iter_result;

          for (size_t j = 1; j < n_args; j++)
          {
            iter_result = exec(ctx, *snode.exec_nodes[j]);
          }

          result.push_back(iter_result);
          iter_scope.clear();
        }

        ctx.pop_context();
      }
    }

    return RTValue::vector(std::move(result));
  }

  /** ForIndexedForm - for-indexed */
  SPECIAL_FORM_IMPL(ForIndexedForm,
                    SIG((FN_ARGS((&Type::VECTOR, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ForIndexedForm::execnode_for_indexed))))

  SFORM_LOWER_IMPL(ForIndexedForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw LispleException("for-indexed: No loop expression - " + ast_node->to_string());
    }

    if (elements[1]->get_type() != Form::VECTOR || elements[1]->size() != 3)
    {
      throw LispleException("for-indexed: Invalid loop expression - " +
                            ast_node->to_string());
    }

    sptr_sobject_v& bind_form = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;

    auto index_node = lower_literal(bind_form[0]);
    bindings.push_back(
      std::make_pair(LexicalBinding::create(std::get<LiteralNode>(index_node->data)),
                     std::make_unique<ExecNode>(Constant::NIL)));

    auto sym_node = lower_literal(bind_form[1]);
    bindings.push_back(
      std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                     std::make_unique<ExecNode>(Constant::NIL)));

    uptr_exec_node_v exec_nodes;
    exec_nodes.push_back(lower_expr(ctx, bind_form.back()));

    for (size_t i = 2; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(exec_nodes)));
  }
  EXECNODE_BODY(ForIndexedForm, execnode_for_indexed)
  {
    sptr_rtval_v result;

    auto* index_binding = snode.bind_forms.front().first.get();
    auto* val_binding = snode.bind_forms.back().first.get();

    sptr_rtval seq = exec(ctx, *snode.exec_nodes[0]);

    if (seq->type != RTValue::Type::NIL && (Type::STRICT_SEQ.is_type_of(*seq)))
    {
      sptr_rtval_v elements = Lisple::get_children(*seq);
      if (elements.size() > 0)
      {
        result.reserve(elements.size());

        ctx.push_context(true);
        Scope& iter_scope = ctx.current_scope();
        size_t n_args = snode.exec_nodes.size();

        // FIXME: This should be size_t, but until such a time that RTValue::Number
        // natively supports unsigned numbers, we will have to stick to int or
        // redundantly reinterpret size_t to int for each iteration, which is not
        // very attractive.
        int index = 0;
        for (auto& item : elements)
        {
          index_binding->apply(iter_scope, RTValue::number(index));
          val_binding->apply(iter_scope, item);

          sptr_rtval iter_result;

          for (size_t j = 1; j < n_args; j++)
          {
            iter_result = exec(ctx, *snode.exec_nodes[j]);
          }

          result.push_back(iter_result);
          iter_scope.clear();
          index++;
        }

        ctx.pop_context();
      }
    }

    return RTValue::vector(std::move(result));
  }

  /** WhileForm - while */
  SPECIAL_FORM_IMPL(WhileForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&WhileForm::execnode_while))))

  SFORM_LOWER_IMPL(WhileForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw LispleException("while: No loop condition expression - " +
                            ast_node->to_string());
    }

    uptr_exec_node_v exec_nodes;
    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_rtval_v{}, std::move(exec_nodes)));
  }
  EXECNODE_BODY(WhileForm, execnode_while)
  {
    sptr_rtval result = Constant::NIL;
    while (Lisple::is_truthy(*exec(ctx, *snode.exec_nodes[0])))
    {
      for (size_t i = 1; i < snode.exec_nodes.size(); i++)
      {
        result = exec(ctx, *snode.exec_nodes[i]);
      }
    }

    return result;
  }

} // namespace Lisple
