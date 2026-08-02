
#ifndef ROO__LANG__SEQ_H
#define ROO__LANG__SEQ_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Appends one or more elements to a vector, returning a new vector.
   *
   * Usage:
   * @code
   * (append [1 2 3] 4)     => [1 2 3 4]
   * (append [] :a)         => [:a]
   * (append [1 2] 3 4 5)   => [1 2 3 4 5]
   * @endcode
   *
   * | Arg #  | Description                              |
   * |--------|------------------------------------------|
   * | 0      | The vector to append to (nil = empty)    |
   * | 1...   | Elements to append                       |
   */
  FUNC(AppendFunction, append)

  /*!
   * @brief Appends one or more elements to the back of a vector, modifying it in place.
   *
   * Usage:
   * @code
   * (append! v 4)       ; v is now [... 4]
   * (append! v 3 4 5)   ; v is now [... 3 4 5]
   * @endcode
   *
   * | Arg #  | Description                              |
   * |--------|------------------------------------------|
   * | 0      | The vector to mutate                     |
   * | 1...   | Elements to append                       |
   */
  FUNC(AppendBangFunction, append_bang)

  /*!
   * @brief ConcatFunction - splice two or more forms together
   * into an vector. Any non sequence arguments will be treated
   * as vectors of that argument
   *
   * Usage: (concat [1 2] [3 4]) => [1 2 3 4]
   *        (concat 1 2 [3 4]) => [1 2 3 4]
   *
   * Param 0... - The sequences to splice together
   */
  FUNC(ConcatFunction, concat)

  /*!
   * @brief ConcatBangFunction - splice two or more forms together
   * into an the first argument vector. Any subsequent non-sequence arguments
   * will be treated as vectors of that argument.
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
   * (contains? my-vector "a value")
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The Seq to test                                                  |
   * | 1     | The value to test for                                            |
   */
  FUNC(ContainsPFunction, contains)

  /*!
   * @brief Tests if a Seq contains at least one element from another Seq.
   *
   * Usage:
   * @code
   * (contains-any? [1 2 3] [0 3])
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The Seq to test                                                  |
   * | 1     | The Seq of values to test for                                    |
   */
  FUNC(ContainsAnyPFunction, contains_any)

  /*!
   * @brief Tests if a Seq contains all elements from another Seq.
   *
   * Usage:
   * @code
   * (contains-all? [1 2 3] [1 3])
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The Seq to test                                                  |
   * | 1     | The Seq of values to test for                                    |
   */
  FUNC(ContainsAllPFunction, contains_all)

  /*!
   * @brief Return the number of elements in a sequence or map.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Collection to count.                                             |
   */
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

  /*!
   * @brief Return the element at an index in a sequence.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Sequence to read from.                                           |
   * | 1     | Zero-based index to retrieve.                                    |
   */
  FUNC(NthFunction, nth);

  /*! @brief Partition a sequence into an vector of equally sized vectors.
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
   * @brief Repeat a value or series of values n times into an vector.
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

  /*!
   * @brief Creates a new vector containing the elements of a sequence in reverse order.
   *
   * Usage:
   * @code
   * (reverse [1 2 3])
   * => [3 2 1]
   * @endcode
   */
  FUNC(ReverseFunction, reverse)

  /*!
   * @brief Return a sequence without its first element.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Sequence to read from.                                           |
   */
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

} // namespace Roo

#endif /* ROO__LANG__SEQ_H */
