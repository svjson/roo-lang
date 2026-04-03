
#include <lisple/exception.h>
#include <lisple/lang/operator.h>

namespace Lisple
{
  /** EqualsPredicatefunction - = */
  FUNC_IMPL(EqualsPredicateFunction,
            SIG((FN_ARGS((&Lisple::Type::ANY), (&Lisple::Type::ANY)),
                 EXEC_DISPATCH(&EqualsPredicateFunction::exec_equals))))

  EXEC_BODY(EqualsPredicateFunction, exec_equals)
  {
    return *args[0] == *args[1] ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
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

  /** DivideFunction - / */
  FUNC_IMPL(DivideFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&DivideFunction::exec_divide))))

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

  /** MultiplyFunction - * */
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

  /** ModulusFunction - mod */
  FUNC_IMPL(ModulusFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&ModulusFunction::exec_modulus))))

  EXEC_BODY(ModulusFunction, exec_modulus)
  {
    return RTValue::number(args[0]->i64() % args[1]->i64());
  }

  /* LessThanFunction */
  FUNC_IMPL(LessThanFunction,
            MULTI_SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                       EXEC_DISPATCH(&LessThanFunction::exec_lt_num)),
                      (FN_ARGS((&Type::STRING), (&Type::STRING)),
                       EXEC_DISPATCH(&LessThanFunction::exec_lt_str))))

  EXEC_BODY(LessThanFunction, exec_lt_num)
  {
    if (*args[0] == *Constant::NIL || *args[1] == *Constant::NIL)
    {
      throw TypeError("Cannot compare " + args[0]->to_string() + " and " +
                      args[1]->to_string());
    }

    return std::get<RTValue::Number>(args[0]->value).get_float() <
               std::get<RTValue::Number>(args[1]->value).get_float()
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  EXEC_BODY(LessThanFunction, exec_lt_str)
  {
    if (*args[0] == *Constant::NIL || *args[1] == *Constant::NIL)
    {
      throw TypeError("Cannot compare " + args[0]->to_string() + " and " +
                      args[1]->to_string());
    }

    return args[0]->to_string() < args[1]->to_string() ? Constant::BOOL_TRUE
                                                       : Constant::BOOL_FALSE;
  }

  /* LessThanOrEqualsFunction */
  FUNC_IMPL(LessThanOrEqualsFunction,
            MULTI_SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                       EXEC_DISPATCH(&LessThanOrEqualsFunction::exec_lte_num)),
                      (FN_ARGS((&Type::STRING), (&Type::STRING)),
                       EXEC_DISPATCH(&LessThanOrEqualsFunction::exec_lte_str))))

  EXEC_BODY(LessThanOrEqualsFunction, exec_lte_num)
  {
    if (*args[0] == *Constant::NIL || *args[1] == *Constant::NIL)
    {
      throw TypeError("Cannot compare " + args[0]->to_string() + " and " +
                      args[1]->to_string());
    }
    return std::get<RTValue::Number>(args[0]->value).get_float() <=
               std::get<RTValue::Number>(args[1]->value).get_float()
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  EXEC_BODY(LessThanOrEqualsFunction, exec_lte_str)
  {
    if (*args[0] == *Constant::NIL || *args[1] == *Constant::NIL)
    {
      throw TypeError("Cannot compare " + args[0]->to_string() + " and " +
                      args[1]->to_string());
    }

    return args[0]->to_string() <= args[1]->to_string() ? Constant::BOOL_TRUE
                                                        : Constant::BOOL_FALSE;
  }

  /* GreaterThanFunction */
  FUNC_IMPL(GreaterThanFunction,
            MULTI_SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                       EXEC_DISPATCH(&GreaterThanFunction::exec_gt_num)),
                      (FN_ARGS((&Type::STRING), (&Type::STRING)),
                       EXEC_DISPATCH(&GreaterThanFunction::exec_gt_str))))

  EXEC_BODY(GreaterThanFunction, exec_gt_num)
  {
    if (*args[0] == *Constant::NIL || *args[1] == *Constant::NIL)
    {
      throw TypeError("Cannot compare " + args[0]->to_string() + " and " +
                      args[1]->to_string());
    }
    return std::get<RTValue::Number>(args[0]->value).get_float() >
               std::get<RTValue::Number>(args[1]->value).get_float()
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  EXEC_BODY(GreaterThanFunction, exec_gt_str)
  {
    if (*args[0] == *Constant::NIL || *args[1] == *Constant::NIL)
    {
      throw TypeError("Cannot compare " + args[0]->to_string() + " and " +
                      args[1]->to_string());
    }

    return args[0]->to_string() > args[1]->to_string() ? Constant::BOOL_TRUE
                                                       : Constant::BOOL_FALSE;
  }

  /* GreaterThanOrEqualsFunction */
  FUNC_IMPL(GreaterThanOrEqualsFunction,
            MULTI_SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                       EXEC_DISPATCH(&GreaterThanOrEqualsFunction::exec_gte_num)),
                      (FN_ARGS((&Type::STRING), (&Type::STRING)),
                       EXEC_DISPATCH(&GreaterThanOrEqualsFunction::exec_gte_str))))

  EXEC_BODY(GreaterThanOrEqualsFunction, exec_gte_num)
  {
    if (*args[0] == *Constant::NIL || *args[1] == *Constant::NIL)
    {
      throw TypeError("Cannot compare " + args[0]->to_string() + " and " +
                      args[1]->to_string());
    }
    return std::get<RTValue::Number>(args[0]->value).get_float() >=
               std::get<RTValue::Number>(args[1]->value).get_float()
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  EXEC_BODY(GreaterThanOrEqualsFunction, exec_gte_str)
  {
    if (*args[0] == *Constant::NIL || *args[1] == *Constant::NIL)
    {
      throw TypeError("Cannot compare " + args[0]->to_string() + " and " +
                      args[1]->to_string());
    }
    return args[0]->to_string() >= args[1]->to_string() ? Constant::BOOL_TRUE
                                                        : Constant::BOOL_FALSE;
  }

} // namespace Lisple
