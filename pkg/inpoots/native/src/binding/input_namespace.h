#ifndef INPOOTS__BINDING__INPUT_NAMESPACE_H
#define INPOOTS__BINDING__INPUT_NAMESPACE_H

#include <string_view>

#include <roo/exec.h>
#include <roo/namespace.h>

namespace Roo::Inpoots
{
  inline constexpr std::string_view NS__INPOOTS__INPUT = "inpoots.input";

  namespace Function
  {
    /*!
     * @brief Run a callback with scoped access to standard input.
     *
     * `options` accepts `:mode :cooked`, `:mode :cbreak`, or `:mode :raw`;
     * raw is the default. Raw and cbreak modes require stdin to be a terminal.
     * The callback receives an input value accepted by `read!`. The
     * original terminal mode is restored before this function returns or
     * propagates an exception, and the input value becomes inactive when the
     * callback exits.
     *
     * @code
     * (with-stdin! {:mode :raw}
     *   (fn [input]
     *     (read! input)))
     * @endcode
     *
     * @return The callback's return value.
     * @since 0.1.0
     */
    FUNC(WithStdinFunction, with_stdin)

    /*!
     * @brief Read the next portable terminal event.
     *
     * Blocks until the scoped input receives a keystroke, pasted text, a
     * resize, or stream EOF. The returned map contains a `:type` keyword and
     * event-specific fields. Terminal control keystrokes may contain `:signal`
     * or `:control` metadata.
     *
     * @code
     * (read! input)
     * => {:type :keystroke :key :key/left}
     * @endcode
     *
     * @return A portable terminal event map.
     * @since 0.1.0
     */
    FUNC(ReadFunction, read)
  } // namespace Function

  class InputNamespace : public Roo::Namespace
  {
   public:
    InputNamespace();
  };
} // namespace Roo::Inpoots

#endif /* INPOOTS__BINDING__INPUT_NAMESPACE_H */
