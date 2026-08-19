
#include "roo/runtime/value.h"
#include "roo/type.h"

#include <algorithm>
#include <cmath>

#include <roo/host/object.h>
#include <roo/lang/seq.h>
#include <roo/runtime/seq.h>

namespace Roo
{
  namespace
  {
    struct MoveNthIndices
    {
      size_t from;
      size_t to;
      bool has_source;
    };

    MoveNthIndices resolve_move_nth_indices(const sptr_val_v& args,
                                            size_t size,
                                            const std::string& operation)
    {
      const std::string placement = args.size() == 3 ? "final" : args[2]->str();
      if (placement != "final" && placement != "before" && placement != "after")
      {
        throw InvocationException(operation +
                                  " placement must be :final, :before, or :after, got " +
                                  args[2]->to_string() + ".");
      }

      const std::int64_t from =
        checked_sequence_index(*args[1], operation + " source index");
      const std::int64_t target =
        checked_sequence_index(*args.back(), operation + " target index");
      if (size == 0) return {0, 0, false};

      const size_t from_index = normalized_sequence_index(from, size);
      const size_t target_index =
        std::min(normalized_sequence_index(target, size), size - 1);
      if (from_index == size) return {from_index, target_index, false};

      size_t to_index = target_index;
      if (from_index != target_index && placement == "before" && from_index < target_index)
      {
        to_index--;
      }
      else if (from_index != target_index && placement == "after" &&
               from_index > target_index)
      {
        to_index++;
      }

      return {from_index, to_index, true};
    }

    void move_nth_elements(sptr_val_v& elements, const MoveNthIndices& indices)
    {
      if (!indices.has_source || indices.from == indices.to) return;

      sptr_val moved = elements[indices.from];
      elements.erase(elements.begin() + static_cast<std::ptrdiff_t>(indices.from));
      elements.insert(elements.begin() + static_cast<std::ptrdiff_t>(indices.to),
                      std::move(moved));
    }

    std::string string_value(const Value& value)
    {
      if (value.type == Value::Type::STRING) return value.str();
      if (value.type == Value::Type::CHAR) return std::string(1, value.ch());
      return value.to_string();
    }

    std::string string_values(const sptr_val& values)
    {
      if (values->type == Value::Type::NIL || !Type::STRICT_SEQ.is_type_of(*values))
      {
        return string_value(*values);
      }

      std::string result;
      for (const sptr_val& value : Roo::get_children(*values))
      {
        result += string_value(*value);
      }
      return result;
    }
  } // namespace

  /** AppendFunction - roo/append */
  FUNC_IMPL(AppendFunction,
            SIG((FN_ARGS((&Type::ANY), (&VARARG, &Type::ANY)),
                 EXEC_DISPATCH(&AppendFunction::exec_append))))

  EXEC_BODY(AppendFunction, exec_append)
  {
    sptr_val_v result;

    if (args[0]->type != Value::Type::NIL)
    {
      for_each_child(*args[0], [&](const sptr_val& element) { result.push_back(element); });
    }

    for (size_t i = 1; i < args.size(); i++)
    {
      result.push_back(args[i]);
    }

    return Value::vector(std::move(result));
  }

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
        for_each_child(*vec, [&](const sptr_val& element) { result.push_back(element); });
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

  /** DropFunction - roo/drop */
  FUNC_IMPL(
    DropFunction,
    MULTI_SIG((FN_ARGS((&Type::STRICT_SEQ_OR_STRING), (&Type::NUMBER)),
               EXEC_DISPATCH(&DropFunction::exec_drop)),
              (FN_ARGS((&Type::STRICT_SEQ_OR_STRING), (&Type::NUMBER), (&Type::NUMBER)),
               EXEC_DISPATCH(&DropFunction::exec_drop))))

