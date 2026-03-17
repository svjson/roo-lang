
#include "exec_tree.h"

namespace Lisple
{
  /**
   * Lower an SObject to an executable node.
   *
   * @param obj The SObject to lower.
   *
   * @return A shared pointer to the resulting ExecNode.
   */
  std::unique_ptr<ExecNode> lower(const sptr_sobject& obj);

  sptr_sobject exec(Context& ctx, const ExecNode& node);

  std::string to_string(const ExecNode& node, std::string indent = "");

} // namespace Lisple
