
#include <algorithm>

#include <lisple/lang/seq_func.h>
#include <lisple/runtime/seq.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
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
    ExecNode enode(CallNode(std::make_unique<ExecNode>(map_fn), {}));
    std::vector<uptr_exec_node>& arg_nodes = std::get<CallNode>(enode.data).args;
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
          arg_nodes[seq_i].swap(p);
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
