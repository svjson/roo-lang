#include "roo/lang/err.h"

#include <algorithm>
#include <exception>
#include <unordered_set>
#include <utility>

#include <roo/bind.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/impl.h>
#include <roo/runtime.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/lower.h>
#include <roo/runtime/value.h>
#include <roo/scope.h>

namespace Roo
{
  namespace
  {
    class PushedContext
    {
     public:
      explicit PushedContext(Context& context)
        : context(context)
      {
      }

      ~PushedContext() { context.pop_context(); }

      PushedContext(const PushedContext&) = delete;
      PushedContext& operator=(const PushedContext&) = delete;

     private:
      Context& context;
    };

    bool selector_matches(const sptr_val& selector,
                          const std::string& error_type,
                          const std::vector<std::string>& parent_types)
    {
      if (selector->type == Value::Type::NIL) return true;

      const std::string& selector_name = selector->str();
      const bool qualified = selector_name.find('/') != std::string::npos;
      auto matches = [&](const std::string& candidate)
      {
        if (qualified) return candidate == selector_name;
        return split_qualifiable(candidate).first == selector_name;
      };

      if (matches(error_type)) return true;
      return std::any_of(parent_types.begin(), parent_types.end(), matches);
    }

    sptr_val snapshot_error_value(const sptr_val& value)
    {
      if (!value) return nullptr;

      sptr_val_v elements;
      switch (value->type)
      {
      case Value::Type::MAP:
      case Value::Type::LIST:
      case Value::Type::VECTOR:
        elements.reserve(value->elements().size());
        for (const auto& element : value->elements())
        {
          elements.push_back(snapshot_error_value(element));
        }
        break;
      default:
        return value;
      }

      if (value->type == Value::Type::MAP) return Value::map(elements);
      if (value->type == Value::Type::LIST) return Value::list(elements);
      return Value::vector(elements);
    }

    bool reserved_error_key(const sptr_val& key)
    {
      if (key->type != Value::Type::KEYWORD) return false;

      static const std::unordered_set<std::string> keys{
        "type",
        "parent-type",
        "parent-types",
        "message",
        "detail",
        "site",
        "frames",
        "cause",
      };
      return keys.contains(key->str());
    }

    void append_metadata(sptr_val_v& fields, const sptr_val& metadata)
    {
      if (metadata->type != Value::Type::MAP)
      {
        throw TypeError("raise metadata must be a map.");
      }

      const sptr_val_v& metadata_fields = metadata->elements();
      for (std::size_t i = 0; i < metadata_fields.size(); i += 2)
      {
        if (reserved_error_key(metadata_fields[i]))
        {
          throw TypeError("raise metadata cannot contain reserved error key " +
                          metadata_fields[i]->to_string() + ".");
        }
        fields.push_back(metadata_fields[i]);
        fields.push_back(metadata_fields[i + 1]);
      }
    }

    sptr_val raise_error_map(const sptr_val_v& args,
                             sptr_val& source_error,
                             KeywordPool& keywords)
    {
      sptr_val_v fields;
      const sptr_val& error = args[0];

      if (args.size() == 1)
      {
        if (error->type == Value::Type::MAP)
        {
          source_error = error;
          return error;
        }
        if (error->type == Value::Type::STRING)
        {
          fields = {Value::keyword("message", keywords), error};
        }
        else if (error->type == Value::Type::KEYWORD)
        {
          fields = {Value::keyword("type", keywords), error};
        }
        else
        {
          throw TypeError("raise expects a string, qualified keyword, or error map.");
        }
        return Value::map(fields);
      }

      if (error->type == Value::Type::STRING && args[1]->type == Value::Type::MAP)
      {
        fields = {Value::keyword("message", keywords), error};
        append_metadata(fields, args[1]);
        return Value::map(fields);
      }

      if (error->type == Value::Type::KEYWORD)
      {
        fields = {Value::keyword("type", keywords), error};
        if (args[1]->type == Value::Type::STRING)
        {
          fields.push_back(Value::keyword("message", keywords));
          fields.push_back(args[1]);
          if (args.size() == 3) append_metadata(fields, args[2]);
          return Value::map(fields);
        }
        if (args.size() == 2 && args[1]->type == Value::Type::MAP)
        {
          append_metadata(fields, args[1]);
          return Value::map(fields);
        }
      }

      throw TypeError("raise arguments do not match a supported call shape.");
    }
  } // namespace

