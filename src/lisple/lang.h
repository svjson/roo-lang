
#ifndef __LANG_H_
#define __LANG_H_

#include <cstdint>

#include "exec.h"
#include "form.h"
#include "namespace.h"
#include "type.h"

namespace Lisple
{
  class Context;

  Namespace make_language_namespace();

  MACRO_DECL(NsMacro, switch_ns);

  MACRO_DECL(DefMacro, define_obj)
  MACRO_DECL(DefunMacro, define_fun)
  MACRO_DECL(LambdaMacro, make_lambda)

  /*!
  * LetMacro - Temporarily bind values to variables that will exist only within
  * the scope of the body of the let macro
  *
  * Usage: (let [var-name (some-function)] body...)
  *        (let [name "Cheech" age 54] body...)
  *
  * Param 0 - Array containing a variable name and variable value pairs
  * Param 1... - Body forms to execute with the temporary bindings
  */
  MACRO_DECL(LetMacro, make_let)

  /*!
   * WhenLetMacro - Temporarily bind values to variables that will exist only
   * within the scope of the body of the when-let macro. The body will execute
   * only if all the bound variables have non-nil values.
   *
   * Usage: (when-let [var value] body...)
   *
   * Param 0 - Array containing variable name and variable value pairs
   * Param 1... - Body forms to execute with the temporary bindings
   */
  MACRO_DECL(WhenLetMacro, make_when_let)

  MACRO_DECL(DoMacro, make_do)
  MACRO_DECL(WhileMacro, make_while)
  MACRO_DECL(IfMacro, make_if)
  /*!
   * WhenMacro - optionally execute forms if conditional expression evaluates
   * to a truthy value.
   *
   * If multiple forms are provided after the conditional, they are evaluated as
   * if implicitly wrapped in a (do form1 form2 ...) form.
   *
   * Usage: (when (condition) (prn "It's true!"))
   *        (when (condition) (prn "It's true!") {:result "successful"})
   *        (when my-var (my-fun))
   *
   * Param 0: A conditional form - anything that can be considered truthy
   *          or falsy
   * Param 1: (vararg) Any number of forms to be evaluated if the conditional
   *          evaluates to a truthy value
   */
  MACRO_DECL(WhenMacro, make_when)

  /*! CaseMacro - branching construct based on testing a single value against
   * an arbitrary number of constant values. The condition and expression pairs
   * will be evaluated sequentially until a match is found.
   * Optionally, :default can be provided as a last condition that will always
   * evaluate to true if no other match has been made.
   *
   * Usage: (case x
   *          10 "x is equal to 10"
   *          20 "x is equal to 20"
   *          :default "x is not 10 or 20")
   *
   * Param 0: The value to match
   * Param 1: A value of expression to match against Param 0
   * Param 2: An expression that will be evaluated if Param 1 matches Param 0
   * Param 3...: Repetitions of 1-2
   */
  MACRO_DECL(CaseMacro, make_case)

  /*! CondMacro - defines a sequence of condition and expression pairs that will
   * be sequentially evaluated until a condition evaluates to a truthy, upon
   * which the corresponding expression will be evaluated and returned.
   *
   * return nil if no condition evaluates to a truthy value
   *
   * Usage: (cond
   *          (= x 10) "x is equal to 10"
   *          (> x 10) "x is greater than 10"
   *          :else "x is less than 10")
   *
   * In the example above, :else is a truthy value and can therefore be used
   * as a last catch-all condition.
   *
   * Param 0: A conditional form
   * Param 1: An expression that will, conditionally, be evaluated
   * Param 2...: Repetitions of Params 0-1
   */
  MACRO_DECL(CondMacro, make_cond)

  MACRO_DECL(ThreadFirstMacro, make_thread_first)

  /*!
   * ForMacro - executes a form for every element in a sequence,
   * binding the element to the local scope. Returns an array with
   * the return value for each iteration
   *
   * Usage: (for [num [1 2 3 4]] (* 2 num)) => [2 4 6 8]
   *
   * Param 0: Binding form, [<var-name> <seq>]
   * Param 1: Form body to execute
   */
  MACRO_DECL(ForMacro, make_for)

  /*! ForIndexedMacro - executes a form for ever element in a sequence, just
   * like (for ...), but takes a leading binding in the binding form containing
   * the zero-based iteration index
   *
   * Usage (for-indexed [index num [1 2 3 4]] (* index num) => [0 2 6 12])
   *
   * Param 0: Binding form, [<index-var-name> <var-name> <seq>]
   * Param 1: Form body to execute
   */
  MACRO_DECL(ForIndexedMacro, make_for)

  FUNC_DECL(PrintFunction, do_print)

  MACRO_DECL(SetBangMacro, do_set_member)

  /*!
   * NilPredicateFunction - test if the result of an expression is NIL
   *
   * Usage: (nil? (:key1 {:key2 "value"})) ==> true
   *
   * Param 0: The expression, value or identifier to test
   */
  FUNC_DECL(NilPredicateFunction, is_nil)

