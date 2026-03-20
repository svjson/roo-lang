
#include "exec_tree.h"

#include <vector>

namespace Lisple
{
  LiteralNode::LiteralNode(const sptr_rtval& v, sptr_sobject ast_node)
    : value(v)
    , ast_node(ast_node)
  {
  }

  LookupNode::LookupNode(Word id)
    : identifier(id)
  {
  }

  MapNode::MapNode(std::vector<uptr_exec_node> elements)
    : elements(std::move(elements))
  {
  }

  VectorNode::VectorNode(std::vector<uptr_exec_node> elements)
    : elements(std::move(elements))
  {
  }

  CallNode::CallNode(uptr_exec_node callee, std::vector<uptr_exec_node> args)
    : callee(std::move(callee))
    , args(std::move(args))
  {
  }

  ExecNodeList::ExecNodeList(const std::vector<ExecNode*>& nodes)
    : nodes(nodes)
  {
  }
} // namespace Lisple
