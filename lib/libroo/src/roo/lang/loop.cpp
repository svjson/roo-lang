

#include "roo/exec.h"
#include "roo/runtime/value.h"
#include "roo/type.h"

#include <limits>

#include <roo/bind.h>
#include <roo/exception.h>
#include <roo/lang/loop.h>
#include <roo/runtime/eval_plan.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/lower.h>
#include <roo/runtime/seq.h>

namespace Roo
{
  /** DoTimesForm - roo/dotimes */
  SPECIAL_FORM_IMPL(DoTimesForm,
                    SIG((FN_ARGS((&Type::VECTOR, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&DoTimesForm::execnode_dotimes))))

  SFORM_LOWER_IMPL(DoTimesForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw RooException("dotimes: No bind form - " + ast_node->to_string());
    }

    if (elements[1]->get_type() != Form::VECTOR || elements[1]->size() < 1 ||
        elements[1]->size() > 2)
    {
      throw RooException("dotimes: Invalid bind form - " + ast_node->to_string());
    }

    sptr_ast_node_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    bindings.reserve(bind_forms.size() == 2 ? 1 : 0);

    if (bind_forms.size() == 2)
    {
      auto sym_node = lower_literal(bind_forms[0]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       std::make_unique<ExecNode>(Constant::NIL)));
    }

    uptr_exec_node_v exec_nodes;
    exec_nodes.reserve(elements.size() - 1);
    exec_nodes.push_back(lower_expr(ctx, bind_forms.back()));

