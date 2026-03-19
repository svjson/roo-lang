
#include "value.h"

#include "../exception.h"
#include "../form.h"
#include "pool.h"

namespace Lisple
{
  RTValue::RTValue(int v)
    : value(RTValue::Number{.num_type = RTValue::NumberType::INT, .int_value = v})
    , type(RTValue::Type::NUMBER)
  {
  }

  RTValue::RTValue(const std::string& s, Type type)
    : value(s)
    , type(type)
  {
  }

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

  sptr_rtval RTValue::nil()
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::NIL;
    val->value = std::monostate{};
    return val;
  }

  sptr_rtval RTValue::boolean(bool b)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::BOOL;
    val->value = b;
    return val;
  }

  sptr_rtval RTValue::number(int v)
  {
    return IntegerPool::get(v);
  }

  sptr_rtval RTValue::number(long v)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::NUMBER;
    val->value = RTValue::Number{.num_type = NumberType::LONG, .long_value = v};
    return val;
  }

  sptr_rtval RTValue::number(double v)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::NUMBER;
    val->value = RTValue::Number{.num_type = NumberType::FLOAT, .float_value = v};
    return val;
  }

  sptr_rtval RTValue::character(char c)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::CHAR;
    val->value = c;
    return val;
  }

  sptr_rtval RTValue::string(const std::string& v)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::STRING;
    val->value = v;
    return val;
  }

  sptr_rtval RTValue::keyword(const std::string& v)
  {
    return KeywordPool::get(v);
  }

  sptr_rtval RTValue::symbol(const std::string& v)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::SYMBOL;
    val->value = v;
    return val;
  }

  sptr_rtval RTValue::object(Object* o)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::OBJECT;
    val->value = o;
    return val;
  }

  sptr_rtval RTValue::list(sptr_rtval_v& v)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::LIST;
    val->value = v;
    return val;
  }

  sptr_rtval RTValue::vector(sptr_rtval_v& v)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::VECTOR;
    val->value = v;
    return val;
  }

  sptr_rtval RTValue::map(sptr_rtval_v& v)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::MAP;
    val->value = v;
    return val;
  }

  sptr_rtval RTValue::function(Function* fn)
  {
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::FUNCTION;
    val->value = fn;
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

  sptr_rtval to_rt_value(const Object& obj)
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

} // namespace Lisple
