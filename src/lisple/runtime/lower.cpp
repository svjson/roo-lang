
#include "lisple/runtime/lower.h"

#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/namespace.h>
#include <lisple/runtime/node.h>
namespace Lisple
{
  int lowered_expressions = 0;
  int lowered_literals = 0;
  int lower_time_exec_resolutions = 0;
  int lower_time_exec_unresolved = 0;

  bool LowerContext::is_allow_lookup() const
  {
    return frames.back().allow_lookup;
  }

  bool LowerContext::is_literal_mode() const
  {
    return frames.back().literal_mode;
  }

  void LowerContext::push_literal_mode()
  {
    frames.push_back({false, true});
  }

  void LowerContext::push(const LCtxFrame& frame)
  {
    frames.push_back(frame);
  }

  void LowerContext::pop()
  {
    if (frames.size() > 1)
    {
      frames.pop_back();
    }
  }

  std::unique_ptr<ExecNode> lower_expr(LowerContext& ctx, const sptr_sobject& obj)
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
        elements.push_back(lower_expr(ctx, child));
      }

      lowered_expressions++;
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
        elements.push_back(lower_expr(ctx, child));
      }

      lowered_expressions++;
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
    {
      if (ctx.is_allow_lookup() && ctx.ctx)
      {
        auto static_val = ctx.ctx->get_current_namespace()->lookup_symbol(obj->as<Word>());
        if (static_val)
        {
          lowered_literals++;
          return std::make_unique<ExecNode>(static_val);
        }
      }
      lowered_expressions++;
      return std::make_unique<ExecNode>(obj, LookupNode(obj->as<Word>()));
    }

    case Form::LIST:
    {
      auto& list = obj->as<List>();
      const auto& children = list.get_children();

      if (children.empty()) throw LispleException("Cannot lower empty list");

      if (list.is_quoted() || ctx.is_literal_mode())
      {
        return lower_literal(obj);
      }
      else
      {
        ctx.push({true});
        uptr_exec_node callee = lower_expr(ctx, children[0]);
        ctx.pop();

        auto* literal_callee = std::get_if<LiteralNode>(&callee.get()->data);

        if (literal_callee)
        {
          if (literal_callee->value->type == RTValue::Type::KEYWORD)
          {
            if (children.size() != 2)
            {
              throw LispleException("Invalid key lookup form: " + list.to_string());
            }

            lowered_expressions++;
            lower_time_exec_resolutions++;
            return std::make_unique<ExecNode>(
              KeyLookupNode(literal_callee->value, lower_expr(ctx, children[1])));
          }
        }

        uptr_exec_node_v args;
        args.reserve(children.size() - 1);

        // FIXME: Temporary hack to stop (:require ..) from lowering as keylookupnode
        if (children[0]->to_string() == "ns")
        {
          ctx.push_literal_mode();
        }

        for (size_t i = 1; i < children.size(); i++)
        {
          args.push_back(lower_expr(ctx, children[i]));
        }

        if (children[0]->to_string() == "ns")
        {
          ctx.pop();
        }

        CallNode call_node = CallNode(std::move(callee), std::move(args));

        if (literal_callee)
        {
          if (auto* sptr_sobject_val =
                std::get_if<sptr_sobject>(&literal_callee->value->value))
          {
            call_node.cached_fn = *sptr_sobject_val;
            lower_time_exec_resolutions++;
          }
        }

        if (call_node.cached_fn == nullptr)
        {
          lower_time_exec_unresolved++;
        }

        lowered_expressions++;

        auto node = std::make_unique<ExecNode>(obj, std::move(call_node));

        return node;
      }
    }

    case Form::DISCARD:
      lowered_expressions++;
      return std::make_unique<ExecNode>(Lisple::Constant::NIL);

    default:
      throw LispleException("Lowering not implemented for form: " +
                            std::to_string((int)obj->get_type()));
    }
  }

  std::unique_ptr<ExecNode> lower_literal(const sptr_sobject& obj)
  {
    lowered_literals++;
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
    case Form::NIL:
      return std::make_unique<ExecNode>(Constant::NIL);
    case Form::HOST_OBJECT:
    {
      sptr_sobject ho = obj;
      return std::make_unique<ExecNode>(obj, LiteralNode(RTValue::object(ho), obj));
    }
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
      return std::make_unique<ExecNode>(
        obj,
        LiteralNode(RTValue::string(Value<std::string>::value_of(*obj)), obj));
    case Form::SYMBOL:
    case Form::WORD:
      return std::make_unique<ExecNode>(
        obj,
        LiteralNode(RTValue::symbol(Value<std::string>::value_of(*obj)), obj));
    case Form::DISCARD:
      return std::make_unique<ExecNode>(Lisple::Constant::NIL);
    default:
      throw LispleException("Lowering not implemented for form: " + obj->to_string());
    }
  }

} // namespace Lisple
