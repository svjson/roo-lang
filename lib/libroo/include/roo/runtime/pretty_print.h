#ifndef ROO__RUNTIME_PRETTY_PRINT_H
#define ROO__RUNTIME_PRETTY_PRINT_H

#include <cstddef>
#include <string>
#include <vector>

#include <roo/runtime/value.h>

namespace Roo::Pretty
{
  /*!
   * @brief A Wadler/Hughes-style pretty-printing document.
   *
   * A PrintNode describes layout intent, not layout - it is built once from
   * whatever structure needs printing and handed to render() separately,
   * so the code that builds it never has to make width/indentation
   * decisions itself. It has no knowledge of Value, AST::ASTNode, or
   * anything else it might have been built from.
   *
   * `LINE()` becomes a single space when its enclosing group renders flat,
   * or a newline plus the current indentation when it breaks. `SOFT_LINE()`
   * is the same except it becomes nothing (not a space) when flat.
   * `HARD_LINE()` always breaks, and forces every enclosing group to break
   * as well, since a group containing one can never be rendered flat.
   */
  struct PrintNode
  {
    enum class Kind
    {
      TEXT,
      CONCAT,
      LINE,
      SOFT_LINE,
      HARD_LINE,
      NEST,
      GROUP
    };

    Kind kind;
    std::string text_value;
    std::vector<PrintNode> parts;
    int nest_width = 0;

    static PrintNode text(std::string value);
    static PrintNode concat(std::vector<PrintNode> parts);
    static PrintNode nest(int width, PrintNode child);
    static PrintNode group(PrintNode child);
    static PrintNode line();
    static PrintNode soft_line();
    static PrintNode hard_line();

    /*!
     * @brief Concatenate `nodes`, inserting a copy of `separator` between
     * each consecutive pair.
     */
    static PrintNode join(std::vector<PrintNode> nodes, const PrintNode& separator);
  };

  struct RenderOptions
  {
    /*!
     * @brief Column budget a group's flattened content must fit within
     * (relative to where the group starts) to render on one line. 0 means
     * no group ever fits, i.e. every group breaks.
     */
    std::size_t width = 0;
  };

  /*!
   * @brief Render `node` to text.
   *
   * Each Group independently renders flat if its content fits the width
   * remaining on the current line, and broken (one item per line, indented
   * per enclosing Nest) otherwise - the standard Wadler/Hughes rule. The
   * fits-check (Lindig's formulation) only measures the group's own
   * flattened content against the remaining budget and aborts as soon as it
   * is exceeded, so it costs O(width) rather than O(subtree size); a group
   * nested inside a broken parent is still free to render flat on its own
   * line if it fits there.
   *
   * There is deliberately no indent-width knob here: how far each Nest
   * indents is a property of the PrintNode tree itself (the width passed to
   * PrintNode::nest at construction time), not a global render-time
   * setting - different subtrees, or different builders, are free to nest
   * by different amounts within the same render() call.
   */
  std::string render(const PrintNode& node, const RenderOptions& options);

  /*!
   * @brief Options controlling print()'s layout.
   *
   * `width` of 0 means no collection ever fits on one line, i.e. every
   * MAP/LIST/VECTOR (or host map/sequence - see Walk::shape_of) breaks its
   * contents onto separate lines - the long-standing default. `indent_width`
   * of 0 is a legitimate, if visually flat, choice - nesting is still
   * conveyed by brackets alone. `max_depth`/`max_elements` of -1 mean
   * unlimited; a non-negative value truncates collections nested deeper
   * than `max_depth`, or containing more than `max_elements` items, to
   * `...` - and, unlike a naive "render then truncate the string", the
   * elements/subtrees past the limit are never walked at all, so this
   * bounds the cost of printing a value regardless of its actual size.
   */
  struct PrintOptions
  {
    int indent_width = 2;
    std::size_t width = 0;
    int max_depth = -1;
    int max_elements = -1;
  };

  /*!
   * @brief Render `value` to text per `options`.
   */
  std::string print(const Value& value, const PrintOptions& options = {});
} // namespace Roo::Pretty

#endif
