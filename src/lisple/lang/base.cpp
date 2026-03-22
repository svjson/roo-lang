
#include "base.h"

#include "../runtime/exec_node.h"
#include "../runtime/value.h"

namespace Lisple
{
  /* DefForm */
  MACRO_IMPL(DefForm,
             MULTI_SIG((FN_ARGS((&Type::WORD, DATA), (&Type::ANY)),
                        EXEC_DISPATCH(&DefForm::inv_def, &DefForm::exec_def)),
                       (FN_ARGS((&Type::WORD, DATA), (&Type::STRING), (&Type::ANY)),
                        EXEC_DISPATCH(&DefForm::inv_def_docstring,
                                      &DefForm::exec_def_docstring))))

  /**
   * Legacy AST-based implementation.
   */
  MACRO_BODY(DefForm, inv_def)
  {
    Lisple::Word& symbol = args[0]->as<Lisple::Word>();
    ctx.store_namespace(symbol, args[1]);
    return ctx.get_current_namespace()->lookup(symbol);
  }

  EXEC_BODY(DefForm, exec_def)
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

  EXEC_BODY(DefForm, exec_def_docstring)
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
                         EXEC_DISPATCH(&AndForm::inv_and, &AndForm::exec_and))))

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

  EXEC_BODY(AndForm, exec_and)
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
                         EXEC_DISPATCH(&OrForm::inv_or, &OrForm::exec_or))))

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

  EXEC_BODY(OrForm, exec_or)
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

} // namespace Lisple
