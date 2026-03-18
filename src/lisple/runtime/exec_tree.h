
#ifndef LISPLE__EXEC_TREE_H
#define LISPLE__EXEC_TREE_H

#include "../form.h"
#include "../type.h"
#include <cstdint>
#include <variant>

namespace Lisple
{
  struct ExecNode;
  using uptr_exec_node = std::unique_ptr<ExecNode>;
  using ptr_exec_node_v = std::vector<ExecNode*>;
  using uptr_exec_node_v = std::vector<uptr_exec_node>;

  enum class ExecNodeType : uint8_t
  {
    LITERAL,
    LOOKUP,
    CALL
  };

  struct LiteralNode
  {
    sptr_sobject value;

    explicit LiteralNode(sptr_sobject v)
      : value(std::move(v))
    {
    }
  };

  struct LookupNode
  {
    Word identifier;

    LookupNode(Word id)
      : identifier(id)
    {
    }
  };

  struct MapNode
  {
    std::vector<uptr_exec_node> elements;

    MapNode(std::vector<uptr_exec_node> elements)
      : elements(std::move(elements))
    {
    }
  };

  struct VectorNode
  {
    std::vector<uptr_exec_node> elements;

    VectorNode(std::vector<uptr_exec_node> elements)
      : elements(std::move(elements))
    {
    }
  };

  struct CallNode
  {
    uptr_exec_node callee;
    std::vector<uptr_exec_node> args;

    CallNode(uptr_exec_node callee, std::vector<uptr_exec_node> args)
      : callee(std::move(callee))
      , args(std::move(args))
    {
    }

    std::string to_string(size_t indent_level = 0);
  };

  using ExecNodeData = std::variant<LiteralNode, LookupNode, MapNode, CallNode, VectorNode>;

  struct ExecNode
  {
    sptr_sobject form;
    ExecNodeData data;

    template <typename T>
    explicit ExecNode(const sptr_sobject& form, T node)
      : form(form)
      , data(std::move(node))
    {
    }
  };

} // namespace Lisple

#endif
