
#ifndef LISPLE__LANG__PREDICATE_H
#define LISPLE__LANG__PREDICATE_H

#include <lisple/exec.h>

namespace Lisple
{
  /*!
   * @brief Tests if a numeric value has a value that is between
   * a lower and an upper bound
   *
   * Usage:
   * @code
   * (between? 10 0 20)
   * => true
   *
   * (between? 5 10 20)
   * => false
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     |Value to test                                                     |
   * | 1     |Lower bounds, exclusive                                           |
   * | 2     |Upper bounds, inclusive                                           |
   */
  FUNC(BetweenPredicateFunction, between);

  /*!
   * @brief Tests a seq or string(sequence of characters) for the presence of
   * any elements.
   *
   * The inverse of @code not-empty? @endcode.
   *
   * Usage:
   * @code
   * (empty? [])
   * => true
   *
   * (empty? [1 2 3])
   * => false
   *
   * (empty? "")
   * => true
   *
   * (empty? "content")
   * => false)
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The seq or string to test                                        |
   */
  FUNC(EmptyPFunction, emptyp)

  /*!
   * @brief Tests a seq or string(sequence of characters) for the presence of any
   * elements.
   *
   * The inverse of @code empty? @endcode
   *
   * Usage:
   * @code
   * (not-empty? [])
   * => false
   *
   * (not-empty? [1 2 3])
   * => true
   *
   * (not-empty? "")
   * => false
   *
   * (not-empty? "content"
   * => true)
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The seq or string to test                                        |
   */
  FUNC(NotEmptyPFunction, not_emptyp)

  FUNC(NotEqualsFunction, not_equals)

  /*!
   * @brief Tests if the result of an expression is nil.
   *
   * Usage:
   * @code
   * (nil? (:key1 {:key2 "value"}))
   * => true
   * @endcode
   *
   * Param 0: The expression, value or identifier to test
   */
  FUNC(NilPFunction, is_nil)

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
