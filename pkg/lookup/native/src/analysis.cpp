#include "analysis.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <roo/context.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/reader.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

#include "source_values.h"
#include <lookup/native.h>

namespace Roo::Lookup
{
  namespace
  {
    bool position_less_than(const SourcePosition& a, const SourcePosition& b)
    {
      return a.line < b.line || (a.line == b.line && a.column < b.column);
    }

    bool position_less_than_or_equal(const SourcePosition& a, const SourcePosition& b)
    {
      return !position_less_than(b, a);
    }

    bool span_contains(const SourceSpan& span, const SourcePosition& position)
    {
      return span.valid() && position.valid() &&
             position_less_than_or_equal(span.start, position) &&
             position_less_than_or_equal(position, span.end);
    }

    bool node_contains(const sptr_ast_node& node, const SourcePosition& position)
    {
      return node && span_contains(node->get_source().span, position);
    }

    bool sequence_node(const AST::ASTNode& node)
    {
      return node.get_type() == Form::LIST || node.get_type() == Form::VECTOR ||
             node.get_type() == Form::MAP;
    }

    const AST::ASTNode* deepest_node_at(const sptr_ast_node& node,
                                        const SourcePosition& position)
    {
      if (!node_contains(node, position))
      {
        return nullptr;
      }

      if (sequence_node(*node))
      {
        for (const auto& child : node->get_children())
        {
          if (const auto* match = deepest_node_at(child, position))
          {
            return match;
          }
        }
      }

      return node.get();
    }

    bool contains_node(const sptr_ast_node& root, const AST::ASTNode* target)
    {
      if (!root || !target)
      {
        return false;
      }
      if (root.get() == target)
      {
        return true;
      }
      if (sequence_node(*root))
      {
        for (const auto& child : root->get_children())
        {
          if (contains_node(child, target))
          {
            return true;
          }
        }
      }
      return false;
    }

    std::optional<std::string> symbol_text(const AST::ASTNode& node)
    {
      if (node.get_type() == Form::SYMBOL)
      {
        return node.as<AST::Symbol>().value;
      }
      return std::nullopt;
    }

    std::string node_text(const AST::ASTNode& node)
    {
      if (auto text = symbol_text(node))
      {
        return *text;
      }
      return node.to_string();
    }

    sptr_val thing_kind(const AST::ASTNode& node)
    {
      switch (node.get_type())
      {
      case Form::SYMBOL:
        return Value::keyword("symbol");
      case Form::KEYWORD:
        return Value::keyword("keyword");
      case Form::STRING:
        return Value::keyword("string");
      case Form::NUMBER:
        return Value::keyword("number");
      case Form::LIST:
        return Value::keyword("list");
      case Form::VECTOR:
        return Value::keyword("vector");
      case Form::MAP:
        return Value::keyword("map");
      default:
        return Value::keyword("form");
      }
    }

    std::optional<std::string> list_head_name(const sptr_ast_node& node)
    {
      if (!node || node->get_type() != Form::LIST || node->get_children().empty())
      {
        return std::nullopt;
      }
      return symbol_text(*node->get_children().front());
    }

    struct Binding
    {
      std::string name;
      SourceRef source;
      std::string signature;
    };

    using Scope = std::vector<Binding>;
    using ScopeStack = std::vector<Scope>;

    std::optional<Binding> binding_for_target(const AST::ASTNode* target,
                                              const std::string& signature)
    {
      if (!target)
      {
        return std::nullopt;
      }
      if (auto name = symbol_text(*target))
      {
        return Binding{*name, target->get_source(), signature};
      }
      return std::nullopt;
    }

    void collect_binding_symbols(const sptr_ast_node& node,
                                 Scope& scope,
                                 const std::string& signature)
    {
      if (!node)
      {
        return;
      }
      if (auto name = symbol_text(*node))
      {
        scope.push_back(Binding{*name, node->get_source(), signature});
        return;
      }
      if (sequence_node(*node))
      {
        for (const auto& child : node->get_children())
        {
          collect_binding_symbols(child, scope, signature);
        }
      }
    }

    std::optional<Binding> resolve_from_scopes(const ScopeStack& scopes,
                                               const std::string& name)
    {
      for (auto scope = scopes.rbegin(); scope != scopes.rend(); ++scope)
      {
        for (auto binding = scope->rbegin(); binding != scope->rend(); ++binding)
        {
          if (binding->name == name)
          {
            return *binding;
          }
        }
      }
      return std::nullopt;
    }

    std::optional<Binding> resolve_local_in_node(const sptr_ast_node& node,
                                                 const AST::ASTNode* target,
                                                 const ScopeStack& scopes);

    std::optional<Binding> resolve_local_in_let(const sptr_ast_node& node,
                                                const AST::ASTNode* target,
                                                const ScopeStack& scopes)
    {
      auto& children = node->get_children();
      if (children.size() < 2 || children[1]->get_type() != Form::VECTOR)
      {
        return std::nullopt;
      }

      Scope let_scope;
      auto& binding_children = children[1]->get_children();
      for (std::size_t i = 0; i < binding_children.size(); i += 2)
      {
        const auto& pattern = binding_children[i];
        if (contains_node(pattern, target))
        {
          return binding_for_target(target, "local binding");
        }

        if (i + 1 < binding_children.size() &&
            contains_node(binding_children[i + 1], target))
        {
          return resolve_local_in_node(binding_children[i + 1], target, scopes);
        }

        collect_binding_symbols(pattern, let_scope, "local binding");
      }

      ScopeStack body_scopes = scopes;
      body_scopes.push_back(let_scope);
      for (std::size_t i = 2; i < children.size(); ++i)
      {
        if (contains_node(children[i], target))
        {
          return resolve_local_in_node(children[i], target, body_scopes);
        }
      }
      return std::nullopt;
    }

