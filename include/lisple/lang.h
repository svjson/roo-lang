
#ifndef __LANG_H_
#define __LANG_H_

#include <lisple/exec.h>
#include <lisple/namespace.h>
#include <lisple/type.h>

namespace Lisple
{
  class Context;

  Namespace make_language_namespace();

  MACRO_DECL(NsMacro, switch_ns);

  /*!
   * @brief Allows retaining code in a source file without risk of evaluation in
   * runtime.
   */
  MACRO_DECL(CommentMacro, comment);

  MACRO_DECL(WhileMacro, make_while)

  /*!
   * @brief branching construct based on testing a single value against an
   * arbitrary number of constant values.
   *
   * The condition and expression pairs
   * will be evaluated sequentially until a match is found.
   * Optionally, :default can be provided as a last condition that will always
   * evaluate to true if no other match has been made.
   *
   * Usage:
   * @code
   * (case x
   *   10 "x is equal to 10"
   *   20 "x is equal to 20"
   *   :default "x is not 10 or 20")
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The value to match                                               |
   * | 1     | A value of expression to match against Arg #0                    |
   * | 2     | An expression that will be evaluated if Param 1 matches Arg #0   |
   * | 3...  | Repetitions of 1-2                                               |
   */
  MACRO_DECL(CaseMacro, make_case)

  /*!
   * @brief Executes a form for every element in a sequence, just like
   * (for ...), but takes a leading binding in the binding form containing
   * the zero-based iteration index
   *
   * Usage:
   * @code
   * (for-indexed [index num [1 2 3 4]]
   *   (* index num) => [0 2 6 12])
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | Binding form, [<index-var-name> <var-name> <seq>]                |
   * | 1     | Form body to execute                                             |
   */
  MACRO_DECL(ForIndexedMacro, make_for)

  FUNC_DECL(PrintFunction, do_print)

  FUNC_DECL(IncludeFunction, include_file)

  /*!
   * @brief Apply a function or executable form to a seq of arguments.
   *
   * Usage:
   * @code
   * (apply + [1 2 3])
   * => 6
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The function to invoke                                           |
   * | 1     | The seq of arguments                                             |
   */
  FUNC_DECL(ApplyFunction, apply_fn)

  /*!
   * @brief Rounds a number up to the nearest integer
   *
   * Usage:
   * @code
   * (ceil 10.2)
   * => 11
   *
   * (ceil 10.0)
   * => 10
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The number to round up                                           |
   */
  FUNC_DECL(CeilFunction, ceil)

  FUNC_DECL(IntFunction, to_int)

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
  FUNC_DECL(BetweenPredicateFunction, between);

  FUNC_DECL(ThresholdFunction, cap_value)

  class MinMaxFunction : public Function
  {
    const bool min;

   public:
    MinMaxFunction(bool min);

    sptr_sobject select_min_or_max(Context&, sptr_sobject_v& args);
  };

  FUNC_DECL(TailFunction, tail)

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
  FUNC_DECL(ContainsPredicateFunction, contains)

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
  FUNC_DECL(TakeFunction, take_fn)

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
  FUNC_DECL(RepeatFunction, repeat)

  /*!
   * @brief Merge keys and values from two or more maps into a new map containing
   * all keys and values. Any duplicates will be overwritten with the last
   * encountered value
   *
   * Usage:
   * @code
   * (merge {:a 1 :b 2}
   *        {:c 3 :d 4})
   * => {:a 1 :b 2 :c 3 :d 4}
   *
   * (merge {:a 1} {:b 2}
   *        {:a 4 :c 3})
   * => {:a 4 :b 2 :c 3}
   */
  FUNC_DECL(MergeFunction, merge_maps)

  FUNC_DECL(VectorFunction, make_vector)

  FUNC_DECL(JoinFunction, join_str)

  /*!
   * @brief Converts string or string representation to all upper-case.
   *
   * Usage:
   * @code
   * (upper-case "mIxEd-CaSe!") => "MIXED-CASE!"
   * (upper-case "The King is dead.") => "THE KING IS DEAD."
   * (upper-case :regular-key) => ":REGULAR-KEY"
   * (upper-case {:x 10 :y 8}) => "{:X 10 :Y 8}"
   * @endcode
   */
  FUNC_DECL(UpperCaseFunction, uppercase);
  /*!
   * @brief Converts string or string representation to all lower-case.
   *
   * Usage:
   * @code
   * (lower-case "mIxEd-CaSe!") => "mixed-case!"
   * (lower-case "The King is dead.") => "the king is dead."
   * (lower-case :UPCASE-KEY) => ":upcase-key"
   * (lower-case {:X 10 :Y 8}) => "{:x 10 :x 8}"
   * @endcode
   */
  FUNC_DECL(LowerCaseFunction, lowercase);

  /*!
   * @brief Extracts the name part of an qualified identifier as a string.
   *
   * Usage:
   * @code
   * (name :accept/ok) => "ok"
   * (name :ok) => "ok"
   * (name 'my-app/some-function) => "some-function"
   * (name 'some-function) => "some-function"
   * @endcode
   */
  FUNC_DECL(NameFunction, extract_name)

  /*!
   * @brief Extracts the namespace part of an qualified identifier as a string.
   *
   * Usage:
   * @code
   * (namespace :accept/ok) => "accept""
   * (namespace :ok) => nil
   * (namespace 'my-app/some-function) => "my-app"
   * (namespace 'some-function) => nil
   * @endcode
   */
  FUNC_DECL(NamespaceFunction, extract_namespace)

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
  FUNC_DECL(ConcatBangFunction, concat_array)

  /*!
   * @brief FlattenFunction - flatten an array or arrays or objects
   * into a single level array
   *
   * Usage: (flatten [[1 2 3] [4 5 6]]) => [1 2 3 4 5 6]
   */
  FUNC_DECL(FlattenFunction, flatten_array)

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
  FUNC_DECL(RemoveNthBangFunction, remove_nth)

  /*!
   * @brief Extracts an array of all keys present in a complex type, ie a map.
   *
   * Usage:
   * @code
   * (keys {:name "A name" :age 23}) => [:name :age]
   * @endcode
   */
  FUNC_DECL(KeysFunction, keys_fn)
  FUNC_DECL(SelectKeysFunction, select_keys_fn)

  /*! @brief Partition a sequence into an array of equally sized arrays.
   *
   * Usage:
   * @code
   * (partition 2 [1 2 3 4 5 6])
   * => [[1 2] [3 4] [5 6]]
   *
   * (partition 4 [1 2 3 4 5 6])
   * => [[1 2 3 4] [5 6]]
   */
  FUNC_DECL(PartitionFunction, partition)

  /*!
   * @brief Evaluate a Lisple form or a string containing lisple code
   *
   * Usage:
   * @code
   * (eval '(+ 1 1))
   * => 2
   *
   * (eval "(+ 1 1)")
   * => 2
   * @endcode
   */
  FUNC_DECL(EvalFunction, eval_string, eval_form);

  /*!
   * @brief Resolve the value of a symbol programmatically
   *
   * Usage:
   * @code
   * (resolve '+)
   * => <fn>
   * @endcode
   */
  FUNC_DECL(ResolveFunction, resolve);

} // namespace Lisple

#endif
