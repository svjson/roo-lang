
#ifndef ROO__LANG__SEQ_H
#define ROO__LANG__SEQ_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Appends one or more elements to a sequential value, returning a new vector.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (append [1 2 3] 4)     => [1 2 3 4]
   * (append [] :a)         => [:a]
   * (append [1 2] 3 4 5)   => [1 2 3 4 5]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | seq       | The sequential value to append to (`nil` = empty)                  |
   * | values... | Elements to append                                                 |
   *
   * @return A new vector containing the original elements followed by `values`.
   *         Non-sequential targets are rejected.
   */
  FUNC(AppendFunction, append)

  /*!
   * @brief Appends one or more elements to the back of a vector, modifying it in place.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (append! v 4)       ; v is now [... 4]
   * (append! v 3 4 5)   ; v is now [... 3 4 5]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | vector    | The vector to mutate                                               |
   * | values... | Elements to append                                                 |
   *
   * @return The mutated vector.
   */
  FUNC(AppendBangFunction, append_bang)

  /*!
   * @brief ConcatFunction - splice two or more forms together
   * into an vector. Any non sequence arguments will be treated
   * as vectors of that argument
   * @since 0.1.0
   *
   * Usage: (concat [1 2] [3 4]) => [1 2 3 4]
   *        (concat 1 2 [3 4]) => [1 2 3 4]
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | seqs... | The seqs to splice together.                                       |
   *
   * @return A new vector containing the spliced values.
   */
  FUNC(ConcatFunction, concat)

  /*!
   * @brief ConcatBangFunction - splice two or more forms together
   * into an the first argument vector. Any subsequent non-sequence arguments
   * will be treated as vectors of that argument.
   * @since 0.1.0
   *
   * Usage: (concat! [1 2] [3 4]) => [1 2 3 4]
   *        (concat! [1] 2 [3 4]) => [1 2 3 4]
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | seqs... | The seqs to splice into the first argument.                        |
   *
   * @return The mutated first vector.
   */
  FUNC(ConcatBangFunction, concat_bang)

  /*!
   * @brief Tests whether a sequence contains a value or a string contains text.
   * @since 0.1.0
   *
   * Sequences use element equality. Strings search for a substring when the
   * needle is a string, or for one character when the needle is a character.
   * An empty string is a substring of every string. Other needle types do not
   * match strings. A `nil` target is treated as an empty collection.
   *
   * For maps, both keys and values can match. Use `contains-key?` when only
   * keys should match.
   *
   * Usage:
   * @code
   * (contains? [1 2 3] 2) => true
   * (contains? "this is my grand string" "grand") => true
   * (contains? "grand" 'g') => true
   * @endcode
   *
   * | Arg    | Description                                                       |
   * | ------ | ----------------------------------------------------------------- |
   * | target | The sequence or string to search.                                 |
   * | needle | The element, substring, or character to find.                    |
   *
   * @return `true` when `target` contains `needle`, otherwise `false`.
   */
  FUNC(ContainsPFunction, contains)

  /*!
   * @brief Tests if a Seq contains at least one element from another Seq.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (contains-any? [1 2 3] [0 3])
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | seq    | The seq to test.                                                   |
   * | values | The seq of values to test for.                                     |
   *
   * @return `true` when `seq` contains any element from `values`, otherwise
   * `false`.
   */
  FUNC(ContainsAnyPFunction, contains_any)

  /*!
   * @brief Tests if a Seq contains all elements from another Seq.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (contains-all? [1 2 3] [1 3])
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | seq    | The seq to test.                                                   |
   * | values | The seq of values to test for.                                     |
   *
   * @return `true` when `seq` contains every element from `values`, otherwise
   * `false`.
   */
  FUNC(ContainsAllPFunction, contains_all)

  /*!
   * @brief Return the number of elements in a sequence or map.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (count [1 2 3])
   * => 3
   *
   * (count {:name "Gonzo" :age 8})
   * => 2
   *
   * (count nil)
   * => 0
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Seq or map to count.                                               |
   *
   * @return The number of elements in `value`.
   */
  FUNC(CountFunction, count)

  /*!
   * @brief Exclude a half-open range from a sequence or string.
   * @since 0.1.0
   *
   * The start index is inclusive and the optional end index is exclusive. If
   * end is omitted, elements from start onward are omitted. Negative indexes
   * are offsets from the end of the sequence. Bounds are clamped.
   *
   * Usage:
   * @code
   * (drop [1 2 3 4 5] 1 4) => [1 5]
   * (drop [1 2 3 4 5] -1) => [1 2 3 4]
   * (drop "abcde" 1 4) => "ae"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | seq   | Sequential value or string to read from.                           |
   * | start | Inclusive start index. Negative indexes count from the end.        |
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | seq   | Sequential value or string to read from.                           |
   * | start | Inclusive start index. Negative indexes count from the end.        |
   * | end   | Exclusive end index. Negative indexes count from the end.          |
   *
   * @return A string for string input; otherwise a new vector.
   */
  FUNC(DropFunction, drop)

  /*!
   * @brief FlattenFunction - flatten one or more sequences objects
   * into a single level vector
   * @since 0.1.0
   *
   * Usage: (flatten [[1 2 3] [4 5 6]]) => [1 2 3 4 5 6]
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | seqs... | Seqs to flatten into a single vector.                              |
   *
   * @return A new vector containing the flattened elements.
   */
  FUNC(FlattenFunction, flatten)

  /*!
   * @brief Get the first element of a sequence
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (head [1 2 3]) => 1
   * (head []) => nil
   * @endcode
   *
   * | Arg  | Description                                                        |
   * | ---- | ------------------------------------------------------------------ |
   * | seq  | The sequence to retrieve the head element from                     |
   *
   * @return The first element of `seq`, or `nil` when it is empty.
   */
  FUNC(HeadFunction, head)

  /*!
   * @brief Insert values at a sequence or string boundary without mutation.
   * @since 0.1.0
   * @see roo/insert-one
   * @see roo/insert!
   *
   * The position is clamped to the sequence bounds. Negative positions count from
   * the end, so -1 inserts before the last element. The original sequence is
   * not mutated.
   *
   * Usage:
   * @code
   * (insert [:a :d] 1 [:b :c])
   * => [:a :b :c :d]
   * (insert "abcd" 2 [8 true])
   * => "ab8truecd"
   * (insert "abcd" 2 8)
   * => "ab8cd"
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | seq    | Sequential value to copy.                                          |
   * | pos    | Insertion boundary. Negative positions count from the end.         |
   * | values | Sequential values to insert.                                       |
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | string | String to copy.                                                    |
   * | pos    | Insertion boundary. Negative positions count from the end.         |
   * | values | Value or sequential values to convert to text and insert.          |
   *
   * For string targets, sequential values are expanded and each element is
   * converted to text. A non-sequential value is converted to text and inserted
   * once. A nil target is an empty sequence when `values` is sequential; with a
   * non-sequential `values` argument it produces nil.
   *
   * @return A new vector or string with `values` inserted at `pos`, or nil for a
   *         nil target and non-sequential `values`.
   */
  FUNC(InsertFunction, insert_seq, insert_string)

  /*!
   * @brief Insert sequential values by mutating a sequence.
   * @since 0.1.0
   * @see roo/insert
   * @see roo/insert-one!
   *
   * Each element of `values` is inserted into the target in order. The source
   * values are captured before mutation, so the source and target may be the
   * same sequence.
   *
   * The position is clamped to the target bounds. Negative positions count
   * from the end, so -1 inserts before the final element. Vector, list, and
   * mutable host-sequence targets are mutated in place and returned. A `nil`
   * target produces and returns a new vector because there is no collection
   * identity to mutate. Maps and immutable strings are rejected.
   *
   * Usage:
   * @code
   * (insert! values 1 [:b :c])
   * @endcode
   *
   * | Arg    | Description                                                         |
   * | ------ | ------------------------------------------------------------------- |
   * | seq    | Sequential value to mutate.                                         |
   * | pos    | Insertion boundary. Negative positions count from the end.          |
   * | values | Sequential values to insert.                                        |
   *
   * @return The mutated `seq`, or a new vector if `seq` was passed with a `nil` value.
   */
  FUNC(InsertBangFunction, insert_bang)

  /*!
   * @brief Insert one value at a sequence or string boundary without mutation.
   * @since 0.1.0
   * @see roo/insert
   * @see roo/insert-one!
   * @see roo/str
   *
   * The value is inserted as exactly one element when the target is a sequence,
   * even when the value is itself sequential. A `nil` target is treated as an
   * empty sequence. The operation returns a new vector and does not mutate the
   * target.
   *
   * For a string target, the value is converted once using its `str`
   * representation and inserted into a new string. Under the current text
   * model, string positions are byte boundaries.
   *
   * The position is clamped to the target bounds. Negative positions count
   * from the end, so -1 inserts before the final element or byte. Maps are not
   * valid targets.
   *
   * Usage:
   * @code
   * (insert-one [:a :c] 1 :b)
   * => [:a :b :c]
   * (insert-one "abcd" 2 true)
   * => "abtruecd"
   * @endcode
   *
   * | Arg   | Description                                                         |
   * | ----- | ------------------------------------------------------------------- |
   * | seq   | Sequential value to copy. Nil is an empty sequence.                 |
   * | pos   | Insertion boundary. Negative positions count from the end.          |
   * | value | Complete value to insert as one element.                            |
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | string | String to copy.                                                    |
   * | pos    | Insertion boundary. Negative positions count from the end.         |
   * | value  | Complete value to convert to text and insert.                      |
   *
   * @return A new vector or string with `value` inserted at `pos`.
   */
  FUNC(InsertOneFunction, insert_one_seq, insert_one_string)

  /*!
   * @brief Insert one value by mutating a sequence.
   * @since 0.1.0
   * @see roo/insert-one
   * @see roo/insert!
   *
   * The value is inserted as exactly one element, even when it is itself
   * sequential. The position is clamped to the target bounds. Negative
   * positions count from the end, so -1 inserts before the final element.
   *
   * Vector, list, and mutable host-sequence targets are mutated in place and
   * returned. A `nil` target produces and returns a new vector because there is
   * no collection identity to mutate. Maps and immutable strings are rejected.
   *
   * Usage:
   * @code
   * (def values [:a :c])
   * (insert-one! values 1 :b)
   * => [:a :b :c]
   * @endcode
   *
   * | Arg   | Description                                                         |
   * | ----- | ------------------------------------------------------------------- |
   * | seq   | Mutable sequential target. Nil produces a new vector.               |
   * | pos   | Insertion boundary. Negative positions count from the end.          |
   * | value | Complete value to insert as one element.                            |
   *
   * @return The mutated `seq`, or a new vector for a `nil` target.
   */
  FUNC(InsertOneBangFunction, insert_one_bang)

  /*!
   * @brief Replace a half-open range in a sequence or string without mutation.
   * @since 0.1.0
   * @see roo/drop
   * @see roo/insert
   * @see roo/replace!
   *
   * The start index is inclusive and the end index is exclusive. Negative
   * indexes count from the end and bounds are clamped. An empty range inserts
   * `values` at `start`; empty replacement values only remove the range.
   *
   * Sequence targets require sequential replacement values and return a new
   * vector. String targets use the same conversion rules as `insert`: a
   * sequential value is expanded and each element is converted to text, while
   * a non-sequential value is converted and inserted once. String indexes are
   * byte boundaries under the current text model.
   *
   * Usage:
   * @code
   * (replace [1 2 3 4 5] 2 4 [:a :b :c])
   * => [1 2 :a :b :c 5]
   * (replace "abcdef" 2 4 "XYZ")
   * => "abXYZef"
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | seq    | Sequential value to copy.                                          |
   * | start  | Inclusive start index. Negative indexes count from the end.        |
   * | end    | Exclusive end index. Negative indexes count from the end.          |
   * | values | Sequential values to splice into the replaced range.               |
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | string | String to copy.                                                    |
   * | start  | Inclusive start index. Negative indexes count from the end.        |
   * | end    | Exclusive end index. Negative indexes count from the end.          |
   * | values | Value or sequential values to convert to replacement text.         |
   *
   * @return A new vector or string with the range replaced.
   */
  FUNC(ReplaceFunction, replace_seq, replace_string)

  /*!
   * @brief Replace a half-open range by mutating a sequence.
   * @since 0.1.0
   * @see roo/replace
   *
   * Vector, list, and mutable host-sequence targets are mutated in place and
   * returned. Replacement values are captured before mutation, so the source
   * and target may be the same sequence. Strings are immutable and rejected.
   *
   * Usage:
   * @code
   * (replace! values 2 4 [:a :b :c])
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | seq    | Sequential value to mutate.                                        |
   * | start  | Inclusive start index. Negative indexes count from the end.        |
   * | end    | Exclusive end index. Negative indexes count from the end.          |
   * | values | Sequential values to splice into the replaced range.               |
   *
   * @return The mutated `seq`, or a new vector if `seq` is nil.
   */
  FUNC(ReplaceBangFunction, replace_bang)

  /*!
   * @brief Move an element by index, returning a new vector.
   * @since 0.1.0
   *
   * The three-argument form places the element at a final index. The optional
   * placement form accepts :final, :before, or :after. Relative placements
   * identify the target in the input ordering and account for the source
   * removal. All indexes use clamped, end-relative normalization. The original
   * sequence is not mutated.
   *
   * Usage:
   * @code
   * (move-nth [:a :b :c :d] 1 3)
   * => [:a :c :d :b]
   *
   * (move-nth [:a :b :c :d] 1 :before 3)
   * => [:a :c :b :d]
   * @endcode
   *
   * | Arg          | Description                                                    |
   * | ------------ | -------------------------------------------------------------- |
   * | seq          | Sequential value to copy.                                      |
   * | from-index   | Index of the element to move.                                  |
   * | target-index | Final index for the moved element.                             |
   *
   * | Arg          | Description                                                    |
   * | ------------ | -------------------------------------------------------------- |
   * | seq          | Sequential value to copy.                                      |
   * | from-index   | Index of the element to move.                                  |
   * | placement    | `:final`, `:before`, or `:after` placement mode.               |
   * | target-index | Final index, or target index in the input ordering.            |
   *
   * @return A new vector with the selected element at the requested placement.
   */
  FUNC(MoveNthFunction, move_nth)

  /*!
   * @brief Move an element by index by mutating a sequence.
   * @since 0.1.0
   *
   * Uses the same final, :final, :before, and :after placement semantics as
   * move-nth. Nil is treated as an empty sequence and produces a new vector
   * because there is no collection identity to mutate.
   *
   * Usage:
   * @code
   * (move-nth! values 1 :before 3)
   * @endcode
   *
   * | Arg          | Description                                                    |
   * | ------------ | -------------------------------------------------------------- |
   * | seq          | Sequential value to mutate.                                    |
   * | from-index   | Index of the element to move.                                  |
   * | target-index | Final index for the moved element.                             |
   *
   * | Arg          | Description                                                    |
   * | ------------ | -------------------------------------------------------------- |
   * | seq          | Sequential value to mutate.                                    |
   * | from-index   | Index of the element to move.                                  |
   * | placement    | `:final`, `:before`, or `:after` placement mode.               |
   * | target-index | Final index, or target index in the input ordering.            |
   *
   * @return The mutated sequence, or a new empty vector for nil.
   */
  FUNC(MoveNthBangFunction, move_nth_bang)

  /*!
   * @brief Interleave elements from sequences into one vector.
   * @since 0.1.0
   *
   * Takes one element from each sequence in order, then repeats at the next
   * index until all sequences are exhausted. Shorter sequences are skipped
   * after their last element.
   *
   * Usage:
   * @code
   * (interleave [1 2 3] [:a :b :c])
   * => [1 :a 2 :b 3 :c]
   *
   * (interleave [1 2 3] [", " ", "])
   * => [1 ", " 2 ", " 3]
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | seqs... | Seqs or strings to interleave.                                     |
   *
   * @return A new vector with the interleaved contents of `seqs`...
   */
  FUNC(InterleaveFunction, interleave)

  /*!
   * @brief Get the last element of a sequence
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (last [1 2 3]) => 3
   * (last []) => nil
   * @endcode
   *
   * | Arg | Description                                                            |
   * | ----| ---------------------------------------------------------------------- |
   * | seq | The sequence to retrieve the last element from                         |
   *
   * @return The last element of `seq`, or `nil` when it is empty.
   */
  FUNC(LastFunction, last);

  /*!
   * @brief Return the element at an index in a sequence.
   * @since 0.1.0
   *
   * | Arg      | Description                                                       |
   * | -------- | ----------------------------------------------------------------- |
   * | seq      | Sequence to read from.                                            |
   * | index    | Zero-based index to retrieve.                                     |
   *
   * @return The element at `index`, or `nil` when the index is out of bounds.
   */
  FUNC(NthFunction, nth);

  /*! @brief Partition a sequence into an vector of equally sized vectors.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (partition 2 [1 2 3 4 5 6])
   * => [[1 2] [3 4] [5 6]]
   *
   * (partition 4 [1 2 3 4 5 6])
   * => [[1 2 3 4] [5 6]]
   * @endcode
   *
   * | Arg  | Description                                                        |
   * | ---- | ------------------------------------------------------------------ |
   * | size | Partition size.                                                    |
   * | seq  | Seq to partition.                                                  |
   *
   * @return A new vector containing the partitions.
   */
  FUNC(PartitionFunction, partition)

  /*!
   * @brief Select a random element from a sequence.
   * Returns nil if the sequence is empty
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (rand-nth [0 1 2 3 4])
   * @endcode
   *
   * | Arg  | Description                                                        |
   * | ---- | ------------------------------------------------------------------ |
   * | seq  | The sequence to retrieve a random element from.                    |
   *
   * @return A random element from `seq`, or `nil` when it is empty.
   */
  FUNC(RandNthFunction, rand_nth)

  /**
   * @brief Returns a vector containing all between two numbers, from the the first number
   * (inclusive) to the last number(exclusive)
   * @since 0.1.0
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
   * | Arg | Description                                                          |
   * | --- | -------------------------------------------------------------------- |
   * | end | The end number, exclusive. The start number is assumed to be 0.      |
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | start | The start number, inclusive.                                       |
   * | end   | The end number, exclusive.                                         |
   *
   * @return A new vector containing the numbers in the requested range.
   */
  FUNC(RangeFunction, range)

  /*!
   * @brief Yield a copy of a sequence, omitting element n
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (remove-nth seq n)
   *
   * (remove-nth [1 2 3] 1)
   * => [1 3]
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | seq   | The seq to copy                                                    |
   * | index | The index of the element to omit                                   |
   *
   * @return A new vector without the element at `index`.
   */
  FUNC(RemoveNthFunction, remove_nth)

  /*!
   * @brief Remove the nth value of a Seq
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (remove-nth! seq n)
   *
   * (remove-nth! [1 2 3] 1)
   * => [1 3]
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | seq      | The seq to mutate                                                  |
   * | index    | The index of the element to remove                                 |
   *
   * @return The removed element.
   */
  FUNC(RemoveNthBangFunction, remove_nth_bang)

  /*!
   * @brief Repeat a value or series of values n times into an vector.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (repeat 3 "value")
   * => ["value" "value" "value"]
   * (repeat 3 :a :b)
   * => [:a :b :a :b :a :b]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | count     | Number of repetitions.                                             |
   * | values... | Values to repeat.                                                  |
   *
   * @return A new vector containing the repeated values.
   */
  FUNC(RepeatFunction, repeat)

  /*!
   * @brief Creates a new vector containing the elements of a sequence in reverse order.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (reverse [1 2 3])
   * => [3 2 1]
   * @endcode
   *
   * | Arg | Description                                                        |
   * | --- | ------------------------------------------------------------------ |
   * | seq | Seq to reverse.                                                    |
   *
   * @return A new vector containing the elements in reverse order.
   */
  FUNC(ReverseFunction, reverse)

  /*!
   * @brief Extract a half-open range from a sequence or string.
   * @since 0.1.0
   *
   * The start index is inclusive and the optional end index is exclusive. If
   * end is omitted, all elements from start onward are returned. Negative
   * indexes are offsets from the end of the sequence. Bounds are clamped.
   *
   * Usage:
   * @code
   * (slice [1 2 3 4 5] 1 4) => [2 3 4]
   * (slice [1 2 3 4 5] -1) => [5]
   * (slice "abcde" 1 4) => "bcd"
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | seq      | Sequential value or string to read from.                           |
   * | start    | Inclusive start index. Negative indexes count from the end.        |
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | seq   | Sequential value or string to read from.                           |
   * | start | Inclusive start index. Negative indexes count from the end.        |
   * | end   | Exclusive end index. Negative indexes count from the end.          |
   *
   * @return A string for string input; otherwise a new vector.
   */
  FUNC(SliceFunction, slice)

  /*!
   * @brief Return a sequence without its first element.
   * @since 0.1.0
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | seq      | Sequence to read from.                                             |
   *
   * @return A new vector containing every element after the first.
   */
  FUNC(TailFunction, tail)

  /*!
   * @brief Creates a new sequence from the n first elements of a sequence.
   * If n is larger than the size of the sequence, all elements are kept.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (take 2 [1 2 3 4 5])
   * => [1 2]
   *
   * (take 200 [1 2 3 4 5])
   * => [1 2 3 4 5]
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | count | Maximum number of elements to keep.                                |
   * | seq   | Seq to read from.                                                  |
   *
   * @return A new vector containing up to `count` elements from the start of
   * `seq`.
   */
  FUNC(TakeFunction, take)

  /*!
   * @brief Creates a vector from its arguments.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (vector 1 2 3) => [1 2 3]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | values... | Values to place in the new vector.                                 |
   *
   * @return A new vector containing `values`.
   */
  FUNC(VectorFunction, vector)

} // namespace Roo

#endif /* ROO__LANG__SEQ_H */
