
#include "roo/host/object.h"

#include <algorithm>

#include <roo/lang/seq_func.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/node.h>
#include <roo/runtime/seq.h>
#include <roo/runtime/value.h>

namespace Roo
{
  /** AnyFunction - roo/any? */
  FUNC_IMPL(AnyFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                 EXEC_DISPATCH(&AnyFunction::exec_any))))

  EXEC_BODY(AnyFunction, exec_any)
  {
    sptr_val_v val_arg = {nullptr};
    auto& fn = args.back()->exec();
    sptr_val_v elements = Roo::get_children(*args[0]);
    for (auto& element : elements)
    {
      val_arg[0] = element;
      sptr_val result = fn.execute(ctx, val_arg);
      if (Roo::is_truthy(*result))
      {
        return Constant::BOOL_TRUE;
      }
    }
    return Constant::BOOL_FALSE;
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
    if (Type::SEQ_OR_STRING.is_type_of(*args[0]))
    {
      original = args[0];
      fn = args[1];
    }
    else
    {
      original = args[1];
      fn = args[0];
    }

    if (*Constant::NIL == *original) return Value::vector({});

    sptr_val_v result;
    result.reserve(count(*original));

    auto& filter_fn = fn->exec();

    sptr_val_v val_args{Constant::NIL};
    sptr_val_v elements = Roo::get_children(*original);
    for (auto val : elements)
    {
      val_args[0] = val;
      sptr_val pred_result = filter_fn.execute(ctx, val_args);
      if (Roo::is_truthy(*pred_result))
      {
        result.push_back(val);
      }
    }

    return Value::vector(std::move(result));
  }

  /** FindFirstFunction - roo/find-first */
  FUNC_IMPL(FindFirstFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&FindFirstFunction::exec_find_first))))

  EXEC_BODY(FindFirstFunction, exec_find_first)
  {
    auto original = args[0];

    auto& filter_fn = args.back()->exec();

    sptr_val_v val_args{nullptr};
    sptr_val_v children = Roo::get_children(*original);
    for (auto val : children)
    {
      val_args[0] = val;
      sptr_val pred_result = filter_fn.execute(ctx, val_args);
      if (Roo::is_truthy(*pred_result))
      {
        return val;
      }
    }

    return Constant::NIL;
  }

  /** FindIndexFunction - roo/find-index */
  FUNC_IMPL(FindIndexFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&FindIndexFunction::exec_find_index))))

  EXEC_BODY(FindIndexFunction, exec_find_index)
  {
    if (*args[0] == *Constant::NIL) return Constant::NIL;

    auto& filter_fn = args.back()->exec();

    sptr_val_v children = Roo::get_children(*args[0]);

    sptr_val_v val_args{nullptr};
    for (size_t i = 0; i < children.size(); i++)
    {
      val_args[0] = children[i];
      auto item = std::make_unique<ExecNode>(children[i]);
      if (Roo::is_truthy(*filter_fn.execute(ctx, val_args)))
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
    if (Type::SEQ_OR_STRING.is_type_of(*args[0]))
    {
      original = args[0];
      fn = args[1];
    }
    else
    {
      original = args[1];
      fn = args[0];
    }

    // FIXME: is_kw flag MESS until keylookupnode vs callnode gets symmetrical
    bool is_kw = fn->type == Value::Type::KEYWORD;
    Executable* map_fn = is_kw ? nullptr : &fn->exec();

    sptr_val_v result;
    sptr_val_v elements = Roo::get_children(*original);
    sptr_val_v map_args{nullptr};
    for (auto& element : elements)
    {
      sptr_val mapped;
      if (is_kw)
      {
        mapped = Dict::get_property(element, fn);
      }
      else
      {
        map_args[0] = element;
        mapped = map_fn->execute(ctx, map_args);
      }

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
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                 EXEC_DISPATCH(&KeepFunction::exec_keep))))

  EXEC_BODY(KeepFunction, exec_keep)
  {
    sptr_val_v values = Roo::get_children(*args[0]);
    Executable& exec = args[1]->exec();

    sptr_val_v result;

    sptr_val_v arg{nullptr};
    for (auto& v : values)
    {
      arg[0] = v;
      sptr_val r = exec.execute(ctx, arg);
      if (*r != *Constant::NIL) result.push_back(r);
    }

    return Value::vector(std::move(result));
  }

  /** MapFunction - roo/map */
  FUNC_IMPL(MapFunction,
            SIG((FN_ARGS((&VARARG, &Type::SEQ_OR_STRING), (&Type::EXEC)),
                 EXEC_DISPATCH(&MapFunction::exec_map))))

  EXEC_BODY(MapFunction, exec_map)
  {
    auto& mapper = args.back();
    sptr_val_v result;

    std::vector<sptr_val_v> seqs;

    for (size_t i = 0; i < args.size() - 1; i++)
    {
      seqs.push_back(Roo::get_children(*args[i]));
    }

    auto max_lmnts_it =
      std::max_element(seqs.begin(),
                       seqs.end(),
                       [](const auto& a, const auto& b) { return a.size() < b.size(); });

    result.reserve((*max_lmnts_it).size());

    // FIXME: is_kw flag MESS until keylookupnode vs callnode gets symmetrical
    bool is_kw = mapper->type == Value::Type::KEYWORD;
    Executable* map_fn = is_kw ? nullptr : &mapper->exec();

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
        if (is_kw)
        {
          if (seqs.size() == 1)
          {
            result.push_back(Dict::get_property(map_args[0], mapper));
          }
          else
          {
            sptr_val_v unit;
            for (size_t seq_i = 0; seq_i < seqs.size(); seq_i++)
            {
              unit.push_back(Dict::get_property(map_args[seq_i], mapper));
            }
            result.push_back(Value::vector(std::move(unit)));
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

    return Value::vector(std::move(result));
  }

  /** ReduceFunction - roo/reduce */
  FUNC_IMPL(ReduceFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::ANY), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&ReduceFunction::exec_reduce))))

  EXEC_BODY(ReduceFunction, exec_reduce)
  {
    sptr_val_v children = Roo::get_children(*args[0]);
    sptr_val result = args[1];
    auto& reducer = args.back()->exec();

    sptr_val iter_result;
    sptr_val_v reduce_args{result, nullptr};
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

  /** RemoveFunction - roo/remove */
  FUNC_IMPL(RemoveFunction,
            MULTI_SIG((FN_ARGS((&Type::EXEC), (&Type::SEQ_OR_STRING)),
                       EXEC_DISPATCH(&RemoveFunction::exec_remove)),
                      (FN_ARGS((&Type::SEQ_OR_STRING), (&Type::EXEC)),
                       EXEC_DISPATCH(&RemoveFunction::exec_remove))))

  EXEC_BODY(RemoveFunction, exec_remove)
  {
    Value* original;
    Executable* remove_fn;

    if (Type::SEQ_OR_STRING.is_type_of(*args[0]))
    {
      original = args[0].get();
      remove_fn = &args[1]->exec();
    }
    else
    {
      original = args[1].get();
      remove_fn = &args[0]->exec();
    }

    sptr_val_v result;
    result.reserve(Roo::count(*original));
    sptr_val_v elements = Roo::get_children(*original);
    Roo::sptr_val_v val_args{nullptr};
    for (auto val : elements)
    {
      val_args[0] = val;
      auto pred_result = remove_fn->execute(ctx, val_args);
      if (!Roo::is_truthy(*pred_result))
      {
        result.push_back(val);
      }
    }

    return Value::vector(std::move(result));
  }

  /** RemoveBangFunction - roo/remove! */
  FUNC_IMPL(RemoveBangFunction,
            SIG((FN_ARGS((&Type::EXEC), (&Type::STRICT_SEQ)),
                 EXEC_DISPATCH(&RemoveBangFunction::exec_remove_bang))))

  EXEC_BODY(RemoveBangFunction, exec_remove_bang)
  {
    auto& remove_fn = args[0]->exec();

    if (args[1]->type == Value::Type::OBJECT && Type::HOST_SEQ.is_type_of(*args[1]))
    {
      sptr_ast_node obj = args[1]->obj();
      AST::Seq& host_seq = obj->as<AST::Seq>();
      sptr_ast_node_v& children = host_seq.get_children();
      auto it = std::remove_if(children.begin(),
                               children.end(),
                               [&](const Roo::sptr_ast_node& element)
                               {
                                 Roo::sptr_ast_node ast_element = element;
                                 Roo::sptr_val_v val_args{to_rt_value(ast_element)};
                                 return Roo::is_truthy(*remove_fn.execute(ctx, val_args));
                               });
      children.erase(it, children.end());

      host_seq.replace_children(children);
    }
    else if (args[1]->type == Value::Type::NATIVE_OBJECT)
    {
      throw TypeError("remove! not implemented for native host sequences.");
    }
    else
    {
      sptr_val_v& children = std::get<sptr_val_v>(args[1]->value);

      auto it = std::remove_if(children.begin(),
                               children.end(),
                               [&](const Roo::sptr_val& element)
                               {
                                 Roo::sptr_val_v val_args{element};
                                 auto pred_result = remove_fn.execute(ctx, val_args);
                                 return Roo::is_truthy(*pred_result);
                               });

      children.erase(it, children.end());
    }

    return args.back();
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
    Executable* remove_fn;

    if (Type::SEQ_OR_STRING.is_type_of(*args[0]))
    {
      original = args[0].get();
      remove_fn = &args[1]->exec();
    }
    else
    {
      original = args[1].get();
      remove_fn = &args[0]->exec();
    }

    sptr_val_v result;
    result.reserve(Roo::count(*original));

    bool removed = false;
    sptr_val_v val_args{nullptr};
    for (auto val : Roo::get_children(*original))
    {
      val_args[0] = val;
      auto test_result = remove_fn->execute(ctx, val_args);
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
    Executable* comparator;

    if (Type::SEQ_OR_STRING.is_type_of(*args[0]))
    {
      seq_arg = args[0].get();
      comparator = &args[1]->exec();
    }
    else
    {
      seq_arg = args[1].get();
      comparator = &args[0]->exec();
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
                  return Roo::is_truthy(*comparator->execute(ctx, cmp_args));
                });
    }

    return Value::vector(std::move(elements));
  }

} // namespace Roo
