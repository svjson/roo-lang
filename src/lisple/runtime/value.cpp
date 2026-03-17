
#include "value.h"

#include "../exception.h"

namespace Lisple
{
  RTValue RTValue::nil()
  {
    return RTValue{Form::NIL, {}};
  }

  RTValue RTValue::number(int v)
  {
    RTValue val;
    val.type = Form::NUMBER;
    val.number_value = {.num_type = NumberType::INT, .int_value = v};
    return val;
  }

  RTValue RTValue::number(long v)
  {
    RTValue val;
    val.type = Form::NUMBER;
    val.number_value = {.num_type = NumberType::LONG, .long_value = v};
    return val;
  }

  RTValue RTValue::number(double v)
  {
    RTValue val;
    val.type = Form::NUMBER;
    val.number_value = {.num_type = NumberType::FLOAT, .float_value = v};
    return val;
  }

  RTValue RTValue::object(Object* o)
  {
    RTValue val;
    val.type = Form::HOST_OBJECT;
    val.object_value = o;
    return val;
  }

  RTValue RTValue::function(Function* fn)
  {
    RTValue val;
    val.type = Form::FUNCTION;
    val.function_value = fn;
    return val;
  }

  RTValue to_rt_value(const Object& obj)
  {
    switch (obj.get_type())
    {
    case Form::NUMBER:
    {
      const auto& n = obj.as<Number>();
      switch (n.num_type)
      {
      case NumberType::INT:
        return RTValue::number(n.int_value());
      case NumberType::LONG:
        return RTValue::number(n.long_value());
      case NumberType::FLOAT:
        return RTValue::number(n.float_value());
      };
    }
    default:
      throw LispleException("Unsupported RTValue type: " + obj.to_string());
    }
  }
} // namespace Lisple
