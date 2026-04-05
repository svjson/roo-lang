
#include "lisple/runtime/eval_plan.h"

#include <lisple/bind.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>
#include <lisple/runtime/node.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
  const EvalMode* EvalPattern::get(size_t index) const
  {
    if (index < this->pattern.size())
    {
      return this->pattern[index];
    }
    else if (this->repeat)
    {
      return this->pattern[index % this->pattern.size()];
    }
    else
    {
      return this->pattern.back();
    }
  }

  uptr_exec_node_v extract_sequence(ExecNode* node)
  {
    auto& lit_node = std::get<LiteralNode>(node->data);
    sptr_rtval_v& lmnt_values = std::get<sptr_rtval_v>(lit_node.value->value);

    uptr_exec_node_v seq;
    for (size_t i = 0; i < lmnt_values.size(); i++)
    {
      seq.push_back(std::make_unique<ExecNode>(
        node->form->get_children()[i],
        LiteralNode(lmnt_values[i], node->form->get_children()[i])));
    }

    return seq;
  }

  void prepare_sequence(Context& ctx,
                        const EvalPattern& pattern,
                        const uptr_exec_node_v& input,
                        uptr_exec_node_v& storage,
                        std::vector<ExecNode*>& output,
                        std::string indent)
  {
    indent += "  ";
    for (size_t i = 0; i < input.size(); ++i)
    {
      const EvalMode* elem_mode = pattern.get(i);

      auto& arg = input[i];

      if (elem_mode && elem_mode->nested)
      {
        std::vector<ExecNode*> nested_output;

        prepare_sequence(ctx,
                         dynamic_cast<const EvalPattern&>(*elem_mode),
                         std::get<VectorNode>(arg->data).elements,
                         storage,
                         nested_output,
                         indent + "    ");

        sptr_sobject_v ast_nodes;

        for (auto* n_node : nested_output)
        {
          ast_nodes.push_back(n_node->form);
        }

        sptr_sobject vec = Array::make(ast_nodes);

        storage.push_back(std::make_unique<ExecNode>(vec, ExecNodeList(nested_output)));
        output.push_back(storage.back().get());
      }
      else if (elem_mode == &Eval::LITERAL)
      {
        storage.push_back(lower_literal(arg->form));
        output.push_back(storage.back().get());
      }
      else if (elem_mode == &Eval::DEFAULT)
      {
        storage.push_back(
          std::make_unique<ExecNode>(Lisple::NIL,
                                     LiteralNode(exec(ctx, *arg), Lisple::NIL)));
        output.push_back(storage.back().get());
      }
      else
      {
        output.push_back(arg.get());
      }
    }
  }

} // namespace Lisple
