
#ifndef __LANG_H_
#define __LANG_H_

#include <lisple/exec.h>
#include <lisple/namespace.h>
#include <lisple/type.h>

namespace Lisple
{
  class Context;

  Namespace make_language_namespace();

  FUNC_DECL(PrintFunction, do_print)

  FUNC_DECL(IncludeFunction, include_file)

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
