
#include "lisple/runtime/exec_node.h"

#include <iostream>
#include <sstream>
#include <vector>

#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/eval_plan.h>
#include <lisple/runtime/node.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  int eval_executions = 0;
  int exec_executions = 0;

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
          std::stringstream ss;
          ss << indent << " - LiteralNode("
             << std::get<LiteralNode>(node.data).value->to_string() << ", RTValue("
             << n.value.get() << "))\n";
          return ss.str();
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
        else if constexpr (std::is_same_v<T, LambdaNode>)
        {
          return indent + " - <lambda>\n";
        }
        else if constexpr (std::is_same_v<T, KeyLookupNode>)
        {
          std::string result = indent + " - KeyLookupNode(" + n.keyword->to_string() + ")\n";
          indent += "  ";
          result += indent + " - Target:\n";
          result += to_string(*n.target, indent + "  ");
          return result;
        }
        else if constexpr (std::is_same_v<T, SpecialFormNode>)
        {
          std::string result = indent + " - SpecialFormNode(" + n.form->to_string() + ")\n";
          indent += "  ";
          if (n.values.size())
          {
            result += indent + " - Values:\n";
            for (auto& v : n.values)
            {
              result += indent + "   - " + v->to_string() + "\n";
            }
          }
          if (n.exec_nodes.size())
          {
            result += indent + " - Nodes:\n";
            for (auto& n : n.exec_nodes)
            {
              result += to_string(*n, indent + "  ");
            }
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

  sptr_rtval exec(Context& ctx, ExecNode& node)
  {
    exec_executions++;
    return std::visit(
      [&](auto& n) -> sptr_rtval
      {
        using T = std::decay_t<decltype(n)>;

        if constexpr (std::is_same_v<T, LiteralNode>)
        {
          return n.value;
        }
        else if constexpr (std::is_same_v<T, LookupNode>)
        {
          return ctx.lookup(n.identifier.to_string());
        }
        else if constexpr (std::is_same_v<T, MapNode>)
        {
          sptr_rtval_v elements;
          elements.reserve(n.elements.size());

          for (auto& lmnt : n.elements)
          {
            elements.push_back(exec(ctx, *lmnt));
          }

          return RTValue::map(elements);
        }
        else if constexpr (std::is_same_v<T, VectorNode>)
        {
          sptr_rtval_v elements;
          elements.reserve(n.elements.size());

          for (auto& lmnt : n.elements)
          {
            elements.push_back(exec(ctx, *lmnt));
          }

          return RTValue::vector(elements);
        }
        else if constexpr (std::is_same_v<T, KeyLookupNode>)
        {
          sptr_rtval target = exec(ctx, *n.target);
          return Lisple::Dict::get_property(target, *n.keyword);
        }
        else if constexpr (std::is_same_v<T, LambdaNode>)
        {
          return RTValue::executable(
            std::make_shared<Lisple::DetachedFunction>(ctx.detach(), n.lambda_fn));
        }
        else if constexpr (std::is_same_v<T, SpecialFormNode>)
        {
          return n.form->exec_node(ctx, n);
        }
        else if constexpr (std::is_same_v<T, CallNode>)
        {
          sptr_executable fn = n.static_callee;

          if (!fn)
          {
            sptr_rtval fn_val = exec(ctx, *n.callee);
            if (sptr_executable* executable = std::get_if<sptr_executable>(&fn_val->value))
            {
              fn = *executable;
            }
            else
            {
              std::cout << "Node: " << fn_val->to_string() << std::endl;
              throw LispleException("Node is not callable");
            }
          }

          Signature* sig = nullptr;
          Executable* x = fn.get();

          if (x && !x->requires_late_binding(n))
          {
            sig = x->get_signature(ctx, n.args);
          }

          if (sig)
          {
            if (sig->supports_rt_value())
            {
              sptr_rtval_v args;
              for (auto& arg : n.args)
              {
                args.push_back(exec(ctx, *arg));
              }
              auto retval = sig->invoke(ctx, args);

              return retval;
            }

            throw InvocationException("Signature does not support lowered execution: " +
                                      sig->to_string());
          }
          else if (x)
          {
            sptr_rtval_v val_args;
            for (auto& arg : n.args)
            {
              val_args.push_back(exec(ctx, *arg));
            }
            return x->execute(ctx, val_args);
          }

          throw InvocationException("Late-bound call target is not executable.");
        }
        else if constexpr (std::is_same_v<T, ExecNodeList>)
        {
          return Constant::NIL;
        }
      },
      node.data);
  }

} // namespace Lisple
