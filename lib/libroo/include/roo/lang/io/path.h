#ifndef ROO__LANG__IO__PATH_H
#define ROO__LANG__IO__PATH_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Join two path segments and normalize the result lexically.
   *
   * Usage:
   * @code
   * (roo.io/join-path "assets" "config.edn")
   * => "assets/config.edn"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | base  | The base path.                                                     |
   * | child | The child path segment.                                            |
   */
  FUNC(JoinPathFunction, join_path)

  /*!
   * @brief Return the parent path of a path string.
   *
   * Usage:
   * @code
   * (roo.io/parent-path "assets/config.edn") => "assets"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | path  | The path to inspect.                                               |
   */
  FUNC(ParentPathFunction, parent_path)

  /*!
   * @brief Return the filename component of a path string.
   *
   * Usage:
   * @code
   * (roo.io/basename "assets/config.edn") => "config.edn"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | path  | The path to inspect.                                               |
   */
  FUNC(BasenameFunction, basename)

  /*!
   * @brief Return the directory component of a path string.
   *
   * Usage:
   * @code
   * (roo.io/dirname "assets/config.edn") => "assets"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | path  | The path to inspect.                                               |
   */
  FUNC(DirnameFunction, dirname)

  /*!
   * @brief Return the extension component of a path string.
   *
   * Usage:
   * @code
   * (roo.io/extension "assets/config.edn") => ".edn"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | path  | The path to inspect.                                               |
   */
  FUNC(ExtensionFunction, extension)
} // namespace Roo

#endif
