
#ifndef LISPLE__EXEC_NODE_H
#define LISPLE__EXEC_NODE_H

#include <lisple/runtime/node.h>

namespace Lisple
{
  extern int eval_executions;
  extern int exec_executions;

  sptr_val exec(Context& ctx, ExecNode& node);

  std::string to_string(const ExecNode& node, std::string indent = "");
  std::string to_string(const ptr_exec_node_v& nodes, std::string indent = "");

} // namespace Lisple

#endif /* EXEC_NODE_H */
