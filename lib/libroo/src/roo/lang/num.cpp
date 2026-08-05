
#include "roo/lang/num.h"

#include "roo/exception.h"
#include "roo/exec.h"
#include "roo/runtime/value.h"

#include <cctype>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

namespace Roo
{
  namespace
  {
    sptr_val integral_number(std::int64_t value)
    {
      if (value >= std::numeric_limits<std::int32_t>::min() &&
          value <= std::numeric_limits<std::int32_t>::max())
      {
        return Value::number(static_cast<int>(value));
      }

      return Value::number(
        Value::Number{.num_type = Value::NumberType::LONG, .long_value = value});
    }

    sptr_val parse_int_string(const std::string& value)
    {
      try
      {
        size_t parsed = 0;
        const std::int64_t result = std::stoll(value, &parsed);
        while (parsed < value.size() &&
               std::isspace(static_cast<unsigned char>(value[parsed])))
        {
          parsed++;
        }
        if (parsed != value.size())
        {
          return Constant::NIL;
        }
        return integral_number(result);
      }
      catch (const std::invalid_argument&)
      {
        return Constant::NIL;
      }
      catch (const std::out_of_range&)
      {
        return Constant::NIL;
      }
    }

    sptr_val float_to_integral_number(double value)
    {
      if (!std::isfinite(value))
      {
        return Constant::NIL;
      }

      const long double min =
        static_cast<long double>(std::numeric_limits<std::int64_t>::min());
      const long double max =
        static_cast<long double>(std::numeric_limits<std::int64_t>::max());
      const long double candidate = static_cast<long double>(value);
      if (candidate < min || candidate > max)
      {
        return Constant::NIL;
      }

      return integral_number(static_cast<std::int64_t>(value));
    }
  } // namespace

