
#include "roo/lang/struct.h"

#include <roo/runtime/dict.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/seq.h>
#include <roo/runtime/value.h>

namespace Roo
{
  namespace
  {
    bool is_exec_arg(const sptr_val& arg)
    {
      return arg->type == Value::Type::FUNCTION || arg->type == Value::Type::KEYWORD;
    }

    bool assoc_keys_are_integers(const sptr_val_v& args)
    {
      for (size_t i = 1; i < args.size() - 1; i += 2)
      {
        if (args[i]->type != Value::Type::NUMBER ||
            (args[i]->num().num_type != Value::NumberType::INT &&
             args[i]->num().num_type != Value::NumberType::LONG))
        {
          return false;
        }
      }
      return true;
    }

    struct UpdateCall
    {
      sptr_val updater;
      sptr_val_v args;
    };

    UpdateCall make_update_call(const sptr_val& current_value,
                                const sptr_val& updater_spec,
                                const std::string& function_name)
    {
      UpdateCall call{nullptr, {current_value}};
      if (updater_spec->type != Value::Type::NIL && Type::EXEC.is_type_of(*updater_spec))
      {
        call.updater = updater_spec;
      }
      else if (updater_spec->type != Value::Type::NIL &&
               updater_spec->type != Value::Type::MAP && Type::SEQ.is_type_of(*updater_spec))
      {
        sptr_val_v spec_parts = Roo::get_children(*updater_spec);
        if (spec_parts.empty())
        {
          throw InvocationException("Updater spec for " + function_name +
                                    " cannot be empty.");
        }
        if (spec_parts[0]->type == Value::Type::NIL ||
            !Type::EXEC.is_type_of(*spec_parts[0]))
        {
          throw TypeError(
            "Updater spec for " + function_name +
            " must begin with a callable value, got: " + spec_parts[0]->to_string());
        }

        call.updater = spec_parts[0];
        call.args.reserve(spec_parts.size());
        for (size_t spec_i = 1; spec_i < spec_parts.size(); spec_i++)
        {
          call.args.push_back(spec_parts[spec_i]);
        }
      }
      else
      {
        throw TypeError(
          "Updater spec for " + function_name +
          " must be callable or a sequence, got: " + updater_spec->to_string());
      }

      return call;
    }

    enum class UpdateMode
    {
      COPY_KEY,
      MUTATE_KEY,
      COPY_PATH,
      MUTATE_PATH
    };

    uptr_exec_node lower_update_form(const SpecialForm* form,
                                     LowerContext& ctx,
                                     const sptr_ast_node& ast_node)
    {
      sptr_ast_node_v& elements = ast_node->get_children();
      if (elements.size() < 3)
      {
        throw InvalidFormException("Expected a target and at least one updater pair.");
      }
      if (elements.size() % 2 != 0)
      {
        throw InvalidFormException("Missing updater for key/path '" +
                                   elements.back()->to_string() + "'.");
      }

      const std::string current_name = "$update-current";
      SymbolBinding current_binding(current_name);

      uptr_exec_node_v exec_nodes;
      exec_nodes.reserve(elements.size() - 1);
      exec_nodes.push_back(lower_expr(ctx, elements[1]));

      sptr_val_v values;
      values.reserve(elements.size() / 2);
      values.push_back(Value::string(current_name));

      for (size_t element_i = 2; element_i < elements.size(); element_i += 2)
      {
        exec_nodes.push_back(lower_expr(ctx, elements[element_i]));

        const sptr_ast_node& updater_form = elements[element_i + 1];
        const bool inline_updater = updater_form->get_type() == Form::VECTOR;
        values.push_back(Value::boolean(inline_updater));

        if (!inline_updater)
        {
          exec_nodes.push_back(lower_expr(ctx, updater_form));
          continue;
        }

        const sptr_ast_node_v& updater_parts = updater_form->get_children();
        if (updater_parts.empty())
        {
          throw InvalidFormException("Updater spec cannot be empty.");
        }
        if (updater_parts.front()->get_type() == Form::KEYWORD && updater_parts.size() != 1)
        {
          throw InvalidFormException("Keyword updater expects no additional arguments: " +
                                     updater_form->to_string());
        }

        auto current_reference = AST::Symbol::make(current_name);
        current_reference->set_source(updater_form->get_source());

        sptr_ast_node_v call_parts;
        call_parts.reserve(updater_parts.size() + 1);
        call_parts.push_back(updater_parts.front());
        call_parts.push_back(current_reference);
        call_parts.insert(call_parts.end(), updater_parts.begin() + 1, updater_parts.end());

        auto updater_call = AST::List::make(call_parts);
        updater_call->set_source(updater_form->get_source());

        ctx.push({});
        ctx.add_lexical_binding(current_binding);
        try
        {
          exec_nodes.push_back(lower_expr(ctx, updater_call));
        }
        catch (...)
        {
          ctx.pop();
          throw;
        }
        ctx.pop();
      }

      return std::make_unique<ExecNode>(
        ast_node,
        SpecialFormNode(form, values, std::move(exec_nodes)));
    }

