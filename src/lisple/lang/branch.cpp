
#include "branch.h"

namespace Lisple
{
  /** CondForm - cond */
  MACRO_IMPL(CondForm,
             SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&CondForm::inv_cond, &CondForm::execnode_cond))))

  MACRO_BODY(CondForm, inv_cond)
  {
    if (args.size() % 2 != 0)
    {
      throw InvocationException("Uneven number of forms passed to cond");
    }
    else if (args.size() == 0)
    {
      throw InvocationException("Empty cond-form");
    }

    sptr_sobject retval = Lisple::NIL;

    ctx.push_context(true);
    for (size_t i = 0; i < args.size(); i += 2)
    {
      sptr_sobject condition = ctx.eval(args[i]);
      if (*condition != *B_FALSE && *condition != *NIL)
      {
        retval = ctx.eval(args[i + 1]);
        break;
      }
    }

    ctx.pop_context();
    return retval;
  }

  EXECNODE_BODY(CondForm, execnode_cond)
  {
    if (args.size() % 2 != 0)
    {
      throw InvocationException("Uneven number of forms passed to cond");
    }
    else if (args.size() == 0)
    {
      throw InvocationException("Empty cond-form");
    }

    sptr_rtval retval = Constant::NIL;
    sptr_rtval cnd_eval = Constant::NIL;

    ctx.push_context(true);
    for (size_t i = 0; i < args.size(); i += 2)
    {
      cnd_eval = exec(ctx, *args[i]);
      if (Lisple::is_truthy(*cnd_eval))
      {
        retval = exec(ctx, *args[i + 1]);
        break;
      }
    }

    ctx.pop_context();
    return retval;
  }

  /** IfForm - if */
  MACRO_IMPL(IfForm,
             MULTI_SIG((FN_ARGS((&Lisple::Type::ANY), (&Lisple::Type::ANY, NO_EVAL)),
                        EXEC_DISPATCH(&IfForm::inv_if, &IfForm::execnode_if)),
                       (FN_ARGS((&Lisple::Type::ANY),
                                (&Lisple::Type::ANY, NO_EVAL),
                                (&Lisple::Type::ANY, NO_EVAL)),
                        EXEC_DISPATCH(&IfForm::inv_if, &IfForm::execnode_if))))

  MACRO_BODY(IfForm, inv_if)
  {
    sptr_sobject retval = Lisple::NIL;

    ctx.push_context(true);
    auto condition = ctx.eval(args[0]);
    if (*condition != *Lisple::B_FALSE && *condition != *Lisple::NIL)
    {
      retval = ctx.eval(args[1]);
    }
    else if (args.size() == 3)
    {
      retval = ctx.eval(args[2]);
    }
    ctx.pop_context();
    return retval;
  }

  EXECNODE_BODY(IfForm, execnode_if)
  {
    if (Lisple::is_truthy(*std::get<LiteralNode>(args[0]->data).value))
    {
      return exec(ctx, *args[1]);
    }
    else if (args.size() == 3)
    {
      return exec(ctx, *args[2]);
    }

    return Constant::NIL;
  }

} // namespace Lisple