  /*!
  * NotFunction - invert boolean or truthy/falsy value
  *
  * Usage: (not true) => false
  *        (not false) => true
  *        (not {:key "value"}) => false
  *        (not nil) => true
  */
  FUNC_DECL(NotFunction, invert_boolean, not_any)

  FUNC_DECL(IncludeFunction, include_file)
  FUNC_DECL(ApplyFunction, apply_fn)

  FUNC_DECL(IntFunction, to_int)
  FUNC_DECL(PlusFunction, do_addition)
  FUNC_DECL(MinusFunction, do_subtraction)
  FUNC_DECL(DivideFunction, do_division)
  FUNC_DECL(MultiplyFunction, do_multiplication)

  /*!
   * LessThanFunction - Performs numerical comparison between two values.
   *
   * Usage: (< 5 10) => true
   *        (< 10 5) => false
   *        (< 10 10) => false
   */
  FUNC_DECL(LessThanFunction, lt_fn)

  /*!
   * LessThanOrEqualsFunction - Performs numerical comparison between two
   * values.
   *
   * Usage: (<= 5 10) => true
   *        (<= 10 5) => false
   *        (<= 10 10) => true
   */
  FUNC_DECL(LessThanOrEqualsFunction, lte_fn)

  /*!
   * GreaterThanFunction - Performs numerical comparison between two values.
   *
   * Usage: (> 5 10) => false
   *        (> 10 5) => true
   *        (> 10 10) => false
   */
  FUNC_DECL(GreaterThanFunction, gt_fn)

  /*!
   * GreaterThanOrEqualsFunction - Performs numerical comparison between two
   * values.
   *
   * Usage: (>= 5 10) => false
   *        (>= 10 5) => true
   *        (>= 10 10) => true
   */
  FUNC_DECL(GreaterThanOrEqualsFunction, gte_fn)

  FUNC_DECL(RangeFunction, make_range)
  FUNC_DECL(ThresholdFunction, cap_value)

  class MinMaxFunction : public Lisple::Function
  {
    const bool min;

   public:
    MinMaxFunction(bool min);

    Lisple::sptr_sobject select_min_or_max(Lisple::Context&, Lisple::sptr_sobject_v& args);
  };

  FUNC_DECL(AndFunction, logical_and)
  MACRO_DECL(OrMacro, logical_or)

  FUNC_DECL(HeadFunction, head)
  FUNC_DECL(TailFunction, tail)
  FUNC_DECL(LastFunction, last)
  FUNC_DECL(RandNthFunction, rand_nth)
  FUNC_DECL(CountFunction, count)

  /*!
   * @brief ContainsPredicateFunction - query if a Seq contains a specific value
   *
   * Usage: (contains? my-array "a value")
   *
   * Param 0 - The Seq to test
   * Param 1 - The value to test for
   */
  FUNC_DECL(ContainsPredicateFunction, contains)
  FUNC_DECL(TakeFunction, take_fn)

  FUNC_DECL(GetFunction, get);
  FUNC_DECL(NthFunction, get_nth);

  /*!
   * @brief AssocFunction - set or replace a key in a map, creating a copy of it.
   *
   * Usage: (assoc my-map key value)
   *
   * Param 0 - The map to create a copy of
   * Param 1 - The key to set
   * Param 2 - The value to associate with the key
   */
  FUNC_DECL(AssocFunction, assoc)

  /*!
   * @brief AssocBangFunction - set or replace a key in a map or map-like
   * structure, mutating it.
   *
   * Usage: (assoc! my-map key value)
   *
   * Param 0 - The map to mutate
   * Param 1 - The key to set
   * Param 2 - The value to associate with the key
   */
  FUNC_DECL(AssocBangFunction, assoc_bang)

  /*!
   * @brief AssocInBangFunction - set or replace a nested key in a map or
   * map-like structure, mutating it.
   *
   * Usage: (assoc-in! my-nested-map [:key :nested-key] new-value)
   *
   * Param 0 - The map to mutate
   * Param 1 - The path to the nested key to set
   * Param 2 - The value to associate with the last key of the key path
   */
  FUNC_DECL(AssocInBangFunction, assoc_in_bang)

  /*!
   * @brief MergeFunction - merge keys and values from two or more maps into
   * a new map containing all keys and values. Any duplicates will be
   * overwritten with the last encountered value
   *
   * Usage: (merge {:a 1 :b 2} {:c 3 :d 4}) => {:a 1 :b 2 :c 3 :d 4}
   *        (merge {:a 1} {:b 2} {:a 4 :c 3}) => {:a 4 :b 2 :c 3}
   */
  FUNC_DECL(MergeFunction, merge_maps)

  FUNC_DECL(VectorFunction, make_vector)
  FUNC_DECL(JoinFunction, join_str)
  FUNC_DECL(StrFunction, concat_str)

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
  FUNC_DECL(FlattenFunction, flatten_array)

