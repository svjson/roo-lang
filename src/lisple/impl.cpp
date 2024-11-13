
#include "impl.h"

#include <memory>
#include <string>

#include "form.h"
#include "type.h"
#include "exception.h"

namespace Lisple
{
  const std::string EMPTY_STRING = "";

  const std::string str_val(const Object& obj) noexcept
  {
    if (Type::NUMBER.is_type_of(obj) ||
        *NIL == obj)
    {
      return obj.to_string();
    }

    if (Type::STRING.is_type_of(obj) ||
        Type::KEY.is_type_of(obj) ||
        Type::SYMBOL.is_type_of(obj) ||
        Type::WORD.is_type_of(obj))
    {
      return obj.as<Value<std::string>>().value;
    }

    return EMPTY_STRING;
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

    if (Type::STRING.is_type_of(*list.head()) ||
        Type::NUMBER.is_type_of(*list.head()))
    {
      prepended = std::make_shared<String>(prepend_val + str_val(*list.head()));
    }
    else if (Type::WORD.is_type_of(*list.head()))
    {
      prepended = std::make_shared<Word>(prepend_val + str_val(*list.head()));
    }
    else if (Type::SYMBOL.is_type_of(*list.head()))
    {
      prepended = std::make_shared<QSymbol>(prepend_val + str_val(*list.head()));
    }
    else if (Type::KEY.is_type_of(*list.head()))
    {
      prepended = std::make_shared<Key>(prepend_val + str_val(*list.head()));
    }

    return subst_sexp_lmnt(list, 0, prepended);
  }

  template <>
  std::string unwrap_primitive<std::string>(const Object& obj)
  {
    return const_cast<std::string&>(obj.as<String>().value);
  }

  template <>
  int unwrap_primitive<int>(const Object& obj)
  {
    return obj.as<Number>().value;
  }

  sptr_sobject wrap_primitive(bool value)
  {
    return Boolean::wrap(value);
  }

  sptr_sobject wrap_primitive(int value)
  {
    return Number::make(value);
  }

  sptr_sobject wrap_primitive(long value)
  {
    return Number::make(value);
  }

  sptr_sobject wrap_primitive(const std::string& value)
  {
    return String::make(value);
  }


}
