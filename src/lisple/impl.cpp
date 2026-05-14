
#include "lisple/impl.h"

#include <memory>
#include <sstream>
#include <string>

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/type.h>

namespace Lisple
{
  const std::string EMPTY_STRING = "";

  const std::string str_val(const Object& obj) noexcept
  {
    if (Type::NUMBER.is_type_of(obj) || *NIL == obj)
    {
      return obj.to_string();
    }

    if (Type::STRING.is_type_of(obj) || Type::KEYWORD.is_type_of(obj) ||
        Type::SYMBOL_VALUE.is_type_of(obj))
    {
      if (auto* wrapper = dynamic_cast<const RuntimeValueWrapper*>(&obj))
      {
        return std::get<std::string>(wrapper->val->value);
      }
      return obj.as<Value<std::string>>().value;
    }

    if (Type::SYMBOL.is_type_of(obj))
    {
      return obj.to_string();
    }

    return EMPTY_STRING;
  }

  char char_val(const Object& obj) noexcept
  {
    if (Type::NUMBER.is_type_of(obj) || *NIL == obj)
    {
      return Lisple::int_val(obj);
    }

    if (Type::CHAR.is_type_of(obj))
    {
      if (auto* wrapper = dynamic_cast<const RuntimeValueWrapper*>(&obj))
      {
        return std::get<char>(wrapper->val->value);
      }
      return obj.as<Value<char>>().value;
    }

    return '\0';
  }

  std::pair<std::string, std::string> split_qualifiable(const std::string& str)
  {
    if (str == "/")
    {
      return {"", str};
    }
    else
    {
      std::stringstream ss(str);
      std::vector<std::string> tokens;
      std::string token;
      while (std::getline(ss, token, '/'))
      {
        tokens.push_back(token);
      }

      switch (tokens.size())
      {
      case 0:
        return {"", ""};
      case 1:
        return {"", tokens.front()};
        break;
      case 2:
        return {tokens.front(), tokens.back()};
        break;
      default:
        throw IdentifierException("Invalid qualifier: " + str);
      }
    }
  }

  short short_val(const Object& obj)
  {
    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to short");
  }

  unsigned short ushort_val(const Object& obj)
  {
    if (Type::NUMBER.is_type_of(obj))
    {
      if (auto* wrapper = dynamic_cast<const RuntimeValueWrapper*>(&obj))
      {
        return std::get<const Lisple::RTValue::Number>(wrapper->val->value).get_int();
      }

      return obj.as<Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to short");
  }

  unsigned int uint_val(const Object& obj)
  {
    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to unsigned int");
  }

  int int_val(const Object& obj)
  {
    if (auto* wrapper = dynamic_cast<const RuntimeValueWrapper*>(&obj))
    {
      if (wrapper->val->type == RTValue::Type::NUMBER)
      {
        return std::get<const RTValue::Number>(wrapper->val->value).get_int();
      }
    }

    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to int");
  }

  uint8_t uint8_val(const Object& obj)
  {
    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to uint8_t");
  }

  float float_val(const Object& obj)
  {
    if (auto* wrapper = dynamic_cast<const RuntimeValueWrapper*>(&obj))
    {
      if (wrapper->val->type == RTValue::Type::NUMBER)
      {
        return std::get<const RTValue::Number>(wrapper->val->value).get_float();
      }
    }

    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<Number>().float_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to float");
  }

  std::shared_ptr<List> prepend_list_head(Object& list_obj, const std::string& prepend_val)
  {
    List& list = list_obj.as<List>();
    sptr_sobject prepended;

    if (Type::STRING.is_type_of(*list.head()) || Type::NUMBER.is_type_of(*list.head()))
    {
      prepended = std::make_shared<String>(prepend_val + str_val(*list.head()));
    }
    else if (Type::SYMBOL.is_type_of(*list.head()))
    {
      prepended = std::make_shared<Symbol>(prepend_val + str_val(*list.head()));
    }
    else if (Type::QUOTED_SYMBOL.is_type_of(*list.head()))
    {
      prepended = std::make_shared<QuotedSymbol>(prepend_val + str_val(*list.head()));
    }
    else if (Type::KEYWORD.is_type_of(*list.head()))
    {
      prepended = std::make_shared<Keyword>(prepend_val + str_val(*list.head()));
    }

    return subst_sexp_lmnt(list, 0, prepended);
  }

  template <> std::string unwrap_primitive<std::string>(const Object& obj)
  {
    return const_cast<std::string&>(obj.as<String>().value);
  }

  template <> int unwrap_primitive<int>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> short unwrap_primitive<short>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> long unwrap_primitive<long>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> unsigned int unwrap_primitive<unsigned int>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> unsigned short unwrap_primitive<unsigned short>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> unsigned long unwrap_primitive<unsigned long>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> float unwrap_primitive<float>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> double unwrap_primitive<double>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> int8_t unwrap_primitive<int8_t>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> uint8_t unwrap_primitive<uint8_t>(const Object& obj)
  {
    return obj.as<Number>().int_value();
  }

  template <> sptr_sobject wrap_primitive<int>(int value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<short>(short value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<long>(long value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<unsigned int>(unsigned int value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<float>(float value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<double>(double value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<int8_t>(int8_t value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<uint8_t>(uint8_t value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<unsigned long>(unsigned long value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<unsigned short>(unsigned short value)
  {
    return Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<bool>(bool value)
  {
    return Boolean::wrap(value);
  }

  template <> sptr_sobject wrap_primitive<std::string>(const std::string& value)
  {
    return String::make(value);
  }

} // namespace Lisple
