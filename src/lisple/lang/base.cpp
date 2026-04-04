#include <lisple/lang/base.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
  /* DefForm */
  MACRO_IMPL(DefForm,
             MULTI_SIG((FN_ARGS((&Type::WORD, DATA), (&Type::ANY)),
                        EXEC_DISPATCH(&DefForm::inv_def, &DefForm::execnode_def)),
                       (FN_ARGS((&Type::WORD, DATA), (&Type::STRING), (&Type::ANY)),
                        EXEC_DISPATCH(&DefForm::inv_def_docstring,
                                      &DefForm::execnode_def_docstring))))

  /**
   * Legacy AST-based implementation.
   */
  MACRO_BODY(DefForm, inv_def)
  {
    Lisple::Word& symbol = args[0]->as<Lisple::Word>();
    ctx.store_namespace(symbol, args[1]);
    return ctx.get_current_namespace()->lookup(symbol);
  }

  EXECNODE_BODY(DefForm, execnode_def)
  {
    std::string& key =
      std::get<std::string>(std::get<LiteralNode>(args[0]->data).value->value);
    sptr_rtval& value = std::get<LiteralNode>(args[1]->data).value;
    ctx.store_namespace(key, value);

    return value;
  }

  /**
   * Legacy AST-based implementation.
   */
  MACRO_BODY(DefForm, inv_def_docstring)
  {
    ctx.store_namespace(args[0]->as<Lisple::Word>(), args[2]);
    return args[2];
  }

  EXECNODE_BODY(DefForm, execnode_def_docstring)
  {
    sptr_rtval& value = std::get<LiteralNode>(args[2]->data).value;
    ctx.store_namespace(
      std::get<std::string>(std::get<LiteralNode>(args[0]->data).value->value),
      value);
    return value;
  }

  /**
   * DoForm - do
   */
  SPECIAL_FORM_IMPL(DoForm,
                    SIG((FN_ARGS((&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&DoForm::inv_do, &DoForm::execnode_do))))

  MACRO_BODY(DoForm, inv_do)
  {
    Lisple::sptr_sobject ret;
    ctx.push_context(true);
    for (auto& arg : args)
    {
      ret = ctx.eval_ast(arg);
    }
    ctx.pop_context();
    return ret;
  }

  EXECNODE_BODY(DoForm, execnode_do)
  {
    Lisple::sptr_rtval ret;
    for (auto& arg : args)
    {
      ret = exec(ctx, *arg);
    }
    return ret;
  }

  /** AndForm - and */
  SPECIAL_FORM_IMPL(AndForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&AndForm::inv_and, &AndForm::execnode_and))))

  /**
   * Legacy AST-based implementation.
   */
  MACRO_BODY(AndForm, inv_and)
  {
    ctx.push_context(true);
    for (auto& arg : args)
    {
      sptr_sobject lmnt = ctx.eval_ast(arg);
      if (!lmnt->is_truthy())
      {
        ctx.pop_context();
        return B_FALSE;
      }
    }
    ctx.pop_context();
    return B_TRUE;
  }

  EXECNODE_BODY(AndForm, execnode_and)
  {
    sptr_rtval val;
    for (auto& arg : args)
    {
      val = exec(ctx, *arg);
      if (!Lisple::is_truthy(*val))
      {
        return Constant::BOOL_FALSE;
      }
    }
    return val;
  }

  /** OrForm - or */
  SPECIAL_FORM_IMPL(OrForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&OrForm::inv_or, &OrForm::execnode_or))))

  MACRO_BODY(OrForm, inv_or)
  {
    ctx.push_context(true);
    for (auto& arg : args)
    {
      sptr_sobject lmnt = ctx.eval_ast(arg);
      if (lmnt->is_truthy())
      {
        ctx.pop_context();
        return lmnt;
      }
    }
    ctx.pop_context();
    return B_FALSE;
  }

  EXECNODE_BODY(OrForm, execnode_or)
  {
    sptr_rtval val;

    for (auto& arg : args)
    {
      val = exec(ctx, *arg);
      if (Lisple::is_truthy(*val))
      {
        return val;
      }
    }

    return Constant::BOOL_FALSE;
  }

  /** RndFunction - rnd */
  FUNC_IMPL(RndFunction,
            MULTI_SIG((FN_ARGS((&Lisple::Type::NUMBER)),
                       EXEC_DISPATCH(&RndFunction::exec_rnd)),
                      (FN_ARGS((&Lisple::Type::NUMBER), (&Lisple::Type::NUMBER)),
                       EXEC_DISPATCH(&RndFunction::exec_rnd))))

  EXEC_BODY(RndFunction, exec_rnd)
  {
    if (args[0]->type != RTValue::Type::NUMBER &&
        (args.size() == 2 && args[1]->type != RTValue::Type::NUMBER))
    {
      return Constant::NIL;
    }
    int min =
      args.size() == 1 ? 0 : std::get<const RTValue::Number>(args[0]->value).get_int();
    int max =
      std::get<const RTValue::Number>(args[args.size() == 1 ? 0 : 1]->value).get_int();

    return RTValue::number((std::rand() % (max - min)) + min);
  }

  SPECIAL_FORM_IMPL(SetBangForm,
                    SIG((FN_ARGS((&Type::ARRAY, DATA), (&Lisple::Type::ANY)),
                         EXEC_DISPATCH(&SetBangForm::inv_set, &SetBangForm::execnode_set))))

  MACRO_BODY(SetBangForm, inv_set)
  {
    Lisple::Array& member_ref = args[0]->as<Lisple::Array>();

    if (member_ref.size() == 1)
    {
      auto identifier = member_ref.get_children()[0]->as<Lisple::Word>();
      Scope& scope = ctx.get_scope_of(identifier);
      scope.mutate(identifier, args.back());
    }
    else if (member_ref.size() == 2)
    {
      auto actual_mem_ref = ctx.eval_ast(args[0]);
      Lisple::Object& prop = *actual_mem_ref->get_children()[0];
      Lisple::Object& owner = *actual_mem_ref->get_children().back();

      if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(&owner))
      {
        auto val = to_rt_value(args.back());
        Lisple::Dict::set_property(wrapper->val, to_rt_value(prop), val);
      }
      else
      {
        owner.set_property(&ctx, prop, args.back());
      }
    }
    else
    {
      throw Lisple::InvocationException("Incorrect member reference: " +
                                        member_ref.to_string());
    }

    return args.back();
  }

  EXECNODE_BODY(SetBangForm, execnode_set)
  {
    auto member_ref_vec = std::get<LiteralNode>(args[0]->data).value;

    auto member_refs = member_ref_vec->elements();
    auto value = std::get<LiteralNode>(args.back()->data).value;

    if (member_refs.size() == 1)
    {
      auto identifier = Lisple::Word::make(member_refs[0]->str());
      Scope& scope = ctx.get_scope_of(*identifier);
      scope.mutate(identifier->value, value);
    }
    else if (member_refs.size() == 2)
    {
      auto actual_mem_ref = ctx.eval(member_ref_vec->to_string());
      auto& prop = actual_mem_ref->elements()[0];
      sptr_rtval owner = actual_mem_ref->elements().back();

      Lisple::Dict::set_property(owner, prop, value);
    }
    else
    {
      throw Lisple::InvocationException("Incorrect member reference: " +
                                        member_ref_vec->to_string());
    }

    return value;
  }

} // namespace Lisple
