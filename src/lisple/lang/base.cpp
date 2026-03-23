
#include "base.h"

#include "../runtime/exec_node.h"
#include "../runtime/value.h"

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
      sptr_sobject lmnt = ctx.eval(arg);
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
        return Constant::FALSE;
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
      sptr_sobject lmnt = ctx.eval(arg);
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

    return Constant::FALSE;
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
    int min = args.size() == 1 ? 0 : std::get<RTValue::Number>(args[0]->value).get_int();
    int max = std::get<RTValue::Number>(args[args.size() == 1 ? 0 : 1]->value).get_int();

    return RTValue::number((std::rand() % (max - min)) + min);
  }

} // namespace Lisple
