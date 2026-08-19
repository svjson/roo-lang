#ifndef ROOPL__BINDING__TARGET_RUNTIME_NAMESPACE_H
#define ROOPL__BINDING__TARGET_RUNTIME_NAMESPACE_H

#include <string_view>

#include <roo/exec.h>
#include <roo/namespace.h>

namespace Roo::Roopl
{
  inline constexpr std::string_view NS__ROOPL__TARGET__RUNTIME = "roopl.target.runtime";

  namespace Function
  {
    /*!
     * @brief Open a separately configured Roo runtime target.
     *
     * `options` accepts optional `:load-paths` and `:package-root` fields. When
     * no package root is supplied, the nearest package containing the current
     * directory is used when one exists.
     *
     * @return An opaque target accepted by `eval!` and `close!`.
     */
    FUNC(OpenTargetFunction, open_target)

    /*!
     * @brief Evaluate a source request in a dedicated Roo runtime.
     *
     * `target` is returned by `open!`. `request` contains string `:source`,
     * `:namespace`, and `:source-name` fields.
     *
     * @return A portable success or error response map.
     */
    FUNC(EvalTargetFunction, eval_target)

    /*!
     * @brief Release a dedicated target's runtime resources.
     *
     * `target` is returned by `open!`. Closing an already closed target is a
     * no-op.
     *
     * @return `nil`.
     */
    FUNC(CloseTargetFunction, close_target)
  } // namespace Function

  class TargetRuntimeNamespace : public Roo::Namespace
  {
   public:
    TargetRuntimeNamespace();
  };
} // namespace Roo::Roopl

#endif /* ROOPL__BINDING__TARGET_RUNTIME_NAMESPACE_H */
