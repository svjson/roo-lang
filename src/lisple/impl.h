
#ifndef __LISPLE__IMPL_H_
#define __LISPLE__IMPL_H_

#include <memory>
#include <string>
#include <type_traits>

#include "form.h"
#include "type.h"
#include "lisple_exception.h"

namespace Lisple
{
  extern const std::string EMPTY_STRING;

  /**
   * Get the string value of lisple symbol
   */
  const std::string str_val(const Object& obj) noexcept;

  /**
   * Get the unsigned int value of lisple symbol
   */
  unsigned int uint_val(const Object& obj);
  uint8_t uint8_val(const Object& obj);
  int int_val(const Object& obj);
  short short_val(const Object& obj);
  float float_val(const Object& obj);

  /**
   * Create a copy of a sexp form with a substituted child element
   */
  template <class T>
  std::shared_ptr<T> subst_sexp_lmnt(const T& sexp, unsigned int index, sptr_sobject subst)
  {
    sptr_sobject_v nchildren = const_cast<T&>(sexp).get_children();
    nchildren.at(index) = subst;
    return std::make_shared<T>(nchildren);
  }

  /**
   * Prepend the string value of the head element of a list
   */
  std::shared_ptr<List> prepend_list_head(Object& list_obj, const std::string& prepend_val);

  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, T>::type unwrap_primitive(const Lisple::Object &obj)
  {
    return obj.as<Lisple::Number>().long_value();
  }

  template <typename T>
  //  T unwrap_primitive(const Lisple::Object&);
  typename std::enable_if<!std::is_integral<T>::value, T>::type unwrap_primitive(const Lisple::Object &obj);

  Lisple::sptr_sobject wrap_primitive(bool value);
  Lisple::sptr_sobject wrap_primitive(int value);
  Lisple::sptr_sobject wrap_primitive(long value);
  Lisple::sptr_sobject wrap_primitive(const std::string& value);


}

#endif
