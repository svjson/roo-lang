#ifndef LOOKUP_NATIVE_READER_H
#define LOOKUP_NATIVE_READER_H

#include <roo/exec.h>

namespace Roo::Lookup
{
  /*!
   * @brief Read a Roo source file into top-level form entries.
   *
   * Each entry contains the parsed Roo form and a source map with file, line,
   * column, start, and end positions.
   *
   * Usage:
   * @code
   * (lookup.reader/read-file-forms! "src/app/core.roo")
   * => [{:form (ns app.core) :source {...}}]
   * ; or if the file cannot be parsed:
   * => [{:error \"Could not read Roo forms from src/app/core.roo: ...\",
   *      :path \"src/app/core.roo\"}]
   * @endcode
   *
   * | Arg # | Description                 |
   * |-------|-----------------------------|
   * | 0     | Roo source file path to read. |
   */
  FUNC(ReadFileFormsFunction, read_file_forms)

  /*!
   * @brief Read all standard input as a string.
   *
   * Usage:
   * @code
   * (lookup.reader/read-stdin!)
   * => "source text"
   * @endcode
   */
  FUNC(ReadStdinFunction, read_stdin)
} // namespace Roo::Lookup

#endif
