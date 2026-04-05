
#include "lisple/exception.h"
#include "lisple/exec.h"

#include <lisple/lang/branch.h>
#include <lisple/runtime/exec_node.h>

namespace Lisple
{
  /** CaseForm - case */
  SPECIAL_FORM_IMPL(CaseForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&CaseForm::inv_case, &CaseForm::execnode_case))))

  SFORM_LOWER_IMPL(CaseForm)
  {
    uptr_exec_node_v exec_nodes;

    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() % 2 != 0)
    {
      throw LispleException("case: Unmatched case pairs: " + ast_node->to_string());
    }

    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_rtval_v{}, std::move(exec_nodes)));
  }

  Key DEFAULT = Key("default");

  MACRO_BODY(CaseForm, inv_case)
  {
    if ((args.size() - 1) % 2 != 0)
    {
      throw InvocationException("Incomplete condition-expression pair passed to case");
    }
    else if (args.size() == 1)
    {
      throw InvocationException("Empty case-form");
    }

    sptr_sobject retval = NIL;

    ctx.push_context(true);
    sptr_sobject value = ctx.eval_ast(args[0]);

    for (size_t i = 1; i < args.size(); i += 2)
    {
      if (*ctx.eval_ast(args[i]) == *value || *args[i] == DEFAULT)
      {
        retval = ctx.eval_ast(args[i + 1]);
        break;
      }
    }

    ctx.pop_context();
    return retval;
  }

  EXECNODE_BODY(CaseForm, execnode_case)
  {
    static sptr_rtval DEFAULT_KEY = RTValue::keyword("default");
    sptr_rtval value = exec(ctx, *snode.exec_nodes[0]);

    for (size_t i = 1; i < snode.exec_nodes.size(); i += 2)
    {
      sptr_rtval case_selector = exec(ctx, *snode.exec_nodes[i]);
      if (*value == *case_selector || *DEFAULT_KEY == *case_selector)
      {
        return exec(ctx, *snode.exec_nodes[i + 1]);
      }
    }
    return Constant::NIL;
  }

  /** CondForm - cond */
  SPECIAL_FORM_IMPL(CondForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&CondForm::inv_cond, &CondForm::execnode_cond))))

  SFORM_LOWER_IMPL(CondForm)
  {
    uptr_exec_node_v exec_nodes;

    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() % 2 != 1)
    {
      throw LispleException("cond: Unmatched condition pairs: " + ast_node->to_string());
    }

    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_rtval_v{}, std::move(exec_nodes)));
  }

  MACRO_BODY(CondForm, inv_cond)
  {
    deprecated_special_form_invocations++;
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
      sptr_sobject condition = ctx.eval_ast(args[i]);
      if (*condition != *B_FALSE && *condition != *NIL)
      {
        retval = ctx.eval_ast(args[i + 1]);
        break;
      }
    }

    ctx.pop_context();
    return retval;
  }

  EXECNODE_BODY(CondForm, execnode_cond)
  {
    Lisple::sptr_rtval cond_result;
    for (size_t i = 0; i < snode.exec_nodes.size(); i += 2)
    {
      cond_result = exec(ctx, *snode.exec_nodes[i]);
      if (Lisple::is_truthy(*cond_result))
      {
        return exec(ctx, *snode.exec_nodes[i + 1]);
      }
    }
    return Constant::NIL;
  }

  /** IfForm - if */
  SPECIAL_FORM_IMPL(IfForm,
                    MULTI_SIG((FN_ARGS((&Lisple::Type::ANY), (&Lisple::Type::ANY, NO_EVAL)),
                               EXEC_DISPATCH(&IfForm::inv_if, &IfForm::execnode_if)),
                              (FN_ARGS((&Lisple::Type::ANY),
                                       (&Lisple::Type::ANY, NO_EVAL),
                                       (&Lisple::Type::ANY, NO_EVAL)),
                               EXEC_DISPATCH(&IfForm::inv_if, &IfForm::execnode_if))))

  SFORM_LOWER_IMPL(IfForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() < 3 || elements.size() > 4)
    {
      throw LispleException("if: Invalid number of branches: " +
                            std::to_string(elements.size() - 2));
    }

    uptr_exec_node_v exec_nodes;
    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_rtval_v{}, std::move(exec_nodes)));
  }

  MACRO_BODY(IfForm, inv_if)
  {
    sptr_sobject retval = Lisple::NIL;

    ctx.push_context(true);
    auto condition = ctx.eval_ast(args[0]);
    if (*condition != *Lisple::B_FALSE && *condition != *Lisple::NIL)
    {
      retval = ctx.eval_ast(args[1]);
    }
    else if (args.size() == 3)
    {
      retval = ctx.eval_ast(args[2]);
    }
    ctx.pop_context();
    return retval;
  }

  EXECNODE_BODY(IfForm, execnode_if)
  {
    if (Lisple::is_truthy(*exec(ctx, *snode.exec_nodes[0])))
    {
      return exec(ctx, *snode.exec_nodes[1]);
    }
    else if (snode.exec_nodes.size() == 3)
    {
      return exec(ctx, *snode.exec_nodes[2]);
    }

    return Constant::NIL;
  }

  /* WhenForm - when */
  SPECIAL_FORM_IMPL(WhenForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&WhenForm::inv_when, &WhenForm::execnode_when))))

  SFORM_LOWER_IMPL(WhenForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();

    if (elements.size() < 2)
    {
      throw LispleException("when: No condition given: " + ast_node->to_string());
    }

    uptr_exec_node_v exec_nodes;
    for (size_t i = 1; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, sptr_rtval_v{}, std::move(exec_nodes)));
  }

  MACRO_BODY(WhenForm, inv_when)
  {
    sptr_sobject retval = NIL;

    ctx.push_context(true);
    auto condition = ctx.eval_ast(args[0]);
    if (condition->is_truthy())
    {
      for (size_t i = 1; i < args.size(); i++)
      {
        retval = RuntimeValueWrapper::make(ctx.eval(args[i]));
      }
    }
    ctx.pop_context();
    return retval;
  }

  EXECNODE_BODY(WhenForm, execnode_when)
  {
    sptr_rtval result = Constant::NIL;

    if (Lisple::is_truthy(*exec(ctx, *snode.exec_nodes[0])))
    {
      for (size_t i = 1; i < snode.exec_nodes.size(); i++)
      {
        result = exec(ctx, *snode.exec_nodes[i]);
      }
    }

    return result;
  }

} // namespace Lisple
