#include "roo/lang/num.h"

#include "roo/exec.h"
#include "roo/runtime/number.h"

namespace Roo
{
  /** ClampFunction - roo/clamp */
  FUNC_IMPL(ClampFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&ClampFunction::exec_clamp))))

  EXEC_BODY(ClampFunction, exec_clamp)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::NIL;

    const float value = args[0]->num().get_float();
    if (args[1]->type != Value::Type::NIL && value < args[1]->num().get_float())
    {
      return args[1];
    }
    if (args[2]->type != Value::Type::NIL && value > args[2]->num().get_float())
    {
      return args[2];
    }

    return args[0];
  }

  /** ClampBetweenFunction - roo/clamp-between */
  FUNC_IMPL(ClampBetweenFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&ClampBetweenFunction::exec_clamp_between))))

  EXEC_BODY(ClampBetweenFunction, exec_clamp_between)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::NIL;
    if (args[1]->type == Value::Type::NIL || args[2]->type == Value::Type::NIL)
    {
      return args[0];
    }

    const float value = std::get<const Value::Number>(args[0]->value).get_float();
    float low = std::get<const Value::Number>(args[1]->value).get_float();
    float high = std::get<const Value::Number>(args[2]->value).get_float();

    if (low < high)
    {
      if (value <= low) return args[1];
      if (value >= high) return args[2];
    }
    else
    {
      if (value >= low) return args[1];
      if (value <= high) return args[2];
    }

    return args[0];
  }

  /** CodepointFunction - roo/codepoint */
  FUNC_IMPL(CodepointFunction,
            SIG((FN_ARGS((&Type::CHAR)), EXEC_DISPATCH(&CodepointFunction::exec_codepoint))))

  EXEC_BODY(CodepointFunction, exec_codepoint)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    return Value::number(static_cast<unsigned char>(args[0]->ch()));
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

  /** DigitPFunction - roo/digit? */
  FUNC_IMPL(DigitPFunction,
            SIG((FN_ARGS((&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&DigitPFunction::exec_digitp))))

  EXEC_BODY(DigitPFunction, exec_digitp)
  {
    for (const auto& arg : args)
    {
      if (!is_digit(*arg)) return Constant::BOOL_FALSE;
    }
    return Constant::BOOL_TRUE;
  }

  /** EvenPFunction - roo/even? */
  FUNC_IMPL(EvenPFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&EvenPFunction::exec_even))))

  EXEC_BODY(EvenPFunction, exec_even)
  {
    return std::get<const Value::Number>(args[0]->value).get_int() % 2 == 0
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  /** FloatFunction - roo/float */
  FUNC_IMPL(FloatFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&FloatFunction::exec_to_float))))

  EXEC_BODY(FloatFunction, exec_to_float)
  {
    return convert_to_float(args[0]);
  }

  /** FloatPFunction - roo/float? */
  FUNC_IMPL(FloatPFunction,
            SIG((FN_ARGS((&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&FloatPFunction::exec_floatp))))

  EXEC_BODY(FloatPFunction, exec_floatp)
  {
    for (const auto& arg : args)
    {
      if (arg->type != Value::Type::NUMBER ||
          arg->num().num_type != Value::NumberType::FLOAT)
      {
        return Constant::BOOL_FALSE;
      }
    }
    return Constant::BOOL_TRUE;
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

  /** IntFunction - roo/int */
  FUNC_IMPL(IntFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&IntFunction::exec_to_int))))

  EXEC_BODY(IntFunction, exec_to_int)
  {
    return convert_to_int(args[0]);
  }

  /** IntPFunction - roo/int? */
  FUNC_IMPL(IntPFunction,
            SIG((FN_ARGS((&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&IntPFunction::exec_intp))))

  EXEC_BODY(IntPFunction, exec_intp)
  {
    for (const auto& arg : args)
    {
      if (arg->type != Value::Type::NUMBER ||
          (arg->num().num_type != Value::NumberType::INT &&
           arg->num().num_type != Value::NumberType::LONG))
      {
        return Constant::BOOL_FALSE;
      }
    }
    return Constant::BOOL_TRUE;
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

  /** NumberFunction - roo/number */
  FUNC_IMPL(NumberFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&NumberFunction::exec_number))))

  EXEC_BODY(NumberFunction, exec_number)
  {
    return convert_to_number(args[0]);
  }

  /** NumberPFunction - roo/number? */
  FUNC_IMPL(NumberPFunction,
            SIG((FN_ARGS((&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&NumberPFunction::exec_num))))

  EXEC_BODY(NumberPFunction, exec_num)
  {
    for (const auto& arg : args)
    {
      if (arg->type != Value::Type::NUMBER) return Constant::BOOL_FALSE;
    }
    return Constant::BOOL_TRUE;
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
