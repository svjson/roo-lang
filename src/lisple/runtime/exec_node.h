
#ifndef LISPLE__EXEC_NODE_H
#define LISPLE__EXEC_NODE_H

#include "exec_tree.h"

namespace Lisple
{

  sptr_sobject exec(Context& ctx, const ExecNode& node);

  std::string to_string(const ExecNode& node, std::string indent = "");
  std::string to_string(const ptr_exec_node_v& nodes, std::string indent = "");

} // namespace Lisple

#endif /* EXEC_NODE_H */
