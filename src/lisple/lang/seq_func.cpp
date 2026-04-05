
#include "lisple/host/object.h"

#include <algorithm>

#include <lisple/host.h>
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
            SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                 EXEC_DISPATCH(&FilterFunction::exec_filter))))

  EXEC_BODY(FilterFunction, exec_filter)
  {
    auto original = args[0];
    if (*Constant::NIL == *original) return RTValue::vector({});

    sptr_rtval_v result;
    result.reserve(count(*original));

    auto& filter_fn = args.back()->exec();

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

    return RTValue::vector(result);
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

    auto& filter_fn = args.back();

    ExecNode enode(CallNode(std::make_unique<ExecNode>(filter_fn), {}));
    std::vector<uptr_exec_node>& arg_nodes = std::get<CallNode>(enode.data).args;
    arg_nodes.push_back(nullptr);

    sptr_rtval_v children = Lisple::get_children(*args[0]);

    for (size_t i = 0; i < children.size(); i++)
    {
      auto item = std::make_unique<ExecNode>(children[i]);
      arg_nodes[0].swap(item);
      if (Lisple::is_truthy(*exec(ctx, enode)))
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

    return RTValue::vector(result);
  }

  /** MapFunction - map */
  FUNC_IMPL(MapFunction,
            SIG((FN_ARGS((&VARARG, &Type::SEQ), (&Type::EXEC)),
                 EXEC_DISPATCH(&MapFunction::exec_map))))

  EXEC_BODY(MapFunction, exec_map)
  {
    auto& map_fn = args.back();
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
    bool is_kw = map_fn->type == RTValue::Type::KEYWORD;

    std::vector<uptr_exec_node> NULLARGS;
    uptr_exec_node NULLTARGET;

    ExecNode enode = (is_kw ? ExecNode(KeyLookupNode(map_fn, nullptr))
                            : ExecNode(CallNode(std::make_unique<ExecNode>(map_fn), {})));

    uptr_exec_node& kw_target =
      is_kw ? std::get<KeyLookupNode>(enode.data).target : NULLTARGET;

    std::vector<uptr_exec_node>& arg_nodes =
      is_kw ? NULLARGS : std::get<CallNode>(enode.data).args;

    for (size_t seq_i = 0; seq_i < seqs.size(); seq_i++)
    {
      arg_nodes.push_back(nullptr);
    }

    bool valid;
    for (size_t i = 0; i < (*max_lmnts_it).size(); i++)
    {
      valid = true;
      for (size_t seq_i = 0; seq_i < seqs.size(); seq_i++)
      {
        if (i < seqs[seq_i].size())
        {
          auto p = std::make_unique<ExecNode>(seqs[seq_i].at(i));
          if (is_kw)
          {
            kw_target.swap(p);
          }
          else
          {
            arg_nodes[seq_i].swap(p);
          }
        }
        else
        {
          valid = false;
          break;
        }
      }

      if (valid)
      {
        result.push_back(exec(ctx, enode));
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
    auto& reducer = args.back();

    ExecNode enode(CallNode(std::make_unique<ExecNode>(reducer), {}));
    std::vector<uptr_exec_node>& arg_nodes = std::get<CallNode>(enode.data).args;
    arg_nodes.push_back(std::make_unique<ExecNode>(args[1]));
    arg_nodes.push_back(nullptr);

    sptr_rtval iter_result;
    for (auto& lmnt : children)
    {
      auto lmnt_node = std::make_unique<ExecNode>(lmnt);
      arg_nodes[1].swap(lmnt_node);
      iter_result = exec(ctx, enode);

      if (iter_result.get() != result.get())
      {
        result.swap(iter_result);
        auto acc_node = std::make_unique<ExecNode>(result);
        arg_nodes[0].swap(acc_node);
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

    return RTValue::vector(result);
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
                                 Lisple::sptr_sobject_v val_args{element};
                                 return remove_fn.execute(ctx, val_args)->is_truthy();
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

  /* RemoveFirstFunction - find-first */
  FUNC_IMPL(RemoveFirstFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::FUNCTION)),
                 EXEC_DISPATCH(&RemoveFirstFunction::exec_remove_first))))

  EXEC_BODY(RemoveFirstFunction, exec_remove_first)
  {
    auto original = args.back();

    sptr_rtval_v result;
    result.reserve(Lisple::count(*original));

    auto& remove_fn = args[0]->exec();

    bool removed = false;
    sptr_rtval_v val_args{nullptr};
    for (auto val : original->elements())
    {
      val_args[0] = val;
      auto test_result = remove_fn.execute(ctx, val_args);
      if (removed || !Lisple::is_truthy(*test_result))
      {
        result.push_back(val);
      }
      else
      {
        removed = true;
      }
    }

    return RTValue::vector(result);
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

  /* SomeFunction */
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
      if (!Lisple::is_truthy(*result))
      {
        return Constant::BOOL_TRUE;
      }
    }
    return Constant::BOOL_FALSE;
  }

  /* SortFunction - sort */
  FUNC_IMPL(SortFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::EXEC)),
                 EXEC_DISPATCH(&SortFunction::exec_sort))))

  EXEC_BODY(SortFunction, exec_sort)
  {
    Lisple::sptr_rtval_v elements = Lisple::get_children(*args[0]);
    if (elements.size() > 1)
    {
      Lisple::sptr_sobject fn_obj = args.back()->obj();
      Executable& comparator = fn_obj->as<Executable>();

      bool modified = false;
      Lisple::sptr_rtval tmp;
      Lisple::sptr_rtval_v sort_cell = {Constant::NIL, Constant::NIL};
      Lisple::sptr_rtval_v sort_cell_reverse = {Constant::NIL, Constant::NIL};
      do
      {
        modified = false;
        for (size_t i = 0; i < elements.size() - 1; i++)
        {
          sort_cell[0] = elements[i];
          sort_cell[1] = elements[i + 1];
          sort_cell_reverse[0] = elements[i + 1];
          sort_cell_reverse[1] = elements[i];

          if (Lisple::is_truthy(*comparator.execute(ctx, sort_cell)) &&
              !Lisple::is_truthy(*comparator.execute(ctx, sort_cell_reverse)))
          {
            tmp = elements[i];
            elements[i] = elements[i + 1];
            elements[i + 1] = tmp;
            modified = true;
          }
        }
      } while (modified);
    }

    return RTValue::vector(std::move(elements));
  }

} // namespace Lisple
