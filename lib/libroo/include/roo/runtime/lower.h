
#ifndef ROO__LOWER_H
#define ROO__LOWER_H

#include <memory>
#include <string>
#include <vector>

#include <roo/export.h>
#include <roo/runtime/node.h>

namespace Roo
{
  class RestBinding;

  extern ROO_API int lowered_expressions;
  extern ROO_API int lowered_literals;
  extern ROO_API int lower_time_exec_resolutions;
  extern ROO_API int lower_time_exec_unresolved;

  extern ROO_API int deprecated_special_form_invocations;

  struct LCtxFrame
  {
    LCtxFrame() = default;
    LCtxFrame(bool allow_lookup, bool literal_mode = false);

    bool allow_lookup = false;
    bool literal_mode = false;
    std::vector<std::string> lexical_bindings;
  };

  struct LowerContext
  {
    Context* ctx = nullptr;
    std::vector<LCtxFrame> frames = {{false}};
    bool is_allow_lookup() const;
    bool is_literal_mode() const;
    bool is_lexically_bound(const std::string& identifier) const;

    void push(const LCtxFrame& frame);
    void push_literal_mode();
    void add_lexical_binding(const LexicalBinding& binding);
    void add_lexical_binding(const RestBinding& binding);
    void pop();
  };

  /**
   * Lower an ASTNode to an executable node.
   *
   * @param obj The ASTNode to lower.
   *
   * @return A unique pointer to the resulting ExecNode.
   */
  std::unique_ptr<ExecNode> lower_expr(LowerContext& ctx, const sptr_ast_node& obj);
  /**
   * Lower an ASTNode to a literal/data node.
   *
   * @param obj ASTNode to lower.
   *
   * @return A unique pointer to the resulting ExecNode
   */
  std::unique_ptr<ExecNode> lower_literal(const sptr_ast_node& obj);

} // namespace Roo

#endif /* ROO__LOWER_H */
