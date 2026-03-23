
#include "seq_func.h"

#include "../runtime/seq.h"
#include <algorithm>

namespace Lisple
{
  /* MapFunction - map */
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

} // namespace Lisple
