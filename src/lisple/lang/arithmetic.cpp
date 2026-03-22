
#include "arithmetic.h"

namespace Lisple
{
  /* PlusFunction - + */
  FUNC_IMPL(PlusFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&PlusFunction::inv_plus, &PlusFunction::exec_plus))))

  /** Legacy AST implementation */
  FUNC_BODY(PlusFunction, inv_plus)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to +");
    }

    if (*NIL == *args[0])
    {
      throw TypeError(
        "Cannot perform arithmetic with nil. Arguments: " + Array(args).to_string() + ".");
    }
    // std::shared_ptr<Number> result = std::dynamic_pointer_cast<Number>(args[0]);
    std::shared_ptr<Number> result = std::make_shared<Number>(args[0]->as<Number>());

    for (size_t i = 1; i < args.size(); i++)
    {
      if (*NIL == *args[i])
      {
        throw TypeError(
          "Cannot perform arithmetic with nil. Arguments: " + Array(args).to_string() + ".");
      }
      result = *result + args[i]->as<Number>();
    }

    return result;
  }

  EXEC_BODY(PlusFunction, exec_plus)
  {
    if (args.size() == 0)
    {
      throw LispleException("No arguments given to +");
    }

    std::vector<RTValue::Number*> numbers;
    numbers.reserve(args.size());
    for (auto& arg : args)
    {
      sptr_rtval& val = std::get<LiteralNode>(arg->data).value;
      if (Constant::NIL.get() == val.get())
      {
        throw TypeError("Cannot perform arithmetic with nil.");
      }
      numbers.push_back(&std::get<RTValue::Number>(val->value));
    }

    float result = numbers[0]->get_float();

    for (size_t i = 1; i < args.size(); i++)
    {
      result += numbers[i]->get_float();
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
