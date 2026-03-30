
#ifndef LISPLE__LANG__PREDICATE_H
#define LISPLE__LANG__PREDICATE_H

#include <lisple/exec.h>

namespace Lisple
{

  /*!
   * @brief Inverts a boolean or truthy/falsy expression or value.
   *
   * Usage:
   * @code
   * (not true)
   * => false
   *
   * (not false)
   * => true
   *
   * (not {:key "value"})
   * => false
   *
   * (not nil)
   * => true
   * @endcode
   */
  FUNC(NotFunction, not)

} // namespace Lisple

#endif /* LISPLE__LANG__PREDICATE_H */
