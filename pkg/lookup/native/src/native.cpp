#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/reader.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

#include <lookup/native.h>
#include <roo-package/native_abi.h>

namespace Roo::Lookup
{
  namespace
  {
    std::string read_file(const std::filesystem::path& path)
    {
      errno = 0;
      std::ifstream input(path, std::ios::binary);
      if (!input)
      {
        std::string message = "Could not read file: " + path.string();
        if (errno != 0)
        {
          message += ": ";
          message += std::strerror(errno);
        }
        throw std::runtime_error(message);
      }

      std::ostringstream contents;
      contents << input.rdbuf();
      if (input.bad())
      {
        throw std::runtime_error("Could not read file: " + path.string());
      }
      return contents.str();
    }

    sptr_val source_position_value(const SourcePosition& position)
    {
      if (!position.valid())
      {
        return Constant::NIL;
      }

      return Value::map({
        Value::keyword("line"),
        Value::number(static_cast<int>(position.line)),
        Value::keyword("column"),
        Value::number(static_cast<int>(position.column)),
      });
    }

    sptr_val source_value(const SourceMap& source_map, const SourceRef& source)
    {
      return Value::map({
        Value::keyword("path"),
        Value::string(source.valid() ? source_map.file_name(source.file_id) : ""),
        Value::keyword("line"),
        source.valid() ? Value::number(static_cast<int>(source.span.start.line))
                       : Constant::NIL,
        Value::keyword("column"),
        source.valid() ? Value::number(static_cast<int>(source.span.start.column))
                       : Constant::NIL,
        Value::keyword("start"),
        source_position_value(source.span.start),
        Value::keyword("end"),
        source_position_value(source.span.end),
      });
    }

    sptr_val range_value(const SourceRef& source)
    {
      if (!source.valid())
      {
        return Constant::NIL;
      }

      return Value::map({
        Value::keyword("start"),
        source_position_value(source.span.start),
        Value::keyword("end"),
        source_position_value(source.span.end),
      });
    }

    sptr_val form_entry_value(const SourceMap& source_map, const sptr_ast_node& form)
    {
      return Value::map({
        Value::keyword("form"),
        to_rt_value(*form),
        Value::keyword("source"),
        source_value(source_map, form->get_source()),
      });
    }

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

    class ReadFileFormsFunction : public Function
    {
     public:
      ReadFileFormsFunction()
        : Function(SIG((FN_ARGS((&Type::STRING)),
                        EXEC_DISPATCH(&ReadFileFormsFunction::exec_read_file_forms))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<ReadFileFormsFunction>());
      }

      sptr_val exec_read_file_forms(Context&, sptr_val_v& args)
      {
        const std::filesystem::path path = args[0]->str();
        std::error_code ec;
        if (!std::filesystem::is_regular_file(path, ec))
        {
          if (ec)
          {
            throw RooException("Could not inspect file: " + path.string() + ": " +
                               ec.message());
          }
          throw RooException("Not a regular file: " + path.string());
        }

        SourceMap source_map;
        const uint32_t source_id = source_map.intern_file(path.string());
        Reader reader;
        sptr_ast_node_v forms;
        try
        {
          forms = reader.read_sexps(read_file(path), source_id, true);
        }
        catch (const std::exception& e)
        {
          throw RooException("Could not read Roo forms from " + path.string() + ": " +
                             e.what());
        }

        sptr_val_v entries;
        entries.reserve(forms.size());
        for (const auto& form : forms)
        {
          entries.push_back(form_entry_value(source_map, form));
        }
        return Value::vector(entries);
      }
    };

    class ReadStdinFunction : public Function
    {
     public:
      ReadStdinFunction()
        : Function(SIG((NO_ARGS, EXEC_DISPATCH(&ReadStdinFunction::exec_read_stdin))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<ReadStdinFunction>());
      }

      sptr_val exec_read_stdin(Context&, sptr_val_v&)
      {
        std::ostringstream contents;
        contents << std::cin.rdbuf();
        return Value::string(contents.str());
      }
    };

    class ThingAtFunction : public Function
    {
     public:
      ThingAtFunction()
        : Function(SIG((FN_ARGS((&Type::STRING),
                                (&Type::STRING),
                                (&Type::STRING),
                                (&Type::STRING),
                                (&Type::STRING)),
                        EXEC_DISPATCH(&ThingAtFunction::exec_thing_at))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<ThingAtFunction>());
      }

      sptr_val exec_thing_at(Context&, sptr_val_v& args)
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
    };
  } // namespace

  std::unique_ptr<Namespace> make_native_namespace()
  {
    auto ns = std::make_unique<Namespace>("lookup.native");
    ns->set_origin(Namespace::Origin::native());
    ns->store("read-file-forms!", ReadFileFormsFunction::make());
    ns->store("read-stdin!", ReadStdinFunction::make());
    ns->store("thing-at!", ThingAtFunction::make());
    return ns;
  }

  std::vector<std::unique_ptr<Namespace>> make_native_namespaces()
  {
    std::vector<std::unique_ptr<Namespace>> namespaces;
    namespaces.push_back(make_native_namespace());
    return namespaces;
  }
} // namespace Roo::Lookup

namespace
{
  int load_lookup_native(const RooNativeHostV1* host)
  {
    auto ns = Roo::Lookup::make_native_namespace();
    return host->register_namespace(host->user, ns.release());
  }

  void unload_lookup_native() {}

  const char* lookup_native_last_error()
  {
    return "";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1* roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1),
    "lookup-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_lookup_native,
    unload_lookup_native,
    lookup_native_last_error,
  };
  return &package;
}
