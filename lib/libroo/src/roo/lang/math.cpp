
#include <cmath>
#include <cstdint>
#include <limits>

#include <roo/lang/math.h>

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

    sptr_val rounded_number(const Value::Number& num, double (*round_fn)(double))
    {
      switch (num.num_type)
      {
      case Value::NumberType::INT:
        return Value::number(num.int_value);
      case Value::NumberType::LONG:
        return integral_number(num.long_value);
      case Value::NumberType::FLOAT:
      default:
        break;
      }

      const double rounded = round_fn(num.float_value);
      if (!std::isfinite(rounded))
      {
        return Constant::NIL;
      }

      const long double min =
        static_cast<long double>(std::numeric_limits<std::int64_t>::min());
      const long double max =
        static_cast<long double>(std::numeric_limits<std::int64_t>::max());
      const long double candidate = static_cast<long double>(rounded);
      if (candidate < min || candidate > max)
      {
        return Constant::NIL;
      }

      return integral_number(static_cast<std::int64_t>(rounded));
    }
  } // namespace

  /* AbsFunction - abs */
  FUNC_IMPL(AbsFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&AbsFunction::exec_abs))))

  EXEC_BODY(AbsFunction, exec_abs)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    return Value::number(
      std::abs(std::get<const Value::Number>(args[0]->value).get_float()));
  }

  /* CeilFunction - ceil */
  FUNC_IMPL(CeilFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&CeilFunction::exec_ceil))))

  EXEC_BODY(CeilFunction, exec_ceil)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    const Value::Number& num = std::get<const Value::Number>(args[0]->value);
    return rounded_number(num, std::ceil);
  }

  /* FloorFunction - floor */
  FUNC_IMPL(FloorFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&FloorFunction::exec_floor))))

  EXEC_BODY(FloorFunction, exec_floor)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    const Value::Number& num = std::get<const Value::Number>(args[0]->value);
    return rounded_number(num, std::floor);
  }

  /* RoundFunction - round */
  FUNC_IMPL(RoundFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&RoundFunction::exec_round))))

  EXEC_BODY(RoundFunction, exec_round)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    const Value::Number& num = std::get<const Value::Number>(args[0]->value);
    return rounded_number(num, std::round);
  }

  /* SinFunction - sin */
  FUNC_IMPL(SinFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&SinFunction::exec_sin))))

  EXEC_BODY(SinFunction, exec_sin)
  {
    return Value::number(
      std::sin(std::get<const Value::Number>(args[0]->value).get_float()));
  }

  /* CosFunction - cos */
  FUNC_IMPL(CosFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&CosFunction::exec_cos))))

  EXEC_BODY(CosFunction, exec_cos)
  {
    return Value::number(
      std::cos(std::get<const Value::Number>(args[0]->value).get_float()));
  }

  /* PowFunction - pow */
  FUNC_IMPL(PowFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&PowFunction::exec_pow))))

  EXEC_BODY(PowFunction, exec_pow)
  {
    if (args[0]->type == Value::Type::NIL || args[1]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    const auto& base = std::get<const Value::Number>(args[0]->value);
    const auto& exponent = std::get<const Value::Number>(args[1]->value);
    return Value::number(std::pow(base.get_float(), exponent.get_float()));
  }

  /* SqrtFunction - sqrt */
  FUNC_IMPL(SqrtFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&SqrtFunction::exec_sqrt))))

  EXEC_BODY(SqrtFunction, exec_sqrt)
  {
    return Value::number(
      std::sqrt(std::get<const Value::Number>(args[0]->value).get_float()));
  }

} // namespace Roo
