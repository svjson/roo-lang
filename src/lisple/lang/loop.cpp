
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

    auto* bnd = std::get_if<LiteralNode>(&args[0]->data);

    if (bnd->value->type == RTValue::Type::VECTOR)
    {
      sptr_rtval_v& bind_forms = std::get<sptr_rtval_v>(bnd->value->value);

      if (bind_forms.size() < 1 || bind_forms.size() > 2)
      {
        throw LispleException("Invalid binding form: " + bnd->ast_node->to_string());
      }

      uptr_exec_node num_iter_node = lower_expr(bnd->ast_node->get_children().back());
      sptr_rtval num_iter_value = exec(ctx, *num_iter_node);

      if (num_iter_value->type == RTValue::Type::NUMBER)
      {
        int iterations = std::get<const RTValue::Number>(num_iter_value->value).get_int();

        if (iterations > 0)
        {
          std::unique_ptr<LexicalBinding> bind_var = nullptr;

          if (bind_forms.size() == 2)
          {
            bind_var =
              std::make_unique<SymbolBinding>(std::get<std::string>(bind_forms[0]->value));
          }

          result.reserve(iterations);

          ctx.push_context(true);
          Scope& iter_scope = ctx.current_scope();
          size_t n_args = args.size();

          for (int i = 0; i < iterations; i++)
          {
            if (bind_var)
            {
              bind_var->apply(iter_scope, RTValue::number(i));
            }

            sptr_rtval iter_result;

            for (size_t j = 1; j < n_args; j++)
            {
              iter_result = exec(ctx, *args[j]);
            }

            result.push_back(iter_result);
            iter_scope.clear();
          }

          ctx.pop_context();
        }
      }
    }

    return RTValue::vector(result);
  }

  /* ForForm - for */
  SPECIAL_FORM_IMPL(ForForm,
                    SIG((FN_ARGS((&Type::SEQ, &Eval::BIND_SYM_VAL),
                                 (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ForForm::inv_for, &ForForm::execnode_for))))

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

    if (ExecNodeList* bnd = std::get_if<ExecNodeList>(&args[0]->data))
    {
      if (bnd->nodes.size() != 2)
      {
        throw LispleException("for: Invalid bind form.");
      }

      auto binding = LexicalBinding::create(std::get<LiteralNode>(bnd->nodes.front()->data));
      sptr_rtval seq = std::get<LiteralNode>(bnd->nodes.back()->data).value;

      sptr_rtval_v elements = Lisple::get_children(*seq);

      sptr_rtval iter_result;
      for (auto& item : elements)
      {
        ctx.push_context(true);
        Scope& iter_scope = ctx.current_scope();
        binding->apply(iter_scope, item);

        for (size_t bi = 1; bi < args.size(); bi++)
        {
          iter_result = exec(ctx, *args[bi]);
        }

        result.push_back(iter_result);
        ctx.pop_context();
      }
    }

    return RTValue::vector(std::move(result));
  }

} // namespace Lisple
