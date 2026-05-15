
#include "lisple/lang/num.h"

#include "lisple/exec.h"
#include "lisple/runtime/value.h"

namespace Lisple
{

  /** EvenPFunction - even? */
  FUNC_IMPL(EvenPFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&EvenPFunction::exec_even))))

  EXEC_BODY(EvenPFunction, exec_even)
  {
    return std::get<const Value::Number>(args[0]->value).get_int() % 2 == 0
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  /** IncFunction - inc */
  FUNC_IMPL(IncFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&IncFunction::exec_inc))))

  EXEC_BODY(IncFunction, exec_inc)
  {
    return Value::number(args[0]->num() +
                         Value::Number{.num_type = Value::NumberType::INT, .int_value = 1});
  }

  /** DecFunction - dec */
  FUNC_IMPL(DecFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&DecFunction::exec_dec))))

  EXEC_BODY(DecFunction, exec_dec)
  {
    return Value::number(args[0]->num() -
                         Value::Number{.num_type = Value::NumberType::INT, .int_value = 1});
  }

  /** IntFunction - int */
  FUNC_IMPL(IntFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&IntFunction::exec_to_int))))

  EXEC_BODY(IntFunction, exec_to_int)
  {
    sptr_val& obj = args[0];

    if (Type::NUMBER.is_type_of(*obj))
    {
      return Value::number(std::get<const Value::Number>(obj->value).get_int());
    }
    else if (Type::CHAR.is_type_of(*obj))
    {
      return Value::number(static_cast<int>(std::get<char>(obj->value)));
    }
    else if (Type::STRING.is_type_of(*obj))
    {
      return Value::number(std::stoi(obj->str()));
    }

    throw LispleException("Cannot convert " + obj->to_string() + " to integer.");
  }

  /** MaxFunction - max */
  FUNC_IMPL(MaxFunction,
            SIG((FN_ARGS((&Type::NUMBER), (VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MaxFunction::exec_max))))

  EXEC_BODY(MaxFunction, exec_max)
  {
    float result_val = std::get<const Value::Number>(args[0]->value).get_float();
    size_t result_index = 0;

    for (size_t i = 1; i < args.size(); i++)
    {
      float num = std::get<const Value::Number>(args[i]->value).get_float();
      if (num > result_val)
      {
        result_val = num;
        result_index = i;
      }
    }

    return args[result_index];
  }

  /** MinFunction - min */
  FUNC_IMPL(MinFunction,
            SIG((FN_ARGS((&Type::NUMBER), (VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MinFunction::exec_min))))

  EXEC_BODY(MinFunction, exec_min)
  {
    float result_val = std::get<const Value::Number>(args[0]->value).get_float();
    size_t result_index = 0;

    for (size_t i = 1; i < args.size(); i++)
    {
      float num = std::get<const Value::Number>(args[i]->value).get_float();
      if (num < result_val)
      {
        result_val = num;
        result_index = i;
      }
    }

    return args[result_index];
  }

  /** NumberPFunction - number? */
  FUNC_IMPL(NumberPFunction,
            SIG((FN_ARGS((&Lisple::Type::ANY)), EXEC_DISPATCH(&NumberPFunction::exec_num))))

  EXEC_BODY(NumberPFunction, exec_num)
  {
    return args[0]->type == Value::Type::NUMBER ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** OddPFunction - odd? */
  FUNC_IMPL(OddPFunction,
            SIG((FN_ARGS((&Lisple::Type::NUMBER)), EXEC_DISPATCH(&OddPFunction::exec_odd))))

  EXEC_BODY(OddPFunction, exec_odd)
  {
    return std::get<const Value::Number>(args[0]->value).get_int() % 2 != 0
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

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

} // namespace Lisple
