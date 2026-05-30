
#include "roo/lang/num.h"

#include <cctype>
#include <stdexcept>
#include <string>

#include "roo/exception.h"
#include "roo/exec.h"
#include "roo/runtime/value.h"

namespace Roo
{
  namespace
  {
    sptr_val parse_int_string(const std::string& value)
    {
      try
      {
        size_t parsed = 0;
        const int result = std::stoi(value, &parsed);
        while (parsed < value.size() &&
               std::isspace(static_cast<unsigned char>(value[parsed])))
        {
          parsed++;
        }
        if (parsed != value.size())
        {
          return Constant::NIL;
        }
        return Value::number(result);
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
  } // namespace

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
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::NIL;
    }

    return Value::number(args[0]->num() +
                         Value::Number{.num_type = Value::NumberType::INT, .int_value = 1});
  }

  /** DecFunction - dec */
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

  /** IntFunction - int */
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
      return Value::number(std::get<const Value::Number>(obj->value).get_int());
    }
    else if (Type::CHAR.is_type_of(*obj))
    {
      return Value::number(static_cast<int>(std::get<char>(obj->value)));
    }
    else if (Type::STRING.is_type_of(*obj))
    {
      return parse_int_string(obj->str());
    }

    throw RooException("Cannot convert " + obj->to_string() + " to integer.");
  }

  /** MaxFunction - max */
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

  /** MinFunction - min */
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

  /** NumberPFunction - number? */
  FUNC_IMPL(NumberPFunction,
            SIG((FN_ARGS((&Roo::Type::ANY)), EXEC_DISPATCH(&NumberPFunction::exec_num))))

  EXEC_BODY(NumberPFunction, exec_num)
  {
    return args[0]->type == Value::Type::NUMBER ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** OddPFunction - odd? */
  FUNC_IMPL(OddPFunction,
            SIG((FN_ARGS((&Roo::Type::NUMBER)), EXEC_DISPATCH(&OddPFunction::exec_odd))))

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

} // namespace Roo
