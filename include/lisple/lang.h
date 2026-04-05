
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

  FUNC_DECL(PrintFunction, do_print)

  FUNC_DECL(IncludeFunction, include_file)

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
