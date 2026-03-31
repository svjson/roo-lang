
#include "lisple/runtime/node.h"

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

  bool CallNode::is_literal_arg_list()
  {
    for (auto& arg : args)
    {
      if (nullptr == std::get_if<LiteralNode>(&arg->data))
      {
        return false;
      }
    }

    return true;
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

  uptr_exec_node ExecNode::clone() const
  {
    return std::make_unique<ExecNode>(
      form,
      std::visit(
        [](const auto& n) -> ExecNodeData
        {
          using T = std::decay_t<decltype(n)>;

          if constexpr (std::is_same_v<T, LiteralNode>)
          {
            return LiteralNode(n.value);
          }
          else if constexpr (std::is_same_v<T, LookupNode>)
          {
            return LookupNode(n.identifier);
          }
          else if constexpr (std::is_same_v<T, CallNode>)
          {
            uptr_exec_node_v args;
            for (const auto& arg : n.args)
              args.push_back(arg->clone());

            return CallNode(n.callee->clone(), std::move(args));
          }
          else if constexpr (std::is_same_v<T, MapNode>)
          {
            uptr_exec_node_v elements;
            for (const auto& lmnt : n.elements)
              elements.push_back(lmnt->clone());

            return MapNode(std::move(elements));
          }
          else if constexpr (std::is_same_v<T, VectorNode>)
          {
            uptr_exec_node_v elements;
            for (const auto& lmnt : n.elements)
              elements.push_back(lmnt->clone());

            return VectorNode(std::move(elements));
          }
          else if constexpr (std::is_same_v<T, ExecNodeList>)
          {
            return ExecNodeList(n.nodes);
          }
          throw LispleException("Cannot clone node type");
        },
        data));
  }

} // namespace Lisple
