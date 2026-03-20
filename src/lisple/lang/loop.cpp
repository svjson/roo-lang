
#include "loop.h"

#include "../bind.h"
#include "../exception.h"
#include "../runtime/lower.h"

namespace Lisple
{
  /** DoTimes - dotimes */
  SPECIAL_FORM_IMPL(DoTimesForm,
                    SIG((FN_ARGS((&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&DoTimesForm::inv_dotimes,
                                       &DoTimesForm::exec_dotimes))))

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

    sptr_sobject num_iter = ctx.eval(seq_expr.children.back());
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
            iter_result = ctx.eval(args[i]);
          }
          result.push_back(std::move(iter_result));
          iter_scope.clear();
        }
        ctx.pop_context();
      }
    }
    return std::make_shared<Array>(std::move(result));
  }

  EXEC_BODY(DoTimesForm, exec_dotimes)
  {
    sptr_sobject_v result;

    auto* bnd = std::get_if<LiteralNode>(&args[0]->data);

    if (bnd->value->type == RTValue::Type::VECTOR)
    {
      sptr_rtval_v& bind_forms = std::get<sptr_rtval_v>(bnd->value->value);

      if (bind_forms.size() < 1 || bind_forms.size() > 2)
      {
        throw LispleException("Invalid binding form: " + bnd->ast_node->to_string());
      }

      uptr_exec_node num_iter_node = lower_expr(bnd->ast_node->get_children().back());
      auto num_iter_evalled = lower_literal(exec(ctx, *num_iter_node));
      sptr_rtval& num_iter_value = std::get<LiteralNode>(num_iter_evalled->data).value;

      if (num_iter_value->type == RTValue::Type::NUMBER)
      {
        int iterations = std::get<RTValue::Number>(num_iter_value->value).get_int();

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
            sptr_sobject si = Number::make(i);

            if (bind_var)
            {
              auto inum = Lisple::Number::make(i);
              auto ilit = LiteralNode(RTValue::number(i), inum);
              bind_var->apply(iter_scope, ilit);
            }

            sptr_sobject iter_result;

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

    return Array::make(result);
  }

} // namespace Lisple
