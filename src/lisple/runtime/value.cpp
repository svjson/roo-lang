
#include "lisple/runtime/value.h"

#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/host/object.h>
#include <lisple/impl.h>
#include <lisple/runtime/pool.h>

namespace Lisple
{
  int rtvalues_constructed = 0;
  int rtvalue_wrappers_constructed = 0;
  int to_ast_conversions = 0;
  int to_rtvalue_conversions = 0;

  std::string rt_type_name(RTValue::Type type)
  {
    switch (type)
    {
    case RTValue::Type::ANY:
      return "ANY";
    case RTValue::Type::NIL:
      return "NIL";
    case RTValue::Type::NUMBER:
      return "NUMBER";
    case RTValue::Type::STRING:
      return "STRING";
    case RTValue::Type::CHAR:
      return "CHAR";
    case RTValue::Type::BOOL:
      return "BOOL";
    case RTValue::Type::SYMBOL:
      return "SYMBOL";
    case RTValue::Type::KEYWORD:
      return "KEYWORD";
    case RTValue::Type::LIST:
      return "LIST";
    case RTValue::Type::VECTOR:
      return "VECTOR";
    case RTValue::Type::MAP:
      return "MAP";
    case RTValue::Type::FUNCTION:
      return "FUNCTION";
    case RTValue::Type::OBJECT:
      return "OBJECT";
    case RTValue::Type::NATIVE_OBJECT:
      return "NATIVE_OBJECT";
    }
    return "<unknown>";
  }

  void throw_bad_rt_variant(const RTValue& value, const std::string& accessor)
  {
    throw TypeError("RTValue::" + accessor + " cannot read " + rt_type_name(value.type) +
                    " with variant index " + std::to_string(value.value.index()) + ": " +
                    value.to_string());
  }

  RTValue::RTValue(int v)
    : value(RTValue::Number{.num_type = RTValue::NumberType::INT, .int_value = v})
    , type(RTValue::Type::NUMBER)
  {
    rtvalues_constructed++;
  }

  RTValue::RTValue(const RTValue::Number& num)
    : value(num)
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

