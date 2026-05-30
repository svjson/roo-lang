#ifndef ROO__LANG__IO__FS_H
#define ROO__LANG__IO__FS_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Return directory entries for a path, optionally filtered by options.
   *
   * Returns a vector of maps containing :name, :path, and :type.
   * Hidden dotfile entries are excluded by default.
   *
   * Usage:
   * @code
   * (roo.io/list-directory! "assets")
   * => [{:name "config.edn" :path "assets/config.edn" :type :file}]
   *
   * (roo.io/list-directory! "assets" {:directories? false :filter "*.edn"})
   * => [{:name "config.edn" :path "assets/config.edn" :type :file}]
   *
   * (roo.io/list-directory! "assets" {:hidden? true :filter ["*.edn" ".*"]})
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The directory path to list.                                      |
   * | 1     | Optional map with :files?, :directories?, :hidden?, :dotfiles?,  |
   * |       | and :filter options. :filter is a string or vector of strings.   |
   */
  FUNC(ListDirectoryBangFunction, list_directory, list_directory_with_options)

  /*!
   * @brief Query whether any filesystem entry exists at a path.
   *
   * Usage:
   * @code
   * (roo.io/exists? "assets/config.edn") => true
   * (roo.io/exists? "missing.edn") => false
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to query.                                               |
   */
  FUNC(ExistsPFunction, exists_p)

  /*!
   * @brief Query whether a path exists and is a regular file.
   *
   * Usage:
   * @code
   * (roo.io/file? "assets/config.edn") => true
   * (roo.io/file? "assets") => false
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to query.                                               |
   */
  FUNC(FilePFunction, file_p)

  /*!
   * @brief Query whether a path exists and is a directory.
   *
   * Usage:
   * @code
   * (roo.io/directory? "assets") => true
   * (roo.io/directory? "assets/config.edn") => false
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to query.                                               |
   */
  FUNC(DirectoryPFunction, directory_p)

  /*!
   * @brief Return metadata about a filesystem path.
   *
   * The returned map contains :exists?, :type, :size, and :modified-ms.
   * Missing paths return nil for fields that have no value.
   *
   * Usage:
   * @code
   * (roo.io/stat! "assets/config.edn")
   * => {:exists? true :type :file :size 123 :modified-ms 1780000000000}
   *
   * (roo.io/stat! "missing.edn")
   * => {:exists? false :type nil :size nil :modified-ms nil}
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to inspect.                                             |
   */
  FUNC(StatBangFunction, stat)

  /*!
   * @brief Resolve a path to an absolute path using the active filesystem.
   *
   * Usage:
   * @code
   * (roo.io/absolute-path! "assets/config.edn")
   * => "/project/assets/config.edn"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path to resolve.                                             |
   */
  FUNC(AbsolutePathBangFunction, absolute_path)

  /*!
   * @brief Return the process current directory according to the active filesystem.
   *
   * Usage:
   * @code
   * (roo.io/current-directory!) => "/project"
   * @endcode
   */
  FUNC(CurrentDirectoryBangFunction, current_directory)

  /*!
   * @brief Return the current user's home directory according to the active filesystem.
   *
   * Usage:
   * @code
   * (roo.io/home-directory!) => "/home/user"
   * @endcode
   */
  FUNC(HomeDirectoryBangFunction, home_directory)
} // namespace Roo

#endif
