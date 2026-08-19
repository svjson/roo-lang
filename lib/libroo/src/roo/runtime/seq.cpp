
#include "roo/runtime/seq.h"

#include <algorithm>
#include <cstdint>

#include <roo/exception.h>
#include <roo/form.h>
#include <roo/host/object.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  sptr_val_v get_children(Value& v)
  {
    switch (v.type)
    {
    case Value::Type::STRING:
    {
      sptr_val_v chars;
      std::string& str = std::get<std::string>(v.value);
      for (size_t i = 0; i < str.size(); i++)
      {
        chars.push_back(Value::character(str.at(i)));
      }
      return chars;
    }
    case Value::Type::NATIVE_OBJECT:
    {
      sptr_native_obj obj = v.nobj();
      return obj->native_children();
    }
    case Value::Type::NIL:
      return {};
    case Value::Type::MAP:
    case Value::Type::LIST:
    case Value::Type::VECTOR:
      return std::get<sptr_val_v>(v.value);
    default:
      break;
    }
    throw RooException("Cannot get children from type: " + std::to_string((int)v.type));
  }

  sptr_val get_child(const Value& seq, size_t index)
  {
    switch (seq.type)
    {
    case Value::Type::NIL:
      return Constant::NIL;
    case Value::Type::LIST:
    case Value::Type::VECTOR:
    case Value::Type::MAP:
    {
      const sptr_val_v& values = std::get<sptr_val_v>(seq.value);
      if (index < values.size())
      {
        return values[index];
      }
      return Roo::Constant::NIL;
    }
    case Value::Type::OBJECT:
    {
      if (auto* wrapper = dynamic_cast<AST::RuntimeValueWrapper*>(seq.obj().get()))
      {
        return get_child(*wrapper->val, index);
      }

      sptr_ast_node_v& children = seq.obj()->get_children();
      if (index >= children.size()) return Constant::NIL;
      return to_rt_value(children[index]);
    }
    case Value::Type::NATIVE_OBJECT:
    {
      if (seq.nobj()->structural_kind() == NativeObjectStructuralKind::VECTOR)
      {
        return seq.nobj()->native_child(index);
      }

      sptr_val_v values = seq.nobj()->native_children();
      if (index < values.size())
      {
        return values[index];
      }
      return Roo::Constant::NIL;
    }
    case Value::Type::STRING:
    {
      const std::string& str = seq.str();
      if (index >= str.size()) return Roo::Constant::NIL;
      return Value::character(str.at(index));
    }
    default:
      throw RooException("get_child is not implemented for type: " +
                         std::to_string((int)seq.type));
    }
  }

  size_t child_count(const Value& v)
  {
    switch (v.type)
    {
    case Value::Type::NIL:
      return 0;
    case Value::Type::STRING:
      return v.str().size();
    case Value::Type::VECTOR:
    case Value::Type::LIST:
    case Value::Type::MAP:
      return std::get<sptr_val_v>(v.value).size();
    case Value::Type::OBJECT:
    {
      if (auto* wrapper = dynamic_cast<AST::RuntimeValueWrapper*>(v.obj().get()))
      {
        return child_count(*wrapper->val);
      }

      return v.obj()->get_children().size();
    }
    case Value::Type::NATIVE_OBJECT:
    {
      if (v.nobj()->structural_kind() == NativeObjectStructuralKind::VECTOR)
      {
        return v.nobj()->size();
      }

      return v.nobj()->native_children().size();
    }
    default:
      throw RooException("child_count is not implemented for type: " +
                         std::to_string((int)v.type));
    }
  }

  bool has_indexed_children(const Value& v)
  {
    switch (v.type)
    {
    case Value::Type::NIL:
    case Value::Type::STRING:
    case Value::Type::VECTOR:
    case Value::Type::LIST:
    case Value::Type::MAP:
      return true;
    case Value::Type::OBJECT:
    {
      if (auto* wrapper = dynamic_cast<AST::RuntimeValueWrapper*>(v.obj().get()))
      {
        return has_indexed_children(*wrapper->val);
      }

      return true;
    }
    case Value::Type::NATIVE_OBJECT:
      return v.nobj()->structural_kind() == NativeObjectStructuralKind::VECTOR;
    default:
      return false;
    }
  }

  std::int64_t checked_sequence_index(const Value& value, const std::string& description)
  {
    const Value::Number& index = value.num();
    if (index.num_type == Value::NumberType::FLOAT)
    {
      throw TypeError(description + " must be an integer.");
    }

    return index.num_type == Value::NumberType::INT ? index.int_value : index.long_value;
  }

  size_t normalized_sequence_index(std::int64_t index, size_t size)
  {
    if (index >= 0)
    {
      const std::uint64_t offset = static_cast<std::uint64_t>(index);
      return offset >= size ? size : static_cast<size_t>(offset);
    }

    const std::uint64_t distance = static_cast<std::uint64_t>(-(index + 1)) + 1;
    return distance >= size ? 0 : size - static_cast<size_t>(distance);
  }

  sptr_val_v get_child_range(Value& sequence, size_t start, size_t end)
  {
    sptr_val_v result;

    if (has_indexed_children(sequence))
    {
      const size_t size = child_count(sequence);
      const size_t first = std::min(start, size);
      const size_t last = std::min(end, size);
      if (last <= first) return result;

      result.reserve(last - first);
      for (size_t i = first; i < last; i++)
      {
        result.push_back(get_child(sequence, i));
      }
      return result;
    }

    sptr_val_v elements = get_children(sequence);
    const size_t first = std::min(start, elements.size());
    const size_t last = std::min(end, elements.size());
    if (last <= first) return result;

    result.reserve(last - first);
    for (size_t i = first; i < last; i++)
    {
      result.push_back(elements[i]);
    }
    return result;
  }

  void set_child(Value& seq, size_t index, const sptr_val& value)
  {
    switch (seq.type)
    {
    case Value::Type::VECTOR:
    case Value::Type::LIST:
    {
      sptr_val_v& elements = std::get<sptr_val_v>(seq.value);
      while (elements.size() <= index)
      {
        elements.push_back(Roo::Constant::NIL);
      }
      elements[index] = value;
      break;
    }
    default:
      throw TypeError("set_child is not implemented for type: " +
                      std::to_string((int)seq.type));
    }
  }

  void insert_values(Value& target, size_t position, sptr_val_v values)
  {
    switch (target.type)
    {
    case Value::Type::LIST:
    case Value::Type::VECTOR:
    {
      sptr_val_v& elements = std::get<sptr_val_v>(target.value);
      const size_t insertion_position = std::min(position, elements.size());
      elements.insert(elements.begin() + static_cast<std::ptrdiff_t>(insertion_position),
                      values.begin(),
                      values.end());
      return;
    }
    case Value::Type::NATIVE_OBJECT:
    {
      if (target.nobj()->structural_kind() == NativeObjectStructuralKind::VECTOR)
      {
        const size_t insertion_position = std::min(position, target.nobj()->size());
        for (size_t i = 0; i < values.size(); i++)
        {
          target.nobj()->insert_child(insertion_position + i, values[i]);
        }
        return;
      }
      break;
    }
    default:
      break;
    }

    throw TypeError("insert_values is not implemented for type: " +
                    std::to_string((int)target.type));
  }

  void replace_values(Value& target, size_t start, size_t end, sptr_val_v values)
  {
    switch (target.type)
    {
    case Value::Type::LIST:
    case Value::Type::VECTOR:
    {
      sptr_val_v& elements = std::get<sptr_val_v>(target.value);
      const size_t replacement_start = std::min(start, elements.size());
      const size_t replacement_end =
        std::max(replacement_start, std::min(end, elements.size()));
      auto insertion = elements.begin() + static_cast<std::ptrdiff_t>(replacement_start);
      elements.erase(insertion,
                     elements.begin() + static_cast<std::ptrdiff_t>(replacement_end));
      elements.insert(elements.begin() + static_cast<std::ptrdiff_t>(replacement_start),
                      values.begin(),
                      values.end());
      return;
    }
    case Value::Type::NATIVE_OBJECT:
    {
      if (target.nobj()->structural_kind() == NativeObjectStructuralKind::VECTOR)
      {
        const size_t replacement_start = std::min(start, target.nobj()->size());
        const size_t replacement_end =
          std::max(replacement_start, std::min(end, target.nobj()->size()));
        target.nobj()->erase_children(replacement_start, replacement_end);
        for (size_t i = 0; i < values.size(); i++)
        {
          target.nobj()->insert_child(replacement_start + i, values[i]);
        }
        return;
      }
      break;
    }
    default:
      break;
    }

    throw TypeError("replace_values is not implemented for type: " +
                    std::to_string((int)target.type));
  }

  sptr_val pop_child(Value& seq)
  {
    switch (seq.type)
    {
    case Value::Type::VECTOR:
    {
      auto& vec = std::get<sptr_val_v>(seq.value);
      auto item = vec.back();
      vec.pop_back();
      return item;
    }
    default:
      throw RooException("get_child is not implemented for type: " +
                         std::to_string((int)seq.type));
    }
  }

  sptr_val peek_child(Value& seq)
  {
    switch (seq.type)
    {
    case Value::Type::STRING:
    {
      const std::string& str = seq.str();
      if (str.empty()) return Constant::NIL;
      return Value::character(str.back());
    }
    case Value::Type::VECTOR:
    case Value::Type::LIST:
    case Value::Type::MAP:
    {
      auto& vec = std::get<sptr_val_v>(seq.value);
      if (vec.empty()) return Constant::NIL;
      return vec.back();
    }
    case Value::Type::OBJECT:
    {
      return to_rt_value(seq.obj()->get_children().back());
    }
    case Value::Type::NATIVE_OBJECT:
    {
      if (seq.nobj()->structural_kind() == NativeObjectStructuralKind::VECTOR)
      {
        size_t size = seq.nobj()->size();
        if (size == 0) return Constant::NIL;
        return seq.nobj()->native_child(size - 1);
      }

      sptr_val_v values = seq.nobj()->native_children();
      if (values.empty()) return Constant::NIL;
      return values.back();
    }
    default:
      throw RooException("peek_child is not implemented for type: " +
                         std::to_string((int)seq.type));
    }
  }

  size_t count(const Value& v)
  {
    switch (v.type)
    {
    case Value::Type::NIL:
      return 0;
    case Value::Type::STRING:
      return std::get<std::string>(v.value).size();
    case Value::Type::VECTOR:
    case Value::Type::LIST:
      return std::get<sptr_val_v>(v.value).size();
    case Value::Type::MAP:
      return std::get<sptr_val_v>(v.value).size() / 2;
    case Value::Type::OBJECT:
      return std::get<sptr_ast_node>(v.value)->size();
    case Value::Type::NATIVE_OBJECT:
      return std::get<sptr_native_obj>(v.value)->size();
    default:
      return 1;
    }
  }

  void append(Value& seq, const sptr_val& value)
  {
    if (seq.type == Value::Type::VECTOR)
    {
      std::get<sptr_val_v>(seq.value).push_back(value);
    }
    else
    {
      throw TypeError("append not implemented for Value type: " +
                      std::to_string((int)seq.type));
    }
  }

} // namespace Roo
