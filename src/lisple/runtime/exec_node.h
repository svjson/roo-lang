
#include "exec_tree.h"

namespace Lisple
{
  /**
   * Lower an SObject to an executable node.
   *
   * @param obj The SObject to lower.
   *
   * @return A unique pointer to the resulting ExecNode.
   */
  std::unique_ptr<ExecNode> lower_expr(const sptr_sobject& obj);
  /**
   * Lower an SObject to a literal/data node.
   *
   * @param obj The SObject to lower.
   *
   * @return A unique pointer to the resulting ExecNode
   */
  std::unique_ptr<ExecNode> lower_literal(const sptr_sobject& obj);

  sptr_sobject exec(Context& ctx, const ExecNode& node);

  std::string to_string(const ExecNode& node, std::string indent = "");

} // namespace Lisple
