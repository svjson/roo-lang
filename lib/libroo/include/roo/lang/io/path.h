#ifndef ROO__LANG__IO__PATH_H
#define ROO__LANG__IO__PATH_H

#include <roo/exec.h>

namespace Roo
{
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

  /*!
   * @brief Join path segments and normalize the result lexically.
   *
   * This function takes an arbitrary number of path segments in addition to
   * a mandatory base path.
   *
   * Any `nil` value among the segments is ignored.
   *
   * Usage:
   * @code
   * (roo.io/join-path "assets" "images" "logo.svg")
   * => "assets/images/logo.svg"
   *
   * (apply roo.io/join-path ["assets" "logo.svg"])
   * => "assets/logo.svg"
   *
   * (roo.io/join-path nil "assets" nil "config.edn")
   * => "assets/config.edn"
   * @endcode
   *
   * | Arg      | Description                                                     |
   * | -------- | --------------------------------------------------------------- |
   * | base     | The base path.                                                  |
   * | segments | Zero or more path segments. Any `nil` values are ignored.       |
   */
  FUNC(JoinPathFunction, join_path)

  /*!
   * @brief Normalize a path lexically without accessing the filesystem.
   *
   * Relative paths remain relative and absolute paths remain absolute. The
   * returned path uses forward slashes on every platform. A `nil` path remains
   * `nil`.
   *
   * Usage:
   * @code
   * (roo.io/normalize-path "src/./app/../main") => "src/main"
   * (roo.io/normalize-path nil) => nil
   * @endcode
   *
   * | Arg  | Description                                                        |
   * | ---- | ------------------------------------------------------------------ |
   * | path | The path string to normalize, or `nil`.                            |
   */
  FUNC(NormalizePathFunction, normalize_path)

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
} // namespace Roo

#endif
