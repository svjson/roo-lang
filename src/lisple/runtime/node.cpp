
#include "lisple/runtime/node.h"

#include <vector>

#include <lisple/bind.h>

namespace Lisple
{
  int exec_nodes_constructed = 0;
  int call_nodes_constructed = 0;
  int literal_nodes_constructed = 0;
  int lookup_nodes_constructed = 0;

  LiteralNode::LiteralNode(const sptr_val& v)
    : value(v)
    , ast_node(Lisple::AST::NIL)
  {
    literal_nodes_constructed++;
  }

  LiteralNode::LiteralNode(const sptr_val& v, sptr_ast_node ast_node)
    : value(v)
    , ast_node(ast_node)
  {
    literal_nodes_constructed++;
  }

  LookupNode::LookupNode(AST::Symbol id)
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

  KeyLookupNode::KeyLookupNode(const sptr_val& keyword, uptr_exec_node target)
    : keyword(keyword)
    , target(std::move(target))
  {
  }

  LambdaNode::LambdaNode(const std::shared_ptr<Function>& lambda_fn)
    : lambda_fn(lambda_fn)
  {
  }

  CallNode::CallNode(uptr_exec_node callee, std::vector<uptr_exec_node> args)
    : callee(std::move(callee))
    , args(std::move(args))
  {
    call_nodes_constructed++;
  }

  SpecialFormNode::SpecialFormNode(const SpecialForm* form,
                                   const sptr_val_v& values,
                                   uptr_exec_node_v exec_nodes)
    : form(form)
    , values(values)
    , exec_nodes(std::move(exec_nodes))
  {
  }

  SpecialFormNode::SpecialFormNode(
    const SpecialForm* form,
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bind_forms,
    uptr_exec_node_v exec_nodes)
    : form(form)
    , bind_forms(std::move(bind_forms))
    , exec_nodes(std::move(exec_nodes))
  {
  }

  SpecialFormNode::SpecialFormNode(const SpecialFormNode& other)
    : form(other.form)
  {
    for (auto& [b, v] : other.bind_forms)
    {
      bind_forms.push_back(std::make_pair(b->clone(), v->clone()));
    }

    for (auto& v : other.values)
    {
      values.push_back(v);
    }

    for (auto& node : other.exec_nodes)
    {
      exec_nodes.push_back(node->clone());
    }
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

  ExecNode::ExecNode(const sptr_val& runtime_value)
    : form(Lisple::AST::NIL)
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
          else if constexpr (std::is_same_v<T, LambdaNode>)
          {
            return LambdaNode(n.lambda_fn);
          }
          else if constexpr (std::is_same_v<T, SpecialFormNode>)
          {
            return SpecialFormNode(n);
          }
          else if constexpr (std::is_same_v<T, KeyLookupNode>)
          {
            return KeyLookupNode(n.keyword, n.target->clone());
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