    ctx.push({});
    for (auto& binding : bindings)
    {
      ctx.add_lexical_binding(*binding.first);
    }
    for (size_t i = 2; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }
    ctx.pop();

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(exec_nodes)));
  }

  EXECNODE_BODY(DoTimesForm, execnode_dotimes)
  {
    sptr_val_v result;

    auto* binding =
      snode.bind_forms.empty() ? nullptr : snode.bind_forms.front().first.get();

    sptr_val iterations_val = exec(ctx, *snode.exec_nodes[0]);

    if (iterations_val->type == Value::Type::NUMBER)
    {
      long iterations = std::get<const Value::Number>(iterations_val->value).get_long();
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
            binding->apply(iter_scope, Value::number(i));
          }

          sptr_val iter_result;

          for (size_t j = 1; j < n_args; j++)
          {
            iter_result = exec(ctx, *snode.exec_nodes[j]);
          }

          result.push_back(iter_result ? iter_result : Constant::NIL);
          iter_scope.clear();
        }

        ctx.pop_context();
      }
    }

    return Value::vector(std::move(result));
  }

  /** ForForm - roo/for */
  SPECIAL_FORM_IMPL(ForForm,
                    SIG((FN_ARGS((&Type::SEQ, &Eval::BIND_SYM_VAL),
                                 (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ForForm::execnode_for))))

  SFORM_LOWER_IMPL(ForForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw RooException("for: No loop expression - " + ast_node->to_string());
    }

    sptr_ast_node_v& bind_elems = elements[1]->get_children();
    const size_t N = bind_elems.size() / 2;

    if (elements[1]->get_type() != Form::VECTOR || bind_elems.size() < 2 ||
        bind_elems.size() % 2 != 0)
    {
      throw RooException("for: Invalid loop expression - expected [sym coll ...] pairs - " +
                         ast_node->to_string());
    }

    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    bindings.reserve(N);

    uptr_exec_node_v exec_nodes;
    exec_nodes.reserve(N + elements.size() - 2);

    for (size_t i = 0; i < bind_elems.size(); i += 2)
    {
      auto sym_node = lower_literal(bind_elems[i]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       std::make_unique<ExecNode>(Constant::NIL)));
      exec_nodes.push_back(lower_expr(ctx, bind_elems[i + 1]));
    }

    ctx.push({});
    for (auto& [binding, _] : bindings)
    {
      ctx.add_lexical_binding(*binding);
    }
    for (size_t i = 2; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }
    ctx.pop();

    sptr_val_v values{Value::number(static_cast<int>(N))};
    return std::make_unique<ExecNode>(
      SpecialFormNode(this, values, std::move(bindings), std::move(exec_nodes)));
  }

  EXECNODE_BODY(ForForm, execnode_for)
  {
    const size_t N = static_cast<size_t>(snode.values[0]->i64());

    std::vector<sptr_val_v> all_elements;
    all_elements.reserve(N);
    size_t min_len = std::numeric_limits<size_t>::max();

    for (size_t i = 0; i < N; i++)
    {
      sptr_val seq = exec(ctx, *snode.exec_nodes[i]);
      if (seq->type == Value::Type::NIL || !(Type::STRICT_SEQ_OR_STRING.is_type_of(*seq) ||
                                             seq->type == Value::Type::NATIVE_OBJECT))
      {
        min_len = 0;
        all_elements.push_back({});
      }
      else
      {
        sptr_val_v elems = Roo::get_children(*seq);
        min_len = std::min(min_len, elems.size());
        all_elements.push_back(std::move(elems));
      }
    }

    if (min_len == std::numeric_limits<size_t>::max())
    {
      min_len = 0;
    }

    sptr_val_v result;
    if (min_len > 0)
    {
      result.reserve(min_len);
      ctx.push_context(true);
      Scope& iter_scope = ctx.current_scope();
      const size_t n_exec = snode.exec_nodes.size();

      for (size_t i = 0; i < min_len; i++)
      {
        for (size_t j = 0; j < N; j++)
        {
          snode.bind_forms[j].first->apply(iter_scope, all_elements[j][i]);
        }

        sptr_val iter_result;
        for (size_t j = N; j < n_exec; j++)
        {
          iter_result = exec(ctx, *snode.exec_nodes[j]);
        }

        result.push_back(iter_result ? iter_result : Constant::NIL);
        iter_scope.clear();
      }

      ctx.pop_context();
    }

    return Value::vector(std::move(result));
  }

  /** DoseqForm - roo/doseq */
  SPECIAL_FORM_IMPL(DoseqForm,
                    SIG((FN_ARGS((&Type::SEQ, &Eval::BIND_SYM_VAL),
                                 (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&DoseqForm::execnode_doseq))))

  SFORM_LOWER_IMPL(DoseqForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw RooException("doseq: No loop expression - " + ast_node->to_string());
    }

    if (elements[1]->get_type() != Form::VECTOR || elements[1]->size() != 2)
    {
      throw RooException("doseq: Invalid loop expression - " + ast_node->to_string());
    }

    sptr_ast_node_v& bind_forms = elements[1]->get_children();
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    bindings.reserve(1);

    auto sym_node = lower_literal(bind_forms[0]);
    bindings.push_back(
      std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                     std::make_unique<ExecNode>(Constant::NIL)));

    uptr_exec_node_v exec_nodes;
    exec_nodes.reserve(elements.size() - 1);
    exec_nodes.push_back(lower_expr(ctx, bind_forms.back()));

    ctx.push({});
    for (auto& binding : bindings)
    {
      ctx.add_lexical_binding(*binding.first);
    }
    for (size_t i = 2; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }
    ctx.pop();

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, std::move(bindings), std::move(exec_nodes)));
  }

  EXECNODE_BODY(DoseqForm, execnode_doseq)
  {
    auto* binding = snode.bind_forms.front().first.get();

    sptr_val seq = exec(ctx, *snode.exec_nodes[0]);

    if (seq->type != Value::Type::NIL && (Type::STRICT_SEQ_OR_STRING.is_type_of(*seq) ||
                                          seq->type == Value::Type::NATIVE_OBJECT))
    {
      sptr_val_v elements = Roo::get_children(*seq);
      if (elements.size() > 0)
      {
        ctx.push_context(true);
        Scope& iter_scope = ctx.current_scope();
        size_t n_args = snode.exec_nodes.size();

        for (auto& item : elements)
        {
          binding->apply(iter_scope, item);

          for (size_t j = 1; j < n_args; j++)
          {
            exec(ctx, *snode.exec_nodes[j]);
          }

          iter_scope.clear();
        }

        ctx.pop_context();
      }
    }

    return Constant::NIL;
  }

  /** ForIndexedForm - roo/for-indexed */
  SPECIAL_FORM_IMPL(ForIndexedForm,
                    SIG((FN_ARGS((&Type::VECTOR, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ForIndexedForm::execnode_for_indexed))))

  SFORM_LOWER_IMPL(ForIndexedForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw RooException("for-indexed: No loop expression - " + ast_node->to_string());
    }

    sptr_ast_node_v& bind_elems = elements[1]->get_children();
    const size_t pair_count = bind_elems.size() > 0 ? (bind_elems.size() - 1) / 2 : 0;

    if (elements[1]->get_type() != Form::VECTOR || bind_elems.size() < 3 ||
        (bind_elems.size() - 1) % 2 != 0)
    {
      throw RooException(
        "for-indexed: Invalid loop expression - expected [idx sym coll ...] - " +
        ast_node->to_string());
    }

    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    bindings.reserve(1 + pair_count);

    auto index_node = lower_literal(bind_elems[0]);
    bindings.push_back(
      std::make_pair(LexicalBinding::create(std::get<LiteralNode>(index_node->data)),
                     std::make_unique<ExecNode>(Constant::NIL)));

    uptr_exec_node_v exec_nodes;
    exec_nodes.reserve(pair_count + elements.size() - 2);

    for (size_t i = 1; i < bind_elems.size(); i += 2)
    {
      auto sym_node = lower_literal(bind_elems[i]);
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(sym_node->data)),
                       std::make_unique<ExecNode>(Constant::NIL)));
      exec_nodes.push_back(lower_expr(ctx, bind_elems[i + 1]));
    }

    ctx.push({});
    for (auto& [binding, _] : bindings)
    {
      ctx.add_lexical_binding(*binding);
    }
    for (size_t i = 2; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }
    ctx.pop();

    sptr_val_v values{Value::number(static_cast<int>(pair_count))};
    return std::make_unique<ExecNode>(
      SpecialFormNode(this, values, std::move(bindings), std::move(exec_nodes)));
  }

  EXECNODE_BODY(ForIndexedForm, execnode_for_indexed)
  {
    const size_t N = static_cast<size_t>(snode.values[0]->i64());

    std::vector<sptr_val_v> all_elements;
    all_elements.reserve(N);
    size_t min_len = std::numeric_limits<size_t>::max();

    for (size_t i = 0; i < N; i++)
    {
      sptr_val seq = exec(ctx, *snode.exec_nodes[i]);
      if (seq->type == Value::Type::NIL || !Type::STRICT_SEQ.is_type_of(*seq))
      {
        min_len = 0;
        all_elements.push_back({});
      }
      else
      {
        sptr_val_v elems = Roo::get_children(*seq);
        min_len = std::min(min_len, elems.size());
        all_elements.push_back(std::move(elems));
      }
    }

    if (min_len == std::numeric_limits<size_t>::max())
    {
      min_len = 0;
    }

    sptr_val_v result;
    if (min_len > 0)
    {
      result.reserve(min_len);
      ctx.push_context(true);
      Scope& iter_scope = ctx.current_scope();
      auto* index_binding = snode.bind_forms[0].first.get();
      const size_t n_exec = snode.exec_nodes.size();

      // FIXME: This should be size_t, but until such a time that Value::Number
      // natively supports unsigned numbers, we will have to stick to int or
      // redundantly reinterpret size_t to int for each iteration, which is not
      // very attractive.
      int index = 0;
      for (size_t i = 0; i < min_len; i++)
      {
        index_binding->apply(iter_scope, Value::number(index));
        for (size_t j = 0; j < N; j++)
        {
          snode.bind_forms[1 + j].first->apply(iter_scope, all_elements[j][i]);
        }

        sptr_val iter_result;
        for (size_t j = N; j < n_exec; j++)
        {
          iter_result = exec(ctx, *snode.exec_nodes[j]);
        }

        result.push_back(iter_result ? iter_result : Constant::NIL);
        iter_scope.clear();
        index++;
      }

      ctx.pop_context();
    }

    return Value::vector(std::move(result));
  }

  /** WhileForm - roo/while */
  SPECIAL_FORM_IMPL(WhileForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&WhileForm::execnode_while))))

  SFORM_LOWER_IMPL(WhileForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw RooException("while: No loop condition expression - " + ast_node->to_string());
    }

    uptr_exec_node_v exec_nodes;
    exec_nodes.reserve(elements.size() - 1);
    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_val_v{}, std::move(exec_nodes)));
  }
  EXECNODE_BODY(WhileForm, execnode_while)
  {
    sptr_val result = Constant::NIL;
    while (Roo::is_truthy(*exec(ctx, *snode.exec_nodes[0])))
    {
      for (size_t i = 1; i < snode.exec_nodes.size(); i++)
      {
        result = exec(ctx, *snode.exec_nodes[i]);
      }
    }

    return result;
  }

} // namespace Roo
