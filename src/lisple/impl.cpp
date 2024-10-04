
#include "impl.h"

#include <memory>
#include <string>

#include "form.h"
#include "type.h"
#include "lisple_exception.h"

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
    List& list = list_obj.as<Lisple::List>();
    sptr_sobject prepended;

    if (Lisple::Type::STRING.is_type_of(*list.head()) ||
        Lisple::Type::NUMBER.is_type_of(*list.head()))
    {
      prepended = std::make_shared<Lisple::String>(prepend_val + str_val(*list.head()));
    }
    else if (Lisple::Type::WORD.is_type_of(*list.head()))
    {
      prepended = std::make_shared<Lisple::Word>(prepend_val + str_val(*list.head()));
    }
    else if (Lisple::Type::SYMBOL.is_type_of(*list.head()))
    {
      prepended = std::make_shared<Lisple::QSymbol>(prepend_val + str_val(*list.head()));
    }
    else if (Lisple::Type::KEY.is_type_of(*list.head()))
    {
      prepended = std::make_shared<Lisple::Key>(prepend_val + str_val(*list.head()));
    }

    return subst_sexp_lmnt(list, 0, prepended);
  }

  template <>
  std::string unwrap_primitive<std::string>(const Lisple::Object& obj)
  {
    return const_cast<std::string&>(obj.as<Lisple::String>().value);
  }

  template <>
  int unwrap_primitive<int>(const Lisple::Object& obj)
  {
    return obj.as<Lisple::Number>().value;
  }

  Lisple::sptr_sobject wrap_primitive(bool value)
  {
    return Lisple::Boolean::wrap(value);
  }

  Lisple::sptr_sobject wrap_primitive(int value)
  {
    return Lisple::Number::make(value);
  }

  Lisple::sptr_sobject wrap_primitive(long value)
  {
    return Lisple::Number::make(value);
  }

  Lisple::sptr_sobject wrap_primitive(const std::string& value)
  {
    return Lisple::String::make(value);
  }


}
