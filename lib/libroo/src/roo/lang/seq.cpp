
#include "roo/runtime/value.h"
#include "roo/type.h"

#include <algorithm>
#include <cmath>

#include <roo/lang/seq.h>
#include <roo/runtime/seq.h>

namespace Roo
{
  namespace
  {
    template <typename Fn> void for_each_child(Value& seq, Fn&& fn)
    {
      if (Roo::has_indexed_children(seq))
      {
        const size_t n_children = Roo::child_count(seq);
        for (size_t i = 0; i < n_children; i++)
        {
          fn(Roo::get_child(seq, i));
        }
        return;
      }

      sptr_val_v children = Roo::get_children(seq);
      for (auto& child : children)
      {
        fn(child);
      }
    }

  } // namespace

  /** AppendBangFunction - roo/append! */
  FUNC_IMPL(AppendBangFunction,
            SIG((FN_ARGS((&Type::STRICT_SEQ), (&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&AppendBangFunction::exec_append_bang))))

  EXEC_BODY(AppendBangFunction, exec_append_bang)
  {
    if (auto* elements = std::get_if<sptr_val_v>(&args[0]->value))
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

  /** ConcatFunction - roo/concat */
  FUNC_IMPL(ConcatFunction,
            SIG((FN_ARGS((&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&ConcatFunction::exec_concat))))

  EXEC_BODY(ConcatFunction, exec_concat)
  {
    sptr_val_v result;

    for (auto& vec : args)
    {
      if (vec->type == Value::Type::NIL)
      {
        result.push_back(Constant::NIL);
      }
      else if (vec->type != Value::Type::MAP && Type::SEQ.is_type_of(*vec))
      {
        if (vec->type == Value::Type::OBJECT &&
            std::get<sptr_ast_node>(vec->value)->get_type() != Form::HOST_SEQ)
        {
          result.push_back(vec);
        }
        else
        {
          for_each_child(*vec, [&](const sptr_val& element) { result.push_back(element); });
        }
      }
      else
      {
        result.push_back(vec);
      }
    }

    return Value::vector(std::move(result));
  }

  /** ConcatBangFunction - roo/concat! */
  FUNC_IMPL(ConcatBangFunction,
            SIG((FN_ARGS((&Type::STRICT_SEQ), (&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&ConcatBangFunction::exec_concat_bang))))

  EXEC_BODY(ConcatBangFunction, exec_concat_bang)
  {
    auto* result = std::get_if<sptr_val_v>(&args[0]->value);
    if (result == nullptr)
    {
      throw TypeError("concat! not implemented for value type: " +
                      std::to_string((int)args[0]->type));
    }

    for (size_t i = 1; i < args.size(); i++)
    {
      auto& vec = args[i];
      if ((Type::VECTOR.is_type_of(*vec) || Type::LIST.is_type_of(*vec)) &&
          *vec != *Constant::NIL)
      {
        for (auto& element : vec->elements())
        {
          result->push_back(element);
        }
      }
      else
      {
        result->push_back(vec);
      }
    }

    return args[0];
  }

  /** ContainsPFunction - roo/contains? */
  FUNC_IMPL(ContainsPFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::ANY)),
                 EXEC_DISPATCH(&ContainsPFunction::exec_contains))))

