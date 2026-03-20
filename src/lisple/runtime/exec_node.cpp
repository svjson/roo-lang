
#include "exec_node.h"

#include "../context.h"
#include "../exec.h"
#include "../type.h"
#include "eval_plan.h"
#include "exec_tree.h"
#include <vector>

namespace Lisple
{
  std::string to_string(const ptr_exec_node_v& nodes, std::string indent)
  {
    std::string result = "";

    for (size_t i = 0; i < nodes.size(); i++)
    {
      result += indent + " - #" + std::to_string(i) + "\n";
      result += to_string(*nodes[i], indent + "  ");
    }

    return result;
  }

  std::string to_string(const ExecNode& node, std::string indent)
  {
    return std::visit(
      [&](auto const& n) -> std::string
      {
        using T = std::decay_t<decltype(n)>;

        if constexpr (std::is_same_v<T, LiteralNode>)
        {
          return indent + " - LiteralNode(" + node.form->to_string() + ")\n";
        }
        else if constexpr (std::is_same_v<T, LookupNode>)
        {
          return indent + " - LookupNode(" + n.identifier.to_string() + ")\n";
        }
        else if constexpr (std::is_same_v<T, MapNode>)
        {
          std::string result =
            indent + " - MapNode(" + std::to_string(n.elements.size()) + ")\n";
          indent += "  ";

          for (auto& lmnt : n.elements)
          {
            result += to_string(*lmnt, indent);
          }

          return result;
        }
        else if constexpr (std::is_same_v<T, VectorNode>)
        {
          std::string result =
            indent + " - VectorNode(" + std::to_string(n.elements.size()) + ")\n";
          indent += "  ";

          for (auto& lmnt : n.elements)
          {
            result += to_string(*lmnt, indent);
          }

          return result;
        }
        else if constexpr (std::is_same_v<T, CallNode>)
        {
          std::string result = indent + " - CallNode\n";
          indent += "  ";
          result += indent + " - Callee:\n";
          result += to_string(*n.callee, indent + "  ");

          result += indent + " - Arguments:\n";
          indent += "  ";
          for (size_t i = 0; i < n.args.size(); i++)
          {
            auto& arg = n.args[i];
            result += to_string(*arg, indent);
          }

          return result;
        }
        else if constexpr (std::is_same_v<T, ExecNodeList>)
        {
          std::string result = indent + " - ExecNodeList\n";
          indent += "  ";
          for (size_t i = 0; i < n.nodes.size(); i++)
          {
            result += to_string(*n.nodes[i], indent);
          }
          return result;
        }

        return indent + " UNHANDLED NODE TYPE\n";
      },

      node.data);
  }

  sptr_sobject exec(Context& ctx, const ExecNode& node)
  {
    return std::visit(
      [&](auto const& n) -> sptr_sobject
      {
        using T = std::decay_t<decltype(n)>;

        if constexpr (std::is_same_v<T, LiteralNode>)
        {
          return n.ast_node;
        }
        else if constexpr (std::is_same_v<T, LookupNode>)
        {
          sptr_sobject result = ctx.lookup(n.identifier);
          return result;
        }
        else if constexpr (std::is_same_v<T, MapNode>)
        {
          sptr_sobject_v elements;
          elements.reserve(n.elements.size());

          for (auto& lmnt : n.elements)
          {
            elements.push_back(exec(ctx, *lmnt));
          }

          return Lisple::Map::make(elements);
        }
        else if constexpr (std::is_same_v<T, VectorNode>)
        {
          sptr_sobject_v elements;
          elements.reserve(n.elements.size());

          for (auto& lmnt : n.elements)
          {
            elements.push_back(exec(ctx, *lmnt));
          }

          return Lisple::Array::make(elements);
        }
        else if constexpr (std::is_same_v<T, CallNode>)
        {
          // std::cout << "Execute CallNode: " << std::endl;
          // std::cout << to_string(node, "") << std::endl;
          sptr_sobject fn = exec(ctx, *n.callee);

          sptr_sobject_v args;
          args.reserve(n.args.size());

          sptr_sobject_v raw_args = node.form->as<List>().tail();

          Signature* sig = fn->get_type() == Form::MACRO
                             ? fn->as<Macro>().get_signature(ctx, raw_args)
                             : nullptr;

          if (sig)
          {
            if (sig->supports_exec_tree())
            {
              ptr_exec_node_v node_args;
              uptr_exec_node_v uptr_node_args;
              node_args.reserve(n.args.size());
              uptr_node_args.reserve(n.args.size());

              prepare_sequence(ctx, *sig->eval_pattern, n.args, uptr_node_args, node_args);

              return sig->invoke(ctx, node_args);
            }
            else
            {
              for (size_t i = 0; i < n.args.size(); i++)
              {
                auto& arg = n.args[i];
                if (sig->should_eval_arg(i))
                {
                  args.push_back(exec(ctx, *arg));
                }
                else
                {
                  args.push_back(arg->form);
                }
              }
            }
          }
          else
          {
            for (auto& arg : n.args)
            {
              args.push_back(exec(ctx, *arg));
            }
          }
          return fn->execute(ctx, args);
        }
        else if constexpr (std::is_same_v<T, ExecNodeList>)
        {
          return Lisple::NIL;
        }
      },
      node.data);
  }

} // namespace Lisple
