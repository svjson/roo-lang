
#ifndef LISPLE__LANG__BASE_H
#define LISPLE__LANG__BASE_H

#include <lisple/exec.h>

namespace Lisple
{

  /*!
   * @brief Defines a var in the current namespace, and optionally
   * assigns it a value.
   *
   * Usage:
   * @code
   * (def x 5)
   * => 5
   *
   * (def y)
   * => nil
   *
   * (def my-map {:a 1 :b 2})
   * => {:a 1 :b 2}
   * @endcode
   */
  SPECIAL_FORM_DECL(DefForm, def, def_docstring)

  /*!
   * @brief Evaluates the body forms sequentially and returns the
   * value of the last form.
   *
   * Usage:
   * @code
   * (do (prn "Hello") (prn "World"))
   * => nil
   *
   * (do
   *   (some-side-effect! 10)
   *   (other-side-effect! 20)
   *   :status/success)
   * => :status/success
   * @endcode
   */
  SPECIAL_FORM_DECL(DoForm, do)

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
   * @brief Extracts the name part of a qualified identifier as a string.
   *
   * Usage:
   * @code
   * (name :accept/ok) => "ok"
   * (name :ok) => "ok"
   * @endcode
   */
  FUNC(NameFunction, name)

  SPECIAL_FORM_DECL(NsForm, ns);

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

  FUNC(RndFunction, rnd)

  /*!
   * @brief Set a new value for a symbol in the lexical scope, namespace or
   * nested structure.
   */
  SPECIAL_FORM_DECL(SetBangForm, set)

  /*!
   * @brief Extracts the qualifier/namespace part of a qualified identifier as
   * a string.
   *
   * Usage:
   * @code
   * (qualifier :accept/ok) => "accept"
   * (qualifier :ok) => nil
   * @endcode
   */
  FUNC(QualifierFunction, qualifier)

} // namespace Lisple

#endif /* LISPLE__LANG__BASE_H */
