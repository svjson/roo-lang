
#ifndef __LISPLE__IMPL_H_
#define __LISPLE__IMPL_H_

#include <memory>
#include <string>

#include "form.h"
#include "type.h"

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
}

#endif
