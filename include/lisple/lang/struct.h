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
  FUNC(AssocFunction, assoc)

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
  FUNC_DECL(AssocInBangFunction, inv_assoc_in_bang)

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

} // namespace Lisple

#endif /* LISPLE__LANG__STRUCT_H */
