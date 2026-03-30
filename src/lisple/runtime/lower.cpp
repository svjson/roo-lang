
#include "lower.h"

#include "../exception.h"

namespace Lisple
{
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

      uptr_exec_node callee = lower_expr(children[0]);
      uptr_exec_node_v args;
      args.reserve(children.size() - 1);

      for (size_t i = 1; i < children.size(); i++)
      {
        args.push_back(lower_expr(children[i]));
      }

      return std::make_unique<ExecNode>(obj, CallNode(std::move(callee), std::move(args)));
    }

    default:
      throw LispleException("Lowering not implemented for form: " +
                            std::to_string((int)obj->get_type()));
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
    default:
      throw LispleException("Lowering not implemented for form: " + obj->to_string());
    }
  }

} // namespace Lisple
