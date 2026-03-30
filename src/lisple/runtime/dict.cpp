
#include "dict.h"

#include "seq.h"
#include <algorithm>
#include <iostream>

namespace Lisple::Dict
{
  sptr_rtval get_property(sptr_rtval& target, const sptr_rtval& property)
  {
    return get_property(target, *property);
  }

  sptr_rtval get_property(sptr_rtval& target, const RTValue& property)
  {
    if (target->type == RTValue::Type::MAP)
    {
      auto children = Lisple::get_children(*target);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        if (*children[i] == property)
        {
          return children[i + 1];
        }
      }
    }
    else if (target->type == RTValue::Type::OBJECT)
    {
      auto key = to_AST(const_cast<RTValue&>(property));
      auto val = std::get<sptr_sobject>(target->value)->get_sptr_property(*key);
      return to_rt_value(val);
    }

    return Constant::NIL;
  }

  sptr_rtval get_property(RTValue& target, const std::string& keyword)
  {
    if (target.type == RTValue::Type::MAP)
    {
      auto children = Lisple::get_children(target);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        if (children[i]->type == RTValue::Type::KEYWORD &&
            std::get<std::string>(children[i]->value) == keyword)
        {
          return children[i + 1];
        }
      }
    }
    else if (target.type == RTValue::Type::OBJECT)
    {
      auto key = Lisple::Key::make(keyword);
      auto val = std::get<sptr_sobject>(target.value)->get_sptr_property(*key);
      return to_rt_value(val);
    }

    return Constant::NIL;
  }

  sptr_rtval get_property_or_throw(RTValue& source, const std::string& keyword)
  {
    if (source.type == RTValue::Type::MAP)
    {
      auto& children = std::get<sptr_rtval_v>(source.value);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        if (children[i]->type == RTValue::Type::KEYWORD &&
            std::get<std::string>(children[i]->value) == keyword)
        {
          return children[i + 1];
        }
      }
    }

    throw InvocationException("Key :" + keyword + " not present in " + source.to_string());
  }

  bool contains_key(RTValue& source, const std::string& keyword)
  {
    if (source.type == RTValue::Type::MAP)
    {
      auto& children = std::get<sptr_rtval_v>(source.value);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        if (children[i]->type == RTValue::Type::KEYWORD &&
            std::get<std::string>(children[i]->value) == keyword)
        {
          return true;
        }
      }
    }

    return false;
  }

  sptr_rtval remove_property(sptr_rtval& target, const sptr_rtval& property)
  {
    sptr_rtval removed_val = Constant::NIL;

    int index = -1;

    sptr_rtval_v& children = std::get<sptr_rtval_v>(target->value);

    for (size_t i = 0; i < children.size(); i += 2)
    {
      // std::cout << " - Inspect #" << i << std::endl;
      // std::cout << "   - " << children[i]->to_string() << " vs " << property->to_string()
      //           << " - " << (*children[i] == *property) << std::endl;
      if (*children[i] == *property)
      {
        index = i;
        break;
      }
    }

    if (index != -1)
    {
      // std::cout << " - Erase entry" << std::endl;
      removed_val = children[index + 1];
      children.erase(children.begin() + index, children.begin() + index + 2);
    }

    return removed_val;
  }

  void set_property(sptr_rtval& target, const sptr_rtval& property, sptr_rtval& value)
  {
    if (target->type == RTValue::Type::MAP)
    {
      sptr_rtval_v& elements = std::get<sptr_rtval_v>(target->value);

      auto it = std::find_if(elements.begin(),
                             elements.end(),
                             [&](const sptr_rtval& l) { return *l == *property; });

      if (it == elements.end())
      {
        elements.push_back(property);
        elements.push_back(value);
      }
      else
      {
        size_t index = std::distance(elements.begin(), it);
        if (index % 2 != 0)
        {
          throw LispleException("Invalid map structure");
        }
        elements[index + 1] = value;
      }
    }
    else if (target->type == RTValue::Type::OBJECT)
    {
      sptr_sobject& ho = std::get<sptr_sobject>(target->value);
      ho->set_property(to_AST(*property), to_AST(*value));
    }
    else
    {
      std::cout << "RTValue::Type==" << (int)target->type << std::endl;
      throw LispleException("Cannot mutate target");
    }
  }

  const std::vector<const RTValue*> map_keys(const std::vector<RTValue>& map_data)
  {
    std::vector<const RTValue*> keys;
    keys.reserve(map_data.size() / 2);
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      keys.push_back(&map_data[i]);
    }
    return keys;
  }

  const std::vector<const RTValue*> map_keys(const sptr_rtval_v& map_data)
  {
    std::vector<const RTValue*> keys;
    keys.reserve(map_data.size() / 2);
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      keys.push_back(map_data[i].get());
    }
    return keys;
  }

  const std::vector<const RTValue*> map_keys(const RTValue& map_data)
  {
    if (const Lisple::sptr_rtval_v* children = std::get_if<sptr_rtval_v>(&map_data.value))
    {
      return map_keys(*children);
    }

    return {};
  }

  std::unordered_set<std::string> map_string_keys(RTValue& value)
  {
    if (value.type != RTValue::Type::MAP)
    {
      return {};
    }

    sptr_rtval_v& map_data = std::get<sptr_rtval_v>(value.value);
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

  std::pair<const sptr_rtval, const sptr_rtval> map_entry(const sptr_rtval_v& map_data,
                                                          const RTValue& key)
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

  std::pair<sptr_rtval, sptr_rtval> map_entry(sptr_rtval_v& map_data, const RTValue& key)
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

} // namespace Lisple::Dict
