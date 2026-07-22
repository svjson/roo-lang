
#ifndef ROO__EXEC_NODE_H
#define ROO__EXEC_NODE_H

#include <roo/runtime/node.h>

namespace Roo
{
  extern ROO_API int eval_executions;
  extern ROO_API int exec_executions;

  sptr_val exec(Context& ctx, ExecNode& node);

  std::string to_string(const ExecNode& node, std::string indent = "");
  std::string to_string(const ptr_exec_node_v& nodes, std::string indent = "");

} // namespace Roo

#endif /* EXEC_NODE_H */
