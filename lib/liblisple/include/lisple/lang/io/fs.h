#ifndef LISPLE__LANG__IO__FS_H
#define LISPLE__LANG__IO__FS_H

#include <lisple/exec.h>

namespace Lisple
{
  /*!
   * @brief Return directory entries for a path, optionally filtered by options.
   *
   * Returns a vector of maps containing :name, :path, and :type.
   * Hidden dotfile entries are excluded by default.
   *
   * Usage:
   * @code
   * (lisple.io/list-directory! "assets")
   * => [{:name "config.edn" :path "assets/config.edn" :type :file}]
   *
   * (lisple.io/list-directory! "assets" {:directories? false :filter "*.edn"})
   * => [{:name "config.edn" :path "assets/config.edn" :type :file}]
   *
   * (lisple.io/list-directory! "assets" {:hidden? true :filter ["*.edn" ".*"]})
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
   * (lisple.io/exists? "assets/config.edn") => true
   * (lisple.io/exists? "missing.edn") => false
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
   * (lisple.io/file? "assets/config.edn") => true
   * (lisple.io/file? "assets") => false
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
   * (lisple.io/directory? "assets") => true
   * (lisple.io/directory? "assets/config.edn") => false
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
   * (lisple.io/stat! "assets/config.edn")
   * => {:exists? true :type :file :size 123 :modified-ms 1780000000000}
   *
   * (lisple.io/stat! "missing.edn")
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
   * (lisple.io/absolute-path! "assets/config.edn")
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
   * (lisple.io/current-directory!) => "/project"
   * @endcode
   */
  FUNC(CurrentDirectoryBangFunction, current_directory)

  /*!
   * @brief Return the current user's home directory according to the active filesystem.
   *
   * Usage:
   * @code
   * (lisple.io/home-directory!) => "/home/user"
   * @endcode
   */
  FUNC(HomeDirectoryBangFunction, home_directory)
} // namespace Lisple

#endif