  /** GuardForm - roo/guard */
  SPECIAL_FORM_IMPL(GuardForm,
                    SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&GuardForm::execnode_guard))))

  SFORM_LOWER_IMPL(GuardForm)
  {
    const sptr_ast_node_v& elements = ast_node->get_children();
    if (elements.size() < 2 || elements[1]->get_type() != Form::LIST)
    {
      throw InvalidFormException(
        "guard expects a parenthesized handler set followed by zero or more body forms.");
    }

    uptr_exec_node_v exec_nodes;
    exec_nodes.reserve(elements.size() - 2);
    for (std::size_t i = 2; i < elements.size(); i++)
    {
      exec_nodes.push_back(lower_expr(ctx, elements[i]));
    }

    sptr_val_v clause_metadata{Value::number(static_cast<long>(exec_nodes.size()))};
    std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
    const auto& handler_forms = elements[1]->get_children();
    clause_metadata.reserve(1 + handler_forms.size() * 3);
    bindings.reserve(handler_forms.size());

    for (const auto& handler_form : handler_forms)
    {
      if (handler_form->get_type() != Form::LIST)
      {
        throw InvalidFormException("guard handler clause must be a list: " +
                                   handler_form->to_string());
      }

      const auto& clause_forms = handler_form->get_children();
      if (clause_forms.empty())
      {
        throw InvalidFormException("guard handler clause cannot be empty.");
      }

      sptr_val selector = Constant::NIL;
      std::size_t binding_index = 0;
      if (clause_forms[0]->get_type() == Form::KEYWORD)
      {
        auto selector_node = lower_literal(ctx, clause_forms[0]);
        selector = std::get<LiteralNode>(selector_node->data).value;
        binding_index = 1;
      }

      if (binding_index >= clause_forms.size() ||
          clause_forms[binding_index]->get_type() != Form::VECTOR)
      {
        throw InvalidFormException(
          "guard handler clause requires an error binding vector: " +
          handler_form->to_string());
      }

      const auto& binding_forms = clause_forms[binding_index]->get_children();
      if ((binding_forms.size() != 1 && binding_forms.size() != 2) ||
          binding_forms[0]->get_type() != Form::SYMBOL)
      {
        throw InvalidFormException(
          "guard error binding must have the form [symbol] or [symbol condition]: " +
          clause_forms[binding_index]->to_string());
      }

      auto binding_node = lower_literal(ctx, binding_forms[0]);
      auto binding = LexicalBinding::create(std::get<LiteralNode>(binding_node->data));

      ctx.push({});
      ctx.add_lexical_binding(*binding);

      const bool has_condition = binding_forms.size() == 2;
      clause_metadata.push_back(selector);
      clause_metadata.push_back(Value::boolean(has_condition));
      clause_metadata.push_back(
        Value::number(static_cast<long>(clause_forms.size() - binding_index - 1)));

      if (binding_forms.size() == 2)
      {
        exec_nodes.push_back(lower_expr(ctx, binding_forms[1]));
      }

      for (std::size_t i = binding_index + 1; i < clause_forms.size(); i++)
      {
        exec_nodes.push_back(lower_expr(ctx, clause_forms[i]));
      }
      ctx.pop();

      bindings.emplace_back(std::move(binding), std::make_unique<ExecNode>(Constant::NIL));
    }

    return std::make_unique<ExecNode>(
      ast_node,
      SpecialFormNode(this, clause_metadata, std::move(bindings), std::move(exec_nodes)));
  }

  EXECNODE_BODY(GuardForm, execnode_guard)
  {
    const std::size_t body_count = static_cast<std::size_t>(snode.values[0]->i64());
    try
    {
      sptr_val result = Constant::NIL;
      for (std::size_t i = 0; i < body_count; i++)
      {
        result = exec(ctx, *snode.exec_nodes[i]);
      }
      return result;
    }
    catch (RooException& error)
    {
      const std::exception_ptr original_exception = std::current_exception();
      const std::string error_type = error.roo_error_type();
      const std::vector<std::string> parent_types = error.roo_parent_error_types();
      const sptr_val error_value = error.to_error_map();
      const sptr_val original_error_value = snapshot_error_value(error_value);
      const std::size_t clause_count = (snode.values.size() - 1) / 3;
      std::size_t node_index = body_count;

      for (std::size_t clause_index = 0; clause_index < clause_count; clause_index++)
      {
        const std::size_t metadata_index = 1 + clause_index * 3;
        const sptr_val& selector = snode.values[metadata_index];
        const bool has_condition = is_truthy(*snode.values[metadata_index + 1]);
        const std::size_t handler_count =
          static_cast<std::size_t>(snode.values[metadata_index + 2]->i64());

        if (!selector_matches(selector, error_type, parent_types))
        {
          node_index += (has_condition ? 1 : 0) + handler_count;
          continue;
        }

        Scope binding_scope;
        ctx.push_context(true, binding_scope);
        PushedContext pushed_context(ctx);
        snode.bind_forms[clause_index].first->apply(ctx.current_scope(), error_value);

        try
        {
          if (has_condition && !is_truthy(*exec(ctx, *snode.exec_nodes[node_index++])))
          {
            node_index += handler_count;
            continue;
          }

          sptr_val result = Constant::NIL;
          for (std::size_t i = 0; i < handler_count; i++)
          {
            result = exec(ctx, *snode.exec_nodes[node_index++]);
          }
          return result;
        }
        catch (const RaisedError& raised)
        {
          if (raised.source_error() == error_value && *error_value == *original_error_value)
          {
            std::rethrow_exception(original_exception);
          }
          throw;
        }
      }
      throw;
    }
  }

  /** RaiseFunction - roo/raise */
  FUNC_IMPL(RaiseFunction,
            MULTI_SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&RaiseFunction::exec_raise)),
                      (FN_ARGS((&Type::ANY), (&Type::ANY)),
                       EXEC_DISPATCH(&RaiseFunction::exec_raise)),
                      (FN_ARGS((&Type::KEYWORD), (&Type::STRING), (&Type::MAP)),
                       EXEC_DISPATCH(&RaiseFunction::exec_raise))))

  EXEC_BODY(RaiseFunction, exec_raise)
  {
    sptr_val source_error;
    const sptr_val error_map =
      raise_error_map(args, source_error, ctx.get_runtime().keyword_pool());
    throw RaisedError(error_map, std::move(source_error));
  }

} // namespace Roo
