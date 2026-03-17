
#include "exec_node.h"

#include "ast.h"
#include "context.h"
#include "exception.h"
#include "exec.h"
#include "type.h"

namespace Lisple
{
  std::unique_ptr<ExecNode> lower(const sptr_sobject& obj)
  {
    switch (obj->get_type())
    {
    case Form::MAP:
    {
      auto& map = obj->as<Map>();
      const auto& children = map.get_children();

      std::vector<uptr_exec_node> elements;
      elements.reserve(children.size());

      for (auto& child : children)
      {
        elements.push_back(lower(child));
      }

      return std::make_unique<ExecNode>(obj, MapNode(std::move(elements)));
    }
    case Form::ARRAY:
    {
      auto& vec = obj->as<Array>();
      const auto& children = vec.get_children();

      std::vector<uptr_exec_node> elements;
      elements.reserve(children.size());

      for (auto& child : children)
      {
        elements.push_back(lower(child));
      }

      return std::make_unique<ExecNode>(obj, VectorNode(std::move(elements)));
    }
    case Form::FUNCTION:
    case Form::CHAR:
    case Form::NUMBER:
    case Form::STRING:
    case Form::BOOLEAN:
    case Form::SYMBOL:
    case Form::HOST_OBJECT:
    case Form::KEY:
    case Form::B_TRUE:
    case Form::B_FALSE:
    case Form::NIL:
      return std::make_unique<ExecNode>(obj, LiteralNode(obj));

    case Form::WORD:
      return std::make_unique<ExecNode>(obj, LookupNode(obj->as<Word>()));

    case Form::LIST:
    {
      auto& list = obj->as<List>();
      const auto& children = list.get_children();

      if (children.empty()) throw LispleException("Cannot lower empty list");

      std::unique_ptr<ExecNode> callee = lower(children[0]);

      std::vector<std::unique_ptr<ExecNode>> args;
      args.reserve(children.size() - 1);

      for (size_t i = 1; i < children.size(); i++)
      {
        args.push_back(lower(children[i]));
      }

      return std::make_unique<ExecNode>(obj, CallNode(std::move(callee), std::move(args)));
    }

    default:
      throw LispleException("Lowering not implemented for form");
    }
  }

  sptr_sobject exec(Context& ctx, const ExecNode& node)
  {
    return std::visit(
      [&](auto const& n) -> sptr_sobject
      {
        using T = std::decay_t<decltype(n)>;

        if constexpr (std::is_same_v<T, LiteralNode>)
        {
          return n.value;
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
          sptr_sobject fn = exec(ctx, *n.callee);

          sptr_sobject_v args;
          args.reserve(n.args.size());

          sptr_sobject_v raw_args = node.form->as<List>().tail();

          Signature* sig = fn->get_type() == Form::MACRO
                             ? fn->as<Macro>().get_signature(ctx, raw_args)
                             : nullptr;

          if (sig)
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
          else
          {
            for (auto& arg : n.args)
            {
              args.push_back(exec(ctx, *arg));
            }
          }

          return fn->execute(ctx, args);
        }
      },
      node.data);
  }

} // namespace Lisple
