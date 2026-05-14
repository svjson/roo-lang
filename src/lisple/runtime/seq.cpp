
#include "lisple/runtime/seq.h"

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/host/object.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  sptr_val_v get_children(Value& v)
  {
    switch (v.type)
    {
    case Value::Type::STRING:
    {
      sptr_val_v chars;
      std::string& str = std::get<std::string>(v.value);
      for (size_t i = 0; i < str.size(); i++)
      {
        chars.push_back(Value::character(str.at(i)));
      }
      return chars;
    }
    case Value::Type::NATIVE_OBJECT:
    {
      sptr_native_obj obj = v.nobj();
      return obj->native_children();
    }
    case Value::Type::NIL:
      return {};
    case Value::Type::MAP:
    case Value::Type::LIST:
    case Value::Type::VECTOR:
      return std::get<sptr_val_v>(v.value);
    case Value::Type::OBJECT:
    {
      sptr_ast_node& val = std::get<sptr_ast_node>(v.value);
      sptr_val_v elements;
      if (val->get_type() == Form::HOST_OBJECT || val->get_type() == Form::HOST_SEQ)
      {
        for (auto& child : val->get_children())
        {
          elements.push_back(to_rt_value(child));
        }

        return elements;
      }
    }
    default:
      break;
    }
    throw LispleException("Cannot get children from type: " + std::to_string((int)v.type));
  }

  sptr_val get_child(const Value& seq, size_t index)
  {
    switch (seq.type)
    {
    case Value::Type::NIL:
      return Constant::NIL;
    case Value::Type::VECTOR:
    case Value::Type::MAP:
    {
      const sptr_val_v& values = std::get<sptr_val_v>(seq.value);
      if (index < values.size())
      {
        return values[index];
      }
      return Lisple::Constant::NIL;
    }
    case Value::Type::OBJECT:
    {
      if (auto* wrapper = dynamic_cast<AST::RuntimeValueWrapper*>(seq.obj().get()))
      {
        return get_child(*wrapper->val, index);
      }

      return to_rt_value(seq.obj()->get_children().at(index));
    }
    case Value::Type::NATIVE_OBJECT:
    {
      sptr_val_v values = seq.nobj()->native_children();
      if (index < values.size())
      {
        return values[index];
      }
      return Lisple::Constant::NIL;
    }
    case Value::Type::STRING:
    {
      const std::string& str = seq.str();
      if (index >= str.size()) return Lisple::Constant::NIL;
      return Value::character(str.at(index));
    }
    default:
      throw LispleException("get_child is not implemented for type: " +
                            std::to_string((int)seq.type));
    }
  }

  void set_child(Value& seq, size_t index, const sptr_val& value)
  {
    switch (seq.type)
    {
    case Value::Type::VECTOR:
    case Value::Type::LIST:
    {
      sptr_val_v& elements = std::get<sptr_val_v>(seq.value);
      while (elements.size() <= index)
      {
        elements.push_back(Lisple::Constant::NIL);
      }
      elements[index] = value;
      break;
    }
    default:
      throw TypeError("set_child is not implemented for type: " +
                      std::to_string((int)seq.type));
    }
  }

  sptr_val pop_child(Value& seq)
  {
    switch (seq.type)
    {
    case Value::Type::VECTOR:
    {
      auto& vec = std::get<sptr_val_v>(seq.value);
      auto item = vec.back();
      vec.pop_back();
      return item;
    }
    default:
      throw LispleException("get_child is not implemented for type: " +
                            std::to_string((int)seq.type));
    }
  }

  sptr_val peek_child(Value& seq)
  {
    switch (seq.type)
    {
    case Value::Type::VECTOR:
    {
      auto& vec = std::get<sptr_val_v>(seq.value);
      if (vec.empty()) return Constant::NIL;
      return vec.back();
    }
    case Value::Type::OBJECT:
    {
      return to_rt_value(seq.obj()->get_children().back());
    }
    default:
      throw LispleException("peek_child is not implemented for type: " +
                            std::to_string((int)seq.type));
    }
  }

  size_t count(const Value& v)
  {
    switch (v.type)
    {
    case Value::Type::STRING:
      return std::get<std::string>(v.value).size();
    case Value::Type::VECTOR:
    case Value::Type::LIST:
      return std::get<sptr_val_v>(v.value).size();
    case Value::Type::MAP:
      return std::get<sptr_val_v>(v.value).size() / 2;
    case Value::Type::OBJECT:
      return std::get<sptr_ast_node>(v.value)->size();
    case Value::Type::NATIVE_OBJECT:
      return std::get<sptr_native_obj>(v.value)->size();
    default:
      return 1;
    }
  }

  void append(Value& seq, const sptr_val& value)
  {
    if (seq.type == Value::Type::VECTOR)
    {
      std::get<sptr_val_v>(seq.value).push_back(value);
    }
    else
    {
      throw TypeError("append not implemented for Value type: " +
                      std::to_string((int)seq.type));
    }
  }

} // namespace Lisple
