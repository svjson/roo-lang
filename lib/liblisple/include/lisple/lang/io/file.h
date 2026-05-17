#ifndef LISPLE__LANG__IO__FILE_H
#define LISPLE__LANG__IO__FILE_H

#include <lisple/exec.h>

namespace Lisple
{
  /*!
   * @brief Read the complete contents of a text file as a string.
   *
   * Usage:
   * @code
   * (lisple.io/slurp! "notes.txt") => "file contents"
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path of the file to read.                                    |
   */
  FUNC(SlurpBangFunction, slurp)

  /*!
   * @brief Write a string to a text file.
   *
   * Usage:
   * @code
   * (lisple.io/spit! "notes.txt" "file contents")
   * => nil
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path of the file to write.                                   |
   * | 1     | The text contents to write to the file.                          |
   */
  FUNC(SpitBangFunction, spit)

  /*!
   * @brief Read one EDN form from a text file.
   *
   * Usage:
   * @code
   * (lisple.io/slurp-edn! "config.edn")
   * => {:name "Lisple" :ok true}
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path of the EDN file to read.                                |
   */
  FUNC(SlurpEdnBangFunction, slurp_edn)

  /*!
   * @brief Write the string representation of a Lisple value to a file as EDN.
   *
   * Usage:
   * @code
   * (lisple.io/spit-edn! "config.edn" {:name "Lisple" :ok true})
   * => nil
   * @endcode
   *
   * | Arg # | Description                                                      |
   * |-------|------------------------------------------------------------------|
   * | 0     | The path of the EDN file to write.                               |
   * | 1     | The value to serialize with to_string().                         |
   */
  FUNC(SpitEdnBangFunction, spit_edn)
} // namespace Lisple

#endif
