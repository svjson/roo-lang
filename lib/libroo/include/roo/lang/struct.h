#ifndef ROO__LANG__STRUCT_H
#define ROO__LANG__STRUCT_H

#include <roo/exec.h>

namespace Roo
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
   * (assoc my-map :key value)
   * (assoc my-map :key1 value1 :key2 value2)
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The object to create a copy of                                     |
   * | key      | The key whose value to set or substitute                           |
   * | value    | The value to associate with the key                                |
   * | pairs... | Optional repetitions of key and value arguments, passed as         |
   * |          | complete pairs                                                     |
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
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | target | The map/object to mutate                                           |
   * | key    | The key to set                                                     |
   * | value  | The value to associate with the key                                |
   */
  FUNC(AssocBangFunction, assoc_bang, assoc_seq_bang)

  /*!
   * @brief Create a copy of a map or map-like structure, setting or
   * replacing a nested key and its value, without mutating the original.
   *
   * Usage:
   * @code
   * (assoc-in my-nested-map [:key :nested-key] new-value)
   * (assoc-in my-nested-map [:key1] value1 [:key2 :nested-key] value2)
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The map or map-like from which to produce a modified copy          |
   * | path     | The path to the nested key to set                                  |
   * | value    | The value to associate with the last key of the key path           |
   * | pairs... | (Optional) repetitions of path and value                           |
   */
  FUNC(AssocInFunction, assoc_in)

  /*!
   * @brief Set or replace a nested key in a map or map-like structure, mutating
   * it.
   *
   * Usage:
   * @code
   * (assoc-in! my-nested-map [:key :nested-key] new-value)
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | target | The map to mutate                                                  |
   * | path   | The path to the nested key to set                                  |
   * | value  | The value to associate with the last key of the key path           |
   */
  FUNC(AssocInBangFunction, assoc_in_bang)

  /*!
   * @brief Create a modified copy of a map-like structure or sequence by
   * applying a function to the current value at one or more keys or indices.
   *
   * Usage:
   * @code
   * (update my-map :count (fn [x] (+ x 1)))
   * (update my-map :count [+ 10])
   * (update my-map :a (fn [x] (+ x 1)) :b [+ 10])
   * (update my-vec 1 [* 10])
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The map/object or sequence to create a modified copy of            |
   * | key      | The key or index to update                                         |
   * | updater  | The updater spec, executable or sequence of executable + args      |
   * | pairs... | Optional repetitions of key and updater                            |
   */
  FUNC(UpdateFunction, update)

  /*!
   * @brief Mutate a map-like structure or sequence by applying a function to
   * the current value at one or more keys or indices.
   *
   * Usage:
   * @code
   * (update! my-map :count (fn [x] (+ x 1)))
   * (update! my-map :count [+ 10])
   * (update! my-vec 1 [* 10])
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The map/object or sequence to mutate                               |
   * | key      | The key or index to update                                         |
   * | updater  | The updater spec, executable or sequence of executable + args      |
   * | pairs... | Optional repetitions of key and updater                            |
   */
  FUNC(UpdateBangFunction, update_bang)

  /*!
   * @brief Create a modified copy of a nested map-like structure or sequence by
   * applying a function to the current value at one or more paths.
   *
   * Usage:
   * @code
   * (update-in my-map [:count] (fn [x] (+ x 1)))
   * (update-in my-map [:count] [+ 10])
   * (update-in my-map [:a] (fn [x] (+ x 1)) [:b :c] [+ 10])
   * (update-in my-map [:nested :count] [* 10])
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The map/object or sequence to create a modified copy of            |
   * | path     | The path to the value to update                                    |
   * | updater  | The updater spec, executable or sequence of executable + args      |
   * | pairs... | Optional repetitions of key and updater                            |
   */
  FUNC(UpdateInFunction, update_in)

  /*!
   * @brief Mutate a nested map-like structure or sequence by applying a function
   * to the current value at one or more paths.
   *
   * Usage:
   * @code
   * (update-in! my-map [:count] (fn [x] (+ x 1)))
   * (update-in! my-map [:count] [+ 10])
   * (update-in! my-map [:nested :count] [* 10])
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The map/object or sequence to mutate                               |
   * | path     | The path to the value to update                                    |
   * | updater  | The updater spec, executable or sequence of executable + args      |
   * | pairs... | Optional repetitions of key and updater                            |
   */
  FUNC(UpdateInBangFunction, update_in_bang)

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
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | target  | The map or complex object                                          |
   * | key     | The property value, usually a key.                                 |
   * | default | Optional default value to return when the property is missing.     |
   */
  FUNC(GetFunction, get);

  /*!
   * @brief Get a nested property by value from a complex object, ie a map
   * or a host object.
   *
   * Usage:
   * @code
   * (get-in {:a 1 :b {:c 2}} [:b :c])
   * => 1
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | target  | The map or complex object                                          |
   * | path    | The property path as vector.                                       |
   * | default | Optional default value to return when the path is missing.         |
   */
  FUNC(GetInFunction, get);

  /*!
   * @brief Create a copy of a map with one or more keys removed.
   *
   * Does nothing for keys that do not exist in the map.
   *
   * Usage:
   * @code
   * (dissoc my-map :key)
   * (dissoc my-map :key1 :key2)
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | target  | The map or map-like value to copy.                                 |
   * | keys... | Keys to remove from the copy.                                      |
   */
  FUNC(DissocFunction, dissoc);

  /*!
   * @brief Create a modified copy of a nested map-like structure, removing
   * one or more nested keys without mutating the original.
   *
   * Usage:
   * @code
   * (dissoc-in my-nested-map [:key :nested-key])
   * (dissoc-in my-nested-map [:key1] [:key2 :nested-key])
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The map or map-like from which to produce a modified copy          |
   * | paths... | One or more paths to nested keys to remove                         |
   */
  FUNC(DissocInFunction, dissoc_in)

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
   *
   * | Arg  | Description                                                        |
   * | ---- | ------------------------------------------------------------------ |
   * | map  | The map to mutate.                                                 |
   * | key  | Key to remove.                                                     |
   */
  FUNC(DissocBangFunction, dissoc_bang)

  /*!
   * @brief Remove a nested key from a map, mutating it.
   *
   * Does nothing if the path does not exist.
   *
   * Returns the removed value.
   *
   * Usage:
   * @code
   * (dissoc-in! my-map [:key :nested-key])
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The map or map-like value to mutate.                               |
   * | paths... | One or more paths to nested keys to remove.                        |
   */
  FUNC(DissocInBangFunction, dissoc_in_bang)

  /*!
   * @brief Returns a vector of all keys present in a map or complex object.
   *
   * Usage:
   * @code
   * (keys {:name "A name" :age 23}) => [:name :age]
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | target | The map or complex object whose keys are returned.                 |
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
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | maps... | Maps whose entries are merged.                                     |
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
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | map   | The map to reduce                                                  |
   * | init  | The initial value of result                                        |
   * | f     | A function to apply for each key-value pair, taking the key and    |
   * |       | the value for each pair in the map accumulated result              |
   */
  FUNC(ReduceKeyValueFunction, reduce_kv)

  /*!
   * @brief Returns a new map containing only the specified keys.
   *
   * Usage:
   * @code
   * (select-keys {:a 1 :b 2 :c 3} [:a :c]) => {:a 1 :c 3}
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | map    | The source map.                                                    |
   * | keys   | Seq of keys to keep.                                               |
   */
  FUNC(SelectKeysFunction, select_keys)

} // namespace Roo

#endif /* ROO__LANG__STRUCT_H */
