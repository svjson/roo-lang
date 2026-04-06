
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

  /*!
   * @brief ConcatBangFunction - splice two or more forms together
   * into an the first argument array. Any subsequent non-sequence arguments
   * will be treated as arrays of that argument.
   *
   * Usage: (concat! [1 2] [3 4]) => [1 2 3 4]
   *        (concat! [1] 2 [3 4]) => [1 2 3 4]
   *
   * Param 0... - The sequences to splice together
   */
  FUNC(ConcatBangFunction, concat_bang)

  /*!
   * @brief Tests if a Seq contains a specific value
   *
   * Usage:
   * @code
   * (contains? my-array "a value")
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The Seq to test                                                  |
   * | 1     | The value to test for                                            |
   */
  FUNC(ContainsPFunction, contains)

  FUNC(CountFunction, count)

  /*!
   * @brief FlattenFunction - flatten one or more sequences objects
   * into a single level vector
   *
   * Usage: (flatten [[1 2 3] [4 5 6]]) => [1 2 3 4 5 6]
   */
  FUNC(FlattenFunction, flatten)

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

  /*! @brief Partition a sequence into an array of equally sized arrays.
   *
   * Usage:
   * @code
   * (partition 2 [1 2 3 4 5 6])
   * => [[1 2] [3 4] [5 6]]
   *
   * (partition 4 [1 2 3 4 5 6])
   * => [[1 2 3 4] [5 6]]
   * @endcode
   */
  FUNC(PartitionFunction, partition)

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

  /*!
   * @brief Remove the nth value of a Seq
   *
   * Usage:
   * @code
   * (remove-nth! seq n)
   *
   * (remove-nth! [1 2 3] 1)
   * => [1 3]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The seq to mutate                                                |
   * | 1     | The index of the element to remove                               |
   */
  FUNC(RemoveNthBangFunction, remove_nth_bang)

  /*!
   * @brief Repeat a value or series of values n times into an array.
   *
   * Usage:
   * @code
   * (repeat 3 "value")
   * => ["value" "value" "value"]
   * (repeat 3 :a :b)
   * => [:a :b :a :b :a :b]
   * @endcode
   */
  FUNC(RepeatFunction, repeat)

  FUNC(TailFunction, tail)

  /*!
   * @brief Creates a new sequence from the n first elements of a sequence.
   * If n is larger than the size of the sequence, all elements are kept.
   *
   * Usage:
   * @code
   * (take 2 [1 2 3 4 5])
   * => [1 2]
   *
   * (take 200 [1 2 3 4 5])
   * => [1 2 3 4 5]
   * @endcode
   */
  FUNC(TakeFunction, take)

  /*!
   * @brief Creates a vector from its arguments.
   *
   * Usage:
   * @code
   * (vector 1 2 3) => [1 2 3]
   * @endcode
   */
  FUNC(VectorFunction, vector)

} // namespace Lisple

#endif /* LISPLE__LANG__SEQ_H */
