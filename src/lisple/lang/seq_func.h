
#ifndef LISPLE__LANG__SEQ_FUNC_H
#define LISPLE__LANG__SEQ_FUNC_H

#include "../exec.h"

namespace Lisple
{
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
  FUNC(MapFunction, map)

} // namespace Lisple

#endif /* LISPLE__LANG__SEQ_FUNC_H */
