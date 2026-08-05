
#ifndef ROO__LANG__SEQ_FUNC_H
#define ROO__LANG__SEQ_FUNC_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Tests if at least one element in a seq satisfies a predicate
   * function.
   *
   * The seq and predicate arguments may be provided in either order.
   *
   * Usage:
   * @code
   * (any? [1 2 3 4] odd?)
   * => true
   *
   * (any? odd? [2 4])
   * => false
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to test                                                    |
   * | predicate | The predicate function.                                            |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | predicate | The predicate function.                                            |
   * | seq       | The seq to test                                                    |
   */
  FUNC(AnyFunction, any)

  /*!
   * @brief Keep only certain elements of a Seq by applying a function/executable
   * to each element.
   *
   * A new Seq is created containing only those elements for which the predicate
   * function returns a truthy value. The seq and function arguments may be
   * provided in either order.
   *
   * Usage:
   * @code
   * (filter my-seq exec)
   *
   * (filter even? [1 2 3 4])
   * => [2 4]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to filter                                                  |
   * | f         | The function/executable to apply to each element                   |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | f         | The function/executable to apply to each element                   |
   * | seq       | The seq to filter                                                  |
   */
  FUNC(FilterFunction, filter)

  /*!
   * @brief Returns the first element of a seq that matches a predicate function,
   * or nil if no match is found.
   *
   * The seq and predicate arguments may be provided in either order.
   *
   * Usage:
   * @code
   * (find-first [1 2 3 4 5] even?)
   * => 2
   *
   * (find-first even? [1 3 4])
   * => 4
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to query                                                   |
   * | predicate | The predicate function.                                            |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | predicate | The predicate function.                                            |
   * | seq       | The seq to query                                                   |
   */
  FUNC(FindFirstFunction, find_first)

  /*!
   * @brief Returns the index of the first element of a seq that matches a predicate
   * function, or nil if no match is found.
   *
   * The seq and predicate arguments may be provided in either order.
   *
   * Usage:
   * @code
   * (find-index [1 3 7 8 5 10 2] even?)
   * => 3
   *
   * (find-index even? [1 3 4])
   * => 2
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to query                                                   |
   * | predicate | The predicate function.                                            |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | predicate | The predicate function.                                            |
   * | seq       | The seq to query                                                   |
   */
  FUNC(FindIndexFunction, find_index)

  /*!
   * @brief Maps a function over a sequence and flattens the results one level
   * into a single sequence.
   *
   * Elements of the mapped result that are sequences are flattened; non-sequence
   * values are included directly. The seq and function arguments may be provided
   * in either order.
   *
   * Usage:
   * @code
   * (flat-map [1 2 3] (fn [n] [n (* n 10)]))
   * => [1 10 2 20 3 30]
   *
   * (flat-map identity [[1 2] 3 [4 5]])
   * => [1 2 3 4 5]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to transform                                               |
   * | f         | A function; seq results are flattened, scalars are kept as-is      |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | f         | A function; seq results are flattened, scalars are kept as-is      |
   * | seq       | The seq to transform                                               |
   */
  FUNC(FlatMapFunction, flat_map)

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
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | seq   | The seq to query                                                   |
   * | value | The value to find                                                  |
   */
  FUNC(IndexOfFunction, index_of)

  /*!
   * @brief Essentially functions as a combination of map and filter, but
   * determines if an element is to be kept based on non-nil/nil instead
   * of truthiness, like map.
   *
   * The seq and function arguments may be provided in either order.
   *
   * Usage:
   * @code
   * (keep [1 2 3 4] (fn [x]
   *                   (when (even? x) (str "Number " x))))
   * => ["Number 2" "Number 4"]
   *
   * (keep (fn [x] (when (even? x) (* x 10))) [1 2 3])
   * => [20]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to transform                                               |
   * | f         | Function applied to each element. Non-nil results are kept.        |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | f         | Function applied to each element. Non-nil results are kept.        |
   * | seq       | The seq to transform                                               |
   */
  FUNC(KeepFunction, keep)

  /*!
   * @brief Transforms elements of one or more seqs by applying a
   * function/executable, creating a new Seq containing the transformed elements.
   * The original seqs are not mutated.
   *
   * The function may be provided before or after the seq arguments.
   *
   * Usage:
   * @code
   * (map my-seq exec)
   *
   * (map [1 2 3] (fn [n] (* 2 n)))
   * => [2 4 6]
   *
   * (map + [1 2 3] [30 20 10])
   * => [31 22 13]
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | seqs... | One or more seqs to transform                                      |
   * | f       | The function/executable to apply to each element group             |
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | f       | The function/executable to apply to each element group             |
   * | seqs... | One or more seqs to transform                                      |
   */
  FUNC(MapFunction, map)

  /*!
   * @brief Performs a functional reduce on a sequence.
   *
   * The seq and reducer function may be provided in either order; the initial
   * value follows the seq in both forms.
   *
   * Usage:
   * @code
   * (reduce sequence
   *         {}
   *         (fn [result element] (assoc result (:id element) element)))
   *
   * (reduce + [1 2 3] 0)
   * => 6
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to reduce                                                  |
   * | init      | The initial value of result                                        |
   * | f         | A function to apply for each element taking the accumulated        |
   * |           | result and current element as arguments                            |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | f         | A function to apply for each element taking the accumulated        |
   * |           | result and current element as arguments                            |
   * | seq       | The seq to reduce                                                  |
   * | init      | The initial value of result                                        |
   */
  FUNC(ReduceFunction, reduce)

  /*!
   * @brief Keep only certain elements of a Seq by applying a function/executable
   * to each element, creating a new Seq without those elements for which the
   * predicate function returns a truthy value.
   *
   * Effectively the inverse of @code filter @endcode. The seq and function
   * arguments may be provided in either order.
   *
   * Usage:
   * @code
   * (remove my-seq exec)
   *
   * (remove even? [1 2 3 4])
   * => [1 3]
   *
   * (remove nil? [1 2 nil 5 6 nil 8 nil])
   * => [1 2 4 5 6 8]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to filter                                                  |
   * | f         | The function/executable to apply to each element                   |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | f         | The function/executable to apply to each element                   |
   * | seq       | The seq to filter                                                  |
   */
  FUNC(RemoveFunction, remove)

  /*!
   * @brief Remove elements from a Seq by applying a function/executable to each
   * element, removing any element for which the predicate function returns a
   * truthy value.
   *
   * The seq and function arguments may be provided in either order.
   *
   * Usage:
   * @code
   * (remove! exec seq)
   *
   * (remove! seq exec)
   *
   * (remove! even? [1 2 3 4])
   * => [1 3]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | f         | The function/executable to apply to each element                   |
   * | seq       | The seq to modify                                                  |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to modify                                                  |
   * | f         | The function/executable to apply to each element                   |
   */
  FUNC(RemoveBangFunction, remove_bang)

  /*!
   * @brief Yield a new Seq containing all elements except the first element that
   * yields a truthy value when applying a function/executable to each element.
   *
   * The seq and function arguments may be provided in either order.
   *
   * Usage:
   * @code
   * (remove-first exec my-seq)
   *
   * (remove-first [1 2 3 4] even?)
   * => [1 3 4]
   *
   * (remove-first nil? [1 2 nil 5 6 nil 8 nil])
   * => [1 2 5 6 nil 8 nil]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | f         | The function/executable to apply to each element                   |
   * | seq       | The seq to filter                                                  |
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The seq to filter                                                  |
   * | f         | The function/executable to apply to each element                   |
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
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | seq     | The seq to query                                                   |
   * | pattern | The partial map pattern to match                                   |
   */
  FUNC(SeqMatchFunction, match)

  /*!
   * @brief Sorts a sequence according to a predicate function.
   *
   * The seq and comparison function may be provided in either order.
   *
   * Usage:
   * @code
   * (sort [5 3 7 2 8 1] <)
   * => [1 2 3 5 7 8]
   *
   * (sort > [5 3 7 2 8 1])
   * => [8 7 5 3 2 1]
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | seq     | The seq to sort                                                    |
   * | compare | Comparison function used to order elements.                        |
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | compare | Comparison function used to order elements.                        |
   * | seq     | The seq to sort                                                    |
   */
  FUNC(SortFunction, sort)

} // namespace Roo

#endif /* ROO__LANG__SEQ_FUNC_H */
