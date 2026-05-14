
#ifndef LISPLE__LOWER_H
#define LISPLE__LOWER_H

#include <memory>

#include <lisple/runtime/node.h>

namespace Lisple
{
  extern int lowered_expressions;
  extern int lowered_literals;
  extern int lower_time_exec_resolutions;
  extern int lower_time_exec_unresolved;

  extern int deprecated_special_form_invocations;

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

} // namespace Lisple

#endif /* LISPLE__LOWER_H */
