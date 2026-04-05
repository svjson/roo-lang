
#include "lisple/runtime/lower.h"
#include "lisple/runtime/node.h"
#include "lisple/runtime/value.h"
#include "lisple/type.h"

#include <variant>

#include <lisple/lang/rewrite.h>
#include <lisple/runtime/exec_node.h>

namespace Lisple
{
  /** ThreadFirstForm - -> */
  SPECIAL_FORM_IMPL(ThreadFirstForm,
                    SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ThreadFirstForm::inv_thread_first,
                                       &ThreadFirstForm::execnode_thread_first))))

  SFORM_LOWER_IMPL(ThreadFirstForm)
  {
    sptr_sobject_v elements = ast_node->get_children();
    if (elements.size() == 1) return lower_literal(Lisple::NIL);
    if (elements.size() == 2) return lower_expr(ctx, elements[1]);

    sptr_sobject current = elements[1];

    for (size_t i = 2; i < elements.size(); i++)
    {
      auto& lmnt = elements[i];
      if (lmnt->get_type() == Form::LIST)
      {
        sptr_sobject_v& list_elements = lmnt->get_children();

        sptr_sobject_v new_list{list_elements[0]};
        new_list.push_back(current);
        for (size_t j = 1; j < list_elements.size(); j++)
        {
          new_list.push_back(list_elements[j]);
        }

        current = List::make(new_list);
      }
      else
      {
        current = List::make({lmnt, current});
      }
    }

    return lower_expr(ctx, current);
  }

  MACRO_BODY(ThreadFirstForm, inv_thread_first)
  {
    deprecated_special_form_invocations++;
    sptr_sobject value = args[0];
    for (size_t i = 1; i < args.size(); i++)
    {
      sptr_sobject ifn = args[i];
      if (ifn->get_type() == Form::LIST)
      {
        sptr_sobject_v ifn_children = ifn->get_children();
        size_t ifn_size = ifn_children.size();
        sptr_sobject_v fn_list;

        fn_list.reserve(ifn->size() + 1);
        if (ifn_size)
        {
          fn_list.push_back(ctx.eval_ast(ifn_children[0]));
        }

        fn_list.push_back(value);

        for (size_t n = 1; n < ifn_size; n++)
        {
          fn_list.push_back(ctx.eval_ast(ifn_children[n]));
        }

        value = List(fn_list).execute(ctx);
      }
      else
      {
        value = List({ctx.eval_ast(ifn), value}).execute(ctx);
      }
    }

    return value;
  }

  EXECNODE_BODY(ThreadFirstForm, execnode_thread_first)
  {
    deprecated_special_form_invocations++;

    return Constant::NIL;
  }

} // namespace Lisple