  EXEC_BODY(DropFunction, exec_drop)
  {
    const size_t size = Roo::child_count(*args[0]);
    const size_t start =
      normalized_sequence_index(checked_sequence_index(*args[1], "drop start"), size);
    const size_t end =
      args.size() == 2
        ? size
        : normalized_sequence_index(checked_sequence_index(*args[2], "drop end"), size);

    if (args[0]->type == Value::Type::STRING)
    {
      const std::string& string = args[0]->str();
      return Value::string(string.substr(0, start) + string.substr(std::max(start, end)));
    }

    sptr_val_v result = get_child_range(*args[0], 0, start);
    sptr_val_v suffix = get_child_range(*args[0], std::max(start, end), size);
    result.reserve(result.size() + suffix.size());
    result.insert(result.end(), suffix.begin(), suffix.end());
    return Value::vector(std::move(result));
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

  /** InterleaveFunction - roo/interleave */
  FUNC_IMPL(InterleaveFunction,
            SIG((FN_ARGS((VARARG, &Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&InterleaveFunction::exec_interleave))))

  EXEC_BODY(InterleaveFunction, exec_interleave)
  {
    std::vector<sptr_val_v> seqs;
    seqs.reserve(args.size());

    size_t max_size = 0;
    size_t result_size = 0;
    for (auto& arg : args)
    {
      sptr_val_v children = Roo::get_children(*arg);
      max_size = std::max(max_size, children.size());
      result_size += children.size();
      seqs.push_back(std::move(children));
    }

    sptr_val_v result;
    result.reserve(result_size);
    for (size_t i = 0; i < max_size; i++)
    {
      for (auto& seq : seqs)
      {
        if (i < seq.size())
        {
          result.push_back(seq[i]);
        }
      }
    }

    return Value::vector(std::move(result));
  }

  /** InsertFunction - roo/insert */
  FUNC_IMPL(InsertFunction,
            MULTI_SIG((FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER), (&Type::STRICT_SEQ)),
                       EXEC_DISPATCH(&InsertFunction::exec_insert_seq)),
                      (FN_ARGS((&Type::STRING), (&Type::NUMBER), (&Type::ANY)),
                       EXEC_DISPATCH(&InsertFunction::exec_insert_string))))

  EXEC_BODY(InsertFunction, exec_insert_seq)
  {
    sptr_val result = Value::vector(Roo::get_children(*args[0]));
    const size_t index =
      normalized_sequence_index(checked_sequence_index(*args[1], "insert position"),
                                std::get<sptr_val_v>(result->value).size());

    Roo::insert_values(*result, index, Roo::get_children(*args[2]));
    return result;
  }

  EXEC_BODY(InsertFunction, exec_insert_string)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::NIL;

