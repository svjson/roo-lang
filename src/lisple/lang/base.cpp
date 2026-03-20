
#include "base.h"

#include "../runtime/exec_node.h"

namespace Lisple
{
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
    sptr_sobject form;
    for (auto& arg : args)
    {
      form = exec(ctx, *arg);
      if (!form->is_truthy())
      {
        return B_FALSE;
      }
    }
    return form;
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
    sptr_sobject form;

    for (auto& arg : args)
    {
      form = exec(ctx, *arg);
      if (form->is_truthy())
      {
        return form;
      }
    }

    return B_FALSE;
  }

} // namespace Lisple
