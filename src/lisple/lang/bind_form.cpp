

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

  /* IfLetForm - if-let */
  SPECIAL_FORM_IMPL(IfLetForm,
                    SIG((FN_ARGS((&Type::ARRAY, &Eval::REPEAT_LAZY_BIND_SYM_VAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&IfLetForm::inv_if_let,
                                       &IfLetForm::execnode_if_let))))

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

        sptr_rtval bind_val = exec(ctx, *bnd->nodes[i + 1]);

        if (!Lisple::is_truthy(*bind_val))
        {
          for (size_t si = 0; si < i / 2; i++)
          {
            ctx.pop_context();
          }
          if (args.size() == 3)
          {
            return exec(ctx, *args[2]);
          }
          else
          {
            return Constant::NIL;
          }
          break;
        }

        binding->apply(binding_scope, bind_val);
        ctx.push_context(true, binding_scope);
      }

      result = exec(ctx, *args[1]);

      for (size_t i = 0; i < bnd->nodes.size() / 2; i++)
      {
        ctx.pop_context();
      }
    }
    else
    {
      throw LispleException("if-let: Invalid bind form.");
    }

    return result;
  }

} // namespace Lisple
