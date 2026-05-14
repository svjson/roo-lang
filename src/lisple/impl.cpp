
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

  const std::string str_val(const AST::ASTNode& obj) noexcept
  {
    if (Type::NUMBER.is_type_of(obj) || *AST::NIL == obj)
    {
      return obj.to_string();
    }

    if (Type::STRING.is_type_of(obj) || Type::KEYWORD.is_type_of(obj) ||
        Type::SYMBOL_VALUE.is_type_of(obj))
    {
      if (auto* wrapper = dynamic_cast<const AST::RuntimeValueWrapper*>(&obj))
      {
        return std::get<std::string>(wrapper->val->value);
      }
      return obj.as<AST::Value<std::string>>().value;
    }

    if (Type::SYMBOL.is_type_of(obj))
    {
      return obj.to_string();
    }

    return EMPTY_STRING;
  }

  char char_val(const AST::ASTNode& obj) noexcept
  {
    if (Type::NUMBER.is_type_of(obj) || *AST::NIL == obj)
    {
      return Lisple::int_val(obj);
    }

    if (Type::CHAR.is_type_of(obj))
    {
      if (auto* wrapper = dynamic_cast<const AST::RuntimeValueWrapper*>(&obj))
      {
        return std::get<char>(wrapper->val->value);
      }
      return obj.as<AST::Value<char>>().value;
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

  short short_val(const AST::ASTNode& obj)
  {
    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<AST::Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to short");
  }

  unsigned short ushort_val(const AST::ASTNode& obj)
  {
    if (Type::NUMBER.is_type_of(obj))
    {
      if (auto* wrapper = dynamic_cast<const AST::RuntimeValueWrapper*>(&obj))
      {
        return std::get<const Lisple::Value::Number>(wrapper->val->value).get_int();
      }

      return obj.as<AST::Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to short");
  }

  unsigned int uint_val(const AST::ASTNode& obj)
  {
    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<AST::Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to unsigned int");
  }

  int int_val(const AST::ASTNode& obj)
  {
    if (auto* wrapper = dynamic_cast<const AST::RuntimeValueWrapper*>(&obj))
    {
      if (wrapper->val->type == Value::Type::NUMBER)
      {
        return std::get<const Value::Number>(wrapper->val->value).get_int();
      }
    }

    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<AST::Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to int");
  }

  uint8_t uint8_val(const AST::ASTNode& obj)
  {
    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<AST::Number>().int_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to uint8_t");
  }

  float float_val(const AST::ASTNode& obj)
  {
    if (auto* wrapper = dynamic_cast<const AST::RuntimeValueWrapper*>(&obj))
    {
      if (wrapper->val->type == Value::Type::NUMBER)
      {
        return std::get<const Value::Number>(wrapper->val->value).get_float();
      }
    }

    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.as<AST::Number>().float_value();
    }

    throw LispleException("Cannot convert " + obj.to_string() + " to float");
  }

  std::shared_ptr<AST::List> prepend_list_head(AST::ASTNode& list_obj,
                                               const std::string& prepend_val)
  {
    AST::List& list = list_obj.as<AST::List>();
    sptr_sobject prepended;

    if (Type::STRING.is_type_of(*list.head()) || Type::NUMBER.is_type_of(*list.head()))
    {
      prepended = std::make_shared<AST::String>(prepend_val + str_val(*list.head()));
    }
    else if (Type::SYMBOL.is_type_of(*list.head()))
    {
      prepended = std::make_shared<AST::Symbol>(prepend_val + str_val(*list.head()));
    }
    else if (Type::QUOTED_SYMBOL.is_type_of(*list.head()))
    {
      prepended = std::make_shared<AST::QuotedSymbol>(prepend_val + str_val(*list.head()));
    }
    else if (Type::KEYWORD.is_type_of(*list.head()))
    {
      prepended = std::make_shared<AST::Keyword>(prepend_val + str_val(*list.head()));
    }

    return subst_sexp_lmnt(list, 0, prepended);
  }

  template <> std::string unwrap_primitive<std::string>(const AST::ASTNode& obj)
  {
    return const_cast<std::string&>(obj.as<AST::String>().value);
  }

  template <> int unwrap_primitive<int>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> short unwrap_primitive<short>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> long unwrap_primitive<long>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> unsigned int unwrap_primitive<unsigned int>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> unsigned short unwrap_primitive<unsigned short>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> unsigned long unwrap_primitive<unsigned long>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> float unwrap_primitive<float>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> double unwrap_primitive<double>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> int8_t unwrap_primitive<int8_t>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> uint8_t unwrap_primitive<uint8_t>(const AST::ASTNode& obj)
  {
    return obj.as<AST::Number>().int_value();
  }

  template <> sptr_sobject wrap_primitive<int>(int value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<short>(short value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<long>(long value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<unsigned int>(unsigned int value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<float>(float value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<double>(double value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<int8_t>(int8_t value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<uint8_t>(uint8_t value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<unsigned long>(unsigned long value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<unsigned short>(unsigned short value)
  {
    return AST::Number::make(value);
  }

  template <> sptr_sobject wrap_primitive<bool>(bool value)
  {
    return AST::Boolean::wrap(value);
  }

  template <> sptr_sobject wrap_primitive<std::string>(const std::string& value)
  {
    return AST::String::make(value);
  }

} // namespace Lisple
