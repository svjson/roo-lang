#ifndef ROO__LANG__STRUCT_H
#define ROO__LANG__STRUCT_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Create a copy of a map or complex object with an additional or
   * replaced key/value-pair.
   * @since 0.1.0
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
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | target | Sequential value to copy.                                          |
   * | index  | Numeric index to associate.                                        |
   * | value  | Value to store at the index.                                       |
   *
   * @return A new map containing the original entries and the given associations.
   */
  FUNC(AssocFunction, assoc, assoc_seq)

  /*!
   * @brief Set or replace a key in a map or map-like structure, mutating it.
   * @since 0.1.0
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
   * | pairs... | Optional repetitions of key and value arguments.                 |
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | target | Sequential value to mutate.                                        |
   * | index  | Numeric index to associate.                                        |
   * | value  | Value to store at the index.                                       |
   *
   * @return The mutated target.
   */
  FUNC(AssocBangFunction, assoc_bang, assoc_seq_bang)

  /*!
   * @brief Create a copy of a map or map-like structure, setting or
   * replacing a nested key and its value, without mutating the original.
   * @since 0.1.0
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
   *
   * @return A new map containing the requested nested associations.
   */
  FUNC(AssocInFunction, assoc_in)

  /*!
   * @brief Set or replace a nested key in a map or map-like structure, mutating
   * it.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (assoc-in! my-nested-map [:key :nested-key] new-value)
   * (assoc-in! my-nested-map [:key1] value1 [:key2 :nested-key] value2)
   * @endcode
   *
   * | Arg      | Description                                                      |
   * | -------- | ---------------------------------------------------------------- |
   * | target   | The map or map-like value to mutate                              |
   * | path     | The path to the nested key to set                                |
   * | value    | The value to associate with the final key of the path            |
   * | pairs... | Optional repetitions of path and value                           |
   *
   * @return The mutated target.
   */
  FUNC(AssocInBangFunction, assoc_in_bang)

  /*!
   * @brief Create a modified copy of a map-like structure or sequence by
   * applying a function to the current value at one or more keys or indices.
   * @since 0.1.0
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
   * | updater  | The updater spec, callable or sequence of callable + args          |
   * | pairs... | Optional repetitions of key and updater                            |
   *
   * @return A modified copy of the target.
   */
  SPECIAL_FORM_DECL(UpdateForm, update)

  /*!
   * @brief Mutate a map-like structure or sequence by applying a function to
   * the current value at one or more keys or indices.
   * @since 0.1.0
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
   * | updater  | The updater spec, callable or sequence of callable + args          |
   * | pairs... | Optional repetitions of key and updater                            |
   *
   * @return The mutated target.
   */
  SPECIAL_FORM_DECL(UpdateBangForm, update_bang)

  /*!
   * @brief Create a modified copy of a nested map-like structure or sequence by
   * applying a function to the current value at one or more paths.
   * @since 0.1.0
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
   * | updater  | The updater spec, callable or sequence of callable + args          |
   * | pairs... | Optional repetitions of key and updater                            |
   *
   * @return A modified copy of the target.
   */
  SPECIAL_FORM_DECL(UpdateInForm, update_in)

  /*!
   * @brief Mutate a nested map-like structure or sequence by applying a function
   * to the current value at one or more paths.
   * @since 0.1.0
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
   * | updater  | The updater spec, callable or sequence of callable + args          |
   * | pairs... | Optional repetitions of key and updater                            |
   *
   * @return The mutated target.
   */
  SPECIAL_FORM_DECL(UpdateInBangForm, update_in_bang)

  /*!
   * @brief Get a property by value from a complex object, ie a map
   * or a host object.
   * @since 0.1.0
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
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | target  | The map or complex object.                                         |
   * | key     | The property value, usually a key.                                 |
   * | default | Default value to return when the property is missing.              |
   *
   * @return The property value, or the supplied default when the property is
   * missing.
   */
  FUNC(GetFunction, get);

  /*!
   * @brief Get a nested property by value from a complex object, ie a map
   * or a host object.
   * @since 0.1.0
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
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | target  | The map or complex object.                                         |
   * | path    | The property path as vector.                                       |
   * | default | Default value to return when the path is missing.                  |
   *
   * @return The nested property value, or the supplied default when the path is
   * missing.
   */
  FUNC(GetInFunction, get);

  /*!
   * @brief Create a copy of a map with one or more keys removed.
   * @since 0.1.0
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
   *
   * @return A new map without the requested keys.
   */
  FUNC(DissocFunction, dissoc);

  /*!
   * @brief Create a modified copy of a nested map-like structure, removing
   * one or more nested keys without mutating the original.
   * @since 0.1.0
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
   *
   * @return A modified copy of the target.
   */
  FUNC(DissocInFunction, dissoc_in)

  /*!
   * @brief Remove one or more keys from a map.
   * @since 0.1.0
   *
   * Does nothing if the key does not exist in the map.
   *
   * Usage:
   * @code
   * (dissoc! my-map :key)
   * (dissoc! my-map :key1 :key2)
   * @endcode
   *
   * | Arg     | Description                                                     |
   * | ------- | --------------------------------------------------------------- |
   * | map     | The map to mutate.                                              |
   * | keys... | Keys to remove.                                                 |
   *
   * @return The mutated map, or `nil` when the target is `nil`.
   */
  FUNC(DissocBangFunction, dissoc_bang)

  /*!
   * @brief Remove a nested key from a map, mutating it.
   * @since 0.1.0
   *
   * Does nothing if the path does not exist.
   *
   * Usage:
   * @code
   * (dissoc-in! my-map [:key :nested-key])
   * (dissoc-in! my-map [:key1] [:key2 :nested-key])
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | target   | The map or map-like value to mutate.                               |
   * | paths... | One or more paths to nested keys to remove.                        |
   *
   * @return The mutated target, or `nil` when the target is `nil`.
   */
  FUNC(DissocInBangFunction, dissoc_in_bang)

  /*!
   * @brief Returns a vector of all keys present in a map or complex object.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (keys {:name "A name" :age 23}) => [:name :age]
   * (keys nil) => []
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | target | The map or complex object whose keys are returned.                 |
   *
   * @return A vector containing the target's keys.
   */
  FUNC(KeysFunction, keys)

  /*!
   * @brief Merges keys and values from two or more maps into a new map.
   * Duplicate keys are overwritten by the last encountered value.
   * @since 0.1.0
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
   *
   * @return A new map containing the merged entries.
   */
  FUNC(MergeFunction, merge)

  /*!
   * @brief Merges keys and values from one or more maps into a map or map-like
   * target, mutating the target.
   * @since 0.1.0
   *
   * Source maps are merged without mutation before their combined entries are
   * applied to the target. Duplicate keys are overwritten by the last
   * encountered value. A nil target creates a new map, while nil sources are
   * ignored.
   *
   * Usage:
   * @code
   * (merge! {:a 1} {:b 2}) => {:a 1 :b 2}
   * (merge! {:a 1} {:b 2} {:a 4 :c 3}) => {:a 4 :b 2 :c 3}
   * (merge! nil {:a 1} nil) => {:a 1}
   * @endcode
   *
   * | Arg        | Description                                                      |
   * | ---------- | ---------------------------------------------------------------- |
   * | target     | The map or map-like value to mutate, or nil to create a map.     |
   * | sources... | Maps whose entries are merged and then applied to the target.    |
   *
   * @return The mutated target, or a new map when the target is `nil`.
   */
  FUNC(MergeBangFunction, merge_bang)

  /*!
   * @brief Perform a functional reduce on all key-value pairs of a Map.
   * @since 0.1.0
   *
   * The map and reducer function may be provided in either order; the initial
   * value follows the map in both forms.
   *
   * Usage:
   * @code
   * (reduce-kv {:a 1 :b 2}
   *            {:total 0}
   *            (fn [result k v] (assoc result
   *                                    :total
   *                                    (+ v (:total result)))))
   * => {:total 3}
   *
   * (reduce-kv (fn [result k v] (assoc result k v)) {:a 1} {})
   * => {:a 1}
   * @endcode
   *
   * | Arg  | Description                                                         |
   * | ---- | ------------------------------------------------------------------- |
   * | map  | The map to reduce                                                   |
   * | init | The initial value of result                                         |
   * | f    | A function applied with the accumulated result, key, and value.     |
   *
   * | Arg  | Description                                                         |
   * | ---- | ------------------------------------------------------------------- |
   * | f    | A function applied with the accumulated result, key, and value.     |
   * | map  | The map to reduce                                                   |
   * | init | The initial value of result                                         |
   *
   * @return The final accumulated value.
   */
  FUNC(ReduceKeyValueFunction, reduce_kv)

  /*!
   * @brief Returns a new map containing only the specified keys.
   * @since 0.1.0
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
   *
   * @return A new map containing entries for the requested keys found in the
   * source.
   */
  FUNC(SelectKeysFunction, select_keys)

  /*!
   * @brief Returns a vector of all values present under the keys of a map or complex
   * object.
   * @since 0.1.0
   *
   * Usage:
   * @code
   * (vals {:name "A name" :age 23}) => ["A name" 23]
   * (vals nil) => []
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | target | The map or complex object whose keys are returned.                 |
   *
   * @return A vector containing the target's values.
   */
  FUNC(ValsFunction, vals)

} // namespace Roo

#endif /* ROO__LANG__STRUCT_H */
