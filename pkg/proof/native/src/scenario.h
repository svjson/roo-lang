#ifndef PROOF__SCENARIO_H
#define PROOF__SCENARIO_H

#include <roo/runtime/node.h>

namespace Roo::Proof
{
  sptr_ast_node_v rewrite_scenario_body(const sptr_ast_node_v& body);
} // namespace Roo::Proof

#endif /* PROOF__SCENARIO_H */
