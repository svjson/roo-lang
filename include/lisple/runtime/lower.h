
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

  struct LowerContext
  {
    Context* ctx = nullptr;
    bool allow_lookup = false;
  };

  /**
   * Lower an SObject to an executable node.
   *
   * @param obj The SObject to lower.
   *
   * @return A unique pointer to the resulting ExecNode.
   */
  std::unique_ptr<ExecNode> lower_expr(LowerContext& ctx, const sptr_sobject& obj);
  /**
   * Lower an SObject to a literal/data node.
   *
   * @param obj The SObject to lower.
   *
   * @return A unique pointer to the resulting ExecNode
   */
  std::unique_ptr<ExecNode> lower_literal(const sptr_sobject& obj);

} // namespace Lisple

#endif /* LISPLE__LOWER_H */
