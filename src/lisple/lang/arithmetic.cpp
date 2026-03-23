
#include "arithmetic.h"

namespace Lisple
{
  /* PlusFunction - + */
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

  /* MinusFunction - - */
  FUNC_IMPL(MinusFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MinusFunction::do_subtraction))))

  FUNC_BODY(MinusFunction, do_subtraction)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to -");
    }

    if (*NIL == *args[0])
    {
      throw TypeError(
        "Cannot perform arithmetic with nil. Arguments: " + Array(args).to_string() + ".");
    }
    // std::shared_ptr<Number> result = std::dynamic_pointer_cast<Number>(args[0]);
    std::shared_ptr<Number> result = std::make_shared<Number>(args[0]->as<Number>());

    if (args.size() == 1)
    {
      result = result->flip_sign();
    }
    else
    {
      for (size_t i = 1; i < args.size(); i++)
      {
        if (*NIL == *args[i])
        {
          throw TypeError("Cannot perform arithmetic with nil. Arguments: " +
                          Array(args).to_string() + ".");
        }
        result = *result - args[i]->as<Number>();
      }
    }

    return result;
  }

  FUNC_IMPL(DivideFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&DivideFunction::do_division))))

  FUNC_BODY(DivideFunction, do_division)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to /");
    }

    // std::shared_ptr<Number> result = std::dynamic_pointer_cast<Number>(args[0]);
    std::shared_ptr<Number> result = std::make_shared<Number>(args[0]->as<Number>());
    for (size_t i = 1; i < args.size(); i++)
    {
      result = *result / args[i]->as<Number>();
    }

    return result;
  }

  FUNC_IMPL(MultiplyFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MultiplyFunction::do_multiplication))));

  FUNC_BODY(MultiplyFunction, do_multiplication)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to *");
    }

    // std::shared_ptr<Number> result = std::dynamic_pointer_cast<Number>(args[0]);
    std::shared_ptr<Number> result = std::make_shared<Number>(args[0]->as<Number>());

    for (size_t i = 1; i < args.size(); i++)
    {
      result = *result * args[i]->as<Number>();
    }

    return result;
  }

} // namespace Lisple