    sptr_val execute_update_body(Context& ctx,
                                 SpecialFormNode& snode,
                                 size_t pair_index,
                                 const sptr_val& current_value,
                                 const std::string& function_name)
    {
      const size_t updater_node_index = 2 + pair_index * 2;
      const bool inline_updater = Roo::is_truthy(*snode.values[pair_index + 1]);

      if (!inline_updater)
      {
        sptr_val updater_spec = exec(ctx, *snode.exec_nodes[updater_node_index]);
        UpdateCall call = make_update_call(current_value, updater_spec, function_name);
        return invoke_indirect_callable(ctx,
                                        call.updater,
                                        call.args,
                                        InvocationOperation::CALLING);
      }

      const std::string current_name = snode.values.front()->str();
      Scope current_scope;
      current_scope.store(current_name, current_value);
      ctx.push_context(true, current_scope);
      try
      {
        sptr_val result = exec(ctx, *snode.exec_nodes[updater_node_index]);
        ctx.pop_context();
        return result;
      }
      catch (...)
      {
        ctx.pop_context();
        throw;
      }
    }

    sptr_val_v require_update_path(const sptr_val& path_value,
                                   const std::string& function_name)
    {
      if (path_value->type == Value::Type::NIL || !Type::SEQ.is_type_of(*path_value))
      {
        throw TypeError("Path for " + function_name +
                        " must be a sequence, got: " + path_value->to_string());
      }

      sptr_val_v path = Roo::get_children(*path_value);
      if (path.empty())
      {
        throw InvocationException("Path for " + function_name + " cannot be empty.");
      }
      return path;
    }

    sptr_val execute_update_form(Context& ctx,
                                 SpecialFormNode& snode,
                                 UpdateMode mode,
                                 const std::string& function_name)
    {
      sptr_val result = exec(ctx, *snode.exec_nodes.front());
      const size_t pair_count = snode.values.size() - 1;

      for (size_t pair_i = 0; pair_i < pair_count; pair_i++)
      {
        const size_t key_node_index = 1 + pair_i * 2;
        sptr_val key_or_path = exec(ctx, *snode.exec_nodes[key_node_index]);

        if (mode == UpdateMode::COPY_PATH || mode == UpdateMode::MUTATE_PATH)
        {
          sptr_val_v path = require_update_path(key_or_path, function_name);
          sptr_val current_value = Dict::get_property_path(result, path);
          sptr_val updated_value =
            execute_update_body(ctx, snode, pair_i, current_value, function_name);

          if (mode == UpdateMode::COPY_PATH)
          {
            result = Dict::assoc_in(result, path, updated_value);
          }
          else
          {
            result = Dict::assoc_in_bang(result, path, updated_value);
          }
          continue;
        }

        sptr_val current_value = Dict::get_property(result, key_or_path);
        sptr_val updated_value =
          execute_update_body(ctx, snode, pair_i, current_value, function_name);

        if (mode == UpdateMode::COPY_KEY)
        {
          sptr_val new_result = Dict::shallow_copy(result);
          Dict::set_property(new_result, key_or_path, updated_value);
          result = new_result;
        }
        else
        {
          Dict::set_property(result, key_or_path, updated_value);
        }
      }

      return result;
    }
  } // namespace

