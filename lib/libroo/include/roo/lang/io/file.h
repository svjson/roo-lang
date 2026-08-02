#ifndef ROO__LANG__IO__FILE_H
#define ROO__LANG__IO__FILE_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Read the complete contents of a text file as a string.
   *
   * Usage:
   * @code
   * (roo.io/slurp! "notes.txt") => "file contents"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | path  | The path of the file to read.                                      |
   */
  FUNC(SlurpBangFunction, slurp)

  /*!
   * @brief Write a string to a text file.
   *
   * Usage:
   * @code
   * (roo.io/spit! "notes.txt" "file contents")
   * => nil
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | path     | The path of the file to write.                                     |
   * | contents | The text contents to write to the file.                            |
   */
  FUNC(SpitBangFunction, spit)

  /*!
   * @brief Copy one file, creating parent directories for the destination.
   *
   * Usage:
   * @code
   * (roo.io/copy-file! "src/logo.png" "dist/logo.png")
   * => nil
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | source | The source file path.                                              |
   * | dest   | The destination file path.                                         |
   */
  FUNC(CopyFileBangFunction, copy_file)

  /*!
   * @brief Remove a file or directory tree if it exists.
   *
   * Usage:
   * @code
   * (roo.io/remove-tree! "dist")
   * => nil
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | path  | The file or directory tree path to remove.                         |
   */
  FUNC(RemoveTreeBangFunction, remove_tree)

  /*!
   * @brief Create a symbolic link.
   *
   * Usage:
   * @code
   * (roo.io/symlink! "/path/to/package" "repo/pkg/name/0.1.0")
   * => nil
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | target | The source path the link points to.                                |
   * | link   | The symbolic link path to create.                                  |
   */
  FUNC(SymlinkBangFunction, symlink)

  /*!
   * @brief Test whether a path is a symbolic link.
   *
   * Usage:
   * @code
   * (roo.io/symlink? "repo/pkg/name/0.1.0") => true
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | path  | The path to inspect.                                               |
   */
  FUNC(SymlinkPFunction, symlink_p)

  /*!
   * @brief Read the target of a symbolic link.
   *
   * Usage:
   * @code
   * (roo.io/read-link! "repo/pkg/name/0.1.0") => "/path/to/package"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | link  | The symbolic link path to inspect.                                 |
   */
  FUNC(ReadLinkBangFunction, read_link)

  /*!
   * @brief Read one EDN form from a text file.
   *
   * Usage:
   * @code
   * (roo.io/slurp-edn! "config.edn")
   * => {:name "Roo" :ok true}
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | path  | The path of the EDN file to read.                                  |
   */
  FUNC(SlurpEdnBangFunction, slurp_edn)

  /*!
   * @brief Write the string representation of a Roo value to a file as EDN.
   *
   * Usage:
   * @code
   * (roo.io/spit-edn! "config.edn" {:name "Roo" :ok true})
   * => nil
   *
   * (roo.io/spit-edn! "config.edn" {:name "Roo" :ok true} {:pretty? true})
   * => nil
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | path    | The path of the EDN file to write.                                 |
   * | value   | The value to serialize with to_string().                           |
   * | options | Optional map with :pretty? boolean and :indent number.             |
   */
  FUNC(SpitEdnBangFunction, spit_edn)
} // namespace Roo

#endif
