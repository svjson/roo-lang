
#ifndef LISPLE__EXEC_TREE_H
#define LISPLE__EXEC_TREE_H

#include <cstdint>
#include <variant>

#include <lisple/form.h>
#include <lisple/runtime/value.h>
#include <lisple/source.h>
#include <lisple/type.h>

namespace Lisple
{
  extern int exec_nodes_constructed;
  extern int call_nodes_constructed;
  extern int literal_nodes_constructed;
  extern int lookup_nodes_constructed;

  struct ExecNode;
  class Function;
  class SpecialForm;
  class LexicalBinding;
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
    sptr_val value;
    // FIXME: This should not be required here
    sptr_ast_node ast_node;

    explicit LiteralNode(const sptr_val& v);
    explicit LiteralNode(const sptr_val& v, sptr_ast_node ast_node);
  };

  struct LookupNode
  {
    AST::Symbol identifier;

    LookupNode(AST::Symbol id);
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

    sptr_executable static_callee = nullptr;
    std::string callee_name;

    CallNode(uptr_exec_node callee, std::vector<uptr_exec_node> args);
    CallNode(uptr_exec_node callee,
             std::vector<uptr_exec_node> args,
             std::string callee_name);

    bool is_literal_arg_list();

    std::string to_string(size_t indent_level = 0);
  };

  struct KeyLookupNode
  {
    sptr_val keyword;
    uptr_exec_node target;

    KeyLookupNode(const sptr_val& keyword, uptr_exec_node target);
  };

  struct LambdaNode
  {
    std::shared_ptr<Function> lambda_fn;

    LambdaNode(const std::shared_ptr<Function>& lambda_fn);
  };

  struct SpecialFormNode
  {
    const SpecialForm* form;
    sptr_val_v values;
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bind_forms;
    uptr_exec_node_v exec_nodes;

    SpecialFormNode(const SpecialFormNode& other);
    SpecialFormNode(SpecialFormNode&& other) noexcept = default;
    SpecialFormNode& operator=(SpecialFormNode&& other) noexcept = default;
    SpecialFormNode(const SpecialForm* form,
                    const sptr_val_v& values,
                    uptr_exec_node_v exec_nodes);
    SpecialFormNode(
      const SpecialForm* form,
      std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bind_forms,
      uptr_exec_node_v exec_nodes);
  };

  struct ExecNodeList
  {
    std::vector<ExecNode*> nodes;

    ExecNodeList(const std::vector<ExecNode*>& nodes);
  };

  using ExecNodeData = std::variant<LiteralNode,
                                    LookupNode,
                                    MapNode,
                                    CallNode,
                                    VectorNode,
                                    KeyLookupNode,
                                    LambdaNode,
                                    SpecialFormNode,
                                    ExecNodeList>;

  struct ExecNode
  {
    sptr_ast_node form;
    SourceRef source;
    ExecNodeData data;

    /**
     * @brief Construct an ExecNode of type T.
     *
     * Sets the optional source form value to NIL when the lowered node does not
     * need to retain its originating AST form.
     */
    template <typename T>
    explicit ExecNode(T node)
      : form(Lisple::AST::NIL)
      , source()
      , data(std::move(node))
    {
      exec_nodes_constructed++;
    }

    /**
     * @brief Construct a literal/value ExecNode containing `runtime_value`
     */
    explicit ExecNode(const sptr_val& runtime_value);

    /**
     * @brief Construct an ExecNode containing the source AST node.
     *
     * Some lowered nodes retain the original AST form for diagnostics,
     * literal-data handling, or host interop.
     */
    template <typename T>
    explicit ExecNode(const sptr_ast_node& form, T node)
      : form(form)
      , source(form ? form->get_source() : SourceRef{})
      , data(std::move(node))
    {
      exec_nodes_constructed++;
    }

    uptr_exec_node clone() const;
  };

} // namespace Lisple

#endif
