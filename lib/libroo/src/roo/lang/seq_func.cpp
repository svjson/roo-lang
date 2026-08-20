
#include <algorithm>

#include <roo/lang/seq_func.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/node.h>
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

    bool is_function_arg(const sptr_val& arg)
    {
      return arg->type == Value::Type::FUNCTION;
    }
  } // namespace

  /** AnyFunction - roo/any? */
  FUNC_IMPL(AnyFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&AnyFunction::exec_any)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&AnyFunction::exec_any))))

  EXEC_BODY(AnyFunction, exec_any)
  {
    bool fn_first = is_exec_arg(args[0]);
    sptr_val seq_arg = fn_first ? args[1] : args[0];
    sptr_val fn_arg = fn_first ? args[0] : args[1];
    sptr_val_v val_arg = {nullptr};
    sptr_val_v elements = Roo::get_children(*seq_arg);
    for (auto& element : elements)
    {
      val_arg[0] = element;
      sptr_val result =
        invoke_indirect_callable(ctx, fn_arg, val_arg, InvocationOperation::CALLING);
      if (Roo::is_truthy(*result))
      {
        return Constant::BOOL_TRUE;
      }
    }
    return Constant::BOOL_FALSE;
  }

  /** EveryFunction - roo/every? */
  FUNC_IMPL(EveryFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&EveryFunction::exec_every)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&EveryFunction::exec_every))))

  EXEC_BODY(EveryFunction, exec_every)
  {
    bool fn_first = is_exec_arg(args[0]);
    sptr_val seq_arg = fn_first ? args[1] : args[0];
    sptr_val fn_arg = fn_first ? args[0] : args[1];
    sptr_val_v val_arg = {nullptr};
    sptr_val_v elements = Roo::get_children(*seq_arg);
    for (auto& element : elements)
    {
      val_arg[0] = element;
      sptr_val result =
        invoke_indirect_callable(ctx, fn_arg, val_arg, InvocationOperation::CALLING);
      if (!Roo::is_truthy(*result))
      {
        return Constant::BOOL_FALSE;
      }
    }
    return Constant::BOOL_TRUE;
  }

  /** FilterFunction - roo/filter */
  FUNC_IMPL(FilterFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&FilterFunction::exec_filter)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&FilterFunction::exec_filter))))

  EXEC_BODY(FilterFunction, exec_filter)
  {
    Roo::sptr_val original;
    Roo::sptr_val fn;
    if (is_exec_arg(args[0]))
    {
      original = args[1];
      fn = args[0];
    }
    else
    {
      original = args[0];
      fn = args[1];
    }

    if (*Constant::NIL == *original) return Value::vector({});

    sptr_val_v result;
    result.reserve(count(*original));

    sptr_val_v val_args{Constant::NIL};
    sptr_val_v elements = Roo::get_children(*original);
    for (auto val : elements)
    {
      val_args[0] = val;
      sptr_val pred_result =
        invoke_indirect_callable(ctx, fn, val_args, InvocationOperation::CALLING);
      if (Roo::is_truthy(*pred_result))
      {
        result.push_back(val);
      }
    }

    return Value::vector(std::move(result));
  }

  /** FindFirstFunction - roo/find-first */
  FUNC_IMPL(FindFirstFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&FindFirstFunction::exec_find_first)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&FindFirstFunction::exec_find_first))))

  EXEC_BODY(FindFirstFunction, exec_find_first)
  {
    bool fn_first = is_exec_arg(args[0]);
    sptr_val seq_arg = fn_first ? args[1] : args[0];
    sptr_val needle_fn = fn_first ? args[0] : args[1];

    sptr_val_v val_args{nullptr};
    sptr_val_v children = Roo::get_children(*seq_arg);
    for (auto val : children)
    {
      val_args[0] = val;
      sptr_val pred_result = invoke_indirect_callable(
        ctx, needle_fn, val_args, InvocationOperation::CALLING);
      if (Roo::is_truthy(*pred_result))
      {
        return val;
      }
    }

    return Constant::NIL;
  }

  /** FindIndexFunction - roo/find-index */
  FUNC_IMPL(FindIndexFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&FindIndexFunction::exec_find_index)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&FindIndexFunction::exec_find_index))))

  EXEC_BODY(FindIndexFunction, exec_find_index)
  {
    bool fn_first = is_function_arg(args[0]);
    sptr_val seq_arg = fn_first ? args[1] : args[0];
    sptr_val needle_fn = fn_first ? args[0] : args[1];
    if (*seq_arg == *Constant::NIL) return Constant::NIL;

    sptr_val_v children = Roo::get_children(*seq_arg);

    sptr_val_v val_args{nullptr};
    for (size_t i = 0; i < children.size(); i++)
    {
      val_args[0] = children[i];
      auto item = std::make_unique<ExecNode>(children[i]);
      if (Roo::is_truthy(
            *invoke_indirect_callable(
              ctx, needle_fn, val_args, InvocationOperation::CALLING)))
      {
        return Value::number(static_cast<int>(i));
      }
    }

    return Constant::NIL;
  }

  /** FlatMapFunction - roo/flat-map */
  FUNC_IMPL(FlatMapFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&FlatMapFunction::exec_flat_map)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&FlatMapFunction::exec_flat_map))))

  EXEC_BODY(FlatMapFunction, exec_flat_map)
  {
    Roo::sptr_val original;
    Roo::sptr_val fn;
    if (is_exec_arg(args[0]))
    {
      original = args[1];
      fn = args[0];
    }
    else
    {
      original = args[0];
      fn = args[1];
    }

    sptr_val_v result;
    sptr_val_v elements = Roo::get_children(*original);
    sptr_val_v map_args{nullptr};
    for (auto& element : elements)
    {
      map_args[0] = element;
      sptr_val mapped =
        invoke_indirect_callable(ctx, fn, map_args, InvocationOperation::CALLING);

      if (Type::SEQ_OR_STRING.is_type_of(*mapped))
      {
        for (auto& child : Roo::get_children(*mapped))
        {
          result.push_back(child);
        }
      }
      else
      {
        result.push_back(mapped);
      }
    }

    return Value::vector(std::move(result));
  }

  /** IndexOfFunction - roo/index-of */
  FUNC_IMPL(IndexOfFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::ANY)),
                 EXEC_DISPATCH(&IndexOfFunction::exec_index_of))))

  EXEC_BODY(IndexOfFunction, exec_index_of)
  {
    if (*args[0] == *Constant::NIL) return Constant::NIL;

    sptr_val_v children = Roo::get_children(*args[0]);
    for (size_t i = 0; i < children.size(); i++)
    {
      if (*children[i] == *args[1])
      {
        return Value::number(static_cast<int>(i));
      }
    }

    return Constant::NIL;
  }

  /** KeepFunction - roo/keep */
  FUNC_IMPL(KeepFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&KeepFunction::exec_keep)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&KeepFunction::exec_keep))))

  EXEC_BODY(KeepFunction, exec_keep)
  {
    bool fn_first = is_exec_arg(args[0]);
    sptr_val seq_arg = fn_first ? args[1] : args[0];
    sptr_val fn_arg = fn_first ? args[0] : args[1];
    sptr_val_v values = Roo::get_children(*seq_arg);

    sptr_val_v result;

    sptr_val_v arg{nullptr};
    for (auto& v : values)
    {
      arg[0] = v;
      sptr_val r =
        invoke_indirect_callable(ctx, fn_arg, arg, InvocationOperation::CALLING);
      if (*r != *Constant::NIL) result.push_back(r);
    }

    return Value::vector(std::move(result));
  }

  /** MapFunction - roo/map */
  FUNC_IMPL(MapFunction,
            MULTI_SIG((FN_ARGS((&VARARG, &Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&MapFunction::exec_map)),
                      (FN_ARGS((&Type::EXEC), (&VARARG, &Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&MapFunction::exec_map))))

  EXEC_BODY(MapFunction, exec_map)
  {
    bool mapper_first = is_exec_arg(args[0]);
    auto& mapper = mapper_first ? args.front() : args.back();
    sptr_val_v result;

    std::vector<sptr_val_v> seqs;

    const size_t first_seq = mapper_first ? 1 : 0;
    const size_t end_seq = mapper_first ? args.size() : args.size() - 1;
    for (size_t i = first_seq; i < end_seq; i++)
    {
      seqs.push_back(Roo::get_children(*args[i]));
    }

    auto max_lmnts_it =
      std::max_element(seqs.begin(),
                       seqs.end(),
                       [](const auto& a, const auto& b) { return a.size() < b.size(); });

    result.reserve((*max_lmnts_it).size());

    sptr_val_v map_args;
    for (size_t seq_i = 0; seq_i < seqs.size(); seq_i++)
    {
      map_args.push_back(nullptr);
    }

    bool valid;
    for (size_t i = 0; i < (*max_lmnts_it).size(); i++)
    {
      valid = true;
      for (size_t seq_i = 0; seq_i < seqs.size(); seq_i++)
      {
        if (i < seqs[seq_i].size())
        {
          map_args[seq_i] = seqs[seq_i][i];
        }
        else
        {
          valid = false;
          break;
        }
      }

      if (valid)
      {
        result.push_back(
          invoke_indirect_callable(ctx, mapper, map_args, InvocationOperation::CALLING));
      }
      else
      {
        result.push_back(Constant::NIL);
      }
    }

    return Value::vector(std::move(result));
  }

  /** ReduceFunction - roo/reduce */
  FUNC_IMPL(ReduceFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::ANY), (&Type::FUNCTION)),
                       EXEC_DISPATCH(&ReduceFunction::exec_reduce)),
                      (FN_ARGS((&Type::FUNCTION), (&Type::SEQ_OR_STRING), (&Type::ANY)),
                       EXEC_DISPATCH(&ReduceFunction::exec_reduce))))

  EXEC_BODY(ReduceFunction, exec_reduce)
  {
    bool reducer_first = is_function_arg(args[0]);
    sptr_val seq_arg = reducer_first ? args[1] : args[0];
    sptr_val result = reducer_first ? args[2] : args[1];
    sptr_val reducer = reducer_first ? args[0] : args[2];
    sptr_val_v children = Roo::get_children(*seq_arg);

    sptr_val iter_result;
    sptr_val_v reduce_args{result, nullptr};
    for (auto& lmnt : children)
    {
      reduce_args[1] = lmnt;
      result =
        invoke_indirect_callable(ctx, reducer, reduce_args, InvocationOperation::CALLING);

      if (iter_result.get() != result.get())
      {
        reduce_args[0] = result;
      }
    }

    return result;
  }

  /** RemoveFunction - roo/remove */
  FUNC_IMPL(RemoveFunction,
            MULTI_SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&RemoveFunction::exec_remove)),
                      (FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&RemoveFunction::exec_remove))))

  EXEC_BODY(RemoveFunction, exec_remove)
  {
    Value* original;
    sptr_val remove_fn;

    if (is_exec_arg(args[0]))
    {
      original = args[1].get();
      remove_fn = args[0];
    }
    else
    {
      original = args[0].get();
      remove_fn = args[1];
    }

    sptr_val_v result;
    result.reserve(Roo::count(*original));
    sptr_val_v elements = Roo::get_children(*original);
    Roo::sptr_val_v val_args{nullptr};
    for (auto val : elements)
    {
      val_args[0] = val;
      auto pred_result = invoke_indirect_callable(
        ctx, remove_fn, val_args, InvocationOperation::CALLING);
      if (!Roo::is_truthy(*pred_result))
      {
        result.push_back(val);
      }
    }

    return Value::vector(std::move(result));
  }

  /** RemoveBangFunction - roo/remove! */
  FUNC_IMPL(RemoveBangFunction,
            MULTI_SIG((FN_ARGS((&Type::EXEC), (&Type::STRICT_SEQ)),
                       EXEC_DISPATCH(&RemoveBangFunction::exec_remove_bang)),
                      (FN_ARGS((&Type::STRICT_SEQ), (&Type::EXEC)),
                       EXEC_DISPATCH(&RemoveBangFunction::exec_remove_bang))))

  EXEC_BODY(RemoveBangFunction, exec_remove_bang)
  {
    bool fn_first = is_exec_arg(args[0]);
    sptr_val seq_arg = fn_first ? args[1] : args[0];
    sptr_val fn_arg = fn_first ? args[0] : args[1];

    if (seq_arg->type == Value::Type::NATIVE_OBJECT)
    {
      throw TypeError("remove! not implemented for native host sequences.");
    }
    else
    {
      sptr_val_v& children = std::get<sptr_val_v>(seq_arg->value);

      auto it = std::remove_if(children.begin(),
                               children.end(),
                               [&](const Roo::sptr_val& element)
                               {
                                 Roo::sptr_val_v val_args{element};
                                 auto pred_result = invoke_indirect_callable(
                                   ctx, fn_arg, val_args, InvocationOperation::CALLING);
                                 return Roo::is_truthy(*pred_result);
                               });

      children.erase(it, children.end());
    }

    return seq_arg;
  }

  /** RemoveFirstFunction - roo/remove-first */
  FUNC_IMPL(RemoveFirstFunction,
            MULTI_SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&RemoveFirstFunction::exec_remove_first)),
                      (FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&RemoveFirstFunction::exec_remove_first))))

  EXEC_BODY(RemoveFirstFunction, exec_remove_first)
  {
    Value* original;
    sptr_val remove_fn;

    if (is_exec_arg(args[0]))
    {
      original = args[1].get();
      remove_fn = args[0];
    }
    else
    {
      original = args[0].get();
      remove_fn = args[1];
    }

    sptr_val_v result;
    result.reserve(Roo::count(*original));

    bool removed = false;
    sptr_val_v val_args{nullptr};
    for (auto val : Roo::get_children(*original))
    {
      val_args[0] = val;
      auto test_result = invoke_indirect_callable(
        ctx, remove_fn, val_args, InvocationOperation::CALLING);
      if (removed || !Roo::is_truthy(*test_result))
      {
        result.push_back(val);
      }
      else
      {
        removed = true;
      }
    }

    return Value::vector(std::move(result));
  }

  /** SeqMatchFunction - roo/seq-match */
  FUNC_IMPL(SeqMatchFunction,
            SIG((FN_ARGS((&Roo::Type::SEQ), (&Roo::Type::MAP)),
                 EXEC_DISPATCH(&SeqMatchFunction::exec_match))))

  bool match_map_like(sptr_val& obj, sptr_val& pattern)
  {
    for (auto& key : Dict::map_sptr_keys(pattern))
    {
      sptr_val prop = Dict::get_property(pattern, *key);
      sptr_val value = Dict::get_property(obj, *key);

      if (*Constant::NIL != *prop && Type::COMPLEX.is_type_of(*prop))
      {
        if (!match_map_like(value, prop))
        {
          return false;
        }
      }
      else if (*prop != *value)
      {
        return false;
      }
    }

    return true;
  }

  EXEC_BODY(SeqMatchFunction, exec_match)
  {
    sptr_val& pattern = args.back();

    for (auto& obj : Roo::get_children(*args[0]))
    {
      if (match_map_like(obj, pattern))
      {
        return obj;
      }
    }

    return Constant::NIL;
  }

  /** SortFunction - roo/sort */
  FUNC_IMPL(SortFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&SortFunction::exec_sort)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&SortFunction::exec_sort))))

  EXEC_BODY(SortFunction, exec_sort)
  {
    Value* seq_arg;
    sptr_val comparator;

    if (is_exec_arg(args[0]))
    {
      seq_arg = args[1].get();
      comparator = args[0];
    }
    else
    {
      seq_arg = args[0].get();
      comparator = args[1];
    }

    Roo::sptr_val_v elements = Roo::get_children(*seq_arg);

    if (elements.size() > 1)
    {
      sptr_val_v cmp_args{nullptr, nullptr};
      std::sort(elements.begin(),
                elements.end(),
                [&](const sptr_val& a, const sptr_val& b)
                {
                  cmp_args[0] = a;
                  cmp_args[1] = b;
                  return Roo::is_truthy(
                    *invoke_indirect_callable(
                      ctx, comparator, cmp_args, InvocationOperation::CALLING));
                });
    }

    return Value::vector(std::move(elements));
  }

} // namespace Roo
