
#include "lisple/runtime/lower.h"
#include "lisple/runtime/node.h"
#include "lisple/runtime/value.h"
#include "lisple/type.h"

#include <variant>

#include <lisple/lang/rewrite.h>
#include <lisple/runtime/exec_node.h>

namespace Lisple
{
  /** CommentForm - comment */
  SPECIAL_FORM_IMPL(CommentForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&CommentForm::execnode_comment))))
  SFORM_LOWER_IMPL(CommentForm)
  {
    return std::make_unique<ExecNode>(Constant::NIL);
  }
  EXECNODE_BODY(CommentForm, execnode_comment)
  {
    return Constant::NIL;
  }

  /** ThreadFirstForm - -> */
  SPECIAL_FORM_IMPL(ThreadFirstForm,
                    SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ThreadFirstForm::execnode_thread_first))))

  SFORM_LOWER_IMPL(ThreadFirstForm)
  {
    sptr_sobject_v elements = ast_node->get_children();
    if (elements.size() == 1) return lower_literal(Lisple::AST::NIL);
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

        current = AST::List::make(new_list);
      }
      else
      {
        current = AST::List::make({lmnt, current});
      }
    }

    return lower_expr(ctx, current);
  }
  EXECNODE_BODY(ThreadFirstForm, execnode_thread_first)
  {
    deprecated_special_form_invocations++;

    return Constant::NIL;
  }

} // namespace Lisple
