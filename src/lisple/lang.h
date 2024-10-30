
#ifndef __LANG_H_
#define __LANG_H_

#include <cstdint>

#include "exec.h"
#include "namespace.h"
#include "type.h"

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

  MACRO_DECL(DefMacro, define_obj)
  MACRO_DECL(DefunMacro, define_fun, define_fun_docstring)
  MACRO_DECL(LambdaMacro, make_lambda)

  /*!
  * @brief Temporarily bind values to variables that will exist only within
  * the scope of the body of the let macro
  *
  * Usage:
  * @code
  * (let [var-name (some-function)] body...)
  * (let [name "Cheech" age 54] body...)
  ' @endcode
  *
  * | Arg # | Description                                                |
  * |-------|------------------------------------------------------------|
  * | 0     | Array containing a variable name and variable value pairs  |
  * | 1...  | Body forms to execute with the temporary bindings          |
  */
  MACRO_DECL(LetMacro, make_let)

  /*!
   * @brief Temporarily bind values to variables that will exist only
   * within the scope of the body of the when-let macro. The body will execute
   * only if all the bound variables have non-nil values.
   *
   * Usage:
   * @code
   * (when-let [var value] body...)
   * @endcode
   *
   * | Arg # | Description                                                |
   * |-------|------------------------------------------------------------|
   * | 0     | Array containing variable name and variable value pairs    |
   * | 1...  | Body forms to execute with the temporary bindings          |
   */
  MACRO_DECL(WhenLetMacro, make_when_let)

  MACRO_DECL(DoMacro, make_do)
  MACRO_DECL(WhileMacro, make_while)

  MACRO_DECL(IfLetMacro, make_if_let)

  MACRO_DECL(IfMacro, make_if)
  /*!
   * @brief Optionally execute forms if conditional expression evaluates
   * to a truthy value.
   *
   * If multiple forms are provided after the conditional, they are evaluated as
   * if implicitly wrapped in a (do form1 form2 ...) form.
   *
   * Usage:
   * @code
   * (when condition
   *   (prn "It's true!"))
   *
   * (when condition
   *   (prn "It's true!")
   *   {:result "successful"})
   *
   * (when my-var
   *   (my-fun))
   * @endcode
   *
   * | Arg # | Description                                                |
   * |-------|------------------------------------------------------------|
   * | 0     | A conditional form - anything that can be considered truthy or falsy |
   * | 1...  | Any number of forms to be evaluated if the conditional evaluates to a truthy value |
   */
  MACRO_DECL(WhenMacro, make_when)

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
   * @brief Defines a sequence of condition and expression pairs that will be
   * sequentially evaluated until a condition evaluates to a truthy, upon
   * which the corresponding expression will be evaluated and returned.
   *
   * @return nil if no condition evaluates to a truthy value
   *
   * Usage:
   * @code
   * (cond
   *   (= x 10) "x is equal to 10"
   *   (> x 10) "x is greater than 10"
   *   :else "x is less than 10")
   * @endcode
   *
   * In the example above, :else is a truthy value and can therefore be used
   * as a last catch-all condition.
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | A conditional form                                               |
   * | 1     | An expression that will, conditionally, be evaluated             |
   * | 2...  | Repetitions of Params 0-1                                        |
   */
  MACRO_DECL(CondMacro, make_cond)

  MACRO_DECL(ThreadFirstMacro, make_thread_first)

  /*!
   * @brief Executes a form for every element in a sequence, binding the element
   * to the local scope. Returns an array constructed from the return value of
   * each iteration
   *
   * Usage:
   * @code
   * (for [num [1 2 3 4]] (* 2 num)) => [2 4 6 8]
   * @endcode
   *
   * Param 0: Binding form, [<var-name> <seq>]
   * Param 1: Form body to execute
   */
  MACRO_DECL(ForMacro, make_for)

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

  MACRO_DECL(SetBangMacro, do_set_member)

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
  FUNC_DECL(NilPredicateFunction, is_nil)

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
  FUNC_DECL(NotFunction, invert_boolean, not_any)

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
   * @brief Returns the absolute value of a Number
   *
   * Usage:
   * @code
   * (abs -5)
   * => 5
   *
   * (abs 5)
   * => 5
   *
   * (abs -5.5)
   * => 5.5
   *
   * (abs 5.5)
   * => 5.5
   * @endcode
   */
  FUNC_DECL(AbsFunction, abs_value)

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
  FUNC_DECL(PlusFunction, do_addition)
  FUNC_DECL(MinusFunction, do_subtraction)
  FUNC_DECL(DivideFunction, do_division)
  FUNC_DECL(MultiplyFunction, do_multiplication)

  /*!
   * @brief Performs numerical comparison between two values.
   *
   * Usage:
   * @code
   * (< 5 10)
   * => true
   *
   * (< 10 5)
   * => false
   *
   * (< 10 10)
   * => false
   * @endcode
   */
  FUNC_DECL(LessThanFunction, lt_fn)

  /*!
   * @brief Performs numerical comparison between two
   * values.
   *
   * Usage:
   * (<= 5 10)
   * => true
   *
   * (<= 10 5)
   * => false
   *
   * (<= 10 10)
   * => true
   * @endcode
   */
  FUNC_DECL(LessThanOrEqualsFunction, lte_fn)

  /*!
   * @brief Performs numerical comparison between two values.
   *
   * Usage:
   * @code
   * (> 5 10)
   * => false
   *
   * (> 10 5)
   * => true
   *
   * (> 10 10)
   * => false
   * @endcode
   */
  FUNC_DECL(GreaterThanFunction, gt_fn)

  /*!
   * @brief Performs numerical comparison between two values.
   *
   * Usage:
   * @code
   * (>= 5 10)
   * => false
   *
   * (>= 10 5)
   * => true
   *
   * (>= 10 10)
   * => true
   * @endcode
   */
  FUNC_DECL(GreaterThanOrEqualsFunction, gte_fn)

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


  FUNC_DECL(RangeFunction, make_range)
  FUNC_DECL(ThresholdFunction, cap_value)

  class MinMaxFunction : public Lisple::Function
  {
    const bool min;

   public:
    MinMaxFunction(bool min);

    Lisple::sptr_sobject select_min_or_max(Lisple::Context&, Lisple::sptr_sobject_v& args);
  };

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
  MACRO_DECL(AndMacro, logical_and)

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
  MACRO_DECL(OrMacro, logical_or)

  FUNC_DECL(HeadFunction, head)
  FUNC_DECL(TailFunction, tail)
  FUNC_DECL(LastFunction, last)
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
  FUNC_DECL(RandNthFunction, rand_nth)
  FUNC_DECL(CountFunction, count)

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
   * @brief Get a property by value from a complex object, ie a map
   * or a host object.
   *
   * Usage:
   * @code
   * (get {:a 1 :b 2} :a)
   * => 1
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The map or complex object                                        |
   * | 1     | The property value, usually a key.                               |
   */
  FUNC_DECL(GetFunction, get);
  FUNC_DECL(NthFunction, get_nth);

  /*!
   * @brief Create a copy of a map or complex object with an additional or
   * replaced key/value-pair.
   *
   * This operation does not mutate the map/host object, but returns a
   * copy that has been modified.
   * Since there is currently no way to automatically create a copy of a host
   * objects, this will always return a map.
   *
   * Usage:
   * @code
   * (assoc my-map key value)
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The object to create a copy of                                   |
   * | 1     | The key to set                                                   |
   * | 2     | The value to associate with the key                              |
   */
  FUNC_DECL(AssocFunction, assoc_map, assoc_ho)

  /*!
   * @brief Set or replace a key in a map or map-like structure, mutating it.
   *
   * Usage:
   * @code
   * (assoc! my-map key value)
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The map to mutate                                                |
   * | 1     | The key to set                                                   |
   * | 2     | The value to associate with the key                              |
   */
  FUNC_DECL(AssocBangFunction, assoc_bang, assoc_seq_bang)

  /*!
   * @brief Set or replace a nested key in a map or map-like structure, mutating
   * it.
   *
   * Usage:
   * @code
   * (assoc-in! my-nested-map [:key :nested-key] new-value)
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The map to mutate                                                |
   * | 1     | The path to the nested key to set                                |
   * | 2     | The value to associate with the last key of the key path         |
   */
  FUNC_DECL(AssocInBangFunction, assoc_in_bang)

  /*!
   * @brief Remove a key from a map.
   *
   * Does nothing if the key does not exist in the map.
   *
   * Returns the removed value.
   *
   * Usage:
   * @code
   * (dissoc! my-map :key)
   * @endcode
   */
  FUNC_DECL(DissocBangFunction, dissoc_bang)

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
  FUNC_DECL(StrFunction, concat_str)

  /*!
   * @brief Appends elements to the back of a seq,
   * modifying the seq.
   *
   * Usage: (append! seq 3)
   *        (append! seq 3 4)
   */
  FUNC_DECL(AppendBangFunction, append_bang)

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
  FUNC_DECL(ConcatFunction, concat_array)

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
  FUNC_DECL(MapFunction, map_seq)

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
  FUNC_DECL(FilterFunction, filter_seq)

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
  FUNC_DECL(SortFunction, sort)

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
  FUNC_DECL(SomeFunction, some)

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
   * => [1 2 4 5 8]
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The function/executable to apply to each element                 |
   * | 1     | The seq to filter                                                |
   */
  FUNC_DECL(RemoveFunction, remove_seq)

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
  FUNC_DECL(RemoveBangFunction, remove_seq)

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
   * | 2     | A function to apply for each element taking the accumulated result and current element as arguments |
   */
  FUNC_DECL(ReduceFunction, reduce)

  /*!
   * @brief Perform a functional reduce on all key-value pairs of a Map.
   *
   * Usage:
   * @code
   * (reduce {:a 1 :b 2}
   *         {:total 0}
   *         (fn [result k v] (assoc result
   *                                 :total
   *                                 (+ v (:total result)))))
   * => {:total 3}
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * |0      | The map to reduce                                                |
   * |1      | The initial value of result                                      |
   * |2      | A function to apply for each key-value pair, taking the accumulated result and the key and the value for each pair in the map |
   */
  FUNC_DECL(ReduceKeyValueFunction, reduce_kv)

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
  FUNC_DECL(FindFirstFunction, find_first_in_seq)

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
  FUNC_DECL(SeqMatchFunction, match)

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
  FUNC_DECL(KeepFunction, keep)

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
  FUNC_DECL(EmptyPredicateFunction, exec_emptyp_seq, exec_emptyp_string)

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
  FUNC_DECL(NotEmptyPredicateFunction, exec_not_emptyp_seq, exec_not_emptyp_string)

  FUNC_DECL(EqualsPredicateFunction, equals_any)
  FUNC_DECL(NotEqualsFunction, not_equals_any)

  class OddEvenPredicateFunction : public Lisple::Function
  {
    uint8_t modulus;

   public:
    OddEvenPredicateFunction(uint8_t modulus);

    Lisple::sptr_sobject exec_oddevenp(Lisple::Context&, Lisple::sptr_sobject_v& args);
  };

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

  FUNC_DECL(RndFunction, random_number)
}

#endif
