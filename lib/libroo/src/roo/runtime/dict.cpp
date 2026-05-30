
#include "roo/runtime/dict.h"

#include "roo/exception.h"
#include "roo/runtime/value.h"
#include "roo/type.h"

#include <algorithm>
#include <iostream>

#include <roo/host/object.h>
#include <roo/runtime/seq.h>

namespace Roo::Dict
{
  sptr_val get_property(const sptr_val& target, const sptr_val& property)
  {
    return get_property(target, *property);
  }

  sptr_val get_property(const sptr_val& target, const Value& property)
  {
    auto [found, value] = find_property(target, property);
    return found ? value : Constant::NIL;
  }

  std::pair<bool, sptr_val> find_property(const sptr_val& target, const sptr_val& property)
  {
    return find_property(target, *property);
  }

  std::pair<bool, sptr_val> find_property(const sptr_val& target, const Value& property)
  {
    switch (target->type)
    {
    case Value::Type::MAP:
    {
      auto children = Roo::get_children(*target);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        if (*children[i] == property)
        {
          return {true, children[i + 1]};
        }
      }
      return {false, Constant::NIL};
    }
    case Value::Type::LIST:
    case Value::Type::VECTOR:
    {
      if (property.type != Value::Type::NUMBER)
      {
        return {false, Constant::NIL};
      }
      int index = property.num().get_int();
      if (index < 0) return {false, Constant::NIL};

      const sptr_val_v& values = std::get<sptr_val_v>(target->value);
      if (index >= static_cast<int>(values.size())) return {false, Constant::NIL};

      return {true, values[index]};
    }
    case Value::Type::NATIVE_OBJECT:
    {
      Roo::sptr_native_obj obj = target->nobj();
      if (!obj->has_property(property)) return {false, Constant::NIL};
      return {true, obj->get_property(property)};
    }
    case Value::Type::OBJECT:
    {
      auto key = to_AST(const_cast<Value&>(property));
      if (!std::get<sptr_ast_node>(target->value)->has_key(*key))
      {
        return {false, Constant::NIL};
      }
      auto val = std::get<sptr_ast_node>(target->value)->get_sptr_property(*key);
      return {true, to_rt_value(val)};
    }
    default:
      return {false, Constant::NIL};
    };
  }

  sptr_val get_property(Value& target, const std::string& keyword)
  {
    if (target.type == Value::Type::MAP)
    {
      auto children = Roo::get_children(target);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        if (children[i]->type == Value::Type::KEYWORD &&
            std::get<std::string>(children[i]->value) == keyword)
        {
          return children[i + 1];
        }
      }
    }
    else if (target.type == Value::Type::OBJECT)
    {
      auto key = Roo::AST::Keyword::make(keyword);
      auto val = std::get<sptr_ast_node>(target.value)->get_sptr_property(*key);
      return to_rt_value(val);
    }
    else if (target.type == Value::Type::NATIVE_OBJECT)
    {
      return target.nobj()->get_property(*Value::keyword(keyword));
    }

    return Constant::NIL;
  }

  sptr_val get_property_or_throw(Value& source, const std::string& keyword)
  {
    if (source.type == Value::Type::MAP)
    {
      auto& children = std::get<sptr_val_v>(source.value);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        if (children[i]->type == Value::Type::KEYWORD &&
            std::get<std::string>(children[i]->value) == keyword)
        {
          return children[i + 1];
        }
      }
    }

    throw InvocationException("AST::Keyword :" + keyword + " not present in " +
                              source.to_string());
  }

  sptr_val get_property_path(const sptr_val& object, const sptr_val_v& path)
  {
    auto [found, value] = find_property_path(object, path);
    return found ? value : Constant::NIL;
  }

  std::pair<bool, sptr_val> find_property_path(const sptr_val& object,
                                               const sptr_val_v& path)
  {
    sptr_val result = object;
    for (size_t i = 0; i < path.size(); i++)
    {
      if (*result == *Constant::NIL)
      {
        return {false, Constant::NIL};
      }

      auto [found, value] = find_property(result, path[i]);
      if (!found) return {false, Constant::NIL};
      result = value;
    }

    return {true, result};
  }

  bool contains_key(Value& source, const std::string& keyword)
  {
    if (source.type == Value::Type::MAP)
    {
      auto& children = std::get<sptr_val_v>(source.value);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        if (children[i]->type == Value::Type::KEYWORD &&
            std::get<std::string>(children[i]->value) == keyword)
        {
          return true;
        }
      }
    }
    else if (source.type == Value::Type::NATIVE_OBJECT)
    {
      return source.nobj()->has_property(*Value::keyword(keyword));
    }

    return false;
  }

  sptr_val remove_property(sptr_val& target, const sptr_val& property)
  {
    sptr_val removed_val = Constant::NIL;

    int index = -1;

    sptr_val_v& children = std::get<sptr_val_v>(target->value);

    for (size_t i = 0; i < children.size(); i += 2)
    {
      if (*children[i] == *property)
      {
        index = i;
        break;
      }
    }

    if (index != -1)
    {
      removed_val = children[index + 1];
      children.erase(children.begin() + index, children.begin() + index + 2);
    }

    return removed_val;
  }

  void set_property(sptr_val& target, const sptr_val& property, const sptr_val& value)
  {
    if (target->type == Value::Type::MAP)
    {
      sptr_val_v& elements = std::get<sptr_val_v>(target->value);

      size_t index = elements.size();
      for (size_t i = 0; i < elements.size(); i += 2)
      {
        if (*elements[i] == *property)
        {
          index = i;
          break;
        }
      }

      if (index == elements.size())
      {
        elements.push_back(property);
        elements.push_back(value);
      }
      else
      {
        elements[index + 1] = value;
      }
    }
    else if (target->type == Value::Type::VECTOR)
    {
      if (property->type != Value::Type::NUMBER)
      {
        throw TypeError("Cannot set property " + property->to_string() + " of vector");
      }
      Roo::set_child(*target, property->num().get_int(), value);
    }
    else if (target->type == Value::Type::OBJECT)
    {
      sptr_ast_node& ho = std::get<sptr_ast_node>(target->value);
      ho->set_property(to_AST(*property), to_AST(*value));
    }
    else if (target->type == Value::Type::NATIVE_OBJECT)
    {
      target->nobj()->set_property(*property, value);
    }
    else
    {
      std::cout << "Value::Type==" << (int)target->type << std::endl;
      throw RooException("Cannot mutate target");
    }
  }

  const std::vector<sptr_val> keys(const Value& dict)
  {
    std::vector<sptr_val> keys;
    size_t count = Roo::count(dict);
    keys.reserve(count / 2);
    for (size_t i = 0; i < count; i++)
    {
      keys.push_back(Roo::get_child(dict, i * 2));
    }
    return keys;
  }

  const std::vector<const Value*> map_keys(const std::vector<Value>& map_data)
  {
    std::vector<const Value*> keys;
    keys.reserve(map_data.size() / 2);
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      keys.push_back(&map_data[i]);
    }
    return keys;
  }

  const std::vector<const Value*> map_keys(const sptr_val_v& map_data)
  {
    std::vector<const Value*> keys;
    keys.reserve(map_data.size() / 2);
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      keys.push_back(map_data[i].get());
    }
    return keys;
  }

  std::vector<sptr_val> map_sptr_keys(const sptr_val& dict)
  {
    std::vector<sptr_val> keys;
    sptr_val_v dc = get_children(*dict);
    for (size_t i = 0; i < dc.size(); i += 2)
    {
      keys.push_back(dc[i]);
    }
    return keys;
  }

  const std::vector<const Value*> map_keys(const Value& map_data)
  {
    if (const Roo::sptr_val_v* children = std::get_if<sptr_val_v>(&map_data.value))
    {
      return map_keys(*children);
    }

    return {};
  }

  std::unordered_set<std::string> map_string_keys(Value& value)
  {
    if (value.type != Value::Type::MAP)
    {
      return {};
    }

    sptr_val_v& map_data = std::get<sptr_val_v>(value.value);
    std::unordered_set<std::string> keys;
    keys.reserve(map_data.size() / 2);
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      if (std::string* key = std::get_if<std::string>(&map_data[i]->value))
      {
        keys.emplace(*key);
      }
      else
      {
        keys.emplace(map_data[i]->to_string());
      }
    }
    return keys;
  }

  std::pair<const sptr_val, const sptr_val> map_entry(const sptr_val_v& map_data,
                                                      const Value& key)
  {
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      if (*map_data[i] == key)
      {
        return {map_data[i], map_data[i + 1]};
      }
    }

    return {nullptr, nullptr};
  }

  std::pair<sptr_val, sptr_val> map_entry(sptr_val_v& map_data, const Value& key)
  {
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      if (*map_data[i] == key)
      {
        return {map_data[i], map_data[i + 1]};
      }
    }

    return {nullptr, nullptr};
  }

  sptr_val shallow_copy(const sptr_val& source)
  {
    if (Type::COMPLEX.is_type_of(*source))
    {
      sptr_val_v children = Roo::get_children(*source);
      return Value::map(children);
    }
    else if (Type::SEQ.is_type_of(*source))
    {
      sptr_val_v children = Roo::get_children(*source);
      return Value::vector(children);
    }

    return source;
  }

  static sptr_val assoc_in_copy(const sptr_val& current,
                                const sptr_val_v& path,
                                size_t index,
                                const sptr_val& value)
  {
    if (index >= path.size())
    {
      throw InvocationException("assoc-in path traversal went out of bounds.");
    }

    sptr_val result;
    switch (current->type)
    {
    case Value::Type::NIL:
      result = Value::map({});
      break;
    case Value::Type::MAP:
    case Value::Type::OBJECT:
    case Value::Type::NATIVE_OBJECT:
    case Value::Type::VECTOR:
    case Value::Type::LIST:
      result = Dict::shallow_copy(current);
      break;
    default:
      throw TypeError("assoc-in cannot traverse through " + current->to_string());
    }

    const sptr_val& key = path[index];

    if (index == path.size() - 1)
    {
      Dict::set_property(result, key, value);
      return result;
    }

    sptr_val child = Dict::get_property(current, *key);
    sptr_val new_child = assoc_in_copy(child, path, index + 1, value);

    Dict::set_property(result, key, new_child);

    return result;
  }

  sptr_val assoc_in(const sptr_val& current, const sptr_val_v& path, const sptr_val& value)
  {
    return assoc_in_copy(current, path, 0, value);
  }

  static bool can_dissoc_in_traverse(const sptr_val& current)
  {
    switch (current->type)
    {
    case Value::Type::NIL:
    case Value::Type::MAP:
    case Value::Type::OBJECT:
    case Value::Type::NATIVE_OBJECT:
    case Value::Type::VECTOR:
    case Value::Type::LIST:
      return true;
    default:
      return false;
    }
  }

  static bool can_dissoc_property(const sptr_val& current)
  {
    switch (current->type)
    {
    case Value::Type::MAP:
    case Value::Type::OBJECT:
    case Value::Type::NATIVE_OBJECT:
      return true;
    default:
      return false;
    }
  }

  static void remove_property_from_copy(sptr_val& target, const sptr_val& property)
  {
    sptr_val_v& children = std::get<sptr_val_v>(target->value);

    for (size_t i = 0; i < children.size(); i += 2)
    {
      if (*children[i] == *property)
      {
        children.erase(children.begin() + i, children.begin() + i + 2);
        return;
      }
    }
  }

  static sptr_val dissoc_in_copy(const sptr_val& current,
                                 const sptr_val_v& path,
                                 size_t index)
  {
    if (index >= path.size())
    {
      throw InvocationException("dissoc-in path traversal went out of bounds.");
    }

    if (*current == *Constant::NIL)
    {
      return Constant::NIL;
    }

    if (!can_dissoc_in_traverse(current))
    {
      throw TypeError("dissoc-in cannot traverse through " + current->to_string());
    }

    const sptr_val& key = path[index];

    if (index == path.size() - 1)
    {
      if (!can_dissoc_property(current))
      {
        throw TypeError("dissoc-in cannot remove property from " + current->to_string());
      }

      sptr_val result = Dict::shallow_copy(current);
      remove_property_from_copy(result, key);
      return result;
    }

    sptr_val result = Dict::shallow_copy(current);
    auto [found, child] = Dict::find_property(current, key);
    if (!found)
    {
      return result;
    }

    sptr_val new_child = dissoc_in_copy(child, path, index + 1);
    Dict::set_property(result, key, new_child);

    return result;
  }

  sptr_val dissoc_in(const sptr_val& current, const sptr_val_v& path)
  {
    return dissoc_in_copy(current, path, 0);
  }

} // namespace Roo::Dict
