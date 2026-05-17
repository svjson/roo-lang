#ifndef LISPLE__LANG__IO__PATH_H
#define LISPLE__LANG__IO__PATH_H

#include <lisple/exec.h>

namespace Lisple
{
  /*!
   * @brief Join two path segments and normalize the result lexically.
   *
   * Usage:
   * @code
   * (lisple.io/join-path "assets" "config.edn")
   * => "assets/config.edn"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The base path.                                                   |
   * | 1     | The child path segment.                                          |
   */
  FUNC(JoinPathFunction, join_path)

  /*!
   * @brief Return the parent path of a path string.
   *
   * Usage:
   * @code
   * (lisple.io/parent-path "assets/config.edn") => "assets"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to inspect.                                             |
   */
  FUNC(ParentPathFunction, parent_path)

  /*!
   * @brief Return the filename component of a path string.
   *
   * Usage:
   * @code
   * (lisple.io/basename "assets/config.edn") => "config.edn"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to inspect.                                             |
   */
  FUNC(BasenameFunction, basename)

  /*!
   * @brief Return the directory component of a path string.
   *
   * Usage:
   * @code
   * (lisple.io/dirname "assets/config.edn") => "assets"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to inspect.                                             |
   */
  FUNC(DirnameFunction, dirname)

  /*!
   * @brief Return the extension component of a path string.
   *
   * Usage:
   * @code
   * (lisple.io/extension "assets/config.edn") => ".edn"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to inspect.                                             |
   */
  FUNC(ExtensionFunction, extension)
} // namespace Lisple

#endif
