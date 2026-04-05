
#include "lisple/runtime/value.h"
#include "lisple/type.h"

#include <cmath>

#include <lisple/lang/seq.h>
#include <lisple/runtime/seq.h>

namespace Lisple
{
  /* AppendBangFunction - append_bang */
  FUNC_IMPL(AppendBangFunction,
            SIG((FN_ARGS((&Type::SEQ), (&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&AppendBangFunction::exec_append_bang))))

  EXEC_BODY(AppendBangFunction, exec_append_bang)
  {
    if (auto* elements = std::get_if<sptr_rtval_v>(&args[0]->value))
    {
      for (size_t i = 1; i < args.size(); i++)
      {
        elements->push_back(args[i]);
      }
    }
    else
    {
      throw TypeError("append! not implemented for value type: " +
                      std::to_string((int)args[0]->type));
    }

    return args[0];
  }

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

  /* ConcatBangFunction - concat */
  FUNC_IMPL(ConcatBangFunction,
            SIG((FN_ARGS((&Type::SEQ), (&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&ConcatBangFunction::exec_concat_bang))))

  EXEC_BODY(ConcatBangFunction, exec_concat_bang)
  {
    sptr_rtval_v& result = std::get<sptr_rtval_v>(args[0]->value);

    for (size_t i = 1; i < args.size(); i++)
    {
      auto& vec = args[i];
      if ((Type::ARRAY.is_type_of(*vec) || Type::LIST.is_type_of(*vec)) &&
          *vec != *Constant::NIL)
      {
        for (auto& element : vec->elements())
        {
          result.push_back(element);
        }
      }
      else
      {
        result.push_back(vec);
      }
    }

    return args[0];
  }

  /** CountFunction - count */
  FUNC_IMPL(CountFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&CountFunction::exec_count))));

  EXEC_BODY(CountFunction, exec_count)
  {
    return RTValue::number((int)Lisple::count(*args[0]));
  }

  /** NthFunction - nth */
  FUNC_IMPL(NthFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::NUMBER)),
                 EXEC_DISPATCH(&NthFunction::exec_nth))))

  EXEC_BODY(NthFunction, exec_nth)
  {
    int n = std::get<const RTValue::Number>(args.back()->value).get_int();
    if (n >= static_cast<int>(Lisple::count(*args.front())) || n < 0)
    {
      return Constant::NIL;
    }

    return Lisple::get_child(*args.front(), n);
  }

  /** HeadFunction */
  FUNC_IMPL(HeadFunction,
            SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&HeadFunction::exec_head))))

  EXEC_BODY(HeadFunction, exec_head)
  {
    if (args[0]->type == RTValue::Type::NIL) return Constant::NIL;
    return Lisple::get_child(*args[0], 0);
  }

  /** LastFunction */
  FUNC_IMPL(LastFunction,
            SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&LastFunction::exec_last))))

  EXEC_BODY(LastFunction, exec_last)
  {
    if (args[0]->type == RTValue::Type::NIL) return Constant::NIL;
    return Lisple::peek_child(*args[0]);
  }

  /** RandNth - rand-nth */
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
      const RTValue::Number& begin_num = std::get<const RTValue::Number>(args[0]->value);
      const RTValue::Number& end_num = std::get<const RTValue::Number>(args[1]->value);

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

  /** RemoveNthFunction - remove-nth! */
  FUNC_IMPL(RemoveNthFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::NUMBER)),
                 EXEC_DISPATCH(&RemoveNthFunction::exec_remove_nth))))

  EXEC_BODY(RemoveNthFunction, exec_remove_nth)
  {
    if (*args[0] == *Constant::NIL) return Constant::NIL;

    int n = std::get<const RTValue::Number>(args.back()->value).get_int();
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

  /** RemoveNthBangFunction - remove-nth! */
  FUNC_IMPL(RemoveNthBangFunction,
            SIG((FN_ARGS((&Type::SEQ), (&Type::NUMBER)),
                 EXEC_DISPATCH(&RemoveNthBangFunction::exec_remove_nth_bang))))

  EXEC_BODY(RemoveNthBangFunction, exec_remove_nth_bang)
  {
    auto& seq = *args[0];
    int n = std::get<const RTValue::Number>(args[1]->value).get_int();

    if (Type::HOST_SEQ.is_type_of(seq))
    {
      auto obj = seq.obj();
      sptr_sobject_v& children = obj->get_children();

      if (n < 0 || n >= static_cast<int>(children.size()))
      {
        return Lisple::Constant::NIL;
      }

      sptr_sobject to_delete = children[n];

      children.erase(children.begin() + n);

      obj->as<Seq>().replace_children(children);

      return to_rt_value(to_delete);
    }

    sptr_rtval_v& children = std::get<sptr_rtval_v>(seq.value);
    sptr_rtval to_delete = children.at(n);

    children.erase(children.begin() + n);

    return to_delete;
  }

  /** TailFunction - tail */
  FUNC_IMPL(TailFunction,
            SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&TailFunction::exec_tail))))

  EXEC_BODY(TailFunction, exec_tail)
  {
    sptr_rtval_v tail;
    size_t count = Lisple::count(*args[0]);
    if (count > 1)
    {
      tail.reserve(count - 1);
      if (Type::HOST_SEQ.is_type_of(*args[0]))
      {
        auto obj_children = args[0]->obj()->get_children();
        for (size_t i = 1; i < obj_children.size(); i++)
        {
          tail.push_back(to_rt_value(*obj_children[i]));
        }
      }
      else
      {
        auto& children = args[0]->elements();
        for (size_t i = 1; i < children.size(); i++)
        {
          tail.push_back(children[i]);
        }
      }
    }

    return RTValue::vector(std::move(tail));
  }

  /* FlattenFunction - flatten */
  FUNC_IMPL(FlattenFunction,
            SIG((FN_ARGS((&Type::SEQ)), EXEC_DISPATCH(&FlattenFunction::exec_flatten))))

  EXEC_BODY(FlattenFunction, exec_flatten)
  {
    sptr_rtval_v result;

    sptr_rtval_v children = Lisple::get_children(*args[0]);
    for (auto& obj : children)
    {
      if (Type::HOST_SEQ.is_type_of(*obj))
      {
        for (auto& cobj : obj->obj()->get_children())
        {
          if (Type::SEQ.is_type_of(*cobj))
          {
            auto flat_args = sptr_rtval_v{to_rt_value(*cobj)};
            auto flattened = exec_flatten(ctx, flat_args);
            for (auto fl_obj : flattened->elements())
            {
              result.push_back(fl_obj);
            }
          }
          else
          {
            result.push_back(to_rt_value(*cobj));
          }
        }
      }
      else if (Type::SEQ.is_type_of(*obj))
      {
        auto flat_args = sptr_rtval_v{obj};
        auto flattened = exec_flatten(ctx, flat_args);
        for (auto& fl_obj : flattened->elements())
        {
          result.push_back(fl_obj);
        }
      }
      else
      {
        result.push_back(obj);
      }
    }

    return RTValue::vector(std::move(result));
  }

  /*
   * TakeFunction - take
   */
  FUNC_IMPL(TakeFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::SEQ)),
                 EXEC_DISPATCH(&TakeFunction::exec_take))))

  EXEC_BODY(TakeFunction, exec_take)
  {
    size_t amount = std::get<const RTValue::Number>(args[0]->value).get_int();
    sptr_rtval_v result;

    if (Type::HOST_SEQ.is_type_of(*args[1]))
    {
      sptr_sobject_v vector = args.back()->obj()->get_children();

      result.reserve(vector.size());
      for (size_t i = 0; i < std::min(amount, vector.size()); i++)
      {
        result.push_back(to_rt_value(*vector[i]));
      }
    }
    else
    {
      sptr_rtval_v elements = args.back()->elements();
      result.reserve(elements.size());
      for (size_t i = 0; i < std::min(amount, elements.size()); i++)
      {
        result.push_back(elements[i]);
      }
    }

    return RTValue::vector(std::move(result));
  }

} // namespace Lisple
