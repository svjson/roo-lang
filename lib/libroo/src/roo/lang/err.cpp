#include "roo/lang/err.h"

#include <algorithm>
#include <utility>

#include <roo/bind.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/impl.h>
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
        auto selector_node = lower_literal(clause_forms[0]);
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

      auto binding_node = lower_literal(binding_forms[0]);
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
      const std::string error_type = error.roo_error_type();
      const std::vector<std::string> parent_types = error.roo_parent_error_types();
      const sptr_val error_value = error.to_error_map();
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
      throw;
    }
  }
} // namespace Roo
