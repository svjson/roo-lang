
#include "exec_node.h"

#include "../context.h"
#include "../exception.h"
#include "../exec.h"
#include "../type.h"
#include "exec_tree.h"
#include <vector>

namespace Lisple
{
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

        return indent + " UNHANDLED NODE TYPE\n";
      },

      node.data);
  }

  std::unique_ptr<ExecNode> lower_expr(const sptr_sobject& obj)
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
        elements.push_back(lower_expr(child));
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
        elements.push_back(lower_expr(child));
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
      return lower_literal(obj);

    case Form::WORD:
      return std::make_unique<ExecNode>(obj, LookupNode(obj->as<Word>()));

    case Form::LIST:
    {
      auto& list = obj->as<List>();
      const auto& children = list.get_children();

      if (children.empty()) throw LispleException("Cannot lower empty list");

      std::unique_ptr<ExecNode> callee = lower_expr(children[0]);

      std::vector<std::unique_ptr<ExecNode>> args;
      args.reserve(children.size() - 1);

      for (size_t i = 1; i < children.size(); i++)
      {
        args.push_back(lower_expr(children[i]));
      }

      return std::make_unique<ExecNode>(obj, CallNode(std::move(callee), std::move(args)));
    }

    default:
      throw LispleException("Lowering not implemented for form");
    }
  }

  std::unique_ptr<ExecNode> lower_literal(const sptr_sobject& obj)
  {
    switch (obj->get_type())
    {
    case Form::CHAR:
      return std::make_unique<ExecNode>(
        obj,
        LiteralNode(RTValue::character(obj->as<Char>().value), obj));
    case Form::LIST:
    {
      sptr_rtval_v elements;
      elements.reserve(obj->get_children().size());
      for (auto& l : obj->get_children())
      {
        auto lit_child = lower_literal(l);
        elements.push_back(std::get<LiteralNode>(lit_child->data).value);
      }
      return std::make_unique<ExecNode>(obj, LiteralNode(RTValue::list(elements), obj));
    }
    case Form::ARRAY:
    {
      sptr_rtval_v elements;
      elements.reserve(obj->get_children().size());
      for (auto& l : obj->get_children())
      {
        auto lit_child = lower_literal(l);
        elements.push_back(std::get<LiteralNode>(lit_child->data).value);
      }
      return std::make_unique<ExecNode>(obj, LiteralNode(RTValue::vector(elements), obj));
    }
    case Form::MAP:
    {
      sptr_rtval_v elements;
      elements.reserve(obj->get_children().size());
      for (auto& l : obj->get_children())
      {
        auto lit_child = lower_literal(l);
        elements.push_back(std::get<LiteralNode>(lit_child->data).value);
      }
      return std::make_unique<ExecNode>(obj, LiteralNode(RTValue::map(elements), obj));
    }
    case Form::HOST_OBJECT:
      return std::make_unique<ExecNode>(obj, LiteralNode(RTValue::object(obj.get()), obj));
    case Form::KEY:
      return std::make_unique<ExecNode>(
        obj,
        LiteralNode(RTValue::keyword(obj->as<Key>().value), obj));
    case Form::NUMBER:
    {
      auto& num_obj = obj->as<Lisple::Number>();
      switch (num_obj.num_type)
      {
      case Lisple::NumberType::INT:
        return std::make_unique<ExecNode>(
          obj,
          LiteralNode(RTValue::number(num_obj.int_value()), obj));
      case Lisple::NumberType::LONG:
        return std::make_unique<ExecNode>(
          obj,
          LiteralNode(RTValue::number(num_obj.long_value()), obj));

      case Lisple::NumberType::FLOAT:
        return std::make_unique<ExecNode>(
          obj,
          LiteralNode(RTValue::number(num_obj.float_value()), obj));
      }

      throw LispleException("Unexpected number type");
    }
    case Form::BOOLEAN:
    case Form::B_TRUE:
    case Form::B_FALSE:
      return std::make_unique<ExecNode>(
        obj,
        LiteralNode(RTValue::boolean(obj->as<Boolean>().value), obj));
    case Form::STRING:
      return std::make_unique<ExecNode>(obj,
                                        LiteralNode(RTValue::string(obj->to_string()), obj));
    case Form::WORD:
      return std::make_unique<ExecNode>(obj,
                                        LiteralNode(RTValue::symbol(obj->to_string()), obj));
    case Form::SYMBOL:
      return std::make_unique<ExecNode>(obj,
                                        LiteralNode(RTValue::symbol(obj->to_string()), obj));
    default:
      throw LispleException("Lowering not implemented for form: " + obj->to_string());
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

              for (size_t i = 0; i < n.args.size(); i++)
              {
                auto& arg = n.args[i];
                if (sig->should_eval_arg(i))
                {
                  uptr_node_args.push_back(lower_expr(exec(ctx, *arg)));
                  node_args.push_back(uptr_node_args.back().get());
                }
                else
                {
                  uptr_node_args.push_back(lower_literal(arg->form));
                  node_args.push_back(uptr_node_args.back().get());
                }
              }

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
      },
      node.data);
  }

} // namespace Lisple
