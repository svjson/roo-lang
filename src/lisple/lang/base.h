
#ifndef LISPLE__LANG__BASE_H
#define LISPLE__LANG__BASE_H

#include "../exec.h"

namespace Lisple
{

  /*!
   * @brief Tests if all arguments are truthy
   *
   * Returns true if all arguments evaluate as true/truthy, or false if any
   * argument evaluates as false/falsy.
   *
   * Arguments are lazily evaluated, and evaluation stops if a falsy value
   * is encountered.
   *
   * Usage:
   * @code
   * (and true [] "string")
   * => true
   *
   * (and true false nil)
   * => false
   * @endcode
   */
  SPECIAL_FORM_DECL(AndForm, and)

  /*!
   * @brief Tests if any of the arguments are truthy, and returns
   * the first truthy argument. Otherwise, nil
   *
   * Arguments are lazily evaluated, and evaluation stops if a truthy value
   * is encountered.
   *
   * Usage:
   * @code
   * (or {:x 1 :y 12} false)
   * => {:x 1 :y 12}
   *
   * (or nil false)
   * => nil
   *
   * (or nil true)
   * => true
   * @endcode
   */
  SPECIAL_FORM_DECL(OrForm, or)

} // namespace Lisple

#endif /* LISPLE__LANG__BASE_H */
