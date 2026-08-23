
#include "roo/runtime/exec_node.h"

#include <exception>
#include <sstream>
#include <vector>

#include <roo/benchmark/counters.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/eval_plan.h>
#include <roo/runtime/node.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    [[noreturn]]
    void rethrow_with_call_context(Context& ctx,
                                   const ExecNode& node,
                                   const CallNode& call,
                                   std::exception& e,
                                   const sptr_val_v* arguments = nullptr)
    {
      const std::string source = node.source.valid() ? ctx.describe_source(node.source) : "";
      if (auto* roo_exception = dynamic_cast<RooException*>(&e))
      {
        roo_exception->set_diagnostic_options(ctx.source_diagnostics_enabled(),
                                              ctx.call_stack_diagnostics_enabled());
        if (arguments)
        {
          roo_exception->add_call_context("calling",
                                          call.callee_name,
                                          *arguments,
                                          source,
                                          true);
        }
        else
        {
          roo_exception->add_call_context("calling", call.callee_name, source, true);
        }
        throw;
      }

      InvocationException wrapped(e.what());
      wrapped.set_cause(std::current_exception());
      wrapped.set_diagnostic_options(ctx.source_diagnostics_enabled(),
                                     ctx.call_stack_diagnostics_enabled());
      if (arguments)
      {
        wrapped.add_call_context("calling", call.callee_name, *arguments, source, true);
      }
      else
      {
        wrapped.add_call_context("calling", call.callee_name, source, true);
      }
      throw wrapped;
    }
  } // namespace

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
             << std::get<LiteralNode>(node.data).value->to_string() << ", Value("
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

  sptr_val exec(Context& ctx, ExecNode& node)
  {
    ROO_BENCHMARK_INC(exec_executions);
    return std::visit(
      [&](auto& n) -> sptr_val
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
          sptr_val_v elements;
          elements.reserve(n.elements.size());

          for (auto& lmnt : n.elements)
          {
            elements.push_back(exec(ctx, *lmnt));
          }

          return Value::map(elements);
        }
        else if constexpr (std::is_same_v<T, VectorNode>)
        {
          sptr_val_v elements;
          elements.reserve(n.elements.size());

          for (auto& lmnt : n.elements)
          {
            elements.push_back(exec(ctx, *lmnt));
          }

          return Value::vector(elements);
        }
        else if constexpr (std::is_same_v<T, KeyLookupNode>)
        {
          sptr_val target = exec(ctx, *n.target);
          return Roo::Dict::get_property(target, *n.keyword);
        }
        else if constexpr (std::is_same_v<T, LambdaNode>)
        {
          return Value::executable(
            std::make_shared<Roo::DetachedFunction>(ctx.detach(), n.lambda_fn));
        }
        else if constexpr (std::is_same_v<T, SpecialFormNode>)
        {
          return n.form->exec_node(ctx, n);
        }
        else if constexpr (std::is_same_v<T, CallNode>)
        {
          sptr_executable fn = n.static_callee;
          sptr_val dynamic_callee;

          if (!fn)
          {
            dynamic_callee = exec(ctx, *n.callee);
            if (sptr_executable* executable =
                  std::get_if<sptr_executable>(&dynamic_callee->value))
            {
              fn = *executable;
            }
          }

          Signature* sig = nullptr;
          Executable* x = fn.get();

          if (x && !x->requires_late_binding(n))
          {
            sig = x->get_signature(ctx, n.args);
          }

          auto invoke_with_evaluated_args = [&](auto&& invocation) -> sptr_val
          {
            sptr_val_v args;
            args.reserve(n.args.size());
            try
            {
              for (auto& arg : n.args)
              {
                args.push_back(exec(ctx, *arg));
              }
            }
            catch (std::exception& e)
            {
              rethrow_with_call_context(ctx, node, n, e);
            }

            try
            {
              return invocation(args);
            }
            catch (std::exception& e)
            {
              rethrow_with_call_context(ctx, node, n, e, &args);
            }
          };

          if (dynamic_callee && dynamic_callee->type != Value::Type::FUNCTION)
          {
            return invoke_with_evaluated_args(
              [&](sptr_val_v& args) { return invoke_callable(ctx, dynamic_callee, args); });
          }
          if (sig)
          {
            return invoke_with_evaluated_args([&](sptr_val_v& args)
                                              { return sig->invoke(ctx, args); });
          }
          if (x)
          {
            return invoke_with_evaluated_args([&](sptr_val_v& args)
                                              { return x->execute(ctx, args); });
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

} // namespace Roo
