
#ifndef LISPLE__LANG__SEQ_FUNC_H
#define LISPLE__LANG__SEQ_FUNC_H

#include <lisple/exec.h>

namespace Lisple
{
  /*!
   * @brief Returns the index of the first element of a seq that matches a predicate
   * function, or nil if no match is found.
   *
   * Usage:
   * @code
   * (find-index [1 3 7 8 5 10 2] even?)
   * => 3
   *
   * (find-index [1 3 4] even?)
   * => nil
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The sequence to query                                            |
   * | 1     | The predicate function                                           |
   */
  FUNC(FindIndexFunction, find_index)

  /*!
   * @brief Transforms elements of a Seq by applying a function/executable
   * to each element, creating a new Seq containing the transformed elements.
   * The original Seq is not mutated.
   *
   * Usage:
   * @code
   * (map my-seq exec)
   *
   * (map [1 2 3]
   *  (fn [n] (* 2 n)))
   * ==> [2 4 6]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The seq to transform                                             |
   * | 1     | The function/executable to apply to each element                 |
   */
  FUNC(MapFunction, map)

  /*!
   * @brief Performs a functional reduce on a sequence
   *
   * Usage:
   * @code
   * (reduce sequence
   *         {}
   *         (fn [result element] (assoc result (:id element) element)))
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The sequence to reduce                                           |
   * | 1     | The initial value of result                                      |
   * | 2     | A function to apply for each element taking the accumulated      |
   * |       | result and current element as arguments                          |
   */
  FUNC(ReduceFunction, reduce)

  /*!
   * @brief Sorts a sequence according to a predicate function.
   *
   * Usage: (sort [5 3 7 2 8 1] <) => [1 2 3 5 7 8]
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The seq to sort                                                  |
   * | 1     | The comparison function, a functiong taking                      |
   */
  FUNC(SortFunction, sort)

} // namespace Lisple

#endif /* LISPLE__LANG__SEQ_FUNC_H */