  /*!
   * @brief MapFunction - transform elements of a Seq by applying a
   * function/executable to each element, creating a new Seq containing
   * the transformed elements. The original Seq is not mutated.
   *
   * Usage: (map my-seq exec)
   *        (map [1 2 3] (fn [n] (* 2 n))) ==> [2 4 6]
   *
   * Param 0 - The seq to create a transformed version of
   * Param 1 - The function/executable to apply to each element
   */
  FUNC_DECL(MapFunction, map_seq)

  /*!
   * @brief FilterFunction - Keep only certain elements of a Seq by applying a
   * function/executable to each element, creating a new Seq containing only
   * those elements for which the predicate function returns a truthy value.
   *
   * Usage: (filter my-seq exec)
   *        (filter [1 2 3 4] (fn [n] (even? n))) => [2 4]
   *
   * Param 0 - The seq to filter
   * Param 1 - The function/executable to apply to each element
   */
  FUNC_DECL(FilterFunction, filter_seq)

  /*!
   * @brief SomeFunction - Query if at least one element in a seq satisfies a
   * predicate function.
   *
   * Usage: (some? [1 2 3 4] odd?) => true
   *        (some? [2 4] odd?) => false
   *
   * Param 0 - The seq to query
   * Param 1 - The predicate function.
   */
  FUNC_DECL(SomeFunction, some)

  /*!
   * @brief RemoveFunction - Keep only certain elements of a Seq by applying a
   * function/executable to each element, creating a new Seq without those
   * elements for which the predicate function returns a truthy value.
   * Effectively the inverse of "filter"
   *
   * Usage: (remove exec my-seq)
   *        (remove (fn [n] (even? n)) [1 2 3 4]) => [1 3]
   *        (remove nil? [1 2 nil 5 6 nil 8 nil]) => [1 2 4 5 8]
   *
   * Param 0 - The function/executable to apply to each element
   * Param 1 - The seq to filter
   */
  FUNC_DECL(RemoveFunction, remove_seq)

  /*!
   * @brief ReduceFunction - Perform a functional reduce on a sequence
   *
   * Usage: (reduce sequence
   *                {}
   *                (fn [result element] (assoc result (:id element) element)))
   *
   * Param 0 - The sequence to reduce
   * Param 1 - The initial value of result
   * Param 2 - A function to apply for each element taking the accumulated
   *           result and current element as arguments
   */
  FUNC_DECL(ReduceFunction, reduce)

  /*!
   * @brief ReduceKeyValueFunctino - Perform a functional reduce on all
   * key-value pairs of a Map.
   *
   * Usage: (reduce {:a 1 :b 2}
   *                {:total 0}
   *                (fn [result k v] (assoc result
   *                                        :total
   *                                        (+ v (:total result)))))
   *        => {:total 3}
   *
   * Param 0 - The map to reduce
   * Param 1 - The initial value of result
   * Param 2 - A function to apply for each key-value pair, taking the
   *           accumulated result and the key and the value for each pair in
   *           the map
   */
  FUNC_DECL(ReduceKeyValueFunction, reduce_kv)

  /*!
   * @brief FindFirstFunction - Returns the first element of a seq that matches
   * a predicate function, or nil if no match is found
   *
   * Usage: (find-first [1 2 3 4 5] even?) => 2
   *        (find-first [1 3 4] even?) = nil
   *
   * Param 0 - The sequence to query
   * Param 1 - The predicate function
   */
  FUNC_DECL(FindFirstFunction, find_first_in_seq)

  /*!
   * @brief SeqMatchFunction - Query a seq against a pattern in form of a
   * partial map. Returns the first element that matches all defined keys
   * in the pattern
   *
   * Usage: (seq-match seq {:id 2 :status {:desc "disabled"}})
   *
   * Param 0 - The sequence to query
   * Param 1 - The partial map pattern to match
   */
  FUNC_DECL(SeqMatchFunction, match)
  FUNC_DECL(KeysFunction, keys_fn)
  FUNC_DECL(SelectKeysFunction, select_keys_fn)

  /*!
   * @brief EmptyPredicateFunction - Query a seq or string(sequence of
   * characters) for the presence of any elements.
   * The inverse of not-empty?.
   *
   * Usage: (empty? []) => true
   *        (empty? [1 2 3]) => false
   *        (empty? "") => true
   *        (empty? "content" => false)
   *
   * Param 0 - The seq or string to test
   */
  FUNC_DECL(EmptyPredicateFunction, exec_emptyp_seq, exec_emptyp_string)

  /*!
   * @brief NotEmptyPredicateFunction - Query a seq or string(sequence of
   * characters) for the presence of any elements.
   * The inverse of empty?
   *
   * Usage: (not-empty? []) => false
   *        (not-empty? [1 2 3]) => true
   *        (not-empty? "") => false
   *        (not-empty? "content" => true)
   *
   * Param 0 - The seq or string to test
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

  class EvalFunction : public Lisple::Function
  {
   public:
    EvalFunction();

    Lisple::sptr_sobject eval_string(Lisple::Context&, Lisple::sptr_sobject_v& args);
    Lisple::sptr_sobject eval_seq(Lisple::Context&, Lisple::sptr_sobject_v& args);
  };

  FUNC_DECL(RndFunction, random_number)
}

#endif