  unsigned short RTValue::Number::get_unsigned_short() const
  {
    switch (num_type)
    {
    case NumberType::INT:
      return static_cast<unsigned short>(int_value);
    case NumberType::LONG:
      return static_cast<unsigned short>(long_value);
    case NumberType::FLOAT:
    default:
      return static_cast<unsigned short>(float_value);
    }
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

  RTValue::Number RTValue::Number::operator+(const Number& other) const
  {
    switch (num_type)
    {
    case NumberType::INT:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::INT,
                               .int_value = int_value + other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = int_value + other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = int_value + other.float_value};
      }
    case NumberType::LONG:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = long_value + other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = long_value + other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = long_value + other.float_value};
      }
    case NumberType::FLOAT:
    default:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value + other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value + other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value + other.float_value};
      }
    }
  }

  RTValue::Number RTValue::Number::operator-(const Number& other) const
  {
    switch (num_type)
    {
    case NumberType::INT:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::INT,
                               .int_value = int_value - other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = int_value - other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = int_value - other.float_value};
      }
    case NumberType::LONG:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = long_value - other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = long_value - other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = long_value - other.float_value};
      }
    case NumberType::FLOAT:
    default:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value - other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value - other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value - other.float_value};
      }
    }
  }

  RTValue::Number RTValue::Number::operator*(const Number& other) const
  {
    switch (num_type)
    {
    case NumberType::INT:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::INT,
                               .int_value = int_value * other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = int_value * other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = int_value * other.float_value};
      }
    case NumberType::LONG:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = long_value * other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::LONG,
                               .long_value = long_value * other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = long_value * other.float_value};
      }
    case NumberType::FLOAT:
    default:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value * other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value * other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value * other.float_value};
      }
    }
  }

  RTValue::Number RTValue::Number::operator/(const Number& other) const
  {
    switch (num_type)
    {
    case NumberType::INT:
      switch (other.num_type)
      {
      case NumberType::INT:
        if (int_value % other.int_value == 0)
          return RTValue::Number{.num_type = NumberType::INT,
                                 .int_value = int_value / other.int_value};
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = (double)int_value / other.int_value};
      case NumberType::LONG:
        if (int_value % other.long_value == 0)
          return RTValue::Number{.num_type = NumberType::LONG,
                                 .long_value = int_value / other.long_value};
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = (double)int_value / other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = int_value / other.float_value};
      }
    case NumberType::LONG:
      switch (other.num_type)
      {
      case NumberType::INT:
        if (long_value % other.int_value == 0)
          return RTValue::Number{.num_type = NumberType::LONG,
                                 .long_value = long_value / other.int_value};
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = (double)long_value / other.int_value};
      case NumberType::LONG:
        if (long_value % other.long_value == 0)
          return RTValue::Number{.num_type = NumberType::LONG,
                                 .long_value = long_value / other.long_value};
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = (double)long_value / other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = long_value / other.float_value};
      }
    case NumberType::FLOAT:
    default:
      switch (other.num_type)
      {
      case NumberType::INT:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value / other.int_value};
      case NumberType::LONG:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value / other.long_value};
      case NumberType::FLOAT:
      default:
        return RTValue::Number{.num_type = NumberType::FLOAT,
                               .float_value = float_value / other.float_value};
      }
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
    return std::make_shared<RTValue>(
      RTValue::Number{.num_type = NumberType::LONG, .long_value = v});
  }

  sptr_rtval RTValue::number(const RTValue::Number& num)
  {
    if (num.num_type == NumberType::INT)
    {
      return IntegerPool::get(num.int_value);
    }
    return std::make_shared<RTValue>(num);
  }

  sptr_rtval RTValue::number(double v)
  {
    int intval = static_cast<int>(v);
    if (intval == v)
    {
      return RTValue::number(intval);
    }
    rtvalues_constructed++;
    return std::make_shared<RTValue>(
      RTValue::Number{.num_type = NumberType::FLOAT, .float_value = v});
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

  sptr_rtval RTValue::native_object(const sptr_native_obj& o)
  {
    rtvalues_constructed++;
    sptr_rtval val = std::make_shared<RTValue>();
    val->type = RTValue::Type::NATIVE_OBJECT;
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
      auto num = std::get<const RTValue::Number>(value);
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
    case RTValue::Type::LIST:
      r += "(";
      r += to_string(std::get<sptr_rtval_v>(value));
      r += ")";
      break;
    case RTValue::Type::FUNCTION:
    case RTValue::Type::OBJECT:
      r += std::get<sptr_sobject>(value)->to_string();
      break;
    case RTValue::Type::NATIVE_OBJECT:
      r += this->nobj()->to_string();
      break;
    case RTValue::Type::VECTOR:
      r += "[";
      r += to_string(std::get<sptr_rtval_v>(value));
      r += "]";
      break;
    case RTValue::Type::STRING:
      r += "\"" + std::get<std::string>(value) + "\"";
      break;
    case RTValue::Type::SYMBOL:
      r += std::get<std::string>(value);
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
      return std::get<const RTValue::Number>(this->value).get_float() ==
             std::get<const RTValue::Number>(other.value).get_float();
    case Type::KEYWORD:
    case Type::STRING:
    case Type::SYMBOL:
      return std::get<std::string>(this->value) == std::get<std::string>(other.value);
    case Type::MAP:
    case Type::LIST:
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
    case Type::NATIVE_OBJECT:
    {
      if (this->nobj()->get_host_type() != other.nobj()->get_host_type()) return false;
      return this->to_string() == other.to_string();
    }
    default:
      throw LispleException("== not implemented for type: " +
                            std::to_string((int)this->type));
    }
  }

  uint8_t RTValue::ui8() const
  {
    const auto* number = std::get_if<const RTValue::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "ui8");
    return number->get_int();
  }

  unsigned short RTValue::ui16() const
  {
    const auto* number = std::get_if<const RTValue::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "ui16");
    return static_cast<unsigned short>(number->get_int());
  }

  int RTValue::i32() const
  {
    const auto* number = std::get_if<const RTValue::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "i32");
    return number->get_int();
  }

  long RTValue::i64() const
  {
    const auto* number = std::get_if<const RTValue::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "i64");
    return number->get_long();
  }

  float RTValue::f32() const
  {
    const auto* number = std::get_if<const RTValue::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "f32");
    return number->get_float();
  }

  double RTValue::f64() const
  {
    const auto* number = std::get_if<const RTValue::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "f64");
    return number->get_double();
  }

  char RTValue::ch() const
  {
    return std::get<char>(value);
  }

  sptr_sobject RTValue::obj() const
  {
    const auto* object = std::get_if<sptr_sobject>(&value);
    if (!object) throw_bad_rt_variant(*this, "obj");
    return *object;
  }

  sptr_native_obj RTValue::nobj() const
  {
    const auto* object = std::get_if<sptr_native_obj>(&value);
    if (!object) throw_bad_rt_variant(*this, "nobj");
    return *object;
  }

  const RTValue::Number& RTValue::num() const
  {
    const auto* number = std::get_if<const RTValue::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "num");
    return *number;
  }

  const sptr_rtval_v& RTValue::elements() const
  {
    const auto* elements = std::get_if<sptr_rtval_v>(&value);
    if (!elements) throw_bad_rt_variant(*this, "elements");
    return *elements;
  }

  sptr_rtval_v& RTValue::mut_elements()
  {
    auto* elements = std::get_if<sptr_rtval_v>(&value);
    if (!elements) throw_bad_rt_variant(*this, "mut_elements");
    return *elements;
  }

  Executable& RTValue::exec() const
  {
    return *dynamic_cast<Executable*>(std::get<sptr_sobject>(value).get());
  }

  const std::string& RTValue::str() const
  {
    const auto* str = std::get_if<std::string>(&value);
    if (!str) throw_bad_rt_variant(*this, "str");
    return *str;
  }

  std::pair<std::string, std::string> RTValue::qual() const
  {
    return Lisple::split_qualifiable(str());
  }

  bool RTValue::is_number(int n) const
  {
    if (type == RTValue::Type::NUMBER)
    {
      auto& num = std::get<const RTValue::Number>(value);
      if (num.num_type == NumberType::INT)
      {
        return n == num.int_value;
      }
    }
    return false;
  }

  bool RTValue::is_number(long n) const
  {
    if (type == RTValue::Type::NUMBER)
    {
      auto& num = std::get<const RTValue::Number>(value);
      if (num.num_type == NumberType::LONG)
      {
        return n == num.long_value;
      }
    }
    return false;
  }

  bool RTValue::is_number(double n) const
  {
    if (type == RTValue::Type::NUMBER)
    {
      auto& num = std::get<const RTValue::Number>(value);
      if (num.num_type == NumberType::FLOAT)
      {
        return n == num.float_value;
      }
    }
    return false;
  }

  bool RTValue::is_string(const std::string& s) const
  {
    if (type == RTValue::Type::STRING)
    {
      return str() == s;
    }
    return false;
  }

  sptr_rtval to_rt_value(const Object& obj)
  {
    if (auto* wrapper = dynamic_cast<const RuntimeValueWrapper*>(&obj)) return wrapper->val;

    switch (obj.get_type())
    {
    case Form::VECTOR:
    {
      sptr_rtval_v elements;

      for (auto& c : dynamic_cast<const Vector*>(&obj)->children)
      {
        elements.push_back(to_rt_value(c));
      }

      return RTValue::vector(std::move(elements));
    }
    case Form::LIST:
    {
      sptr_rtval_v elements;

      for (auto& c : dynamic_cast<const List*>(&obj)->children)
      {
        elements.push_back(to_rt_value(c));
      }

      return RTValue::list(std::move(elements));
    }
    case Form::BOOLEAN:
      return RTValue::boolean(Value<bool>::value_of(obj));
    case Form::CHAR:
      return RTValue::character(Value<char>::value_of(obj));
    case Form::KEYWORD:
      return RTValue::keyword(Value<std::string>::value_of(obj));
    case Form::MAP:
    {
      sptr_rtval_v elements;

      for (auto& c : dynamic_cast<const Map*>(&obj)->children)
      {
        elements.push_back(to_rt_value(c));
      }

      return RTValue::map(std::move(elements));
    }
    case Form::NIL:
      return Constant::NIL;
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
      throw LispleException("to_rt_value: Unsupported NumberType: " + obj.to_string());
    }
    case Form::STRING:
      return RTValue::string(Value<std::string>::value_of(obj));
    case Form::QUOTED_SYMBOL:
      return RTValue::symbol(Value<std::string>::value_of(obj));
    case Form::SYMBOL:
      return RTValue::symbol(Value<std::string>::value_of(obj));
    default:
      throw LispleException("to_rt_value(Object&): Unsupported value type #" +
                            std::to_string(static_cast<int>(obj.get_type())));
    }
  }

  sptr_rtval to_rt_value(std::shared_ptr<Object>& obj)
  {
    if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(obj.get())) return wrapper->val;

    to_rtvalue_conversions++;
    switch (obj->get_type())
    {
    case Form::VECTOR:
    case Form::BOOLEAN:
    case Form::CHAR:
    case Form::MAP:
    case Form::LIST:
      return to_rt_value(*obj);
    case Form::FUNCTION:
      return RTValue::executable(obj);
    case Form::MACRO:
      return RTValue::object(obj);
    case Form::HOST_OBJECT:
      return RTValue::object(obj);
    case Form::HOST_SEQ:
      return RTValue::object(obj);
    case Form::KEYWORD:
      return RTValue::keyword(Value<std::string>::value_of(*obj));
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
    case Form::QUOTED_SYMBOL:
    case Form::SYMBOL:
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
        return Lisple::Keyword::make(*s);
      }
      else
      {
        throw LispleException("Invalid RTValue(KEYWORD)");
      }
    }
    case RTValue::Type::LIST:
    {
      sptr_sobject_v elements;

      for (auto& element : std::get<sptr_rtval_v>(val.value))
      {
        elements.push_back(to_AST(*element));
      }

      return Lisple::List::make(std::move(elements));
    }
    case RTValue::Type::MAP:
    {
      sptr_sobject_v elements;

      for (auto& element : std::get<sptr_rtval_v>(val.value))
      {
        elements.push_back(RuntimeValueWrapper::make(element));
      }

      return Lisple::Map::make(std::move(elements));
    }
    case RTValue::Type::NIL:
      return Lisple::NIL;
    case RTValue::Type::NUMBER:
    {
      if (const RTValue::Number* num = std::get_if<const RTValue::Number>(&val.value))
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
      return Lisple::Symbol::make(std::get<std::string>(val.value));
    case RTValue::Type::VECTOR:
    {
      sptr_sobject_v elements;

      for (auto& element : std::get<sptr_rtval_v>(val.value))
      {
        elements.push_back(to_AST(*element));
      }

      return Lisple::Vector::make(std::move(elements));
    }

    default:
      throw LispleException("to_AST: Unsupported value type #" +
                            std::to_string(static_cast<int>(val.type)));
    }
  }

  bool is_truthy(const RTValue& val)
  {
    switch (val.type)
    {
    case RTValue::Type::BOOL:
      return std::get<bool>(val.value);
    case RTValue::Type::FUNCTION:
    case RTValue::Type::KEYWORD:
    case RTValue::Type::CHAR:
    case RTValue::Type::OBJECT:
    case RTValue::Type::NATIVE_OBJECT:
    case RTValue::Type::NUMBER:
    case RTValue::Type::SYMBOL:
    case RTValue::Type::STRING:
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
