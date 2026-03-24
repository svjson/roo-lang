
#include "seq.h"

#include "../form.h"
#include "../type.h"
#include "value.h"

#include "lisple/exception.h"

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
    case RTValue::Type::MAP:
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

  size_t count(RTValue& v)
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
    default:
      return 1;
    }
  }

} // namespace Lisple