  EXEC_BODY(ContainsPFunction, exec_contains)
  {
    if (*Constant::NIL == *args[0]) return Constant::BOOL_FALSE;

    if (Roo::has_indexed_children(*args[0]))
    {
      const size_t n_children = Roo::child_count(*args[0]);
      for (size_t i = 0; i < n_children; i++)
      {
        if (*Roo::get_child(*args[0], i) == *args.back()) return Constant::BOOL_TRUE;
      }
      return Constant::BOOL_FALSE;
    }

    sptr_val_v vector = Roo::get_children(*args[0]);
    return std::find_if(vector.begin(),
                        vector.end(),
                        [&args](const sptr_val& lmnt)
                        { return *lmnt == *args.back(); }) != vector.end()
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  /** ContainsAnyPFunction - roo/contains-any? */
  FUNC_IMPL(ContainsAnyPFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&ContainsAnyPFunction::exec_contains_any))))

  EXEC_BODY(ContainsAnyPFunction, exec_contains_any)
  {
    sptr_val_v haystack;
    if (*Constant::NIL != *args[0])
    {
      if (Roo::has_indexed_children(*args[0]))
      {
        const size_t n_children = Roo::child_count(*args[0]);
        haystack.reserve(n_children);
        for (size_t i = 0; i < n_children; i++)
        {
          haystack.push_back(Roo::get_child(*args[0], i));
        }
      }
      else
      {
        haystack = Roo::get_children(*args[0]);
      }
    }

    sptr_val_v needles;
    if (*Constant::NIL != *args[1])
    {
      if (Roo::has_indexed_children(*args[1]))
      {
        const size_t n_children = Roo::child_count(*args[1]);
        needles.reserve(n_children);
        for (size_t i = 0; i < n_children; i++)
        {
          needles.push_back(Roo::get_child(*args[1], i));
        }
      }
      else
      {
        needles = Roo::get_children(*args[1]);
      }
    }

    for (const sptr_val& needle : needles)
    {
      for (const sptr_val& value : haystack)
      {
        if (*value == *needle) return Constant::BOOL_TRUE;
      }
    }

    return Constant::BOOL_FALSE;
  }

  /** ContainsAllPFunction - roo/contains-all? */
  FUNC_IMPL(ContainsAllPFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&ContainsAllPFunction::exec_contains_all))))

  EXEC_BODY(ContainsAllPFunction, exec_contains_all)
  {
    sptr_val_v haystack;
    if (*Constant::NIL != *args[0])
    {
      if (Roo::has_indexed_children(*args[0]))
      {
        const size_t n_children = Roo::child_count(*args[0]);
        haystack.reserve(n_children);
        for (size_t i = 0; i < n_children; i++)
        {
          haystack.push_back(Roo::get_child(*args[0], i));
        }
      }
      else
      {
        haystack = Roo::get_children(*args[0]);
      }
    }

    sptr_val_v needles;
    if (*Constant::NIL != *args[1])
    {
      if (Roo::has_indexed_children(*args[1]))
      {
        const size_t n_children = Roo::child_count(*args[1]);
        needles.reserve(n_children);
        for (size_t i = 0; i < n_children; i++)
        {
          needles.push_back(Roo::get_child(*args[1], i));
        }
      }
      else
      {
        needles = Roo::get_children(*args[1]);
      }
    }

    for (const sptr_val& needle : needles)
    {
      bool found = false;
      for (const sptr_val& value : haystack)
      {
        if (*value == *needle)
        {
          found = true;
          break;
        }
      }

      if (!found) return Constant::BOOL_FALSE;
    }

    return Constant::BOOL_TRUE;
  }

  /** CountFunction - roo/count */
  FUNC_IMPL(CountFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&CountFunction::exec_count))));

  EXEC_BODY(CountFunction, exec_count)
  {
    return Value::number((int)Roo::count(*args[0]));
  }

  /** NthFunction - roo/nth */
  FUNC_IMPL(NthFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::NUMBER)),
                 EXEC_DISPATCH(&NthFunction::exec_nth))))

  EXEC_BODY(NthFunction, exec_nth)
  {
    if (args.back()->type != Value::Type::NUMBER)
    {
      throw TypeError("nth index must be a number.");
    }

    const Value::Number& index = args.back()->num();
    if (index.num_type == Value::NumberType::FLOAT)
    {
      throw TypeError("nth index must be an integer.");
    }

    int n = index.get_int();
    if (n < 0)
    {
      return Constant::NIL;
    }

    return Roo::get_child(*args.front(), static_cast<size_t>(n));
  }

  /** PartitionFunction - roo/partition */
  FUNC_IMPL(PartitionFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&PartitionFunction::exec_partition))))

  EXEC_BODY(PartitionFunction, exec_partition)
  {
    size_t part_size = static_cast<size_t>(args[0]->i32());
    sptr_val_v result;

    sptr_val_v part;
    for_each_child(*args[1],
                   [&](const sptr_val& child)
                   {
                     part.push_back(child);
                     if (part.size() == part_size)
                     {
                       result.push_back(Value::vector(std::move(part)));
                       part = sptr_val_v{};
                     }
                   });
    if (!part.empty())
    {
      result.push_back(Value::vector(std::move(part)));
    }

    return Value::vector(std::move(result));
  }

  /** HeadFunction - roo/head */
  FUNC_IMPL(HeadFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING)), EXEC_DISPATCH(&HeadFunction::exec_head))))

  EXEC_BODY(HeadFunction, exec_head)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::NIL;
    return Roo::get_child(*args[0], 0);
  }

  /** LastFunction - roo/last */
  FUNC_IMPL(LastFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING)), EXEC_DISPATCH(&LastFunction::exec_last))))

  EXEC_BODY(LastFunction, exec_last)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::NIL;
    return Roo::peek_child(*args[0]);
  }

  /** RandNthFunction - roo/rand-nth */
  FUNC_IMPL(RandNthFunction,
            SIG((FN_ARGS((&Roo::Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&RandNthFunction::exec_rand_nth))))

  EXEC_BODY(RandNthFunction, exec_rand_nth)
  {
    if (Roo::has_indexed_children(*args[0]))
    {
      const size_t n_children = Roo::child_count(*args[0]);
      if (n_children == 0)
      {
        return Constant::NIL;
      }
      return Roo::get_child(*args[0], ctx.random_int(0, static_cast<int>(n_children)));
    }

    sptr_val_v elements = Roo::get_children(*args[0]);
    if (elements.empty())
    {
      return Constant::NIL;
    }
    return elements[ctx.random_int(0, static_cast<int>(elements.size()))];
  }

  /** RangeFunction - roo/range */
  FUNC_IMPL(RangeFunction,
            MULTI_SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&RangeFunction::exec_range)),
                      (FN_ARGS((&Type::NUMBER), (&Type::NUMBER)),
                       EXEC_DISPATCH(&RangeFunction::exec_range))))

  EXEC_BODY(RangeFunction, exec_range)
  {
    int begin = args.size() == 1 ? 0 : args[0]->i32();
    int end = args.back()->i32();

    sptr_val_v result;
    result.reserve(std::abs(end - begin) + 1);

    for (int i = begin; begin < end ? i <= end : i >= end; begin < end ? i++ : i--)
    {
      result.push_back(Value::number(i));
    }

    return Value::vector(std::move(result));
  }

  /** RemoveNthFunction - roo/remove-nth */
  FUNC_IMPL(RemoveNthFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING), (&Type::NUMBER)),
                 EXEC_DISPATCH(&RemoveNthFunction::exec_remove_nth))))

  EXEC_BODY(RemoveNthFunction, exec_remove_nth)
  {
    if (*args[0] == *Constant::NIL) return Constant::NIL;

    int n = std::get<const Value::Number>(args.back()->value).get_int();
    sptr_val_v elements = Roo::get_children(*args[0]);
    if (n >= static_cast<int>(elements.size()) || n < 0)
    {
      return args[0];
    }

    sptr_val_v new_seq;
    new_seq.reserve(elements.size());

    for (size_t i = 0; i < elements.size(); i++)
    {
      if (static_cast<int>(i) != n)
      {
        new_seq.push_back(elements[i]);
      }
    }

    return Value::vector(std::move(new_seq));
  }

  /** RemoveNthBangFunction - roo/remove-nth! */
  FUNC_IMPL(RemoveNthBangFunction,
            SIG((FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER)),
                 EXEC_DISPATCH(&RemoveNthBangFunction::exec_remove_nth_bang))))

  EXEC_BODY(RemoveNthBangFunction, exec_remove_nth_bang)
  {
    auto& seq = *args[0];
    int n = std::get<const Value::Number>(args[1]->value).get_int();

    if (seq.type == Value::Type::OBJECT && Type::HOST_SEQ.is_type_of(seq))
    {
      auto obj = seq.obj();
      sptr_ast_node_v& children = obj->get_children();

      if (n < 0 || n >= static_cast<int>(children.size()))
      {
        return Roo::Constant::NIL;
      }

      sptr_ast_node to_delete = children[n];

      children.erase(children.begin() + n);

      obj->as<AST::Seq>().replace_children(children);

      return to_rt_value(to_delete);
    }
    if (seq.type == Value::Type::NATIVE_OBJECT)
    {
      throw TypeError("remove-nth! not implemented for native host sequences.");
    }

    sptr_val_v& children = std::get<sptr_val_v>(seq.value);
    sptr_val to_delete = children.at(n);

    children.erase(children.begin() + n);

    return to_delete;
  }

  /** RepeatFunction - roo/repeat */
  FUNC_IMPL(RepeatFunction,
            SIG((FN_ARGS((&Type::NUMBER), (VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&RepeatFunction::exec_repeat))))

  EXEC_BODY(RepeatFunction, exec_repeat)
  {
    int n = args[0]->num().get_int();
    Roo::sptr_val_v vector;
    vector.reserve(n * (args.size() - 1));
    for (int ni = 0; ni < n; ni++)
    {
      for (size_t i = 1; i < args.size(); i++)
      {
        vector.push_back(args[i]);
      }
    }

    return Value::vector(std::move(vector));
  }

  /** ReverseFunction - roo/reverse */
  FUNC_IMPL(ReverseFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&ReverseFunction::exec_reverse))))

  EXEC_BODY(ReverseFunction, exec_reverse)
  {
    sptr_val_v result;
    if (Roo::has_indexed_children(*args[0]))
    {
      const size_t n_children = Roo::child_count(*args[0]);
      result.reserve(n_children);
      for (size_t i = n_children; i > 0; i--)
      {
        result.push_back(Roo::get_child(*args[0], i - 1));
      }
      return Value::vector(std::move(result));
    }

    result = Roo::get_children(*args[0]);
    std::reverse(result.begin(), result.end());
    return Value::vector(std::move(result));
  }

  /** TailFunction - roo/tail */
  FUNC_IMPL(TailFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING)), EXEC_DISPATCH(&TailFunction::exec_tail))))

  EXEC_BODY(TailFunction, exec_tail)
  {
    sptr_val_v tail;
    if (Roo::has_indexed_children(*args[0]))
    {
      const size_t n_children = Roo::child_count(*args[0]);
      if (n_children > 1)
      {
        tail.reserve(n_children - 1);
        for (size_t i = 1; i < n_children; i++)
        {
          tail.push_back(Roo::get_child(*args[0], i));
        }
      }
      return Value::vector(std::move(tail));
    }

    sptr_val_v children = Roo::get_children(*args[0]);
    if (children.size() > 1)
    {
      tail.reserve(children.size() - 1);
      for (size_t i = 1; i < children.size(); i++)
      {
        tail.push_back(children[i]);
      }
    }

    return Value::vector(std::move(tail));
  }

  /** FlattenFunction - roo/flatten */
  FUNC_IMPL(FlattenFunction,
            SIG((FN_ARGS((&Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&FlattenFunction::exec_flatten))))

  EXEC_BODY(FlattenFunction, exec_flatten)
  {
    sptr_val_v result;

    sptr_val_v children = Roo::get_children(*args[0]);
    for (auto& obj : children)
    {
      if (Type::STRICT_SEQ.is_type_of(*obj))
      {
        auto flat_args = sptr_val_v{obj};
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

    return Value::vector(std::move(result));
  }

  /** TakeFunction - roo/take */
  FUNC_IMPL(TakeFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&TakeFunction::exec_take))))

  EXEC_BODY(TakeFunction, exec_take)
  {
    size_t amount = std::get<const Value::Number>(args[0]->value).get_int();
    sptr_val_v result;

    if (!Roo::has_indexed_children(*args[1]))
    {
      sptr_val_v elements = Roo::get_children(*args[1]);
      size_t actual_amount = std::min(amount, elements.size());
      result.reserve(actual_amount);
      for (size_t i = 0; i < actual_amount; i++)
      {
        result.push_back(elements[i]);
      }

      return Value::vector(std::move(result));
    }

    size_t actual_amount = std::min(amount, Roo::child_count(*args[1]));
    result.reserve(actual_amount);
    for (size_t i = 0; i < actual_amount; i++)
    {
      result.push_back(Roo::get_child(*args[1], i));
    }

    return Value::vector(std::move(result));
  }

  /** VectorFunction - roo/vector */
  FUNC_IMPL(VectorFunction,
            SIG((FN_ARGS((&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&VectorFunction::exec_vector))))

  EXEC_BODY(VectorFunction, exec_vector)
  {
    return Value::vector(args);
  }

} // namespace Roo
