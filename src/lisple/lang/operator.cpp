
#include "operator.h"

#include "lisple/exception.h"

namespace Lisple
{
  /** EqualsPredicatefunction - = */
  FUNC_IMPL(EqualsPredicateFunction,
            SIG((FN_ARGS((&Lisple::Type::ANY), (&Lisple::Type::ANY)),
                 EXEC_DISPATCH(&EqualsPredicateFunction::exec_equals))))

  EXEC_BODY(EqualsPredicateFunction, exec_equals)
  {
    return *args[0] == *args[1] ? Constant::TRUE : Constant::FALSE;
  }

  /** PlusFunction - + */
  FUNC_IMPL(PlusFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)), EXEC_DISPATCH(&PlusFunction::exec_plus))))

  EXEC_BODY(PlusFunction, exec_plus)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to +");
    }

    float result = 0.0;

    for (size_t i = 0; i < args.size(); i++)
    {
      if (args[i]->type != RTValue::Type::NUMBER)
      {
        throw TypeError("Cannot perform arithmetic with nil. Arguments: " +
                        RTValue::vector(args)->to_string() + ".");
      }
      result += std::get<RTValue::Number>(args[i]->value).get_float();
    }

    return RTValue::number(result);
  }

  /** MinusFunction - - */
  FUNC_IMPL(MinusFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MinusFunction::exec_subtract))))

  EXEC_BODY(MinusFunction, exec_subtract)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to -");
    }

    float result = std::get<RTValue::Number>(args[0]->value).get_float();
    if (args.size() == 1)
    {
      result = -result;
    }

    for (size_t i = 1; i < args.size(); i++)
    {
      if (args[i]->type != RTValue::Type::NUMBER)
      {
        throw TypeError("Cannot perform arithmetic with nil. Arguments: " +
                        RTValue::vector(args)->to_string() + ".");
      }
      result -= std::get<RTValue::Number>(args[i]->value).get_float();
    }
    return RTValue::number(result);
  }

  FUNC_IMPL(DivideFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&DivideFunction::exec_divide))))

  /** DivideFunction - / */
  EXEC_BODY(DivideFunction, exec_divide)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to /");
    }

    if (args.size() == 1)
    {
      return args[0];
    }

    float result = std::get<RTValue::Number>(args[0]->value).get_float();

    for (size_t i = 1; i < args.size(); i++)
    {
      if (args[i]->type != RTValue::Type::NUMBER)
      {
        throw TypeError("Cannot divide by nil/non-number. Arguments: " +
                        RTValue::vector(args)->to_string() + ".");
      }
      float divisor = std::get<RTValue::Number>(args[i]->value).get_float();
      if (divisor == 0)
      {
        throw LispleException("Division by zero");
      }
      result /= divisor;
    }

    return RTValue::number(result);
  }

  /** DivideFunction - * */
  FUNC_IMPL(MultiplyFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MultiplyFunction::exec_multiply))));

  EXEC_BODY(MultiplyFunction, exec_multiply)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to *");
    }

    if (args.size() == 1)
    {
      return args[0];
    }

    float result = std::get<RTValue::Number>(args[0]->value).get_float();

    for (size_t i = 1; i < args.size(); i++)
    {
      if (args[i]->type != RTValue::Type::NUMBER)
      {
        throw TypeError("Cannot multiply by nil/non-number. Arguments: " +
                        RTValue::vector(args)->to_string() + ".");
      }
      result *= std::get<RTValue::Number>(args[i]->value).get_float();
    }

    return RTValue::number(result);
  }

} // namespace Lisple
