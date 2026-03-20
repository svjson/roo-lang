
#include "eval_plan.h"

#include "../exception.h"
#include "exec_node.h"
#include "lower.h"
#include <iostream>

#include "lisple/runtime/exec_tree.h"
#include "lisple/runtime/value.h"

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
    // std::cout << "extract_sequence: " << std::endl;
    // std::cout << Lisple::to_string(*node, "") << std::endl;

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
    // std::cout << indent << "prepare_sequence()" << std::endl;
    indent += "  ";
    for (size_t i = 0; i < input.size(); ++i)
    {
      // std::cout << indent << "<seq-element#" << i << "/" << (input.size() - 1) << ">"
      //<< std::endl;
      // std::cout << indent << "  <input>" << std::endl;
      // std::cout << to_string(*input[i], indent + "    ");
      // std::cout << indent << "  </input>" << std::endl;

      const EvalMode* elem_mode = pattern.get(i);

      auto& arg = input[i];

      if (elem_mode && elem_mode->nested)
      {
        // std::cout << indent << "  <nested>" << std::endl;

        std::vector<ExecNode*> nested_output;

        // std::cout << to_string(*arg, "") << std::endl;

        prepare_sequence(ctx,
                         dynamic_cast<const EvalPattern&>(*elem_mode),
                         std::get<VectorNode>(arg->data).elements,
                         storage,
                         nested_output,
                         indent + "    ");

        sptr_sobject_v ast_nodes;
        // std::cout << indent << "    <Nested nodes>" << std::endl;
        for (auto* n_node : nested_output)
        {
          // std::cout << to_string(*n_node, "      " + indent);
          ast_nodes.push_back(n_node->form);
        }
        // std::cout << indent << "    </Nested Nodes>" << std::endl;

        sptr_sobject vec = Array::make(ast_nodes);

        storage.push_back(std::make_unique<ExecNode>(vec, ExecNodeList(nested_output)));
        output.push_back(storage.back().get());
        // std::cout << indent << "  </nested>" << std::endl;
      }
      else if (elem_mode == &Eval::LITERAL)
      {
        // std::cout << indent << "  <literal>" << std::endl;
        storage.push_back(lower_literal(arg->form));
        output.push_back(storage.back().get());
        // std::cout << indent << "  </literal>" << std::endl;
      }
      else if (elem_mode == &Eval::DEFAULT)
      {
        // std::cout << indent << "  <eval>" << std::endl;
        storage.push_back(lower_literal(exec(ctx, *arg)));
        output.push_back(storage.back().get());
        // std::cout << indent << "  </eval>" << std::endl;
      }
      else
      {
        // std::cout << indent << "  <passthrough>" << std::endl;
        // std::cout << to_string(*arg, indent + "  ");
        output.push_back(arg.get());
        // std::cout << indent << "  </passthrough>" << std::endl;
      }
      // std::cout << indent << "</seq-element#" << i << "/" << (input.size() - 1) << ">"
      //<< std::endl;
    }
  }

} // namespace Lisple