    std::optional<Binding> resolve_local_in_params_and_body(
      const std::vector<sptr_ast_node>& children,
      const AST::ASTNode* target,
      const ScopeStack& scopes,
      std::size_t params_index,
      std::size_t body_index)
    {
      if (children.size() <= params_index ||
          children[params_index]->get_type() != Form::VECTOR)
      {
        return std::nullopt;
      }

      if (contains_node(children[params_index], target))
      {
        return binding_for_target(target, "parameter");
      }

      Scope param_scope;
      collect_binding_symbols(children[params_index], param_scope, "parameter");

      ScopeStack body_scopes = scopes;
      body_scopes.push_back(param_scope);
      for (std::size_t i = body_index; i < children.size(); ++i)
      {
        if (contains_node(children[i], target))
        {
          return resolve_local_in_node(children[i], target, body_scopes);
        }
      }
      return std::nullopt;
    }

    std::optional<Binding> resolve_local_in_node(const sptr_ast_node& node,
                                                 const AST::ASTNode* target,
                                                 const ScopeStack& scopes)
    {
      if (!node || !contains_node(node, target))
      {
        return std::nullopt;
      }

      if (node.get() == target)
      {
        if (auto name = symbol_text(*target))
        {
          return resolve_from_scopes(scopes, *name);
        }
        return std::nullopt;
      }

      if (auto head = list_head_name(node))
      {
        if (*head == "let")
        {
          return resolve_local_in_let(node, target, scopes);
        }
        if (*head == "defun")
        {
          return resolve_local_in_params_and_body(node->get_children(),
                                                  target,
                                                  scopes,
                                                  2,
                                                  3);
        }
        if (*head == "fn")
        {
          return resolve_local_in_params_and_body(node->get_children(),
                                                  target,
                                                  scopes,
                                                  1,
                                                  2);
        }
      }

      if (sequence_node(*node))
      {
        for (const auto& child : node->get_children())
        {
          if (contains_node(child, target))
          {
            return resolve_local_in_node(child, target, scopes);
          }
        }
      }
      return std::nullopt;
    }

    std::optional<Binding> resolve_local(const sptr_ast_node_v& forms,
                                         const AST::ASTNode* target)
    {
      ScopeStack scopes;
      for (const auto& form : forms)
      {
        if (contains_node(form, target))
        {
          return resolve_local_in_node(form, target, scopes);
        }
      }
      return std::nullopt;
    }

    sptr_val resolution_value(const SourceMap& source_map, const Binding& binding)
    {
      return Value::map({
        Value::keyword("kind"),
        Value::keyword("local"),
        Value::keyword("name"),
        Value::string(binding.name),
        Value::keyword("source"),
        source_value(source_map, binding.source),
        Value::keyword("signature"),
        Value::string(binding.signature),
      });
    }

    sptr_val thing_at_value(const std::string& file_name,
                            const std::string& source,
                            uint32_t line,
                            uint32_t column)
    {
      SourceMap source_map;
      const uint32_t source_id = source_map.intern_file(file_name);
      Reader reader;
      sptr_ast_node_v forms = reader.read_sexps(source, source_id, true);
      SourcePosition position{line, column};

      const AST::ASTNode* target = nullptr;
      for (const auto& form : forms)
      {
        if (const auto* match = deepest_node_at(form, position))
        {
          target = match;
          break;
        }
      }

      if (!target)
      {
        return Value::map({
          Value::keyword("status"),
          Value::keyword("missing"),
        });
      }

      const auto resolution = resolve_local(forms, target);
      sptr_val_v thing{
        Value::keyword("text"),
        Value::string(node_text(*target)),
        Value::keyword("kind"),
        thing_kind(*target),
        Value::keyword("role"),
        resolution ? Value::keyword(resolution->source.span.start.line ==
                                          target->get_source().span.start.line &&
                                        resolution->source.span.start.column ==
                                          target->get_source().span.start.column
                                      ? "local-binding"
                                      : "local-reference")
                   : Value::keyword("reference"),
      };

      if (resolution)
      {
        thing.push_back(Value::keyword("resolution"));
        thing.push_back(resolution_value(source_map, *resolution));
      }

      return Value::map({
        Value::keyword("status"),
        Value::keyword("ok"),
        Value::keyword("range"),
        range_value(target->get_source()),
        Value::keyword("thing"),
        Value::map(thing),
      });
    }

  } // namespace

  /** ThingAtFunction - lookup.analysis/thing-at! */
  FUNC_IMPL(ThingAtFunction,
            SIG((FN_ARGS((&Type::STRING),
                         (&Type::STRING),
                         (&Type::STRING),
                         (&Type::STRING),
                         (&Type::STRING)),
                 EXEC_DISPATCH(&ThingAtFunction::exec_thing_at))))

  EXEC_BODY(ThingAtFunction, exec_thing_at)
  {
    (void)args[0];
    try
    {
      const std::string& file_name = args[1]->str();
      const uint32_t line = static_cast<uint32_t>(std::stoul(args[2]->str()));
      const uint32_t column = static_cast<uint32_t>(std::stoul(args[3]->str()));
      const std::string& source = args[4]->str();
      return thing_at_value(file_name, source, line, column);
    }
    catch (const std::exception& e)
    {
      return Value::map({
        Value::keyword("status"),
        Value::keyword("error"),
        Value::keyword("message"),
        Value::string(e.what()),
      });
    }
  }

  std::unique_ptr<Namespace> make_analysis_namespace()
  {
    auto ns = std::make_unique<Namespace>("lookup.analysis");
    ns->set_origin(Namespace::Origin::native());
    ns->store("thing-at!", ThingAtFunction::make());
    return ns;
  }
} // namespace Roo::Lookup
