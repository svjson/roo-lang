
#ifndef LISPLE__LANG__SEQ_H
#define LISPLE__LANG__SEQ_H

#include <lisple/exec.h>

namespace Lisple
{
  /*!
   * @brief Appends elements to the back of a seq, modifying the
   * seq.
   *
   * Usage: (append! seq 3)
   *        (append! seq 3 4)
   */
  FUNC(AppendBangFunction, append_bang)

  /*!
   * @brief ConcatFunction - splice two or more forms together
   * into an array. Any non sequence arguments will be treated
   * as arrays of that argument
   *
   * Usage: (concat [1 2] [3 4]) => [1 2 3 4]
   *        (concat 1 2 [3 4]) => [1 2 3 4]
   *
   * Param 0... - The sequences to splice together
   */
  FUNC(ConcatFunction, concat)

  FUNC(CountFunction, count)

  /*!
   * @brief Get the first element of a sequence
   *
   * Usage:
   * @code
   * (head [1 2 3]) => 1
   * (head []) => nil
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The sequence to retrieve the head element from                   |
   */
  FUNC(HeadFunction, head)

  /*!
   * @brief Get the last element of a sequence
   *
   * Usage:
   * @code
   * (last [1 2 3]) => 1
   * (last []) => nil
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The sequence to retrieve the last element from                   |
   */
  FUNC(LastFunction, last);

  FUNC(NthFunction, nth);

  /*!
   * @brief Select a random element from a sequence.
   * Returns nil if the sequence is empty
   *
   * Usage:
   * @code
   * (rand-nth [0 1 2 3 4])
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The sequence to retrieve a random element from.                  |
   */
  FUNC(RandNthFunction, rand_nth)

  /**
   * @brief Returns a vector containing all between two numbers, from the the first number
   * (inclusive) to the last number(exclusive)
   *
   * Usage:
   * @code
   * (range 0 5)
   * => [0 1 2 3 4]
   *
   * (range 3)
   * => [0 1 2]
   *
   * (range 5 0)
   * => []
   *
   * (range -2 3)
   * => [-2 -1 0 1 2]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The start number, inclusive. If only one argument is provided,   |
   * |       | this is treated as the end number, and the start number is       |
   * |       | assumed to be 0.                                                 |
   * | 1     | The end number, exclusive. (Optional)                            |
   */
  FUNC(RangeFunction, range)

  /*!
   * @brief Yield a copy of a sequence, omitting element n
   *
   * Usage:
   * @code
   * (remove-nth seq n)
   *
   * (remove-nth [1 2 3] 1)
   * => [1 3]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The seq to copy                                                  |
   * | 1     | The index of the element to omit                                 |
   */
  FUNC(RemoveNthFunction, remove_nth)

} // namespace Lisple

#endif /* LISPLE__LANG__SEQ_H */
