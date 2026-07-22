
#ifndef ROO__LOWER_H
#define ROO__LOWER_H

#include <memory>

#include <roo/export.h>
#include <roo/runtime/node.h>

namespace Roo
{
  ROO_API extern int lowered_expressions;
  ROO_API extern int lowered_literals;
  ROO_API extern int lower_time_exec_resolutions;
  ROO_API extern int lower_time_exec_unresolved;

  ROO_API extern int deprecated_special_form_invocations;

  struct LCtxFrame
  {
    bool allow_lookup = false;
    bool literal_mode = false;
  };

  struct LowerContext
  {
    Context* ctx = nullptr;
    std::vector<LCtxFrame> frames = {{false}};
    bool is_allow_lookup() const;
    bool is_literal_mode() const;

    void push(const LCtxFrame& frame);
    void push_literal_mode();
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
