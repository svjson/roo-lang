
#include "lisple/host/object.h"

#include <algorithm>

#include <lisple/lang/seq_func.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/node.h>
#include <lisple/runtime/seq.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
  /* FilterFunction */
  FUNC_IMPL(FilterFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                       EXEC_DISPATCH(&FilterFunction::exec_filter)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ)),
                       EXEC_DISPATCH(&FilterFunction::exec_filter))))

  EXEC_BODY(FilterFunction, exec_filter)
  {
    Lisple::sptr_rtval original;
    Lisple::sptr_rtval fn;
    if (Type::SEQ.is_type_of(*args[0]))
    {
      original = args[0];
      fn = args[1];
    }
    else
    {
      original = args[1];
      fn = args[0];
    }

    if (*Constant::NIL == *original) return RTValue::vector({});

    sptr_rtval_v result;
    result.reserve(count(*original));

    auto& filter_fn = fn->exec();

    sptr_rtval_v val_args{Constant::NIL};
    sptr_rtval_v elements = Lisple::get_children(*original);
    for (auto val : elements)
    {
      val_args[0] = val;
      sptr_rtval pred_result = filter_fn.execute(ctx, val_args);
      if (Lisple::is_truthy(*pred_result))
      {
        result.push_back(val);
      }
    }

    return RTValue::vector(std::move(result));
  }

  /** FindFirstFunction - find-first */
  FUNC_IMPL(FindFirstFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&FindFirstFunction::exec_find_first))))

  EXEC_BODY(FindFirstFunction, exec_find_first)
  {
    auto original = args[0];

    auto& filter_fn = args.back()->exec();

    sptr_rtval_v val_args{nullptr};
    for (auto val : args[0]->elements())
    {
      val_args[0] = val;
      sptr_rtval pred_result = filter_fn.execute(ctx, val_args);
      if (Lisple::is_truthy(*pred_result))
      {
        return val;
      }
    }

    return Constant::NIL;
  }

  /** FindIndexFunction - find-index */
  FUNC_IMPL(FindIndexFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&FindIndexFunction::exec_find_index))))

  EXEC_BODY(FindIndexFunction, exec_find_index)
  {
    if (*args[0] == *Constant::NIL) return Constant::NIL;

    auto& filter_fn = args.back()->exec();

    sptr_rtval_v children = Lisple::get_children(*args[0]);

    sptr_rtval_v val_args{nullptr};
    for (size_t i = 0; i < children.size(); i++)
    {
      val_args[0] = children[i];
      auto item = std::make_unique<ExecNode>(children[i]);
      if (Lisple::is_truthy(*filter_fn.execute(ctx, val_args)))
      {
        return RTValue::number(static_cast<int>(i));
      }
    }

    return Constant::NIL;
  }

  /* KeepFunction - keep */
  FUNC_IMPL(KeepFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                 EXEC_DISPATCH(&KeepFunction::exec_keep))))

  EXEC_BODY(KeepFunction, exec_keep)
  {
    const sptr_rtval_v& values = args[0]->elements();
    Executable& exec = args[1]->exec();

    sptr_rtval_v result;

    sptr_rtval_v arg{nullptr};
    for (auto& v : values)
    {
      arg[0] = v;
      sptr_rtval r = exec.execute(ctx, arg);
      if (*r != *Constant::NIL) result.push_back(r);
    }

    return RTValue::vector(std::move(result));
  }

  /** MapFunction - map */
  FUNC_IMPL(MapFunction,
            SIG((FN_ARGS((&VARARG, &Type::SEQ), (&Type::EXEC)),
                 EXEC_DISPATCH(&MapFunction::exec_map))))

  EXEC_BODY(MapFunction, exec_map)
  {
    auto& mapper = args.back();
    sptr_rtval_v result;

    std::vector<sptr_rtval_v> seqs;

    for (size_t i = 0; i < args.size() - 1; i++)
    {
      seqs.push_back(Lisple::get_children(*args[i]));
    }

    auto max_lmnts_it =
      std::max_element(seqs.begin(),
                       seqs.end(),
                       [](const auto& a, const auto& b) { return a.size() < b.size(); });

    result.reserve((*max_lmnts_it).size());

    // FIXME: is_kw flag MESS until keylookupnode vs callnode gets symmetrical
    bool is_kw = mapper->type == RTValue::Type::KEYWORD;
    Executable* map_fn = is_kw ? nullptr : &mapper->exec();

    sptr_rtval_v map_args;
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
        if (is_kw)
        {
          if (seqs.size() == 1)
          {
            result.push_back(Dict::get_property(map_args[0], mapper));
          }
          else
          {
            sptr_rtval_v unit;
            for (size_t seq_i = 0; seq_i < seqs.size(); seq_i++)
            {
              unit.push_back(Dict::get_property(map_args[seq_i], mapper));
            }
            result.push_back(RTValue::vector(std::move(unit)));
          }
        }
        else
        {
          result.push_back(map_fn->execute(ctx, map_args));
        }
      }
      else
      {
        result.push_back(Constant::NIL);
      }
    }

    return RTValue::vector(std::move(result));
  }

  /** ReduceFunction - reduce */
  FUNC_IMPL(ReduceFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::ANY), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&ReduceFunction::exec_reduce))))

  EXEC_BODY(ReduceFunction, exec_reduce)
  {
    sptr_rtval_v children = Lisple::get_children(*args[0]);
    sptr_rtval result = args[1];
    auto& reducer = args.back()->exec();

    sptr_rtval iter_result;
    sptr_rtval_v reduce_args{result, nullptr};
    for (auto& lmnt : children)
    {
      reduce_args[1] = lmnt;
      result = reducer.execute(ctx, reduce_args);

      if (iter_result.get() != result.get())
      {
        reduce_args[0] = result;
      }
    }

    return result;
  }

  /** RemoveFunction - remove */
  FUNC_IMPL(RemoveFunction,
            MULTI_SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ)),
                       EXEC_DISPATCH(&RemoveFunction::exec_remove)),
                      (FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                       EXEC_DISPATCH(&RemoveFunction::exec_remove))))

  EXEC_BODY(RemoveFunction, exec_remove)
  {
    RTValue* original;
    Executable* remove_fn;

    if (Type::SEQ.is_type_of(*args[0]))
    {
      original = args[0].get();
      remove_fn = &args[1]->exec();
    }
    else
    {
      original = args[1].get();
      remove_fn = &args[0]->exec();
    }

    sptr_rtval_v result;
    result.reserve(Lisple::count(*original));
    sptr_rtval_v elements = Lisple::get_children(*original);
    Lisple::sptr_rtval_v val_args{nullptr};
    for (auto val : elements)
    {
      val_args[0] = val;
      auto pred_result = remove_fn->execute(ctx, val_args);
      if (!Lisple::is_truthy(*pred_result))
      {
        result.push_back(val);
      }
    }

    return RTValue::vector(std::move(result));
  }

  /** RemoveBangFunction - remove! */
  FUNC_IMPL(RemoveBangFunction,
            SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ)),
                 EXEC_DISPATCH(&RemoveBangFunction::exec_remove_bang))))

  EXEC_BODY(RemoveBangFunction, exec_remove_bang)
  {
    auto& remove_fn = args[0]->exec();

    if (Type::HOST_SEQ.is_type_of(*args[1]))
    {
      sptr_sobject obj = args[1]->obj();
      Seq& host_seq = obj->as<Seq>();
      sptr_sobject_v& children = host_seq.get_children();
      auto it = std::remove_if(children.begin(),
                               children.end(),
                               [&](const Lisple::sptr_sobject& element)
                               {
                                 Lisple::sptr_sobject ast_element = element;
                                 Lisple::sptr_rtval_v val_args{to_rt_value(ast_element)};
                                 return Lisple::is_truthy(*remove_fn.execute(ctx, val_args));
                               });
      children.erase(it, children.end());

      host_seq.replace_children(children);
    }
    else
    {
      sptr_rtval_v& children = std::get<sptr_rtval_v>(args[1]->value);

      auto it = std::remove_if(children.begin(),
                               children.end(),
                               [&](const Lisple::sptr_rtval& element)
                               {
                                 Lisple::sptr_rtval_v val_args{element};
                                 auto pred_result = remove_fn.execute(ctx, val_args);
                                 return Lisple::is_truthy(*pred_result);
                               });

      children.erase(it, children.end());
    }

    return args.back();
  }

  /* RemoveFirstFunction - remove-first */
  FUNC_IMPL(RemoveFirstFunction,
            MULTI_SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ)),
                       EXEC_DISPATCH(&RemoveFirstFunction::exec_remove_first)),
                      (FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                       EXEC_DISPATCH(&RemoveFirstFunction::exec_remove_first))))

  EXEC_BODY(RemoveFirstFunction, exec_remove_first)
  {
    RTValue* original;
    Executable* remove_fn;

    if (Type::SEQ.is_type_of(*args[0]))
    {
      original = args[0].get();
      remove_fn = &args[1]->exec();
    }
    else
    {
      original = args[1].get();
      remove_fn = &args[0]->exec();
    }

    sptr_rtval_v result;
    result.reserve(Lisple::count(*original));

    bool removed = false;
    sptr_rtval_v val_args{nullptr};
    for (auto val : original->elements())
    {
      val_args[0] = val;
      auto test_result = remove_fn->execute(ctx, val_args);
      if (removed || !Lisple::is_truthy(*test_result))
      {
        result.push_back(val);
      }
      else
      {
        removed = true;
      }
    }

    return RTValue::vector(std::move(result));
  }

  /* SeqMatchFunction */
  FUNC_IMPL(SeqMatchFunction,
            SIG((FN_ARGS((&Lisple::Type::SEQ), (&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&SeqMatchFunction::exec_match))))

  bool match_map_like(sptr_rtval& obj, sptr_rtval& pattern)
  {
    for (auto& key : Dict::map_sptr_keys(pattern))
    {
      sptr_rtval prop = Dict::get_property(pattern, *key);
      sptr_rtval value = Dict::get_property(obj, *key);

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
    sptr_rtval& pattern = args.back();

    for (auto& obj : Lisple::get_children(*args[0]))
    {
      if (match_map_like(obj, pattern))
      {
        return obj;
      }
    }

    return Constant::NIL;
  }

  /** SomeFunction - some? */
  FUNC_IMPL(SomeFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                 EXEC_DISPATCH(&SomeFunction::exec_some))))

  EXEC_BODY(SomeFunction, exec_some)
  {
    sptr_rtval_v val_arg = {nullptr};
    auto& fn = args.back()->exec();
    for (auto& element : args[0]->elements())
    {
      val_arg[0] = element;
      sptr_rtval result = fn.execute(ctx, val_arg);
      if (Lisple::is_truthy(*result))
      {
        return Constant::BOOL_TRUE;
      }
    }
    return Constant::BOOL_FALSE;
  }

  /* SortFunction - sort */
  FUNC_IMPL(SortFunction,
            MULTI_SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                       EXEC_DISPATCH(&SortFunction::exec_sort)),
                      (FN_ARGS((&Type::EXEC), (&Type::SEQ)),
                       EXEC_DISPATCH(&SortFunction::exec_sort))))

  EXEC_BODY(SortFunction, exec_sort)
  {
    RTValue* seq_arg;
    Executable* comparator;

    if (Type::SEQ.is_type_of(*args[0]))
    {
      seq_arg = args[0].get();
      comparator = &args[1]->exec();
    }
    else
    {
      seq_arg = args[1].get();
      comparator = &args[0]->exec();
    }

    Lisple::sptr_rtval_v elements = Lisple::get_children(*seq_arg);

    if (elements.size() > 1)
    {
      sptr_rtval_v cmp_args{nullptr, nullptr};
      std::sort(elements.begin(),
                elements.end(),
                [&](const sptr_rtval& a, const sptr_rtval& b)
                {
                  cmp_args[0] = a;
                  cmp_args[1] = b;
                  return Lisple::is_truthy(*comparator->execute(ctx, cmp_args));
                });
    }

    return RTValue::vector(std::move(elements));
  }

} // namespace Lisple
