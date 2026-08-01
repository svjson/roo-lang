
#include <cstdint>
#include <cstring>

#include <roo/exception.h>
#include <roo/lang/operator.h>

namespace Roo
{
  namespace
  {
    const Value::Number& number_arg(const sptr_val& arg, const char* function_name)
    {
      if (!arg || arg->type != Value::Type::NUMBER)
      {
        throw TypeError(std::string(function_name) + " expects number arguments.");
      }
      return arg->num();
    }

    std::int64_t integer_arg(const sptr_val& arg, const char* function_name)
    {
      const Value::Number& num = number_arg(arg, function_name);
      if (num.num_type == Value::NumberType::FLOAT)
      {
        throw TypeError(std::string(function_name) + " expects integer arguments.");
      }
      if (num.num_type == Value::NumberType::INT)
      {
        return num.int_value;
      }
      return num.long_value;
    }

    unsigned int shift_bits(sptr_val_v& args, const char* function_name)
    {
      if (args.size() == 1)
      {
        return 1;
      }

      std::int64_t bits = integer_arg(args[1], function_name);
      if (bits < 0 || bits > 63)
      {
        throw RooException(std::string(function_name) +
                           " shift count must be between 0 and 63.");
      }
      return static_cast<unsigned int>(bits);
    }

    long long from_bits(std::uint64_t bits)
    {
      std::int64_t signed_bits = 0;
      static_assert(sizeof(signed_bits) == sizeof(bits));
      std::memcpy(&signed_bits, &bits, sizeof(bits));
      return static_cast<long long>(signed_bits);
    }

    std::int64_t arithmetic_right_shift(std::int64_t value, unsigned int bits)
    {
      if (bits == 0)
      {
        return value;
      }
      if (value >= 0)
      {
        return value >> bits;
      }
      return ~((~value) >> bits);
    }
  } // namespace

  /** EqualsPFunction - roo/= */
  FUNC_IMPL(EqualsPFunction,
            SIG((FN_ARGS((&Roo::Type::ANY), (&Roo::Type::ANY)),
                 EXEC_DISPATCH(&EqualsPFunction::exec_equals))))

