
#include "lisple/runtime/seq.h"

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/host/object.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  sptr_rtval_v get_children(RTValue& v)
  {
    switch (v.type)
    {
    case RTValue::Type::STRING:
    {
      sptr_rtval_v chars;
      std::string& str = std::get<std::string>(v.value);
      for (size_t i = 0; i < str.size(); i++)
      {
        chars.push_back(RTValue::character(str.at(i)));
      }
      return chars;
    }
    case RTValue::Type::NATIVE_OBJECT:
    {
      sptr_native_obj obj = v.nobj();
      return obj->native_children();
    }
    case RTValue::Type::NIL:
      return {};
    case RTValue::Type::MAP:
    case RTValue::Type::LIST:
    case RTValue::Type::VECTOR:
      return std::get<sptr_rtval_v>(v.value);
    case RTValue::Type::OBJECT:
    {
      sptr_sobject& val = std::get<sptr_sobject>(v.value);
      sptr_rtval_v elements;
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

  sptr_rtval get_child(const RTValue& seq, size_t index)
  {
    switch (seq.type)
    {
    case RTValue::Type::NIL:
      return Constant::NIL;
    case RTValue::Type::VECTOR:
    case RTValue::Type::MAP:
    {
      const sptr_rtval_v& values = std::get<sptr_rtval_v>(seq.value);
      if (index < values.size())
      {
        return values[index];
      }
      return Lisple::Constant::NIL;
    }
    case RTValue::Type::OBJECT:
    {
      if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(seq.obj().get()))
      {
        return get_child(*wrapper->val, index);
      }

      return to_rt_value(seq.obj()->get_children().at(index));
    }
    case RTValue::Type::NATIVE_OBJECT:
    {
      sptr_rtval_v values = seq.nobj()->native_children();
      if (index < values.size())
      {
        return values[index];
      }
      return Lisple::Constant::NIL;
    }
    case RTValue::Type::STRING:
    {
      const std::string& str = seq.str();
      if (index >= str.size()) return Lisple::Constant::NIL;
      return RTValue::character(str.at(index));
    }
    default:
      throw LispleException("get_child is not implemented for type: " +
                            std::to_string((int)seq.type));
    }
  }

  void set_child(RTValue& seq, size_t index, const sptr_rtval& value)
  {
    switch (seq.type)
    {
    case RTValue::Type::VECTOR:
    case RTValue::Type::LIST:
    {
      sptr_rtval_v& elements = std::get<sptr_rtval_v>(seq.value);
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

  sptr_rtval pop_child(RTValue& seq)
  {
    switch (seq.type)
    {
    case RTValue::Type::VECTOR:
    {
      auto& vec = std::get<sptr_rtval_v>(seq.value);
      auto item = vec.back();
      vec.pop_back();
      return item;
    }
    default:
      throw LispleException("get_child is not implemented for type: " +
                            std::to_string((int)seq.type));
    }
  }

  sptr_rtval peek_child(RTValue& seq)
  {
    switch (seq.type)
    {
    case RTValue::Type::VECTOR:
    {
      auto& vec = std::get<sptr_rtval_v>(seq.value);
      if (vec.empty()) return Constant::NIL;
      return vec.back();
    }
    case RTValue::Type::OBJECT:
    {
      return to_rt_value(seq.obj()->get_children().back());
    }
    default:
      throw LispleException("peek_child is not implemented for type: " +
                            std::to_string((int)seq.type));
    }
  }

  size_t count(const RTValue& v)
  {
    switch (v.type)
    {
    case RTValue::Type::STRING:
      return std::get<std::string>(v.value).size();
    case RTValue::Type::VECTOR:
    case RTValue::Type::LIST:
      return std::get<sptr_rtval_v>(v.value).size();
    case RTValue::Type::MAP:
      return std::get<sptr_rtval_v>(v.value).size() / 2;
    case RTValue::Type::OBJECT:
      return std::get<sptr_sobject>(v.value)->size();
    case RTValue::Type::NATIVE_OBJECT:
      return std::get<sptr_native_obj>(v.value)->size();
    default:
      return 1;
    }
  }

  void append(RTValue& seq, const sptr_rtval& value)
  {
    if (seq.type == RTValue::Type::VECTOR)
    {
      std::get<sptr_rtval_v>(seq.value).push_back(value);
    }
    else
    {
      throw TypeError("append not implemented for RTValue type: " +
                      std::to_string((int)seq.type));
    }
  }

} // namespace Lisple
