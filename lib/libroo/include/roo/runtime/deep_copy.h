#ifndef ROO__RUNTIME__DEEP_COPY_H
#define ROO__RUNTIME__DEEP_COPY_H

#include <roo/export.h>
#include <roo/runtime/value.h>

namespace Roo
{
  /**
   * @brief Deep-copy a Roo value graph into ordinary Roo collection shapes.
   * @since 0.1.0
   *
   * Mutable collection structure is rebuilt, while atomic values are shared.
   * Native maps, host objects, and native sequences are demoted to ordinary
   * Roo maps and vectors. Repeated references remain repeated references in
   * the copy.
   *
   * @param root Root of the value graph to copy.
   * @return The copied root.
   * @throws RooException if the graph contains a cycle or an invalid value.
   */
  ROO_API sptr_val deep_copy(const sptr_val& root);

  /**
   * @brief Deep-copy several roots as one value graph.
   * @since 0.1.0
   *
   * Capturing the roots together preserves aliases between them.
   *
   * @param roots Roots of the value graph to copy.
   * @return The copied roots in their original order.
   * @throws RooException if the graph contains a cycle or an invalid value.
   */
  ROO_API sptr_val_v deep_copy(const sptr_val_v& roots);

  /**
   * @brief Deep-copy a value graph under the strict runtime-transfer policy.
   * @since 0.1.0
   *
   * In addition to normal deep-copy behavior, this rejects leaves whose
   * runtime identity or captured execution state cannot safely cross into an
   * independent Runtime.
   *
   * @param root Root of the value graph to copy.
   * @return The copied root.
   * @throws RooException if any value is not transferable or the graph
   * contains a cycle.
   */
  ROO_API sptr_val deep_copy_for_runtime_transfer(const sptr_val& root);

  /**
   * @brief Deep-copy several roots under the strict runtime-transfer policy.
   * @since 0.1.0
   *
   * Capturing the roots together preserves aliases between them.
   *
   * @param roots Roots of the value graph to copy.
   * @return The copied roots in their original order.
   * @throws RooException if any value is not transferable or the graph
   * contains a cycle.
   */
  ROO_API sptr_val_v deep_copy_for_runtime_transfer(const sptr_val_v& roots);
} // namespace Roo

#endif