  EXEC_BODY(EqualsPFunction, exec_equals)
  {
    return *args[0] == *args[1] ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** PlusFunction - roo/+ */
  FUNC_IMPL(PlusFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)), EXEC_DISPATCH(&PlusFunction::exec_plus))))

  EXEC_BODY(PlusFunction, exec_plus)
  {
    if (args.size() == 0)
    {
      throw RooException("No arguments given to +");
    }

    Value::Number result = number_arg(args[0], "+");

    for (size_t i = 1; i < args.size(); i++)
    {
      result = result + number_arg(args[i], "+");
    }

    return Value::number(result);
  }

  /** MinusFunction - roo/- */
  FUNC_IMPL(MinusFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MinusFunction::exec_subtract))))

  EXEC_BODY(MinusFunction, exec_subtract)
  {
    if (args.size() == 0)
    {
      throw RooException("No arguments given to -");
    }

    Value::Number result = number_arg(args[0], "-");

    if (args.size() == 1)
    {
      result = Value::Number{.num_type = Value::NumberType::INT, .int_value = 0} - result;
    }

    for (size_t i = 1; i < args.size(); i++)
    {
      result = result - number_arg(args[i], "-");
    }

    return Value::number(result);
  }

  /** DivideFunction - roo// */
  FUNC_IMPL(DivideFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&DivideFunction::exec_divide))))

  EXEC_BODY(DivideFunction, exec_divide)
  {
    if (args.size() == 0)
    {
      throw RooException("No arguments given to /");
    }

    if (args.size() == 1)
    {
      number_arg(args[0], "/");
      return args[0];
    }

    Value::Number result = number_arg(args[0], "/");

    for (size_t i = 1; i < args.size(); i++)
    {
      const Value::Number& divisor = number_arg(args[i], "/");
      if (divisor.get_double() == 0)
      {
        throw RooException("Division by zero");
      }
      result = result / divisor;
    }

    return Value::number(result);
  }

  // MultiplyFunction - roo/*
  FUNC_IMPL(MultiplyFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&MultiplyFunction::exec_multiply))));

  EXEC_BODY(MultiplyFunction, exec_multiply)
  {
    if (args.size() == 0)
    {
      throw RooException("No arguments given to *");
    }

    if (args.size() == 1)
    {
      number_arg(args[0], "*");
      return args[0];
    }

    Value::Number result = number_arg(args[0], "*");

    for (size_t i = 1; i < args.size(); i++)
    {
      result = result * number_arg(args[i], "*");
    }

    return Value::number(result);
  }

  /** ShiftLeftFunction - roo/<< */
  FUNC_IMPL(ShiftLeftFunction,
            std::make_unique<Signature>(FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                                        EXEC_DISPATCH(&ShiftLeftFunction::exec_shift_left),
                                        1,
                                        false))

  EXEC_BODY(ShiftLeftFunction, exec_shift_left)
  {
    const std::uint64_t value = static_cast<std::uint64_t>(integer_arg(args[0], "<<"));
    return Value::number(from_bits(value << shift_bits(args, "<<")));
  }

  /** ShiftRightFunction - roo/>> */
  FUNC_IMPL(ShiftRightFunction,
            std::make_unique<Signature>(FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                                        EXEC_DISPATCH(&ShiftRightFunction::exec_shift_right),
                                        1,
                                        false))

  EXEC_BODY(ShiftRightFunction, exec_shift_right)
  {
    const auto value = static_cast<std::int64_t>(integer_arg(args[0], ">>"));
    return Value::number(
      static_cast<long long>(arithmetic_right_shift(value, shift_bits(args, ">>"))));
  }

  /** LogicalShiftLeftFunction - roo/<<< */
  FUNC_IMPL(LogicalShiftLeftFunction,
            std::make_unique<Signature>(
              FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
              EXEC_DISPATCH(&LogicalShiftLeftFunction::exec_logical_shift_left),
              1,
              false))

  EXEC_BODY(LogicalShiftLeftFunction, exec_logical_shift_left)
  {
    const std::uint64_t value = static_cast<std::uint64_t>(integer_arg(args[0], "<<<"));
    return Value::number(from_bits(value << shift_bits(args, "<<<")));
  }

  /** LogicalShiftRightFunction - roo/>>> */
  FUNC_IMPL(LogicalShiftRightFunction,
            std::make_unique<Signature>(
              FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
              EXEC_DISPATCH(&LogicalShiftRightFunction::exec_logical_shift_right),
              1,
              false))

  EXEC_BODY(LogicalShiftRightFunction, exec_logical_shift_right)
  {
    const std::uint64_t value = static_cast<std::uint64_t>(integer_arg(args[0], ">>>"));
    return Value::number(from_bits(value >> shift_bits(args, ">>>")));
  }

  /** BitAndFunction - roo/bit-and */
  FUNC_IMPL(BitAndFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&BitAndFunction::exec_bit_and))))

  EXEC_BODY(BitAndFunction, exec_bit_and)
  {
    if (args.empty())
    {
      throw RooException("No arguments given to bit-and");
    }

    std::uint64_t result = static_cast<std::uint64_t>(integer_arg(args[0], "bit-and"));
    for (size_t i = 1; i < args.size(); i++)
    {
      result &= static_cast<std::uint64_t>(integer_arg(args[i], "bit-and"));
    }
    return Value::number(from_bits(result));
  }

  /** BitOrFunction - roo/bit-or */
  FUNC_IMPL(BitOrFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&BitOrFunction::exec_bit_or))))

  EXEC_BODY(BitOrFunction, exec_bit_or)
  {
    if (args.empty())
    {
      throw RooException("No arguments given to bit-or");
    }

    std::uint64_t result = static_cast<std::uint64_t>(integer_arg(args[0], "bit-or"));
    for (size_t i = 1; i < args.size(); i++)
    {
      result |= static_cast<std::uint64_t>(integer_arg(args[i], "bit-or"));
    }
    return Value::number(from_bits(result));
  }

  /** BitXorFunction - roo/bit-xor */
  FUNC_IMPL(BitXorFunction,
            SIG((FN_ARGS((VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&BitXorFunction::exec_bit_xor))))

  EXEC_BODY(BitXorFunction, exec_bit_xor)
  {
    if (args.empty())
    {
      throw RooException("No arguments given to bit-xor");
    }

    std::uint64_t result = static_cast<std::uint64_t>(integer_arg(args[0], "bit-xor"));
    for (size_t i = 1; i < args.size(); i++)
    {
      result ^= static_cast<std::uint64_t>(integer_arg(args[i], "bit-xor"));
    }
    return Value::number(from_bits(result));
  }

  /** BitNotFunction - roo/bit-not */
  FUNC_IMPL(BitNotFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&BitNotFunction::exec_bit_not))))

  EXEC_BODY(BitNotFunction, exec_bit_not)
  {
    return Value::number(
      from_bits(~static_cast<std::uint64_t>(integer_arg(args[0], "bit-not"))));
  }

  /** ModulusFunction - roo/mod */
  FUNC_IMPL(ModulusFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&ModulusFunction::exec_modulus))))

  EXEC_BODY(ModulusFunction, exec_modulus)
  {
    return Value::number(args[0]->i64() % args[1]->i64());
  }

  /** LessThanFunction - roo/< */
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

    return std::get<const Value::Number>(args[0]->value).get_float() <
               std::get<const Value::Number>(args[1]->value).get_float()
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

    return args[0]->str() < args[1]->str() ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** LessThanOrEqualsFunction - roo/<= */
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
    return std::get<const Value::Number>(args[0]->value).get_float() <=
               std::get<const Value::Number>(args[1]->value).get_float()
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

    return args[0]->str() <= args[1]->str() ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** GreaterThanFunction - roo/> */
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
    return std::get<const Value::Number>(args[0]->value).get_float() >
               std::get<const Value::Number>(args[1]->value).get_float()
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

    return args[0]->str() > args[1]->str() ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** GreaterThanOrEqualsFunction - roo/>= */
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
    return std::get<const Value::Number>(args[0]->value).get_float() >=
               std::get<const Value::Number>(args[1]->value).get_float()
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
    return args[0]->str() >= args[1]->str() ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

} // namespace Roo
