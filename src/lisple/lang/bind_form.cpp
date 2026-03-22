
#include "bind_form.h"

#include "../bind.h"
#include "../exception.h"
#include "../runtime/exec_node.h"

namespace Lisple
{
  /** LetForm - let */
  SPECIAL_FORM_IMPL(LetForm,
                    SIG((FN_ARGS((&Type::ARRAY, &Eval::REPEAT_LAZY_BIND_SYM_VAL),
                                 (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&LetForm::inv_let, &LetForm::exec_let))))
  /**
   * Legacy AST-based implementation
   */
  MACRO_BODY(LetForm, inv_let)
  {
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
      auto init_expr = ctx.eval(bindings.get_children()[i + 1]);
      binding->apply(var_scope, init_expr);
      ctx.push_context(true, var_scope);
    }

    sptr_sobject result;

    for (size_t i = 1; i < args.size(); i++)
    {
      result = ctx.eval(args[i]);
    }

    for (size_t i = 0; i < bindings.size() / 2; i++)
    {
      ctx.pop_context();
    }

    return result;
  }

  EXEC_BODY(LetForm, exec_let)
  {
    sptr_rtval result;

    if (ExecNodeList* bnd = std::get_if<ExecNodeList>(&args[0]->data))
    {
      if (bnd->nodes.size() % 2 != 0)
      {
        throw LispleException(
          "Wrong number of parameters in binding form of let expression: " +
          std::to_string(bnd->nodes.size()));
      }

      uptr_exec_node_v bound_values;
      for (size_t i = 0; i < bnd->nodes.size(); i += 2)
      {
        Scope binding_scope;

        LiteralNode& bind_expr = std::get<LiteralNode>(bnd->nodes[i]->data);

        auto binding = LexicalBinding::create(bind_expr);

        bound_values.push_back(std::make_unique<ExecNode>(
          Lisple::NIL,
          LiteralNode(exec(ctx, *bnd->nodes[i + 1]), Lisple::NIL)));

        if (auto* value = std::get_if<LiteralNode>(&bound_values.back()->data))
        {
          binding->apply(binding_scope, value->value);
          ctx.push_context(true, binding_scope);
        }
        else
        {
          throw LispleException("let: Invalid value node.");
        }
      }

      for (size_t i = 1; i < args.size(); i++)
      {
        result = exec(ctx, *args[i]);
      }

      for (size_t i = 0; i < bnd->nodes.size() / 2; i++)
      {
        ctx.pop_context();
      }
    }
    else
    {
      throw LispleException("let: Invalid bind form.");
    }

    return result;
  }
} // namespace Lisple
