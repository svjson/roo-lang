
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
    if (Type::STRING.is_type_of(obj) ||
        Type::KEY.is_type_of(obj) ||
        Type::SYMBOL.is_type_of(obj) ||
        Type::WORD.is_type_of(obj))
    {
      return obj.as<Value<std::string>>().value;
    }

    if (Type::NUMBER.is_type_of(obj))
    {
      return obj.to_string();
    }

    return EMPTY_STRING;
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

}
