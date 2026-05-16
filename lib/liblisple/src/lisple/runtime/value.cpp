
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

  std::string val_type_name(Value::Type type)
  {
    switch (type)
    {
    case Value::Type::ANY:
      return "ANY";
    case Value::Type::NIL:
      return "NIL";
    case Value::Type::NUMBER:
      return "NUMBER";
    case Value::Type::STRING:
      return "STRING";
    case Value::Type::CHAR:
      return "CHAR";
    case Value::Type::BOOL:
      return "BOOL";
    case Value::Type::SYMBOL:
      return "SYMBOL";
    case Value::Type::KEYWORD:
      return "KEYWORD";
    case Value::Type::LIST:
      return "LIST";
    case Value::Type::VECTOR:
      return "VECTOR";
    case Value::Type::MAP:
      return "MAP";
    case Value::Type::FUNCTION:
      return "FUNCTION";
    case Value::Type::OBJECT:
      return "OBJECT";
    case Value::Type::NATIVE_OBJECT:
      return "NATIVE_OBJECT";
    }
    return "<unknown>";
  }

  std::string_view type_string(Value& value)
  {
    switch (value.type)
    {
    case Value::Type::NIL:
      return "nil";
    case Value::Type::NUMBER:
      return "number";
    case Value::Type::STRING:
      return "string";
    case Value::Type::CHAR:
      return "char";
    case Value::Type::BOOL:
      return "boolean";
    case Value::Type::SYMBOL:
      return "symbol";
    case Value::Type::KEYWORD:
      return "keyword";
    case Value::Type::LIST:
      return "list";
    case Value::Type::VECTOR:
      return "vector";
    case Value::Type::MAP:
      return "map";
    case Value::Type::FUNCTION:
      return "executable";
    case Value::Type::OBJECT:
      return "object";
    case Value::Type::NATIVE_OBJECT:
      return value.nobj()->get_host_type()->to_string();
    default:
      return "invalid";
    }
  }

  void throw_bad_rt_variant(const Value& value, const std::string& accessor)
  {
    throw TypeError("Value::" + accessor + " cannot read " + val_type_name(value.type) +
                    " with variant index " + std::to_string(value.value.index()) + ": " +
                    value.to_string());
  }

  Value::Value(int v)
    : value(Value::Number{.num_type = Value::NumberType::INT, .int_value = v})
    , type(Value::Type::NUMBER)
  {
    rtvalues_constructed++;
  }

  Value::Value(const Value::Number& num)
    : value(num)
    , type(Value::Type::NUMBER)
  {
    rtvalues_constructed++;
  }

  Value::Value(const std::string& s, Type type)
    : value(s)
    , type(type)
  {
    rtvalues_constructed++;
  }

  Value::Value(bool v)
    : value(v)
    , type(Value::Type::BOOL)
  {
    rtvalues_constructed++;
  }

  Value::Value(std::monostate)
    : value(std::monostate())
    , type(Value::Type::NIL)
  {
    rtvalues_constructed++;
  }

  unsigned short Value::Number::get_unsigned_short() const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      return static_cast<unsigned short>(int_value);
    case Value::NumberType::LONG:
      return static_cast<unsigned short>(long_value);
    case Value::NumberType::FLOAT:
    default:
      return static_cast<unsigned short>(float_value);
    }
  }

  int Value::Number::get_int() const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      return int_value;
    case Value::NumberType::LONG:
      return static_cast<int>(long_value);
    case Value::NumberType::FLOAT:
    default:
      return float_value;
    }
  }

  long Value::Number::get_long() const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      return static_cast<long>(int_value);
    case Value::NumberType::LONG:
      return long_value;
    case Value::NumberType::FLOAT:
    default:
      return float_value;
    }
  }

  float Value::Number::get_float() const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      return int_value;
    case Value::NumberType::LONG:
      return static_cast<int>(long_value);
    case Value::NumberType::FLOAT:
    default:
      return float_value;
    }
  }

  double Value::Number::get_double() const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      return int_value;
    case Value::NumberType::LONG:
      return long_value;
    case Value::NumberType::FLOAT:
    default:
      return float_value;
    }
  }

  Value::Number Value::Number::operator+(const Number& other) const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::INT,
                             .int_value = int_value + other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = int_value + other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = int_value + other.float_value};
      }
    case Value::NumberType::LONG:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = long_value + other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = long_value + other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = long_value + other.float_value};
      }
    case Value::NumberType::FLOAT:
    default:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value + other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value + other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value + other.float_value};
      }
    }
  }

  Value::Number Value::Number::operator-(const Number& other) const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::INT,
                             .int_value = int_value - other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = int_value - other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = int_value - other.float_value};
      }
    case Value::NumberType::LONG:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = long_value - other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = long_value - other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = long_value - other.float_value};
      }
    case Value::NumberType::FLOAT:
    default:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value - other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value - other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value - other.float_value};
      }
    }
  }

  Value::Number Value::Number::operator*(const Number& other) const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::INT,
                             .int_value = int_value * other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = int_value * other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = int_value * other.float_value};
      }
    case Value::NumberType::LONG:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = long_value * other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::LONG,
                             .long_value = long_value * other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = long_value * other.float_value};
      }
    case Value::NumberType::FLOAT:
    default:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value * other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value * other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value * other.float_value};
      }
    }
  }

  Value::Number Value::Number::operator/(const Number& other) const
  {
    switch (num_type)
    {
    case Value::NumberType::INT:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        if (int_value % other.int_value == 0)
          return Value::Number{.num_type = Value::NumberType::INT,
                               .int_value = int_value / other.int_value};
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = (double)int_value / other.int_value};
      case Value::NumberType::LONG:
        if (int_value % other.long_value == 0)
          return Value::Number{.num_type = Value::NumberType::LONG,
                               .long_value = int_value / other.long_value};
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = (double)int_value / other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = int_value / other.float_value};
      }
    case Value::NumberType::LONG:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        if (long_value % other.int_value == 0)
          return Value::Number{.num_type = Value::NumberType::LONG,
                               .long_value = long_value / other.int_value};
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = (double)long_value / other.int_value};
      case Value::NumberType::LONG:
        if (long_value % other.long_value == 0)
          return Value::Number{.num_type = Value::NumberType::LONG,
                               .long_value = long_value / other.long_value};
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = (double)long_value / other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = long_value / other.float_value};
      }
    case Value::NumberType::FLOAT:
    default:
      switch (other.num_type)
      {
      case Value::NumberType::INT:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value / other.int_value};
      case Value::NumberType::LONG:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value / other.long_value};
      case Value::NumberType::FLOAT:
      default:
        return Value::Number{.num_type = Value::NumberType::FLOAT,
                             .float_value = float_value / other.float_value};
      }
    }
  }

  sptr_val Value::boolean(bool b)
  {
    return b ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  sptr_val Value::number(int v)
  {
    return IntegerPool::get(v);
  }

  sptr_val Value::number(long v)
  {
    return std::make_shared<Value>(
      Value::Number{.num_type = Value::NumberType::LONG, .long_value = v});
  }

  sptr_val Value::number(const Value::Number& num)
  {
    if (num.num_type == Value::NumberType::INT)
    {
      return IntegerPool::get(num.int_value);
    }
    return std::make_shared<Value>(num);
  }

  sptr_val Value::number(double v)
  {
    int intval = static_cast<int>(v);
    if (intval == v)
    {
      return Value::number(intval);
    }
    rtvalues_constructed++;
    return std::make_shared<Value>(
      Value::Number{.num_type = Value::NumberType::FLOAT, .float_value = v});
  }

  sptr_val Value::character(char c)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::CHAR;
    val->value = c;
    return val;
  }

  sptr_val Value::string(const std::string& v)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::STRING;
    val->value = v;
    return val;
  }

  sptr_val Value::keyword(const std::string& v)
  {
    return KeywordPool::get(v);
  }

  sptr_val Value::symbol(const std::string& v)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::SYMBOL;
    val->value = v;
    return val;
  }

  sptr_val Value::object(const sptr_ast_node& o)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::OBJECT;
    val->value = o;
    return val;
  }

  sptr_val Value::native_object(const sptr_native_obj& o)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::NATIVE_OBJECT;
    val->value = o;
    return val;
  }

  sptr_val Value::list(const sptr_val_v& v)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::LIST;
    val->value = v;
    return val;
  }

  sptr_val Value::vector(const sptr_val_v& v)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::VECTOR;
    val->value = v;
    return val;
  }

  sptr_val Value::map(const sptr_val_v& v)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::MAP;
    val->value = v;

    return val;
  }

  sptr_val Value::executable(const sptr_executable& fn)
  {
    rtvalues_constructed++;
    sptr_val val = std::make_shared<Value>();
    val->type = Value::Type::FUNCTION;
    val->value = fn;
    return val;
  }

  std::string Value::to_string(const sptr_val_v& values)
  {
    std::string r;
    for (size_t i = 0; i < values.size(); i++)
    {
      if (i > 0) r += " ";
      r += values[i]->to_string();
    }
    return r;
  }

  std::string Value::to_string() const
  {
    std::string r = "";

    switch (type)
    {
    case Value::Type::BOOL:
      r += std::get<bool>(value) ? "true" : "false";
      break;
    case Value::Type::CHAR:
      r += "'" + std::string(1, std::get<char>(value)) + "'";
      break;
    case Value::Type::KEYWORD:
      r += ":" + std::get<std::string>(value);
      break;
    case Value::Type::MAP:
      r += "{";
      r += to_string(std::get<sptr_val_v>(value));
      r += "}";
      break;
    case Value::Type::NIL:
      r += "nil";
      break;
    case Value::Type::NUMBER:
    {
      auto num = std::get<const Value::Number>(value);
      switch (num.num_type)
      {
      case Value::NumberType::INT:
        r += std::to_string(num.int_value);
        break;
      case Value::NumberType::LONG:
        r += std::to_string(num.long_value);
        break;
      case Value::NumberType::FLOAT:
        r += std::to_string(num.float_value);
        break;
      }
    }
    break;
    case Value::Type::LIST:
      r += "(";
      r += to_string(std::get<sptr_val_v>(value));
      r += ")";
      break;
    case Value::Type::OBJECT:
      r += std::get<sptr_ast_node>(value)->to_string();
      break;
    case Value::Type::FUNCTION:
      r += std::get<sptr_executable>(value)->to_string();
      break;
    case Value::Type::NATIVE_OBJECT:
      r += this->nobj()->to_string();
      break;
    case Value::Type::VECTOR:
      r += "[";
      r += to_string(std::get<sptr_val_v>(value));
      r += "]";
      break;
    case Value::Type::STRING:
      r += "\"" + std::get<std::string>(value) + "\"";
      break;
    case Value::Type::SYMBOL:
      r += std::get<std::string>(value);
      break;
    default:
      r += "<to_string not implemented for #" + std::to_string((int)type) + ">";
    }

    return r;
  }

  bool Value::operator==(const Value& other) const
  {
    if (this->type != other.type)
    {
      if (this->type == Type::NATIVE_OBJECT) return this->nobj()->equals_value(other);
      if (other.type == Type::NATIVE_OBJECT) return other.nobj()->equals_value(*this);
      return false;
    }

    switch (this->type)
    {
    case Type::BOOL:
      return std::get<bool>(this->value) == std::get<bool>(other.value);
    case Type::CHAR:
      return std::get<char>(this->value) == std::get<char>(other.value);
    case Type::NIL:
      return other.type == Type::NIL;
    case Type::NUMBER:
      return std::get<const Value::Number>(this->value).get_float() ==
             std::get<const Value::Number>(other.value).get_float();
    case Type::KEYWORD:
    case Type::STRING:
    case Type::SYMBOL:
      return std::get<std::string>(this->value) == std::get<std::string>(other.value);
    case Type::MAP:
    case Type::LIST:
    case Type::VECTOR:
    {
      const sptr_val_v& a = std::get<sptr_val_v>(value);
      const sptr_val_v& b = std::get<sptr_val_v>(other.value);
      if (a.size() != b.size()) return false;
      for (size_t i = 0; i < a.size(); i++)
      {
        if (!(*a[i] == *b[i])) return false;
      }
      return true;
    }
    case Type::NATIVE_OBJECT:
      return this->nobj()->equals_value(other);
    default:
      throw LispleException("== not implemented for type: " +
                            std::to_string((int)this->type));
    }
  }

  uint8_t Value::ui8() const
  {
    const auto* number = std::get_if<const Value::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "ui8");
    return number->get_int();
  }

  unsigned short Value::ui16() const
  {
    const auto* number = std::get_if<const Value::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "ui16");
    return static_cast<unsigned short>(number->get_int());
  }

  int Value::i32() const
  {
    const auto* number = std::get_if<const Value::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "i32");
    return number->get_int();
  }

  long Value::i64() const
  {
    const auto* number = std::get_if<const Value::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "i64");
    return number->get_long();
  }

  float Value::f32() const
  {
    const auto* number = std::get_if<const Value::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "f32");
    return number->get_float();
  }

  double Value::f64() const
  {
    const auto* number = std::get_if<const Value::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "f64");
    return number->get_double();
  }

  char Value::ch() const
  {
    return std::get<char>(value);
  }

  sptr_ast_node Value::obj() const
  {
    const auto* object = std::get_if<sptr_ast_node>(&value);
    if (!object) throw_bad_rt_variant(*this, "obj");
    return *object;
  }

  sptr_native_obj Value::nobj() const
  {
    const auto* object = std::get_if<sptr_native_obj>(&value);
    if (!object) throw_bad_rt_variant(*this, "nobj");
    return *object;
  }

  const Value::Number& Value::num() const
  {
    const auto* number = std::get_if<const Value::Number>(&value);
    if (!number) throw_bad_rt_variant(*this, "num");
    return *number;
  }

  const sptr_val_v& Value::elements() const
  {
    const auto* elements = std::get_if<sptr_val_v>(&value);
    if (!elements) throw_bad_rt_variant(*this, "elements");
    return *elements;
  }

  sptr_val_v& Value::mut_elements()
  {
    auto* elements = std::get_if<sptr_val_v>(&value);
    if (!elements) throw_bad_rt_variant(*this, "mut_elements");
    return *elements;
  }

  sptr_executable Value::exec_ptr() const
  {
    const auto* executable = std::get_if<sptr_executable>(&value);
    if (!executable) throw_bad_rt_variant(*this, "exec_ptr");
    return *executable;
  }

  Executable& Value::exec() const
  {
    return *std::get<sptr_executable>(value);
  }

  const std::string& Value::str() const
  {
    const auto* str = std::get_if<std::string>(&value);
    if (!str) throw_bad_rt_variant(*this, "str");
    return *str;
  }

  std::pair<std::string, std::string> Value::qual() const
  {
    return Lisple::split_qualifiable(str());
  }

  bool Value::is_number(int n) const
  {
    if (type == Value::Type::NUMBER)
    {
      auto& num = std::get<const Value::Number>(value);
      if (num.num_type == Value::NumberType::INT)
      {
        return n == num.int_value;
      }
    }
    return false;
  }

  bool Value::is_number(long n) const
  {
    if (type == Value::Type::NUMBER)
    {
      auto& num = std::get<const Value::Number>(value);
      if (num.num_type == Value::NumberType::LONG)
      {
        return n == num.long_value;
      }
    }
    return false;
  }

  bool Value::is_number(double n) const
  {
    if (type == Value::Type::NUMBER)
    {
      auto& num = std::get<const Value::Number>(value);
      if (num.num_type == Value::NumberType::FLOAT)
      {
        return n == num.float_value;
      }
    }
    return false;
  }

  bool Value::is_string(const std::string& s) const
  {
    if (type == Value::Type::STRING)
    {
      return str() == s;
    }
    return false;
  }

  sptr_val to_rt_value(const AST::ASTNode& obj)
  {
    if (auto* wrapper = dynamic_cast<const AST::RuntimeValueWrapper*>(&obj))
      return wrapper->val;

    switch (obj.get_type())
    {
    case Form::VECTOR:
    {
      sptr_val_v elements;

      for (auto& c : dynamic_cast<const AST::Vector*>(&obj)->children)
      {
        elements.push_back(to_rt_value(c));
      }

      return Value::vector(std::move(elements));
    }
    case Form::LIST:
    {
      sptr_val_v elements;

      for (auto& c : dynamic_cast<const AST::List*>(&obj)->children)
      {
        elements.push_back(to_rt_value(c));
      }

      return Value::list(std::move(elements));
    }
    case Form::BOOLEAN:
      return Value::boolean(AST::Value<bool>::value_of(obj));
    case Form::CHAR:
      return Value::character(AST::Value<char>::value_of(obj));
    case Form::KEYWORD:
      return Value::keyword(AST::Value<std::string>::value_of(obj));
    case Form::MAP:
    {
      sptr_val_v elements;

      for (auto& c : dynamic_cast<const AST::Map*>(&obj)->children)
      {
        elements.push_back(to_rt_value(c));
      }

      return Value::map(std::move(elements));
    }
    case Form::NIL:
      return Constant::NIL;
    case Form::NUMBER:
    {
      const auto& n = obj.as<AST::Number>();
      switch (n.num_type)
      {
      case AST::NumberType::INT:
        return Value::number(n.int_value());
      case AST::NumberType::LONG:
        return Value::number(n.long_value());
      case AST::NumberType::FLOAT:
        return Value::number(n.float_value());
      };
      throw LispleException("to_rt_value: Unsupported AST::NumberType: " + obj.to_string());
    }
    case Form::STRING:
      return Value::string(AST::Value<std::string>::value_of(obj));
    case Form::QUOTED_SYMBOL:
      return Value::symbol(AST::Value<std::string>::value_of(obj));
    case Form::SYMBOL:
      return Value::symbol(AST::Value<std::string>::value_of(obj));
    default:
      throw LispleException("to_rt_value(AST::ASTNode&): Unsupported value type #" +
                            std::to_string(static_cast<int>(obj.get_type())));
    }
  }

  sptr_val to_rt_value(std::shared_ptr<AST::ASTNode>& obj)
  {
    if (auto* wrapper = dynamic_cast<AST::RuntimeValueWrapper*>(obj.get()))
      return wrapper->val;

    to_rtvalue_conversions++;
    switch (obj->get_type())
    {
    case Form::VECTOR:
    case Form::BOOLEAN:
    case Form::CHAR:
    case Form::MAP:
    case Form::LIST:
      return to_rt_value(*obj);
    case Form::HOST_OBJECT:
      return Value::object(obj);
    case Form::HOST_SEQ:
      return Value::object(obj);
    case Form::KEYWORD:
      return Value::keyword(AST::Value<std::string>::value_of(*obj));
    case Form::NIL:
      return Constant::NIL;
    case Form::NUMBER:
    {
      const auto& n = obj->as<AST::Number>();
      switch (n.num_type)
      {
      case AST::NumberType::INT:
        return Value::number(n.int_value());
      case AST::NumberType::LONG:
        return Value::number(n.long_value());
      case AST::NumberType::FLOAT:
        return Value::number(n.float_value());
      };
      throw LispleException("to_rt_value: Unsupported AST::NumberType: " + obj->to_string());
    }
    case Form::STRING:
      return Value::string(AST::Value<std::string>::value_of(*obj));
    case Form::QUOTED_SYMBOL:
    case Form::SYMBOL:
      return Value::symbol(AST::Value<std::string>::value_of(*obj));
    default:
      throw LispleException("to_rt_value: Unsupported Value type(" +
                            std::to_string((int)obj->get_type()) + "): " + obj->to_string());
    }
  }

  sptr_ast_node to_AST(Value& val)
  {
    to_ast_conversions++;
    switch (val.type)
    {
    case Value::Type::BOOL:
    {
      if (bool* bv = std::get_if<bool>(&val.value))
      {
        return Lisple::AST::Boolean::wrap(*bv);
      }
      else
      {
        throw LispleException("Invalid Value(BOOL)");
      }
    }
    case Value::Type::CHAR:
      return AST::Char::make(std::get<char>(val.value));
    case Value::Type::FUNCTION:
    {
      return std::make_shared<AST::RuntimeValueWrapper>(std::make_shared<Value>(val));
    }
    case Value::Type::KEYWORD:
    {
      if (std::string* s = std::get_if<std::string>(&val.value))
      {
        return Lisple::AST::Keyword::make(*s);
      }
      else
      {
        throw LispleException("Invalid Value(KEYWORD)");
      }
    }
    case Value::Type::LIST:
    {
      sptr_ast_node_v elements;

      for (auto& element : std::get<sptr_val_v>(val.value))
      {
        elements.push_back(to_AST(*element));
      }

      return Lisple::AST::List::make(std::move(elements));
    }
    case Value::Type::MAP:
    {
      sptr_ast_node_v elements;

      for (auto& element : std::get<sptr_val_v>(val.value))
      {
        elements.push_back(AST::RuntimeValueWrapper::make(element));
      }

      return Lisple::AST::Map::make(std::move(elements));
    }
    case Value::Type::NIL:
      return Lisple::AST::NIL;
    case Value::Type::NUMBER:
    {
      if (const Value::Number* num = std::get_if<const Value::Number>(&val.value))
      {
        switch (num->num_type)
        {
        case Value::NumberType::INT:
          return Lisple::AST::Number::make(num->int_value);
        case Value::NumberType::LONG:
          return Lisple::AST::Number::make(num->long_value);
        case Value::NumberType::FLOAT:
          return Lisple::AST::Number::make(num->float_value);
        }
      }

      throw LispleException("Invalid Value(NUMBER)");
    }
    case Value::Type::OBJECT:
      return std::get<sptr_ast_node>(val.value);
    case Value::Type::STRING:
      return Lisple::AST::String::make(std::get<std::string>(val.value));
    case Value::Type::SYMBOL:
      return Lisple::AST::Symbol::make(std::get<std::string>(val.value));
    case Value::Type::VECTOR:
    {
      sptr_ast_node_v elements;

      for (auto& element : std::get<sptr_val_v>(val.value))
      {
        elements.push_back(to_AST(*element));
      }

      return Lisple::AST::Vector::make(std::move(elements));
    }

    default:
      throw LispleException("to_AST: Unsupported value type #" +
                            std::to_string(static_cast<int>(val.type)));
    }
  }

  bool is_truthy(const Value& val)
  {
    switch (val.type)
    {
    case Value::Type::BOOL:
      return std::get<bool>(val.value);
    case Value::Type::FUNCTION:
    case Value::Type::KEYWORD:
    case Value::Type::CHAR:
    case Value::Type::OBJECT:
    case Value::Type::NATIVE_OBJECT:
    case Value::Type::NUMBER:
    case Value::Type::SYMBOL:
    case Value::Type::STRING:
    case Value::Type::VECTOR:
    case Value::Type::MAP:
      return true;
    case Value::Type::NIL:
      return false;
    default:
      throw LispleException("is_truthy: Unsupported value type: #" +
                            std::to_string(static_cast<int>(val.type)));
    }
  }

} // namespace Lisple