    std::string result = args[0]->str();
    const size_t index =
      normalized_sequence_index(checked_sequence_index(*args[1], "insert position"),
                                result.size());
    result.insert(index, string_values(args[2]));
    return Value::string(result);
  }

  /** InsertBangFunction - roo/insert! */
  FUNC_IMPL(InsertBangFunction,
            SIG((FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER), (&Type::STRICT_SEQ)),
                 EXEC_DISPATCH(&InsertBangFunction::exec_insert_bang))))

  EXEC_BODY(InsertBangFunction, exec_insert_bang)
  {
    sptr_val target = args[0]->type == Value::Type::NIL ? Value::vector({}) : args[0];
    const size_t index =
      normalized_sequence_index(checked_sequence_index(*args[1], "insert! position"),
                                Roo::child_count(*target));

    Roo::insert_values(*target, index, Roo::get_children(*args[2]));
    return target;
  }

  /** InsertOneFunction - roo/insert-one */
  FUNC_IMPL(InsertOneFunction,
            MULTI_SIG((FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER), (&Type::ANY)),
                       EXEC_DISPATCH(&InsertOneFunction::exec_insert_one_seq)),
                      (FN_ARGS((&Type::STRING), (&Type::NUMBER), (&Type::ANY)),
                       EXEC_DISPATCH(&InsertOneFunction::exec_insert_one_string))))

  EXEC_BODY(InsertOneFunction, exec_insert_one_seq)
  {
    sptr_val result = Value::vector(Roo::get_children(*args[0]));
    const size_t index =
      normalized_sequence_index(checked_sequence_index(*args[1], "insert-one position"),
                                std::get<sptr_val_v>(result->value).size());

    Roo::insert_values(*result, index, {args[2]});
    return result;
  }

  EXEC_BODY(InsertOneFunction, exec_insert_one_string)
  {
    const size_t index =
      normalized_sequence_index(checked_sequence_index(*args[1], "insert-one position"),
                                args[0]->str().size());
    std::string result = args[0]->str();
    result.insert(index, string_value(*args[2]));
    return Value::string(result);
  }

  /** InsertOneBangFunction - roo/insert-one! */
  FUNC_IMPL(InsertOneBangFunction,
            SIG((FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER), (&Type::ANY)),
                 EXEC_DISPATCH(&InsertOneBangFunction::exec_insert_one_bang))))

  EXEC_BODY(InsertOneBangFunction, exec_insert_one_bang)
  {
    sptr_val target = args[0]->type == Value::Type::NIL ? Value::vector({}) : args[0];
    const size_t index =
      normalized_sequence_index(checked_sequence_index(*args[1], "insert-one! position"),
                                Roo::child_count(*target));

    Roo::insert_values(*target, index, {args[2]});
    return target;
  }

  /** ReplaceFunction - roo/replace */
  FUNC_IMPL(
    ReplaceFunction,
    MULTI_SIG(
      (FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER), (&Type::NUMBER), (&Type::STRICT_SEQ)),
       EXEC_DISPATCH(&ReplaceFunction::exec_replace_seq)),
      (FN_ARGS((&Type::STRING), (&Type::NUMBER), (&Type::NUMBER), (&Type::ANY)),
       EXEC_DISPATCH(&ReplaceFunction::exec_replace_string))))

  EXEC_BODY(ReplaceFunction, exec_replace_seq)
  {
    sptr_val result = Value::vector(Roo::get_children(*args[0]));
    const size_t size = Roo::child_count(*result);
    const size_t start =
      normalized_sequence_index(checked_sequence_index(*args[1], "replace start"), size);
    const size_t end =
      normalized_sequence_index(checked_sequence_index(*args[2], "replace end"), size);

    Roo::replace_values(*result, start, end, Roo::get_children(*args[3]));
    return result;
  }

  EXEC_BODY(ReplaceFunction, exec_replace_string)
  {
    std::string result = args[0]->str();
    const size_t start =
      normalized_sequence_index(checked_sequence_index(*args[1], "replace start"),
                                result.size());
    const size_t end =
      normalized_sequence_index(checked_sequence_index(*args[2], "replace end"),
                                result.size());

    result.replace(start, std::max(start, end) - start, string_values(args[3]));
    return Value::string(result);
  }

  /** ReplaceBangFunction - roo/replace! */
  FUNC_IMPL(
    ReplaceBangFunction,
    SIG((FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER), (&Type::NUMBER), (&Type::STRICT_SEQ)),
         EXEC_DISPATCH(&ReplaceBangFunction::exec_replace_bang))))

  EXEC_BODY(ReplaceBangFunction, exec_replace_bang)
  {
    sptr_val target = args[0]->type == Value::Type::NIL ? Value::vector({}) : args[0];
    const size_t size = Roo::child_count(*target);
    const size_t start =
      normalized_sequence_index(checked_sequence_index(*args[1], "replace! start"), size);
    const size_t end =
      normalized_sequence_index(checked_sequence_index(*args[2], "replace! end"), size);

    Roo::replace_values(*target, start, end, Roo::get_children(*args[3]));
    return target;
  }

  /** MoveNthFunction - roo/move-nth */
  FUNC_IMPL(MoveNthFunction,
            MULTI_SIG((FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER), (&Type::NUMBER)),
                       EXEC_DISPATCH(&MoveNthFunction::exec_move_nth)),
                      (FN_ARGS((&Type::STRICT_SEQ),
                               (&Type::NUMBER),
                               (&Type::KEYWORD),
                               (&Type::NUMBER)),
                       EXEC_DISPATCH(&MoveNthFunction::exec_move_nth))))

  EXEC_BODY(MoveNthFunction, exec_move_nth)
  {
    sptr_val_v result = Roo::get_children(*args[0]);
    move_nth_elements(result, resolve_move_nth_indices(args, result.size(), "move-nth"));
    return Value::vector(std::move(result));
  }

  /** MoveNthBangFunction - roo/move-nth! */
  FUNC_IMPL(MoveNthBangFunction,
            MULTI_SIG((FN_ARGS((&Type::STRICT_SEQ), (&Type::NUMBER), (&Type::NUMBER)),
                       EXEC_DISPATCH(&MoveNthBangFunction::exec_move_nth_bang)),
                      (FN_ARGS((&Type::STRICT_SEQ),
                               (&Type::NUMBER),
                               (&Type::KEYWORD),
                               (&Type::NUMBER)),
                       EXEC_DISPATCH(&MoveNthBangFunction::exec_move_nth_bang))))

  EXEC_BODY(MoveNthBangFunction, exec_move_nth_bang)
  {
    sptr_val target = args[0]->type == Value::Type::NIL ? Value::vector({}) : args[0];
    const MoveNthIndices indices =
      resolve_move_nth_indices(args, Roo::child_count(*target), "move-nth!");

    if (!indices.has_source || indices.from == indices.to) return target;

    if (target->type == Value::Type::VECTOR || target->type == Value::Type::LIST)
    {
      move_nth_elements(std::get<sptr_val_v>(target->value), indices);
      return target;
    }

    if (target->type == Value::Type::NATIVE_OBJECT)
    {
      target->nobj()->move_child(indices.from, indices.to);
      return target;
    }

    throw TypeError(target->to_string() + " is not a valid mutable sequence for move-nth!.");
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
    if (args[1]->type != Value::Type::NUMBER)
    {
      throw TypeError("remove-nth: index must be a number.");
    }

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
    if (args[1]->type != Value::Type::NUMBER)
    {
      throw TypeError("remove-nth!: index must be a number.");
    }

    auto& seq = *args[0];
    int n = std::get<const Value::Number>(args[1]->value).get_int();

    if (seq.type == Value::Type::NATIVE_OBJECT)
    {
      throw TypeError("remove-nth!: not implemented for native host sequences.");
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

  /** SliceFunction - roo/slice */
  FUNC_IMPL(
    SliceFunction,
    MULTI_SIG((FN_ARGS((&Type::STRICT_SEQ_OR_STRING), (&Type::NUMBER)),
               EXEC_DISPATCH(&SliceFunction::exec_slice)),
              (FN_ARGS((&Type::STRICT_SEQ_OR_STRING), (&Type::NUMBER), (&Type::NUMBER)),
               EXEC_DISPATCH(&SliceFunction::exec_slice))))

  EXEC_BODY(SliceFunction, exec_slice)
  {
    const size_t size = Roo::child_count(*args[0]);
    const size_t start =
      normalized_sequence_index(checked_sequence_index(*args[1], "slice start"), size);
    const size_t end =
      args.size() == 2
        ? size
        : normalized_sequence_index(checked_sequence_index(*args[2], "slice end"), size);

    if (args[0]->type == Value::Type::STRING)
    {
      if (end <= start) return Value::string("");
      return Value::string(args[0]->str().substr(start, end - start));
    }
    return Value::vector(get_child_range(*args[0], start, end));
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
    if (args[0]->type != Value::Type::NUMBER)
    {
      throw TypeError("take: count must be a number.");
    }

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
