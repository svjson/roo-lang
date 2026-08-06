
#include "roo/runtime/lower.h"
#include "roo/runtime/node.h"
#include "roo/runtime/value.h"
#include "roo/type.h"

#include <roo/lang/rewrite.h>
#include <roo/runtime/exec_node.h>

namespace Roo
{
  /** CommentForm - roo/comment */
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

  /** ThreadFirstForm - roo/-> */
  SPECIAL_FORM_IMPL(ThreadFirstForm,
                    SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&ThreadFirstForm::execnode_thread_first))))

  SFORM_LOWER_IMPL(ThreadFirstForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements.size() == 1) return lower_literal(Roo::AST::NIL);
    if (elements.size() == 2) return lower_expr(ctx, elements[1]);

    sptr_ast_node current = elements[1];

    for (size_t i = 2; i < elements.size(); i++)
    {
      auto& lmnt = elements[i];
      if (lmnt->get_type() == Form::LIST)
      {
        sptr_ast_node_v& list_elements = lmnt->get_children();

        sptr_ast_node_v new_list;
        new_list.reserve(list_elements.size() + 1);
        new_list.push_back(list_elements[0]);
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

  /** CondThreadFirstForm - roo/cond-> */
  SPECIAL_FORM_IMPL(CondThreadFirstForm,
                    SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&CondThreadFirstForm::execnode_cond_thread_first))))

  SFORM_LOWER_IMPL(CondThreadFirstForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();

    if (elements.size() < 2) return lower_literal(Roo::AST::NIL);
    if (elements.size() == 2) return lower_expr(ctx, elements[1]);

    if ((elements.size() - 2) % 2 != 0)
    {
      throw RooException("cond->: Requires pairs of [test form], got " +
                         std::to_string(elements.size() - 2) + " trailing args");
    }

    uptr_exec_node_v exec_nodes;
    sptr_val_v values;
    exec_nodes.reserve(elements.size());

    exec_nodes.push_back(lower_expr(ctx, elements[1]));

    for (size_t i = 2; i < elements.size(); i += 2)
    {
      sptr_ast_node& test = elements[i];
      sptr_ast_node& form = elements[i + 1];

      exec_nodes.push_back(lower_expr(ctx, test));

      size_t extra_arg_count = 0;
      if (form->get_type() == Form::LIST)
      {
        sptr_ast_node_v& list_elems = form->get_children();
        exec_nodes.push_back(lower_expr(ctx, list_elems[0]));
        for (size_t j = 1; j < list_elems.size(); j++)
        {
          exec_nodes.push_back(lower_expr(ctx, list_elems[j]));
        }
        extra_arg_count = list_elems.size() - 1;
      }
      else
      {
        exec_nodes.push_back(lower_expr(ctx, form));
      }

      values.push_back(Value::number(static_cast<int>(extra_arg_count)));
    }

    return std::make_unique<ExecNode>(
      SpecialFormNode(this, values, std::move(exec_nodes)));
  }

  EXECNODE_BODY(CondThreadFirstForm, execnode_cond_thread_first)
  {
    sptr_val current = exec(ctx, *snode.exec_nodes[0]);

    size_t node_idx = 1;
    for (size_t pair_idx = 0; pair_idx < snode.values.size(); pair_idx++)
    {
      const size_t extra_arg_count =
        static_cast<size_t>(snode.values[pair_idx]->i64());

      if (Roo::is_truthy(*exec(ctx, *snode.exec_nodes[node_idx])))
      {
        node_idx++;
        sptr_val fn_val = exec(ctx, *snode.exec_nodes[node_idx]);
        node_idx++;

        sptr_val_v call_args;
        call_args.reserve(1 + extra_arg_count);
        call_args.push_back(current);
        for (size_t j = 0; j < extra_arg_count; j++)
        {
          call_args.push_back(exec(ctx, *snode.exec_nodes[node_idx + j]));
        }
        node_idx += extra_arg_count;

        current = fn_val->exec().execute(ctx, call_args);
      }
      else
      {
        node_idx += 2 + extra_arg_count;
      }
    }

    return current;
  }

} // namespace Roo
