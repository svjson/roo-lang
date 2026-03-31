
#include <cmath>

#include <lisple/lang/seq.h>
#include <lisple/runtime/seq.h>

namespace Lisple
{
  /* ConcatFunction - concat */
  FUNC_IMPL(ConcatFunction,
            SIG((FN_ARGS((&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&ConcatFunction::exec_concat))))

  EXEC_BODY(ConcatFunction, exec_concat)
  {
    sptr_rtval_v result;

    for (auto& vec : args)
    {
      if (vec->type == RTValue::Type::NIL)
      {
        result.push_back(Constant::NIL);
      }
      else if (vec->type != RTValue::Type::MAP && Type::SEQ.is_type_of(*vec))
      {
        if (vec->type == RTValue::Type::OBJECT &&
            std::get<sptr_sobject>(vec->value)->get_type() != Form::HOST_SEQ)
        {
          result.push_back(vec);
        }
        else
        {
          sptr_rtval_v elements = Lisple::get_children(*vec);
          for (auto& element : elements)
          {
            result.push_back(element);
          }
        }
      }
      else
      {
        result.push_back(vec);
      }
    }

    return RTValue::vector(std::move(result));
  }

  /* CountFunction - count */
  FUNC_IMPL(CountFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&CountFunction::exec_count))));

  EXEC_BODY(CountFunction, exec_count)
  {
    return RTValue::number((int)Lisple::count(*args[0]));
  }

  FUNC_IMPL(NthFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::NUMBER)),
                 EXEC_DISPATCH(&NthFunction::exec_nth))))

  EXEC_BODY(NthFunction, exec_nth)
  {
    int n = std::get<RTValue::Number>(args.back()->value).get_int();
    if (n >= static_cast<int>(Lisple::count(*args.front())) || n < 0)
    {
      return Constant::NIL;
    }

    sptr_rtval_v children = Lisple::get_children(*args.front());

    return children[n];
  }

  /* HeadFunction */
  FUNC_IMPL(HeadFunction,
            SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&HeadFunction::exec_head))))

  EXEC_BODY(HeadFunction, exec_head)
  {
    if (args[0]->type == RTValue::Type::NIL) return Constant::NIL;
    return Lisple::get_child(*args[0], 0);
  }

  /* RandNth - rand-nth */
  FUNC_IMPL(RandNthFunction,
            SIG((FN_ARGS((&Lisple::Type::SEQ)),
                 EXEC_DISPATCH(&RandNthFunction::exec_rand_nth))))

  EXEC_BODY(RandNthFunction, exec_rand_nth)
  {
    sptr_rtval_v elements = Lisple::get_children(*args[0]);
    if (elements.empty())
    {
      return Constant::NIL;
    }
    return elements[std::rand() % (elements.size())];
  }

  /** RangeFunction - range */
  FUNC_IMPL(RangeFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&RangeFunction::exec_range))))

  sptr_rtval box_number(float num)
  {
    if (floorf(num) || num == 0.0)
    {
      return RTValue::number(static_cast<int>(num));
    }
    return RTValue::number(num);
  }

  EXEC_BODY(RangeFunction, exec_range)
  {
    sptr_rtval_v result;

    if (args[0]->type == RTValue::Type::NUMBER && args[1]->type == RTValue::Type::NUMBER)
    {
      RTValue::Number& begin_num = std::get<RTValue::Number>(args[0]->value);
      RTValue::Number& end_num = std::get<RTValue::Number>(args[1]->value);

      if (begin_num.num_type == RTValue::NumberType::INT)
      {
        int begin = begin_num.int_value;
        int end = end_num.get_int();

        result.reserve(std::abs(end - begin));

        for (int i = begin; begin < end ? i <= end : i >= end; begin < end ? i++ : i--)
        {
          result.push_back(RTValue::number(i));
        }
      }
      else
      {
        float begin = begin_num.get_float();
        float end = end_num.get_float();

        for (float i = begin; begin < end ? i <= end : i >= end; begin < end ? i++ : i--)
        {
          result.push_back(box_number(i));
        }
      }
    }
    return RTValue::vector(std::move(result));
  }

  /* RemoveNthFunction - remove-nth! */
  FUNC_IMPL(RemoveNthFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::NUMBER)),
                 EXEC_DISPATCH(&RemoveNthFunction::exec_remove_nth))))

  EXEC_BODY(RemoveNthFunction, exec_remove_nth)
  {
    if (*args[0] == *Constant::NIL) return Constant::NIL;

    int n = std::get<RTValue::Number>(args.back()->value).get_int();
    if (n >= static_cast<int>(Lisple::count(*args.back())) || n < 0)
    {
      return args[0];
    }

    sptr_rtval_v elements = Lisple::get_children(*args[0]);
    sptr_rtval_v new_seq;
    new_seq.reserve(elements.size());

    for (size_t i = 0; i < elements.size(); i++)
    {
      if (static_cast<int>(i) != n)
      {
        new_seq.push_back(elements[i]);
      }
    }

    return RTValue::vector(std::move(new_seq));
  }

} // namespace Lisple
