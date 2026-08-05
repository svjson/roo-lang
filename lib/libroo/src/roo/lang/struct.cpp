
#include "roo/lang/struct.h"

#include <roo/runtime/dict.h>
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

    struct UpdateCall
    {
      Executable* updater = nullptr;
      sptr_val_v args;
    };

    UpdateCall make_update_call(const sptr_val& current_value,
                                const sptr_val& updater_spec,
                                const std::string& function_name)
    {
      UpdateCall call{nullptr, {current_value}};
      if (updater_spec->type != Value::Type::NIL && Type::EXEC.is_type_of(*updater_spec))
      {
        call.updater = &updater_spec->exec();
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
            " must begin with an executable, got: " + spec_parts[0]->to_string());
        }

        call.updater = &spec_parts[0]->exec();
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
          " must be executable or a sequence, got: " + updater_spec->to_string());
      }

      return call;
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
    sptr_val_v new_content = Roo::get_children(*args[0]);
    int idx = args[1]->i32();
    sptr_val& value = args.back();

    if (idx >= static_cast<int>(new_content.size()))
    {
      for (int i = 0; i < idx - static_cast<int>(new_content.size()); i++)
      {
        new_content.push_back(Roo::Constant::NIL);
      }
    }

    new_content[idx] = value;

    return Value::vector(std::move(new_content));
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

    for (size_t i = 1; i < args.size() - 1; i += 2)
    {
      Dict::set_property(args[0], args[i], args[i + 1]);
    }

    return args[0];
  }

  EXEC_BODY(AssocBangFunction, exec_assoc_seq_bang)
  {
    if (auto* elements = std::get_if<sptr_val_v>(&args[0]->value))
    {
      size_t index = args[1]->i32();
      (*elements)[index] = args.back();
    }
    else
    {
      throw TypeError("assoc! on seq by index not implemented for value type: " +
                      std::to_string((int)args[0]->type));
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
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::VECTOR), (&Type::ANY)),
                 EXEC_DISPATCH(&AssocInBangFunction::exec_assoc_in_bang))))

  EXEC_BODY(AssocInBangFunction, exec_assoc_in_bang)
  {
    const sptr_val_v& assoc_path = args[1]->elements();
    if (assoc_path.empty())
    {
      throw InvocationException("Path for assoc-in! cannot be empty.");
    }
    sptr_val value = args.back();
    const sptr_val& assoc_key = assoc_path.back();

    sptr_val target = args[0];
    for (size_t i = 0; i < assoc_path.size() - 1; i++)
    {
      target = Dict::get_property(target, *assoc_path[i]);
    }

    Dict::set_property(target, assoc_key, value);

    return args[0];
  }

  /** UpdateFunction - roo/update */
  FUNC_IMPL(
    UpdateFunction,
    MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&Type::ANY), (&VARARG, &Type::ANY)),
               EXEC_DISPATCH(&UpdateFunction::exec_update)),
              (FN_ARGS((&Type::SEQ), (&Type::NUMBER), (&Type::ANY), (&VARARG, &Type::ANY)),
               EXEC_DISPATCH(&UpdateFunction::exec_update))))

  EXEC_BODY(UpdateFunction, exec_update)
  {
    if (args.size() % 2 == 0)
    {
      throw Roo::InvocationException("No updater given for key '" +
                                     args.back()->to_string() + " '");
    }

    sptr_val result = args[0];
    for (size_t update_arg_i = 1; update_arg_i < args.size() - 1; update_arg_i += 2)
    {
      sptr_val current_value = Dict::get_property(result, args[update_arg_i]);
      const sptr_val& updater_spec = args[update_arg_i + 1];

      UpdateCall call = make_update_call(current_value, updater_spec, "update");
      sptr_val updated_value = call.updater->execute(ctx, call.args);
      sptr_val new_result = Dict::shallow_copy(result);
      Dict::set_property(new_result, args[update_arg_i], updated_value);
      result = new_result;
    }

    return result;
  }

  /** UpdateBangFunction - roo/update! */
  FUNC_IMPL(
    UpdateBangFunction,
    MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&Type::ANY), (&VARARG, &Type::ANY)),
               EXEC_DISPATCH(&UpdateBangFunction::exec_update_bang)),
              (FN_ARGS((&Type::SEQ), (&Type::NUMBER), (&Type::ANY), (&VARARG, &Type::ANY)),
               EXEC_DISPATCH(&UpdateBangFunction::exec_update_bang))))

  EXEC_BODY(UpdateBangFunction, exec_update_bang)
  {
    if (args.size() % 2 == 0)
    {
      throw Roo::InvocationException("No updater given for key '" +
                                     args.back()->to_string() + " '");
    }

    for (size_t update_arg_i = 1; update_arg_i < args.size() - 1; update_arg_i += 2)
    {
      sptr_val current_value = Dict::get_property(args[0], args[update_arg_i]);
      const sptr_val& updater_spec = args[update_arg_i + 1];

      UpdateCall call = make_update_call(current_value, updater_spec, "update!");
      sptr_val updated_value = call.updater->execute(ctx, call.args);
      Dict::set_property(args[0], args[update_arg_i], updated_value);
    }

    return args[0];
  }

  /** UpdateInFunction - roo/update-in */
  FUNC_IMPL(
    UpdateInFunction,
    MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&Type::ANY), (&VARARG, &Type::ANY)),
               EXEC_DISPATCH(&UpdateInFunction::exec_update_in)),
              (FN_ARGS((&Type::SEQ), (&Type::ANY), (&Type::ANY), (&VARARG, &Type::ANY)),
               EXEC_DISPATCH(&UpdateInFunction::exec_update_in))))

  EXEC_BODY(UpdateInFunction, exec_update_in)
  {
    if (args.size() % 2 == 0)
    {
      throw Roo::InvocationException("No updater given for path '" +
                                     args.back()->to_string() + " '");
    }

    sptr_val result = args[0];
    for (size_t update_arg_i = 1; update_arg_i < args.size() - 1; update_arg_i += 2)
    {
      const sptr_val& assoc_path_value = args[update_arg_i];
      if (assoc_path_value->type == Value::Type::NIL ||
          !Type::SEQ.is_type_of(*assoc_path_value))
      {
        throw TypeError("Path for update-in must be a sequence, got: " +
                        assoc_path_value->to_string());
      }

      const sptr_val_v assoc_path = Roo::get_children(*assoc_path_value);
      if (assoc_path.empty())
      {
        throw InvocationException("Path for update-in cannot be empty.");
      }

      sptr_val current_value = Dict::get_property_path(result, assoc_path);
      const sptr_val& updater_spec = args[update_arg_i + 1];

      UpdateCall call = make_update_call(current_value, updater_spec, "update-in");
      sptr_val updated_value = call.updater->execute(ctx, call.args);
      result = Dict::assoc_in(result, assoc_path, updated_value);
    }

    return result;
  }

  /** UpdateInBangFunction - roo/update-in! */
  FUNC_IMPL(
    UpdateInBangFunction,
    MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&Type::ANY), (&VARARG, &Type::ANY)),
               EXEC_DISPATCH(&UpdateInBangFunction::exec_update_in_bang)),
              (FN_ARGS((&Type::SEQ), (&Type::ANY), (&Type::ANY), (&VARARG, &Type::ANY)),
               EXEC_DISPATCH(&UpdateInBangFunction::exec_update_in_bang))))

  EXEC_BODY(UpdateInBangFunction, exec_update_in_bang)
  {
    if (args.size() % 2 == 0)
    {
      throw Roo::InvocationException("No updater given for path '" +
                                     args.back()->to_string() + " '");
    }

    for (size_t update_arg_i = 1; update_arg_i < args.size() - 1; update_arg_i += 2)
    {
      const sptr_val& assoc_path_value = args[update_arg_i];
      if (assoc_path_value->type == Value::Type::NIL ||
          !Type::SEQ.is_type_of(*assoc_path_value))
      {
        throw TypeError("Path for update-in! must be a sequence, got: " +
                        assoc_path_value->to_string());
      }

      const sptr_val_v assoc_path = Roo::get_children(*assoc_path_value);
      if (assoc_path.empty())
      {
        throw InvocationException("Path for update-in! cannot be empty.");
      }

      sptr_val current_value = Dict::get_property_path(args[0], assoc_path);
      const sptr_val& updater_spec = args[update_arg_i + 1];

      UpdateCall call = make_update_call(current_value, updater_spec, "update-in!");
      sptr_val updated_value = call.updater->execute(ctx, call.args);

      sptr_val target = args[0];
      for (size_t i = 0; i < assoc_path.size() - 1; i++)
      {
        target = Dict::get_property(target, *assoc_path[i]);
      }

      Dict::set_property(target, assoc_path.back(), updated_value);
    }

    return args[0];
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
            SIG((FN_ARGS((&Type::MAP), (&Type::ANY)),
                 EXEC_DISPATCH(&DissocBangFunction::exec_dissoc_bang))))

  EXEC_BODY(DissocBangFunction, exec_dissoc_bang)
  {
    if (*Constant::NIL == *args[0])
    {
      return Constant::NIL;
    }

    return Dict::remove_property(args[0], args[1]);
  }

  /** DissocInBangFunction - roo/dissoc-in! */
  FUNC_IMPL(DissocInBangFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::VECTOR)),
                 EXEC_DISPATCH(&DissocInBangFunction::exec_dissoc_in_bang))))

  EXEC_BODY(DissocInBangFunction, exec_dissoc_in_bang)
  {
    const sptr_val_v& dissoc_path = args[1]->elements();
    if (dissoc_path.empty())
    {
      throw InvocationException("Path for dissoc-in! cannot be empty.");
    }

    sptr_val target = args[0];
    for (size_t i = 0; i < dissoc_path.size() - 1; i++)
    {
      if (*target == *Constant::NIL)
      {
        return Constant::NIL;
      }

      switch (target->type)
      {
      case Value::Type::NIL:
      case Value::Type::MAP:
      case Value::Type::OBJECT:
      case Value::Type::NATIVE_OBJECT:
      case Value::Type::VECTOR:
      case Value::Type::LIST:
        break;
      default:
        throw TypeError("dissoc-in! cannot traverse through " + target->to_string());
      }

      auto [found, value] = Dict::find_property(target, dissoc_path[i]);
      if (!found)
      {
        return Constant::NIL;
      }

      target = value;
    }

    if (*target == *Constant::NIL)
    {
      return Constant::NIL;
    }

    if (target->type != Value::Type::MAP)
    {
      throw TypeError("dissoc-in! cannot remove property from " + target->to_string());
    }

    return Dict::remove_property(target, dissoc_path.back());
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
      const sptr_val_v& other = Roo::get_children(*args[i]);
      for (size_t j = 0; j < other.size(); j += 2)
      {
        bool found = false;
        for (size_t k = 0; k < new_content.size(); k += 2)
        {
          if (*new_content[k] == *other[j])
          {
            new_content[k + 1] = other[j + 1];
            found = true;
            break;
          }
        }
        if (!found)
        {
          new_content.push_back(other[j]);
          new_content.push_back(other[j + 1]);
        }
      }
    }

    return Value::map(std::move(new_content));
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
    Executable& reducer = (reducer_first ? args[0] : args[2])->exec();

    for (auto key : Dict::map_sptr_keys(map_arg))
    {
      sptr_val_v reducer_args{result, key, Dict::get_property(map_arg, *key)};

      sptr_val new_result = reducer.execute(ctx, reducer_args);
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

} // namespace Roo
