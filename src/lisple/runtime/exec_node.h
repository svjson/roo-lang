
#ifndef LISPLE__EXEC_NODE_H
#define LISPLE__EXEC_NODE_H

#include "exec_tree.h"

namespace Lisple
{
  extern int eval_executions;
  extern int exec_executions;

  sptr_rtval exec(Context& ctx, ExecNode& node);

  /**
   * @brief A variant of exec for legacy execution paths that expect
   * the AST-oriented sptr_sobject as return value.
   */
  sptr_sobject eval(Context& ctx, ExecNode& node);

  std::string to_string(const ExecNode& node, std::string indent = "");
  std::string to_string(const ptr_exec_node_v& nodes, std::string indent = "");

} // namespace Lisple

#endif /* EXEC_NODE_H */
