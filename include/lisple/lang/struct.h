#ifndef LISPLE__LANG__STRUCT_H
#define LISPLE__LANG__STRUCT_H

#include <lisple/exec.h>

namespace Lisple
{
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
  FUNC(AssocFunction, assoc, assoc_seq)

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
   * | 0     | The map/object to mutate                                         |
   * | 1     | The key to set                                                   |
   * | 2     | The value to associate with the key                              |
   */
  FUNC(AssocBangFunction, assoc_bang, assoc_seq_bang)

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
  FUNC(AssocInBangFunction, assoc_in_bang)

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
  FUNC(GetFunction, get);

  FUNC(DissocFunction, dissoc);

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
  FUNC(DissocBangFunction, dissoc_bang)

  /*!
   * @brief Returns a vector of all keys present in a map or complex object.
   *
   * Usage:
   * @code
   * (keys {:name "A name" :age 23}) => [:name :age]
   * @endcode
   */
  FUNC(KeysFunction, keys)

  /*!
   * @brief Merges keys and values from two or more maps into a new map.
   * Duplicate keys are overwritten by the last encountered value.
   *
   * Usage:
   * @code
   * (merge {:a 1 :b 2} {:c 3 :d 4}) => {:a 1 :b 2 :c 3 :d 4}
   * (merge {:a 1} {:b 2} {:a 4 :c 3}) => {:a 4 :b 2 :c 3}
   * @endcode
   */
  FUNC(MergeFunction, merge)

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
   * | 0     | The map to reduce                                                |
   * | 1     | The initial value of result                                      |
   * | 2     | A function to apply for each key-value pair, taking the key and  |
   * |       | the value for each pair in the map accumulated result            |
   */
  FUNC(ReduceKeyValueFunction, reduce_kv)

  /*!
   * @brief Returns a new map containing only the specified keys.
   *
   * Usage:
   * @code
   * (select-keys {:a 1 :b 2 :c 3} [:a :c]) => {:a 1 :c 3}
   * @endcode
   */
  FUNC(SelectKeysFunction, select_keys)

} // namespace Lisple

#endif /* LISPLE__LANG__STRUCT_H */
