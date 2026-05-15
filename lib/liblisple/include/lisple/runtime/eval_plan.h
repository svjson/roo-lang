
#ifndef LISPLE__EVAL_PLAN_H
#define LISPLE__EVAL_PLAN_H

#include <cstdint>
#include <vector>

#include <lisple/context.h>
#include <lisple/runtime/node.h>

namespace Lisple
{
  enum class eval_mode : uint8_t
  {
    DEFAULT,
    POSTPONE,
    LITERAL
  };

  class EvalMode
  {
   public:
    virtual ~EvalMode() = default;
    bool nested = false;
  };

  class SingleEvalMode : public EvalMode
  {
   public:
    explicit SingleEvalMode(eval_mode m)
      : mode(m)
    {
    }
    eval_mode mode;
  };

  class EvalPattern : public EvalMode
  {
   public:
    explicit EvalPattern(const std::vector<const EvalMode*>& pattern, bool repeat = false)
      : pattern(pattern)
      , repeat(repeat)
    {
      this->nested = true;
    }

    const EvalMode* get(size_t index) const;

    const std::vector<const EvalMode*> pattern;
    bool repeat;
  };

  namespace Eval
  {
    inline const SingleEvalMode DEFAULT{eval_mode::DEFAULT};
    inline const SingleEvalMode POSTPONE{eval_mode::POSTPONE};
    inline const SingleEvalMode LITERAL{eval_mode::LITERAL};
    inline const EvalPattern BIND_SYM_VAL{{&Eval::LITERAL, &Eval::DEFAULT}};
    inline const EvalPattern REPEAT_BIND_SYM_VAL{{&Eval::LITERAL, &Eval::DEFAULT}, true};
    inline const EvalPattern REPEAT_LAZY_BIND_SYM_VAL{{&Eval::LITERAL, &Eval::POSTPONE},
                                                      true};
  } // namespace Eval

  uptr_exec_node_v extract_sequence([[maybe_unused]] ExecNode* node);

  void prepare_sequence(Context& ctx,
                        const EvalPattern& pattern,
                        const uptr_exec_node_v& input,
                        std::vector<std::unique_ptr<ExecNode>>& storage,
                        std::vector<ExecNode*>& output,
                        std::string indent = "");

} // namespace Lisple

#endif /* LISPLE__EVAL_PLAN_H */
