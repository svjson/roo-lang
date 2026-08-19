#include <roo/runtime/number.h>

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

    sptr_val float_to_integral_number(double value)
    {
      if (!std::isfinite(value)) return Constant::NIL;

      const long double min =
        static_cast<long double>(std::numeric_limits<std::int64_t>::min());
      const long double max =
        static_cast<long double>(std::numeric_limits<std::int64_t>::max());
      const long double candidate = static_cast<long double>(value);
      if (candidate < min || candidate > max) return Constant::NIL;

      return integral_number(static_cast<std::int64_t>(value));
    }

    sptr_val parse_float_string(const std::string& value)
    {
      try
      {
        size_t parsed = 0;
        const float result = std::stof(value, &parsed);
        while (parsed < value.size() &&
               std::isspace(static_cast<unsigned char>(value[parsed])))
        {
          parsed++;
        }
        if (parsed != value.size() || !std::isfinite(result)) return Constant::NIL;
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
        if (parsed == value.size()) return integral_number(result);

        size_t float_parsed = 0;
        const double float_result = std::stod(value, &float_parsed);
        while (float_parsed < value.size() &&
               std::isspace(static_cast<unsigned char>(value[float_parsed])))
        {
          float_parsed++;
        }
        return float_parsed == value.size() ? float_to_integral_number(float_result)
                                            : Constant::NIL;
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

    sptr_val parse_number_string(const std::string& value)
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
        if (parsed == value.size()) return integral_number(result);
      }
      catch (const std::invalid_argument&)
      {
      }
      catch (const std::out_of_range&)
      {
        return Constant::NIL;
      }

      return parse_float_string(value);
    }

    bool is_textual_number(const Value& value)
    {
      return value.type == Value::Type::STRING || value.type == Value::Type::KEYWORD ||
             value.type == Value::Type::SYMBOL;
    }

    std::string character_text(const Value& value)
    {
      return std::string(1, std::get<char>(value.value));
    }
  } // namespace

  sptr_val convert_to_int(const sptr_val& value)
  {
    if (value->type == Value::Type::NIL) return Constant::NIL;
    if (value->type == Value::Type::NUMBER)
    {
      const Value::Number& number = value->num();
      switch (number.num_type)
      {
      case Value::NumberType::INT:
        return Value::number(number.int_value);
      case Value::NumberType::LONG:
        return integral_number(number.long_value);
      case Value::NumberType::FLOAT:
        return float_to_integral_number(number.float_value);
      }
    }
    if (value->type == Value::Type::CHAR) return parse_int_string(character_text(*value));
    if (is_textual_number(*value)) return parse_int_string(value->str());
    return Constant::NIL;
  }

  sptr_val convert_to_float(const sptr_val& value)
  {
    if (value->type == Value::Type::NIL) return Constant::NIL;
    if (value->type == Value::Type::NUMBER)
    {
      return Value::number(
        Value::Number{.num_type = Value::NumberType::FLOAT, .float_value = value->num().get_float()});
    }
    if (value->type == Value::Type::CHAR) return parse_float_string(character_text(*value));
    if (is_textual_number(*value)) return parse_float_string(value->str());
    return Constant::NIL;
  }

  sptr_val convert_to_number(const sptr_val& value)
  {
    if (value->type == Value::Type::NIL) return Constant::NIL;
    if (value->type == Value::Type::NUMBER) return value;
    if (value->type == Value::Type::CHAR) return parse_number_string(character_text(*value));
    if (is_textual_number(*value)) return parse_number_string(value->str());
    return Constant::NIL;
  }

  bool is_digit(const Value& value)
  {
    if (value.type == Value::Type::CHAR)
    {
      const char character = std::get<char>(value.value);
      return character >= '0' && character <= '9';
    }

    if (value.type == Value::Type::STRING || value.type == Value::Type::KEYWORD ||
        value.type == Value::Type::SYMBOL)
    {
      const std::string& text = value.str();
      return text.size() == 1 && text[0] >= '0' && text[0] <= '9';
    }

    if (value.type == Value::Type::NUMBER)
    {
      const Value::Number& number = value.num();
      if (number.num_type == Value::NumberType::INT)
      {
        return number.int_value >= 0 && number.int_value <= 9;
      }
      if (number.num_type == Value::NumberType::LONG)
      {
        return number.long_value >= 0 && number.long_value <= 9;
      }
    }

    return false;
  }
} // namespace Roo