  /** AssocFunction - roo/assoc */
  FUNC_IMPL(AssocFunction,
            MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                       EXEC_DISPATCH(&AssocFunction::exec_assoc)),
                      (FN_ARGS((&Type::SEQ), (&Type::NUMBER), (&Type::ANY)),
                       EXEC_DISPATCH(&AssocFunction::exec_assoc_seq))))

  EXEC_BODY(AssocFunction, exec_assoc)
  {
    if (args.size() % 2 == 0)
    {
      throw Roo::InvocationException("No value given for key '" + args.back()->to_string() +
                                     " '");
    }

    if (args[0]->type == Value::Type::NIL)
    {
      if (assoc_keys_are_integers(args)) return exec_assoc_seq(ctx, args);
    }

    sptr_val_v new_content = Roo::get_children(*args[0]);
    for (size_t assoc_arg_i = 1; assoc_arg_i < args.size() - 1; assoc_arg_i += 2)
    {
      sptr_val& assoc_key = args[assoc_arg_i];
      sptr_val& value = args[assoc_arg_i + 1];

      bool found = false;
      for (size_t i = 0; i < new_content.size(); i += 2)
      {
        if (*new_content[i] == *assoc_key)
        {
          new_content[i + 1] = value;
          found = true;
          break;
        }
      }

      if (!found)
      {
        new_content.push_back(assoc_key);
        new_content.push_back(value);
      }
    }

    return Value::map(std::move(new_content));
  }

  EXEC_BODY(AssocFunction, exec_assoc_seq)
  {
    sptr_val result = Value::vector(Roo::get_children(*args[0]));
    for (size_t i = 1; i < args.size() - 1; i += 2)
    {
      const std::int64_t index = checked_sequence_index(*args[i], "assoc index");
      if (index < 0)
      {
        throw TypeError("assoc index must not be negative.");
      }
      Roo::set_child(*result, static_cast<size_t>(index), args[i + 1]);
    }

    return result;
  }

  /** AssocBangFunction - roo/assoc! */
  FUNC_IMPL(AssocBangFunction,
            MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                       EXEC_DISPATCH(&AssocBangFunction::exec_assoc_bang)),
                      (FN_ARGS((&Type::SEQ), (&Type::NUMBER), (&Type::ANY)),
                       EXEC_DISPATCH(&AssocBangFunction::exec_assoc_seq_bang))))

  EXEC_BODY(AssocBangFunction, exec_assoc_bang)
  {
    if (args.size() % 2 == 0)
    {
      throw Roo::InvocationException("No value given for key '" + args.back()->to_string() +
                                     " '");
    }

    if (args[0]->type == Value::Type::NIL)
    {
      sptr_val_v normalized_args = args;
      if (assoc_keys_are_integers(args))
      {
        normalized_args[0] = Value::vector({});
        return exec_assoc_seq_bang(ctx, normalized_args);
      }
      normalized_args[0] = Value::map({});
      return exec_assoc_bang(ctx, normalized_args);
    }

    for (size_t i = 1; i < args.size() - 1; i += 2)
    {
      Dict::set_property(args[0], args[i], args[i + 1]);
    }

    return args[0];
  }

  EXEC_BODY(AssocBangFunction, exec_assoc_seq_bang)
  {
    for (size_t i = 1; i < args.size() - 1; i += 2)
    {
      const std::int64_t index = checked_sequence_index(*args[i], "assoc! index");
      if (index < 0)
      {
        throw TypeError("assoc! index must not be negative.");
      }
      Roo::set_child(*args[0], static_cast<size_t>(index), args[i + 1]);
    }

    return args[0];
  }

  /** AssocInFunction - roo/assoc-in */
  FUNC_IMPL(AssocInFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&AssocInFunction::exec_assoc_in))))

  EXEC_BODY(AssocInFunction, exec_assoc_in)
  {
    if (args.size() % 2 == 0)
    {
      throw Roo::InvocationException("No value given for path '" + args.back()->to_string() +
                                     " '");
    }

    sptr_val result = args[0];
    for (size_t assoc_arg_i = 1; assoc_arg_i < args.size() - 1; assoc_arg_i += 2)
    {
      const sptr_val& assoc_path_value = args[assoc_arg_i];
      if (assoc_path_value->type == Value::Type::NIL ||
          !Type::SEQ.is_type_of(*assoc_path_value))
      {
        throw TypeError("Path for assoc-in must be a sequence, got: " +
                        assoc_path_value->to_string());
      }

      const sptr_val_v assoc_path = Roo::get_children(*assoc_path_value);
      if (assoc_path.empty())
      {
        throw InvocationException("Path for assoc-in cannot be empty.");
      }

      result = Dict::assoc_in(result, assoc_path, args[assoc_arg_i + 1]);
    }

    return result;
  }

  /** AssocInBangFunction - roo/assoc-in! */
  FUNC_IMPL(AssocInBangFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&AssocInBangFunction::exec_assoc_in_bang))))

  EXEC_BODY(AssocInBangFunction, exec_assoc_in_bang)
  {
    if (args.size() % 2 == 0)
    {
      throw Roo::InvocationException("No value given for path '" +
                                     args.back()->to_string() + " '");
    }

    sptr_val result = args[0];
    for (size_t assoc_arg_i = 1; assoc_arg_i < args.size() - 1; assoc_arg_i += 2)
    {
      const sptr_val& assoc_path_value = args[assoc_arg_i];
      if (assoc_path_value->type == Value::Type::NIL ||
          !Type::SEQ.is_type_of(*assoc_path_value))
      {
        throw TypeError("Path for assoc-in! must be a sequence, got: " +
                        assoc_path_value->to_string());
      }

      const sptr_val_v assoc_path = Roo::get_children(*assoc_path_value);
      if (assoc_path.empty())
      {
        throw InvocationException("Path for assoc-in! cannot be empty.");
      }

      result = Dict::assoc_in_bang(result, assoc_path, args[assoc_arg_i + 1]);
    }

    return result;
  }

  /** UpdateForm - roo/update */
  SPECIAL_FORM_IMPL(UpdateForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&UpdateForm::execnode_update))))

  SFORM_LOWER_IMPL(UpdateForm)
  {
    return lower_update_form(this, ctx, ast_node);
  }

  EXECNODE_BODY(UpdateForm, execnode_update)
  {
    return execute_update_form(ctx, snode, UpdateMode::COPY_KEY, "update");
  }

  /** UpdateBangForm - roo/update! */
  SPECIAL_FORM_IMPL(UpdateBangForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&UpdateBangForm::execnode_update_bang))))

  SFORM_LOWER_IMPL(UpdateBangForm)
  {
    return lower_update_form(this, ctx, ast_node);
  }

  EXECNODE_BODY(UpdateBangForm, execnode_update_bang)
  {
    return execute_update_form(ctx, snode, UpdateMode::MUTATE_KEY, "update!");
  }

  /** UpdateInForm - roo/update-in */
  SPECIAL_FORM_IMPL(UpdateInForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&UpdateInForm::execnode_update_in))))

  SFORM_LOWER_IMPL(UpdateInForm)
  {
    return lower_update_form(this, ctx, ast_node);
  }

  EXECNODE_BODY(UpdateInForm, execnode_update_in)
  {
    return execute_update_form(ctx, snode, UpdateMode::COPY_PATH, "update-in");
  }

  /** UpdateInBangForm - roo/update-in! */
  SPECIAL_FORM_IMPL(UpdateInBangForm,
                    SIG((FN_ARGS((&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&Type::ANY, NO_EVAL),
                                 (&VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&UpdateInBangForm::execnode_update_in_bang))))

  SFORM_LOWER_IMPL(UpdateInBangForm)
  {
    return lower_update_form(this, ctx, ast_node);
  }

  EXECNODE_BODY(UpdateInBangForm, execnode_update_in_bang)
  {
    return execute_update_form(ctx, snode, UpdateMode::MUTATE_PATH, "update-in!");
  }

  /** GetFunction - roo/get */
  FUNC_IMPL(GetFunction,
            MULTI_SIG((FN_ARGS((&Type::ANY), (&Type::ANY)),
                       EXEC_DISPATCH(&GetFunction::exec_get)),
                      (FN_ARGS((&Type::ANY), (&Type::ANY), (&Type::ANY)),
                       EXEC_DISPATCH(&GetFunction::exec_get))))

  EXEC_BODY(GetFunction, exec_get)
  {
    auto [found, value] = Dict::find_property(args[0], args[1]);
    if (!found && args.size() == 3) return args[2];
    return found ? value : Constant::NIL;
  }

  /** GetInFunction - roo/get-in */
  FUNC_IMPL(GetInFunction,
            MULTI_SIG((FN_ARGS((&Type::ANY), (&Type::VECTOR)),
                       EXEC_DISPATCH(&GetInFunction::exec_get)),
                      (FN_ARGS((&Type::ANY), (&Type::VECTOR), (&Type::ANY)),
                       EXEC_DISPATCH(&GetInFunction::exec_get))))

  EXEC_BODY(GetInFunction, exec_get)
  {
    auto [found, value] = Dict::find_property_path(args[0], args[1]->elements());
    if (!found && args.size() == 3) return args[2];
    return found ? value : Constant::NIL;
  }

  /** DissocFunction - roo/dissoc */
  FUNC_IMPL(DissocFunction,
            SIG((FN_ARGS((&Type::MAP), (&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&DissocFunction::exec_dissoc))))

  EXEC_BODY(DissocFunction, exec_dissoc)
  {
    if (*Constant::NIL == *args[0])
    {
      return Constant::NIL;
    }

    const sptr_val_v& map_elements = args[0]->elements();
    sptr_val_v new_map_elements;

    for (size_t i = 0; i < map_elements.size(); i += 2)
    {
      bool remove = false;
      for (size_t key_i = 1; key_i < args.size(); key_i++)
      {
        if (*map_elements[i] == *args[key_i])
        {
          remove = true;
          break;
        }
      }

      if (!remove)
      {
        new_map_elements.push_back(map_elements[i]);
        new_map_elements.push_back(map_elements[i + 1]);
      }
    }

    return Value::map(new_map_elements);
  }

  /** DissocInFunction - roo/dissoc-in */
  FUNC_IMPL(DissocInFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&DissocInFunction::exec_dissoc_in))))

  EXEC_BODY(DissocInFunction, exec_dissoc_in)
  {
    sptr_val result = args[0];

    for (size_t dissoc_arg_i = 1; dissoc_arg_i < args.size(); dissoc_arg_i++)
    {
      const sptr_val& dissoc_path_value = args[dissoc_arg_i];
      if (dissoc_path_value->type == Value::Type::NIL ||
          !Type::SEQ.is_type_of(*dissoc_path_value))
      {
        throw TypeError("Path for dissoc-in must be a sequence, got: " +
                        dissoc_path_value->to_string());
      }

      const sptr_val_v dissoc_path = Roo::get_children(*dissoc_path_value);
      if (dissoc_path.empty())
      {
        throw InvocationException("Path for dissoc-in cannot be empty.");
      }

      result = Dict::dissoc_in(result, dissoc_path);
    }

    return result;
  }

  /** DissocBangFunction - roo/dissoc! */
  FUNC_IMPL(DissocBangFunction,
            SIG((FN_ARGS((&Type::MAP), (&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&DissocBangFunction::exec_dissoc_bang))))

  EXEC_BODY(DissocBangFunction, exec_dissoc_bang)
  {
    if (*Constant::NIL == *args[0])
    {
      return Constant::NIL;
    }

    for (size_t i = 1; i < args.size(); i++)
    {
      Dict::remove_property(args[0], args[i]);
    }
    return args[0];
  }

  /** DissocInBangFunction - roo/dissoc-in! */
  FUNC_IMPL(DissocInBangFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&DissocInBangFunction::exec_dissoc_in_bang))))

  EXEC_BODY(DissocInBangFunction, exec_dissoc_in_bang)
  {
    sptr_val result = args[0];
    for (size_t dissoc_arg_i = 1; dissoc_arg_i < args.size(); dissoc_arg_i++)
    {
      const sptr_val& dissoc_path_value = args[dissoc_arg_i];
      if (dissoc_path_value->type == Value::Type::NIL ||
          !Type::SEQ.is_type_of(*dissoc_path_value))
      {
        throw TypeError("Path for dissoc-in! must be a sequence, got: " +
                        dissoc_path_value->to_string());
      }

      const sptr_val_v dissoc_path = Roo::get_children(*dissoc_path_value);
      if (dissoc_path.empty())
      {
        throw InvocationException("Path for dissoc-in! cannot be empty.");
      }

      result = Dict::dissoc_in_bang(result, dissoc_path);
    }

    return result;
  }

  /** KeysFunction - roo/keys */
  FUNC_IMPL(KeysFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&KeysFunction::exec_keys))))

  EXEC_BODY(KeysFunction, exec_keys)
  {
    return Value::vector(Dict::map_sptr_keys(args[0]));
  }

  /** MergeFunction - roo/merge */
  FUNC_IMPL(MergeFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&VARARG, &Type::COMPLEX)),
                 EXEC_DISPATCH(&MergeFunction::exec_merge))))

  EXEC_BODY(MergeFunction, exec_merge)
  {
    sptr_val_v new_content = Roo::get_children(*args[0]);

    for (size_t i = 1; i < args.size(); i++)
    {
      if (args[i]->type == Value::Type::NIL) continue;
      Dict::merge_map_content(new_content, Roo::get_children(*args[i]));
    }

    return Value::map(std::move(new_content));
  }

  /** MergeBangFunction - roo/merge! */
  FUNC_IMPL(MergeBangFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&VARARG, &Type::COMPLEX)),
                 EXEC_DISPATCH(&MergeBangFunction::exec_merge_bang))))

  EXEC_BODY(MergeBangFunction, exec_merge_bang)
  {
    sptr_val_v merged_content;
    if (args.size() > 1)
    {
      merged_content = Roo::get_children(*args[1]);
      for (size_t i = 2; i < args.size(); i++)
      {
        if (args[i]->type == Value::Type::NIL) continue;
        Dict::merge_map_content(merged_content, Roo::get_children(*args[i]));
      }
    }

    sptr_val result = args[0]->type == Value::Type::NIL ? Value::map({}) : args[0];
    for (size_t i = 0; i < merged_content.size(); i += 2)
    {
      Dict::set_property(result, merged_content[i], merged_content[i + 1]);
    }

    return result;
  }

  /** ReduceKeyValueFunction - roo/reduce-kv */
  FUNC_IMPL(ReduceKeyValueFunction,
            MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&Type::EXEC)),
                       EXEC_DISPATCH(&ReduceKeyValueFunction::exec_reduce_kv)),
                      (FN_ARGS((&Type::EXEC), (&Type::COMPLEX), (&Type::ANY)),
                       EXEC_DISPATCH(&ReduceKeyValueFunction::exec_reduce_kv))))

  EXEC_BODY(ReduceKeyValueFunction, exec_reduce_kv)
  {
    bool reducer_first = is_exec_arg(args[0]);
    sptr_val map_arg = reducer_first ? args[1] : args[0];
    sptr_val result = reducer_first ? args[2] : args[1];
    sptr_val reducer = reducer_first ? args[0] : args[2];

    for (auto key : Dict::map_sptr_keys(map_arg))
    {
      sptr_val_v reducer_args{result, key, Dict::get_property(map_arg, *key)};

      sptr_val new_result =
        invoke_indirect_callable(ctx, reducer, reducer_args, InvocationOperation::CALLING);
      if (new_result.get() != result.get())
      {
        result.swap(new_result);
      }
    }

    return result;
  }

  /** SelectKeysFunction - roo/select-keys */
  FUNC_IMPL(SelectKeysFunction,
            SIG((FN_ARGS((&Type::ANY), (&Type::SEQ)),
                 EXEC_DISPATCH(&SelectKeysFunction::exec_select_keys))))

  EXEC_BODY(SelectKeysFunction, exec_select_keys)
  {
    sptr_val_v new_content;
    for (auto& key : Roo::get_children(*args[1]))
    {
      sptr_val value = Dict::get_property(args[0], *key);
      if (value->type != Value::Type::NIL)
      {
        new_content.push_back(key);
        new_content.push_back(value);
      }
    }
    return Value::map(std::move(new_content));
  }

  /** ValsFunction - roo/vals */
  FUNC_IMPL(ValsFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&ValsFunction::exec_vals))))

  EXEC_BODY(ValsFunction, exec_vals)
  {
    return Value::vector(Dict::map_sptr_vals(args[0]));
  }

} // namespace Roo
