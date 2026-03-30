
#include "value.h"

#include "../exception.h"
#include "../form.h"
#include "pool.h"

#include "lisple/impl.h"

namespace Lisple
{
  int rtvalues_constructed = 0;
  int rtvalue_wrappers_constructed = 0;
  int to_ast_conversions = 0;
  int to_rtvalue_conversions = 0;

  RTValue::RTValue(int v)
    : value(RTValue::Number{.num_type = RTValue::NumberType::INT, .int_value = v})
    , type(RTValue::Type::NUMBER)
  {
    rtvalues_constructed++;
  }

  RTValue::RTValue(const std::string& s, Type type)
    : value(s)
    , type(type)
  {
    rtvalues_constructed++;
  }

  RTValue::RTValue(bool v)
    : value(v)
    , type(RTValue::Type::BOOL)
  {
    rtvalues_constructed++;
  }

  RTValue::RTValue(std::monostate)
    : value(std::monostate())
    , type(RTValue::Type::NIL)
  {
    rtvalues_constructed++;
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

  long RTValue::Number::get_long() const
  {
    switch (num_type)
    {
    case NumberType::INT:
      return static_cast<long>(int_value);
    case NumberType::LONG:
      return long_value;
    case NumberType::FLOAT:
    default:
      return float_value;
    }
  }

  float RTValue::Number::get_float() const
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

  double RTValue::Number::get_double() const
  {
    switch (num_type)
    {
    case NumberType::INT:
      return int_value;
    case NumberType::LONG:
      return long_value;
    case NumberType::FLOAT:
    default:
      return float_value;
    }
  }

  sptr_rtval RTValue::boolean(bool b)
  {
    return b ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  sptr_rtval RTValue::number(int v)
  {
    return IntegerPool::get(v);
  }

  sptr_rtval RTValue::number(long v)
  {
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::NUMBER;
    val->value = RTValue::Number{.num_type = NumberType::LONG, .long_value = v};
    return val;
  }

  sptr_rtval RTValue::number(double v)
  {
    int intval = static_cast<int>(v);
    if (intval == v)
    {
      return RTValue::number(intval);
    }
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::NUMBER;
    val->value = RTValue::Number{.num_type = NumberType::FLOAT, .float_value = v};
    return val;
  }

  sptr_rtval RTValue::character(char c)
  {
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::CHAR;
    val->value = c;
    return val;
  }

  sptr_rtval RTValue::string(const std::string& v)
  {
    rtvalues_constructed++;
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
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::SYMBOL;
    val->value = v;
    return val;
  }

  sptr_rtval RTValue::object(const sptr_sobject& o)
  {
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::OBJECT;
    val->value = o;
    return val;
  }

  sptr_rtval RTValue::list(const sptr_rtval_v& v)
  {
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::LIST;
    val->value = v;
    return val;
  }

  sptr_rtval RTValue::vector(const sptr_rtval_v& v)
  {
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::VECTOR;
    val->value = v;
    return val;
  }

  sptr_rtval RTValue::map(const sptr_rtval_v& v)
  {
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::MAP;
    val->value = v;

    // std::cout << "===> CREATING NEW MAP (" << val.get() << ")" << std::endl;
    // for (size_t i = 0; i < v.size(); i += 2)
    // {
    //   std::cout << " - key(" << v[i].get() << "): " << v[i]->to_string() << std::endl;
    //   std::cout << " - val(" << v[i + 1].get() << "): " << v[i + 1]->to_string()
    //             << std::endl;
    // }

    return val;
  }

  sptr_rtval RTValue::executable(const sptr_sobject& fn)
  {
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::FUNCTION;
    val->value = fn;
    return val;
  }

  std::string RTValue::to_string(const sptr_rtval_v& values)
  {
    std::string r;
    for (size_t i = 0; i < values.size(); i++)
    {
      if (i > 0) r += " ";
      r += values[i]->to_string();
    }
    return r;
  }

  std::string RTValue::to_string() const
  {
    std::string r = "";

    switch (type)
    {
    case RTValue::Type::BOOL:
      r += std::get<bool>(value) ? "true" : "false";
      break;
    case RTValue::Type::CHAR:
      r += "'" + std::string(1, std::get<char>(value)) + "'";
      break;
    case RTValue::Type::KEYWORD:
      r += ":" + std::get<std::string>(value);
      break;
    case RTValue::Type::MAP:
      r += "{";
      r += to_string(std::get<sptr_rtval_v>(value));
      r += "}";
      break;
    case RTValue::Type::NIL:
      r += "nil";
      break;
    case RTValue::Type::NUMBER:
    {
      auto num = std::get<RTValue::Number>(value);
      switch (num.num_type)
      {
      case NumberType::INT:
        r += std::to_string(num.int_value);
        break;
      case NumberType::LONG:
        r += std::to_string(num.long_value);
        break;
      case NumberType::FLOAT:
        r += std::to_string(num.float_value);
        break;
      }
    }
    break;
    case RTValue::Type::FUNCTION:
    case RTValue::Type::OBJECT:
      r += std::get<sptr_sobject>(value)->to_string();
      break;
    case RTValue::Type::VECTOR:
      r += "[";
      r += to_string(std::get<sptr_rtval_v>(value));
      r += "]";
      break;
    case RTValue::Type::STRING:
    case RTValue::Type::SYMBOL:
      r += to_AST(*const_cast<RTValue*>(this))->to_string();
      break;
    default:
      r += "<to_string not implemented for #" + std::to_string((int)type) + ">";
    }

    return r;
  }

  bool RTValue::operator==(const RTValue& other) const
  {
    if (this->type != other.type) return false;

    switch (this->type)
    {
    case Type::BOOL:
      return std::get<bool>(this->value) == std::get<bool>(other.value);
    case Type::CHAR:
      return std::get<char>(this->value) == std::get<char>(other.value);
    case Type::NIL:
      return other.type == Type::NIL;
    case Type::NUMBER:
      return std::get<RTValue::Number>(this->value).get_float() ==
             std::get<RTValue::Number>(other.value).get_float();
    case Type::KEYWORD:
    case Type::STRING:
    case Type::SYMBOL:
      return std::get<std::string>(this->value) == std::get<std::string>(other.value);
    case Type::VECTOR:
    {
      const sptr_rtval_v& a = std::get<sptr_rtval_v>(value);
      const sptr_rtval_v& b = std::get<sptr_rtval_v>(other.value);
      if (a.size() != b.size()) return false;
      for (size_t i = 0; i < a.size(); i++)
      {
        if (*a[i] != *b[i]) return false;
      }
      return true;
    }
    default:
      throw LispleException("== not implemented for type: " +
                            std::to_string((int)this->type));
    }
  }

  uint8_t RTValue::ui8() const
  {
    return std::get<Number>(value).get_int();
  }

  int RTValue::i32() const
  {
    return std::get<Number>(value).get_int();
  }

  long RTValue::i64() const
  {
    return std::get<Number>(value).get_long();
  }

  float RTValue::f32() const
  {
    return std::get<Number>(value).get_float();
  }

  double RTValue::f64() const
  {
    return std::get<Number>(value).get_double();
  }

  sptr_sobject RTValue::obj() const
  {
    return std::get<sptr_sobject>(value);
  }

  const std::string& RTValue::str() const
  {
    return std::get<std::string>(value);
  }

  std::pair<std::string, std::string> RTValue::qual() const
  {
    return Lisple::split_qualifiable(str());
  }

  sptr_rtval to_rt_value(std::shared_ptr<Object>& obj)
  {
    if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(obj.get())) return wrapper->val;

    to_rtvalue_conversions++;
    switch (obj->get_type())
    {
    case Form::ARRAY:
    {
      sptr_rtval_v elements;

      for (auto& c : obj->get_children())
      {
        elements.push_back(to_rt_value(c));
      }

      return RTValue::vector(elements);
    }
    case Form::BOOLEAN:
      return RTValue::boolean(Value<bool>::value_of(*obj));
      // To allow polymorphic invocation of Object::execute, all functions and executables
      // are stored as Object* for the time being.
    case Form::CHAR:
      return RTValue::character(Value<char>::value_of(*obj));
    case Form::FUNCTION:
      return RTValue::executable(obj);
    case Form::MACRO:
      return RTValue::object(obj);
    case Form::HOST_OBJECT:
      return RTValue::object(obj);
    case Form::HOST_SEQ:
      return RTValue::object(obj);
    case Form::KEY:
      return RTValue::keyword(Value<std::string>::value_of(*obj));
    case Form::MAP:
    {
      sptr_rtval_v elements;

      for (auto& c : obj->get_children())
      {
        elements.push_back(to_rt_value(c));
      }

      return RTValue::map(elements);
    }
    case Form::NIL:
      return Constant::NIL;
    case Form::NUMBER:
    {
      const auto& n = obj->as<Number>();
      switch (n.num_type)
      {
      case NumberType::INT:
        return RTValue::number(n.int_value());
      case NumberType::LONG:
        return RTValue::number(n.long_value());
      case NumberType::FLOAT:
        return RTValue::number(n.float_value());
      };
      throw LispleException("to_rt_value: Unsupported NumberType: " + obj->to_string());
    }
    case Form::STRING:
      return RTValue::string(Value<std::string>::value_of(*obj));
    case Form::SYMBOL:
    case Form::WORD:
      return RTValue::symbol(Value<std::string>::value_of(*obj));
    default:
      throw LispleException("to_rt_value: Unsupported RTValue type(" +
                            std::to_string((int)obj->get_type()) + "): " + obj->to_string());
    }
  }

  sptr_sobject to_AST(RTValue& val)
  {
    to_ast_conversions++;
    switch (val.type)
    {
    case RTValue::Type::BOOL:
    {
      if (bool* bv = std::get_if<bool>(&val.value))
      {
        return Lisple::Boolean::wrap(*bv);
      }
      else
      {
        throw LispleException("Invalid RTValue(BOOL)");
      }
    }
    case RTValue::Type::CHAR:
      return Char::make(std::get<char>(val.value));
    case RTValue::Type::FUNCTION:
    {
      return std::get<sptr_sobject>(val.value);
    }
    case RTValue::Type::KEYWORD:
    {
      if (std::string* s = std::get_if<std::string>(&val.value))
      {
        return Lisple::Key::make(*s);
      }
      else
      {
        throw LispleException("Invalid RTValue(KEYWORD)");
      }
    }
    case RTValue::Type::MAP:
    {
      sptr_sobject_v elements;

      for (auto& element : std::get<sptr_rtval_v>(val.value))
      {
        elements.push_back(RuntimeValueWrapper::make(element));
      }

      return Lisple::Map::make(elements);
    }
    case RTValue::Type::NIL:
      return Lisple::NIL;
    case RTValue::Type::NUMBER:
    {
      if (RTValue::Number* num = std::get_if<RTValue::Number>(&val.value))
      {
        switch (num->num_type)
        {
        case RTValue::NumberType::INT:
          return Lisple::Number::make(num->int_value);
        case RTValue::NumberType::LONG:
          return Lisple::Number::make(num->long_value);
        case RTValue::NumberType::FLOAT:
          return Lisple::Number::make(num->float_value);
        }
      }

      throw LispleException("Invalid RTValue(NUMBER)");
    }
    case RTValue::Type::OBJECT:
      return std::get<sptr_sobject>(val.value);
    case RTValue::Type::STRING:
      return Lisple::String::make(std::get<std::string>(val.value));
    case RTValue::Type::SYMBOL:
      return Lisple::Word::make(std::get<std::string>(val.value));
    case RTValue::Type::VECTOR:
    {
      sptr_sobject_v elements;

      for (auto& element : std::get<sptr_rtval_v>(val.value))
      {
        elements.push_back(to_AST(*element));
      }

      return Lisple::Array::make(elements);
    }

    default:
      throw LispleException("to_AST: Unsupported value type #" +
                            std::to_string(static_cast<int>(val.type)));
    }
  }

  bool is_truthy(RTValue& val)
  {
    switch (val.type)
    {
    case RTValue::Type::BOOL:
      return std::get<bool>(val.value);
    case RTValue::Type::FUNCTION:
    case RTValue::Type::KEYWORD:
    case RTValue::Type::OBJECT:
    case RTValue::Type::NUMBER:
    case RTValue::Type::SYMBOL:
    case RTValue::Type::VECTOR:
    case RTValue::Type::MAP:
      return true;
    case RTValue::Type::NIL:
      return false;
    default:
      throw LispleException("is_truthy: Unsupported value type: #" +
                            std::to_string(static_cast<int>(val.type)));
    }
  }

} // namespace Lisple
