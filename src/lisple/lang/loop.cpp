

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
                    SIG((FN_ARGS((&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&DoTimesForm::inv_dotimes,
                                       &DoTimesForm::execnode_dotimes))))

  SFORM_LOWER_IMPL(DoTimesForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw LispleException("dotimes: No bind form - " + ast_node->to_string());
    }

    if (elements[1]->get_type() != Form::ARRAY || elements[1]->size() < 1 ||
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

  /**
   * Legacy AST-based implementation
   */
  MACRO_BODY(DoTimesForm, inv_dotimes)
  {
    size_t n_args = args.size();
    sptr_sobject_v result;

    Array& seq_expr = args[0]->as<Lisple::Array>();
    if (seq_expr.size() < 1 || seq_expr.size() > 2)
    {
      throw LispleException("Invalid binding form: " + seq_expr.to_string());
    }

    std::unique_ptr<ArgumentBinding> bind_var = nullptr;

    sptr_sobject num_iter = ctx.eval_ast(seq_expr.children.back());
    if (num_iter->get_type() == Form::NUMBER)
    {
      int iterations = num_iter->as<Number>().int_value();

      if (seq_expr.size() == 2)
      {
        bind_var = ArgumentBinding::create(*seq_expr.get_children()[0]);
      }

      if (iterations > 0)
      {
        result.reserve(iterations);

        ctx.push_context(true);
        Scope& iter_scope = ctx.current_scope();
        for (int i = 0; i < iterations; i++)
        {
          sptr_sobject si = Number::make(i);
          if (bind_var) bind_var->apply(iter_scope, si);
          sptr_sobject iter_result;
          for (size_t i = 1; i < n_args; i++)
          {
            iter_result = ctx.eval_ast(args[i]);
          }
          result.push_back(std::move(iter_result));
          iter_scope.clear();
        }
        ctx.pop_context();
      }
    }
    return std::make_shared<Array>(std::move(result));
  }

  EXECNODE_BODY(DoTimesForm, execnode_dotimes)
  {
    sptr_rtval_v result;

    auto snode = std::get<SpecialFormNode>(args[0]->data);

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

    return RTValue::vector(result);
  }

  /* ForForm - for */
  SPECIAL_FORM_IMPL(ForForm,
                    SIG((FN_ARGS((&Type::SEQ, &Eval::BIND_SYM_VAL),
                                 (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ForForm::inv_for, &ForForm::execnode_for))))

  SFORM_LOWER_IMPL(ForForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw LispleException("for: No loop expression - " + ast_node->to_string());
    }

    if (elements[1]->get_type() != Form::ARRAY || elements[1]->size() != 2)
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

  MACRO_BODY(ForForm, inv_for)
  {
    size_t n_args = args.size();
    sptr_sobject_v result;
    sptr_sobject_v& seq_expr = args[0]->get_children();

    sptr_sobject obj_iterable = ctx.eval_ast(seq_expr.back());
    if (*Lisple::NIL != *obj_iterable)
    {
      if (!Type::SEQ.is_type_of(*obj_iterable) && !Type::STRING.is_type_of(*obj_iterable))
      {
        throw TypeError("For macro requires an iterable. Wrong type: " +
                        obj_iterable->to_string());
      }

      auto seq_binding = ArgumentBinding::create(*seq_expr[0]);

      result.reserve(obj_iterable->size());
      auto& iter_elements = obj_iterable->get_children();

      ctx.push_context(true);
      Scope& iter_scope = ctx.current_scope();
      for (auto it = iter_elements.begin(); it != iter_elements.end(); ++it)
      {
        seq_binding->apply(iter_scope, *it);
        sptr_sobject iter_result;
        for (size_t i = 1; i < n_args; i++)
        {
          iter_result = ctx.eval_ast(args[i]);
        }
        result.push_back(std::move(iter_result));
        iter_scope.clear();
      }
      ctx.pop_context();
    }
    return std::make_shared<Array>(std::move(result));
  }

  EXECNODE_BODY(ForForm, execnode_for)
  {
    sptr_rtval_v result;

    auto snode = std::get<SpecialFormNode>(args[0]->data);

    auto* binding = snode.bind_forms.front().first.get();

    sptr_rtval seq = exec(ctx, *snode.exec_nodes[0]);

    if (seq->type != RTValue::Type::NIL && Type::SEQ.is_type_of(*seq))
    {
      const sptr_rtval_v& elements = seq->elements();
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

} // namespace Lisple
