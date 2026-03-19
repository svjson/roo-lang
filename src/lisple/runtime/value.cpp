
#include "value.h"

#include "../exception.h"
#include "../form.h"

namespace Lisple
{
  int RTValue::Number::get_int() const
  {
    switch (num_type)
    {
    case NumberType::INT:
      return int_value;
    case NumberType::LONG:
      return static_cast<int>(long_value);
    case NumberType::FLOAT:
    default:
      return float_value;
    }
  }

  RTValue RTValue::nil()
  {
    RTValue val;
    val.type = RTValue::Type::NIL;
    val.value = std::monostate{};
    return val;
  }

  RTValue RTValue::boolean(bool b)
  {
    RTValue val;
    val.type = RTValue::Type::BOOL;
    val.value = b;
    return val;
  }

  RTValue RTValue::number(int v)
  {
    RTValue val;
    val.type = RTValue::Type::NUMBER;
    val.value = RTValue::Number{.num_type = NumberType::INT, .int_value = v};
    return val;
  }

  RTValue RTValue::number(long v)
  {
    RTValue val;
    val.type = RTValue::Type::NUMBER;
    val.value = RTValue::Number{.num_type = NumberType::LONG, .long_value = v};
    return val;
  }

  RTValue RTValue::number(double v)
  {
    RTValue val;
    val.type = RTValue::Type::NUMBER;
    val.value = RTValue::Number{.num_type = NumberType::FLOAT, .float_value = v};
    return val;
  }

  RTValue RTValue::character(char c)
  {
    RTValue val;
    val.type = RTValue::Type::CHAR;
    val.value = c;
    return val;
  }

  RTValue RTValue::string(const std::string& v)
  {
    RTValue val;
    val.type = RTValue::Type::STRING;
    val.value = v;
    return val;
  }

  RTValue RTValue::keyword(const std::string& v)
  {
    RTValue val;
    val.type = RTValue::Type::KEYWORD;
    val.value = v;
    return val;
  }

  RTValue RTValue::symbol(const std::string& v)
  {
    RTValue val;
    val.type = RTValue::Type::SYMBOL;
    val.value = v;
    return val;
  }

  RTValue RTValue::object(Object* o)
  {
    RTValue val;
    val.type = RTValue::Type::OBJECT;
    val.value = o;
    return val;
  }

  RTValue RTValue::list(std::vector<RTValue>& v)
  {
    RTValue val;
    val.type = RTValue::Type::LIST;
    val.value = v;
    return val;
  }

  RTValue RTValue::vector(std::vector<RTValue>& v)
  {
    RTValue val;
    val.type = RTValue::Type::VECTOR;
    val.value = v;
    return val;
  }

  RTValue RTValue::map(std::vector<RTValue>& v)
  {
    RTValue val;
    val.type = RTValue::Type::MAP;
    val.value = v;
    return val;
  }

  RTValue RTValue::function(Function* fn)
  {
    RTValue val;
    val.type = RTValue::Type::FUNCTION;
    val.value = fn;
    return val;
  }

  bool RTValue::operator==(const RTValue& other) const
  {
    if (this->type != other.type) return false;

    switch (this->type)
    {
    case Type::KEYWORD:
    case Type::STRING:
    case Type::SYMBOL:
      return std::get<std::string>(this->value) == std::get<std::string>(other.value);
    default:
      throw LispleException("== not implemented for type");
    }
  }

  RTValue to_rt_value(const Object& obj)
  {
    switch (obj.get_type())
    {
    case Form::NUMBER:
    {
      const auto& n = obj.as<Number>();
      switch (n.num_type)
      {
      case NumberType::INT:
        return RTValue::number(n.int_value());
      case NumberType::LONG:
        return RTValue::number(n.long_value());
      case NumberType::FLOAT:
        return RTValue::number(n.float_value());
      };
      throw LispleException("Unsupported NumberType: " + obj.to_string());
    }
    default:
      throw LispleException("Unsupported RTValue type: " + obj.to_string());
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

  std::pair<const RTValue*, const RTValue*> map_entry(const std::vector<RTValue>& map_data,
                                                      const RTValue& key)
  {
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      if (map_data[i] == key)
      {
        return {&map_data[i], &map_data[i + 1]};
      }
    }

    return {nullptr, nullptr};
  }

  std::pair<RTValue*, RTValue*> map_entry(std::vector<RTValue>& map_data, const RTValue& key)
  {
    for (size_t i = 0; i < map_data.size(); i += 2)
    {
      if (map_data[i] == key)
      {
        return {&map_data[i], &map_data[i + 1]};
      }
    }

    return {nullptr, nullptr};
  }

} // namespace Lisple
