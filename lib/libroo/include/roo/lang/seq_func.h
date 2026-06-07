
#ifndef ROO__LANG__SEQ_FUNC_H
#define ROO__LANG__SEQ_FUNC_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Keep only certain elements of a Seq by applying a function/executable
   * to each element.
   *
   * A new Seq is created containing only hose elements for which the predicate
   * function returns a truthy value.
   *
   * Usage:
   * @code
   * (filter my-seq exec)
   *
   * (filter [1 2 3 4] (fn [n] (even? n)))
   * => [2 4]
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The seq to filter                                                |
   * | 1     | The function/executable to apply to each element                 |
   */
  FUNC(FilterFunction, filter)

  /*!
   * @brief Returns the first element of a seq that matches a predicate function,
   * or nil if no match is found.
   *
   * Usage:
   * @code
   * (find-first [1 2 3 4 5] even?)
   * => 2
   *
   * (find-first [1 3 4] even?)
   * => nil
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The sequence to query                                            |
   * | 1     | The predicate function                                           |
   */
  FUNC(FindFirstFunction, find_first)

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
   * @brief Returns the index of the first element in a seq equal to a value,
   * or nil if no match is found.
   *
   * Usage:
   * @code
   * (index-of ["a" "b" "c"] "b")
   * => 1
   *
   * (index-of [1 3 4] 2)
   * => nil
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The sequence to query                                            |
   * | 1     | The value to find                                                |
   */
  FUNC(IndexOfFunction, index_of)

  /*!
   * @brief Essentially functions as a combination of map and filter, but
   * determines if an element is to be kept based on non-nil/nil instead
   * of truthiness, like map.
   *
   * Usage:
   * @code
   * (keep [1 2 3 4] (fn [x]
   *                   (when (even? nil) (str "Number " x))))
   * => ["Number 2" "Number 4"]
   * @endcode
   */
  FUNC(KeepFunction, keep)

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
   * @brief Keep only certain elements of a Seq by applying a function/executable
   * to each element, creating a new Seq without those elements for which the
   * predicate function returns a truthy value.
   *
   * Effectively the inverse of @code filter @endcode
   *
   * Usage:
   * @code
   * (remove exec my-seq)
   *
   * (remove (fn [n] (even? n)) [1 2 3 4])
   * => [1 3]
   *
   * (remove nil? [1 2 nil 5 6 nil 8 nil])
   * => [1 2 4 5 6 8]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The function/executable to apply to each element                 |
   * | 1     | The seq to filter                                                |
   */
  FUNC(RemoveFunction, remove)

  /*!
   * @brief Remove elements from a Seq by applying a function/executable to each
   * element, removing any element for which the predicate function returns a
   * truthy value.
   *
   * Usage:
   * @code
   * (remove! exec seq)
   *
   * (remove! (fn [n] (even? n)) [1 2 3 4]
   * => [1 3])
   *
   * (remove nil? [1 2 nil 5 6 nil 8 nil])
   * => [1 2 4 5 8]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The function/executable to apply to each element                 |
   * | 1     | The seq to modify                                                |
   */
  FUNC(RemoveBangFunction, remove_bang)

  /*!
   * @brief Yield a new Seq containing all elements except the first element that
   * yields a truthy value when applying a function/executable to each element.
   *
   * Usage:
   * @code
   * (remove-first exec my-seq)
   *
   * (remove-first (fn [n] (even? n)) [1 2 3 4])
   * => [1 3 4]
   *
   * (remove nil? [1 2 nil 5 6 nil 8 nil])
   * => [1 2 4 5 6 nil 8 nil]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The function/executable to apply to each element                 |
   * | 1     | The seq to filter                                                |
   */
  FUNC(RemoveFirstFunction, remove_first)

  /*!
   * @brief Query a seq against a pattern in form of a partial map. Returns the
   * first element that matches all defined keys in the pattern
   *
   * Usage:
   * @code
   * (seq-match seq {:id 2 :status {:desc "disabled"}})
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The sequence to query                                            |
   * | 1     | The partial map pattern to match                                 |
   */
  FUNC(SeqMatchFunction, match)

  /*!
   * @brief Tests if at least one element in a seq satisfies a predicate
   * function.
   *
   * Usage:
   * @code
   * (some? [1 2 3 4] odd?)
   * => true
   *
   * (some? [2 4] odd?)
   * => false
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The seq to test                                                  |
   * | 1     | The predicate function.                                          |
   */
  FUNC(SomeFunction, some)

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

} // namespace Roo

#endif /* ROO__LANG__SEQ_FUNC_H */
