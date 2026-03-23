
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
} // namespace Lisple
