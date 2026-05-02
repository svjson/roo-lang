
#include "lisple/lang/struct.h"

#include <lisple/host.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/seq.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
  static sptr_rtval apply_update_fn(Context& ctx,
                                    const sptr_rtval& updater_fn,
                                    const sptr_rtval& current_value,
                                    const sptr_rtval_v& args,
                                    size_t extra_args_begin)
  {
    Executable& updater = updater_fn->exec();
    sptr_rtval_v updater_args;
    updater_args.reserve(args.size() - extra_args_begin + 1);
    updater_args.push_back(current_value);

    for (size_t i = extra_args_begin; i < args.size(); i++)
    {
      updater_args.push_back(args[i]);
    }

    return updater.execute(ctx, updater_args);
  }

  /* AssocFunction - assoc */
  FUNC_IMPL(AssocFunction,
            MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                       EXEC_DISPATCH(&AssocFunction::exec_assoc)),
                      (FN_ARGS((&Type::SEQ), (&Type::NUMBER), (&Type::ANY)),
                       EXEC_DISPATCH(&AssocFunction::exec_assoc_seq))))

  EXEC_BODY(AssocFunction, exec_assoc)
  {
    if (args.size() % 2 == 0)
    {
      throw Lisple::InvocationException("No value given for key '" +
                                        args.back()->to_string() + " '");
    }

    sptr_rtval_v new_content = Lisple::get_children(*args[0]);
    for (size_t assoc_arg_i = 1; assoc_arg_i < args.size() - 1; assoc_arg_i += 2)
    {
      sptr_rtval& assoc_key = args[assoc_arg_i];
      sptr_rtval& value = args[assoc_arg_i + 1];

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

    return RTValue::map(std::move(new_content));
  }

  EXEC_BODY(AssocFunction, exec_assoc_seq)
  {
    sptr_rtval_v new_content = Lisple::get_children(*args[0]);
    int idx = args[1]->i32();
    sptr_rtval& value = args.back();

    if (idx >= static_cast<int>(new_content.size()))
    {
      for (int i = 0; i < idx - static_cast<int>(new_content.size()); i++)
      {
        new_content.push_back(Lisple::Constant::NIL);
      }
    }

    new_content[idx] = value;

    return RTValue::vector(std::move(new_content));
  }

  /* AssocBangFunction - assoc! */
  FUNC_IMPL(AssocBangFunction,
            MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                       EXEC_DISPATCH(&AssocBangFunction::exec_assoc_bang)),
                      (FN_ARGS((&Type::SEQ), (&Type::NUMBER), (&Type::ANY)),
                       EXEC_DISPATCH(&AssocBangFunction::exec_assoc_seq_bang))))

  EXEC_BODY(AssocBangFunction, exec_assoc_bang)
  {
    if (args.size() % 2 == 0)
    {
      throw Lisple::InvocationException("No value given for key '" +
                                        args.back()->to_string() + " '");
    }

    for (size_t i = 1; i < args.size() - 1; i += 2)
    {
      Dict::set_property(args[0], args[i], args[i + 1]);
    }

    return args[0];
  }

  EXEC_BODY(AssocBangFunction, exec_assoc_seq_bang)
  {
    if (auto* elements = std::get_if<sptr_rtval_v>(&args[0]->value))
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

  /* AssocInFunction - assoc-in */
  FUNC_IMPL(AssocInFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&AssocInFunction::exec_assoc_in))))

  EXEC_BODY(AssocInFunction, exec_assoc_in)
  {
    if (args.size() % 2 == 0)
    {
      throw Lisple::InvocationException("No value given for path '" +
                                        args.back()->to_string() + " '");
    }

    sptr_rtval result = args[0];
    for (size_t assoc_arg_i = 1; assoc_arg_i < args.size() - 1; assoc_arg_i += 2)
    {
      const sptr_rtval& assoc_path_value = args[assoc_arg_i];
      if (!Type::SEQ.is_type_of(*assoc_path_value))
      {
        throw TypeError("Path for assoc-in must be a sequence, got: " +
                        assoc_path_value->to_string());
      }

      const sptr_rtval_v& assoc_path = assoc_path_value->elements();
      if (assoc_path.empty())
      {
        throw InvocationException("Path for assoc-in cannot be empty.");
      }

      result = Dict::assoc_in(result, assoc_path, args[assoc_arg_i + 1]);
    }

    return result;
  }

  /* AssocInBangFunction - assoc-in! */
  FUNC_IMPL(AssocInBangFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::ARRAY), (&Type::ANY)),
                 EXEC_DISPATCH(&AssocInBangFunction::exec_assoc_in_bang))))

  EXEC_BODY(AssocInBangFunction, exec_assoc_in_bang)
  {
    const sptr_rtval_v& assoc_path = args[1]->elements();
    if (assoc_path.empty())
    {
      throw InvocationException("Path for assoc-in! cannot be empty.");
    }
    sptr_rtval value = args.back();
    const sptr_rtval& assoc_key = assoc_path.back();

    sptr_rtval target = args[0];
    for (size_t i = 0; i < assoc_path.size() - 1; i++)
    {
      target = Dict::get_property(target, *assoc_path[i]);
    }

    Dict::set_property(target, assoc_key, value);

    return args[0];
  }

  /* UpdateFunction - update */
  FUNC_IMPL(UpdateFunction,
            MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&Type::EXEC), (&VARARG, &Type::ANY)),
                       EXEC_DISPATCH(&UpdateFunction::exec_update)),
                      (FN_ARGS((&Type::SEQ), (&Type::NUMBER), (&Type::EXEC), (&VARARG, &Type::ANY)),
                       EXEC_DISPATCH(&UpdateFunction::exec_update))))

  EXEC_BODY(UpdateFunction, exec_update)
  {
    sptr_rtval current_value = Dict::get_property(args[0], args[1]);
    sptr_rtval updated_value = apply_update_fn(ctx, args[2], current_value, args, 3);

    sptr_rtval result = Dict::shallow_copy(args[0]);
    Dict::set_property(result, args[1], updated_value);

    return result;
  }

  /** GetFunction - get */
  FUNC_IMPL(GetFunction,
            SIG((FN_ARGS((&Type::ANY), (&Type::ANY)),
                 EXEC_DISPATCH(&GetFunction::exec_get))))

  EXEC_BODY(GetFunction, exec_get)
  {
    return Dict::get_property(args[0], args[1]);
  }

  /** GetInFunction - get-in */
  FUNC_IMPL(GetInFunction,
            SIG((FN_ARGS((&Type::ANY), (&Type::ARRAY)),
                 EXEC_DISPATCH(&GetInFunction::exec_get))))

  EXEC_BODY(GetInFunction, exec_get)
  {
    return Dict::get_property_path(args[0], args[1]->elements());
  }

  /** DissocFunction - dissoc */
  FUNC_IMPL(DissocFunction,
            SIG((FN_ARGS((&Type::MAP), (&Type::ANY)),
                 EXEC_DISPATCH(&DissocFunction::exec_dissoc))))

  EXEC_BODY(DissocFunction, exec_dissoc)
  {
    if (*Constant::NIL == *args[0])
    {
      return Constant::NIL;
    }

    const sptr_rtval_v& map_elements = args[0]->elements();
    sptr_rtval_v new_map_elements;

    for (size_t i = 0; i < map_elements.size(); i += 2)
    {
      if (*map_elements[i] != *args[1])
      {
        new_map_elements.push_back(map_elements[i]);
        new_map_elements.push_back(map_elements[i + 1]);
      }
    }

    return RTValue::map(new_map_elements);
  }

  /* DissocBangFunction - dissoc! */
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

  /** KeysFunction - keys */
  FUNC_IMPL(KeysFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&KeysFunction::exec_keys))))

  EXEC_BODY(KeysFunction, exec_keys)
  {
    return RTValue::vector(Dict::map_sptr_keys(args[0]));
  }

  /** MergeFunction - merge */
  FUNC_IMPL(MergeFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&VARARG, &Type::COMPLEX)),
                 EXEC_DISPATCH(&MergeFunction::exec_merge))))

  EXEC_BODY(MergeFunction, exec_merge)
  {
    sptr_rtval_v new_content = Lisple::get_children(*args[0]);

    for (size_t i = 1; i < args.size(); i++)
    {
      if (args[i]->type == RTValue::Type::NIL) continue;
      const sptr_rtval_v& other = Lisple::get_children(*args[i]);
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

    return RTValue::map(std::move(new_content));
  }

  /* ReduceKeyValueFunction - reduce-kv */
  FUNC_IMPL(ReduceKeyValueFunction,
            SIG((FN_ARGS((&Type::MAP), (&Type::ANY), (&Type::EXEC)),
                 EXEC_DISPATCH(&ReduceKeyValueFunction::exec_reduce_kv))))

  EXEC_BODY(ReduceKeyValueFunction, exec_reduce_kv)
  {
    sptr_rtval result = args[1];
    Executable& reducer = args.back()->exec();

    for (auto key : Dict::map_sptr_keys(args[0]))
    {
      sptr_rtval_v reducer_args{result, key, Dict::get_property(args[0], *key)};

      sptr_rtval new_result = reducer.execute(ctx, reducer_args);
      if (new_result.get() != result.get())
      {
        result.swap(new_result);
      }
    }

    return result;
  }

  /** SelectKeysFunction - select-keys */
  FUNC_IMPL(SelectKeysFunction,
            SIG((FN_ARGS((&Type::ANY), (&Type::SEQ)),
                 EXEC_DISPATCH(&SelectKeysFunction::exec_select_keys))))

  EXEC_BODY(SelectKeysFunction, exec_select_keys)
  {
    sptr_rtval_v new_content;
    for (auto& key : Lisple::get_children(*args[1]))
    {
      sptr_rtval value = Dict::get_property(args[0], *key);
      if (value->type != RTValue::Type::NIL)
      {
        new_content.push_back(key);
        new_content.push_back(value);
      }
    }
    return RTValue::map(std::move(new_content));
  }

} // namespace Lisple
