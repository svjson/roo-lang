
#ifndef LISPLE__EXEC_TREE_H
#define LISPLE__EXEC_TREE_H

#include "../form.h"
#include "../type.h"
#include "value.h"
#include <cstdint>
#include <variant>

namespace Lisple
{
  extern int exec_nodes_constructed;
  extern int call_nodes_constructed;
  extern int literal_nodes_constructed;
  extern int lookup_nodes_constructed;

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
    sptr_rtval value;
    // FIXME: This should not be required here
    sptr_sobject ast_node;

    explicit LiteralNode(const sptr_rtval& v);
    explicit LiteralNode(const sptr_rtval& v, sptr_sobject ast_node);
  };

  struct LookupNode
  {
    Word identifier;

    LookupNode(Word id);
  };

  struct MapNode
  {
    std::vector<uptr_exec_node> elements;

    MapNode(std::vector<uptr_exec_node> elements);
  };

  struct VectorNode
  {
    std::vector<uptr_exec_node> elements;

    VectorNode(std::vector<uptr_exec_node> elements);
  };

  struct CallNode
  {
    uptr_exec_node callee;
    std::vector<uptr_exec_node> args;

    sptr_sobject cached_fn = nullptr;

    CallNode(uptr_exec_node callee, std::vector<uptr_exec_node> args);

    std::string to_string(size_t indent_level = 0);
  };

  struct ExecNodeList
  {
    std::vector<ExecNode*> nodes;

    ExecNodeList(const std::vector<ExecNode*>& nodes);
  };

  using ExecNodeData =
    std::variant<LiteralNode, LookupNode, MapNode, CallNode, VectorNode, ExecNodeList>;

  struct ExecNode
  {
    sptr_sobject form;
    ExecNodeData data;

    /**
     * @brief Construct an ExecNode of type T.
     *
     * Sets the legacy form value to NIL.
     */
    template <typename T>
    explicit ExecNode(T node)
      : form(Lisple::NIL)
      , data(std::move(node))
    {
      exec_nodes_constructed++;
    }

    /**
     * @brief Construct a literal/value ExecNode containing `runtime_value`
     */
    explicit ExecNode(const sptr_rtval& runtime_value);

    /**
     * @brief Construct an ExecNode containing the source AST node.
     *
     * The AST node (form) is required for compatibility with the legacy
     * AST-as-runtime implementation.
     */
    template <typename T>
    explicit ExecNode(const sptr_sobject& form, T node)
      : form(form)
      , data(std::move(node))
    {
      exec_nodes_constructed++;
    }
  };

} // namespace Lisple

#endif
