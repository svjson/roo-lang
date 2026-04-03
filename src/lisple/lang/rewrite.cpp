
#include <variant>

#include <lisple/lang/rewrite.h>
#include <lisple/runtime/exec_node.h>

namespace Lisple
{
  /** ThreadFirstForm - -> */
  MACRO_IMPL(ThreadFirstForm,
             SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY, NO_EVAL)),
                  EXEC_DISPATCH(&ThreadFirstForm::inv_thread_first,
                                &ThreadFirstForm::execnode_thread_first))))

  MACRO_BODY(ThreadFirstForm, inv_thread_first)
  {
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
    sptr_rtval value = std::get<LiteralNode>(args[0]->data).value;

    for (size_t i = 1; i < args.size(); i++)
    {

      if (auto* call_node = std::get_if<CallNode>(&args[i]->data))
      {
        auto new_exec_node =
          std::make_unique<ExecNode>(CallNode(call_node->callee->clone(), {}));

        auto& call_args = std::get<CallNode>(new_exec_node->data).args;
        call_args.reserve(call_node->args.size() + 1);
        call_args.push_back(std::make_unique<ExecNode>(value));
        for (auto& arg : call_node->args)
        {
          call_args.push_back(arg->clone());
        }

        value = exec(ctx, *new_exec_node);
      }
      else if (std::holds_alternative<LookupNode>(args[i]->data) ||
               std::holds_alternative<LiteralNode>(args[i]->data))
      {
        auto callee = args[i]->clone();
        uptr_exec_node_v call_args;
        call_args.push_back(std::make_unique<ExecNode>(value));
        auto new_exec_node =
          std::make_unique<ExecNode>(CallNode(std::move(callee), std::move(call_args)));

        value = exec(ctx, *new_exec_node);
      }

      else
      {
        throw LispleException("Invalid in thread-first form: " +
                              Lisple::to_string(*args[i], ""));
      }
    }
    return value;
  }

} // namespace Lisple
