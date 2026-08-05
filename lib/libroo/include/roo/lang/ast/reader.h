#ifndef ROO__LANG__AST__READER_H
#define ROO__LANG__AST__READER_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Read a Roo source file into top-level AST node entries.
   *
   * Usage:
   * @code
   * (roo.ast/slurp! "src/app/core.roo")
   * => [{:type :list :form (ns app.core) :source {...} :children [...]}]
   * @endcode
   *
   * Each entry contains the parsed Roo form as Roo data, its AST node type,
   * source locations with file path, line, column, start, and end positions, and
   * nested child nodes for sequence forms.
   *
   * | Arg       | Description                                                  |
   * |-----------|--------------------------------------------------------------|
   * | file-path | The path of the source file to read.                         |
   */
  FUNC(AstSlurpBangFunction, slurp)

  /*!
   * @brief Read a Roo source string into top-level AST node entries.
   *
   * Usage:
   * @code
   * (roo.ast/read-string "(def answer 42)")
   * (roo.ast/read-string "(def answer 42)" {:path "inline.roo"})
   * @endcode
   *
   * | Arg #      | Description                                                      |
   * |------------|------------------------------------------------------------------|
   * | source-str | The source text to read.                                         |
   * | opts        | Optional map with :path used in source locations.               |
   */
  FUNC(AstReadStringFunction, read_string)
} // namespace Roo

#endif
