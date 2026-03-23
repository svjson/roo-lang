
#include "exec_tree.h"

#include <vector>

namespace Lisple
{
  int exec_nodes_constructed = 0;
  int call_nodes_constructed = 0;
  int literal_nodes_constructed = 0;
  int lookup_nodes_constructed = 0;

  LiteralNode::LiteralNode(const sptr_rtval& v)
    : value(v)
    , ast_node(Lisple::NIL)
  {
    literal_nodes_constructed++;
  }

  LiteralNode::LiteralNode(const sptr_rtval& v, sptr_sobject ast_node)
    : value(v)
    , ast_node(ast_node)
  {
    literal_nodes_constructed++;
  }

  LookupNode::LookupNode(Word id)
    : identifier(id)
  {
    lookup_nodes_constructed++;
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
    call_nodes_constructed++;
  }

  ExecNodeList::ExecNodeList(const std::vector<ExecNode*>& nodes)
    : nodes(nodes)
  {
  }

  ExecNode::ExecNode(const sptr_rtval& runtime_value)
    : form(Lisple::NIL)
    , data(LiteralNode(runtime_value))
  {
    exec_nodes_constructed++;
  }

} // namespace Lisple