  /** EvenPFunction - roo/even? */
  FUNC_IMPL(EvenPFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&EvenPFunction::exec_even))))

  EXEC_BODY(EvenPFunction, exec_even)
  {
    return std::get<const Value::Number>(args[0]->value).get_int() % 2 == 0
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  /** IncFunction - roo/inc */
  FUNC_IMPL(IncFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&IncFunction::exec_inc))))

  EXEC_BODY(IncFunction, exec_inc)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    return Value::number(args[0]->num() +
                         Value::Number{.num_type = Value::NumberType::INT, .int_value = 1});
  }

  /** DecFunction - roo/dec */
  FUNC_IMPL(DecFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&DecFunction::exec_dec))))

  EXEC_BODY(DecFunction, exec_dec)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    return Value::number(args[0]->num() -
                         Value::Number{.num_type = Value::NumberType::INT, .int_value = 1});
  }

  /** IntFunction - roo/int */
  FUNC_IMPL(IntFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&IntFunction::exec_to_int))))

  EXEC_BODY(IntFunction, exec_to_int)
  {
    sptr_val& obj = args[0];

    if (!obj || obj->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    if (Type::NUMBER.is_type_of(*obj))
    {
      const Value::Number& num = std::get<const Value::Number>(obj->value);
      switch (num.num_type)
      {
      case Value::NumberType::INT:
        return Value::number(num.int_value);
      case Value::NumberType::LONG:
        return integral_number(num.long_value);
      case Value::NumberType::FLOAT:
      default:
        return float_to_integral_number(num.float_value);
      }
    }
    else if (Type::CHAR.is_type_of(*obj))
    {
      return integral_number(static_cast<std::int64_t>(std::get<char>(obj->value)));
    }
    else if (Type::STRING.is_type_of(*obj))
    {
      return parse_int_string(obj->str());
    }

    throw RooException("Cannot convert " + obj->to_string() + " to integer.");
  }

  /** FloatFunction - roo/float */
  FUNC_IMPL(FloatFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&FloatFunction::exec_to_float))))

  EXEC_BODY(FloatFunction, exec_to_float)
  {
    sptr_val& obj = args[0];

    if (!obj || obj->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    if (Type::NUMBER.is_type_of(*obj))
    {
      return Value::number(
        Value::Number{.num_type = Value::NumberType::FLOAT,
                      .float_value = std::get<const Value::Number>(obj->value).get_float()});
    }
    else if (Type::CHAR.is_type_of(*obj))
    {
      return Value::number(
        Value::Number{.num_type = Value::NumberType::FLOAT,
                      .float_value = static_cast<float>(std::get<char>(obj->value))});
    }
    else if (Type::STRING.is_type_of(*obj))
    {
      try
      {
        size_t parsed = 0;
        const std::string value = obj->str();
        const float result = std::stof(value, &parsed);
        while (parsed < value.size() &&
               std::isspace(static_cast<unsigned char>(value[parsed])))
        {
          parsed++;
        }
        if (parsed != value.size())
        {
          return Constant::NIL;
        }
        return Value::number(
          Value::Number{.num_type = Value::NumberType::FLOAT, .float_value = result});
      }
      catch (const std::invalid_argument&)
      {
        return Constant::NIL;
      }
      catch (const std::out_of_range&)
      {
        return Constant::NIL;
      }
    }

    throw RooException("Cannot convert " + obj->to_string() + " to float.");
  }

  /** MaxFunction - roo/max */
  FUNC_IMPL(MaxFunction,
            SIG((FN_ARGS((&Type::NUMBER), (VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MaxFunction::exec_max))))

  EXEC_BODY(MaxFunction, exec_max)
  {
    float result_val = 0;
    size_t result_index = args.size();

    for (size_t i = 0; i < args.size(); i++)
    {
      if (args[i]->type == Value::Type::NIL)
      {
        continue;
      }

      float num = std::get<const Value::Number>(args[i]->value).get_float();
      if (result_index == args.size() || num > result_val)
      {
        result_val = num;
        result_index = i;
      }
    }

    if (result_index == args.size())
    {
      return Constant::NIL;
    }

    return args[result_index];
  }

  /** ClampFunction - roo/clamp */
  FUNC_IMPL(ClampFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&ClampFunction::exec_clamp))))

  EXEC_BODY(ClampFunction, exec_clamp)
  {
    if (args[0]->type == Value::Type::NIL ||
        args[1]->type == Value::Type::NIL ||
        args[2]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    const float value = std::get<const Value::Number>(args[0]->value).get_float();
    const float low   = std::get<const Value::Number>(args[1]->value).get_float();
    const float high  = std::get<const Value::Number>(args[2]->value).get_float();

    if (value <= low)  { return args[1]; }
    if (value >= high) { return args[2]; }
    return args[0];
  }

  /** MinFunction - roo/min */
  FUNC_IMPL(MinFunction,
            SIG((FN_ARGS((&Type::NUMBER), (VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MinFunction::exec_min))))

  EXEC_BODY(MinFunction, exec_min)
  {
    float result_val = 0;
    size_t result_index = args.size();

    for (size_t i = 0; i < args.size(); i++)
    {
      if (args[i]->type == Value::Type::NIL)
      {
        continue;
      }

      float num = std::get<const Value::Number>(args[i]->value).get_float();
      if (result_index == args.size() || num < result_val)
      {
        result_val = num;
        result_index = i;
      }
    }

    if (result_index == args.size())
    {
      return Constant::NIL;
    }

    return args[result_index];
  }

  /** NumberPFunction - roo/number? */
  FUNC_IMPL(NumberPFunction,
            SIG((FN_ARGS((&Roo::Type::ANY)), EXEC_DISPATCH(&NumberPFunction::exec_num))))

  EXEC_BODY(NumberPFunction, exec_num)
  {
    return args[0]->type == Value::Type::NUMBER ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** OddPFunction - roo/odd? */
  FUNC_IMPL(OddPFunction,
            SIG((FN_ARGS((&Roo::Type::NUMBER)), EXEC_DISPATCH(&OddPFunction::exec_odd))))

  EXEC_BODY(OddPFunction, exec_odd)
  {
    return std::get<const Value::Number>(args[0]->value).get_int() % 2 != 0
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  /** ZeroPFunction - roo/zero? */
  FUNC_IMPL(ZeroPFunction,
            SIG((FN_ARGS((&Type::NUMBER), (VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&ZeroPFunction::exec_zero))))

  EXEC_BODY(ZeroPFunction, exec_zero)
  {
    for (auto& arg : args)
    {
      if (arg->f64() != 0.0)
      {
        return Constant::BOOL_FALSE;
      }
    }

    return Constant::BOOL_TRUE;
  }

} // namespace Roo
