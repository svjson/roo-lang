#ifndef LISPLE__VALUE_H
#define LISPLE__VALUE_H

#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

namespace Lisple
{
  extern int rtvalues_constructed;
  extern int rtvalue_wrappers_constructed;
  extern int to_ast_conversions;
  extern int to_rtvalue_conversions;

  namespace AST
  {
    class ASTNode;
  }
  class Executable;
  struct Value;
  template <class T> class HostObject;
  struct NativeObjectBase;

  using sptr_val = std::shared_ptr<Value>;
  using sptr_val_v = std::vector<std::shared_ptr<Value>>;
  using sptr_executable = std::shared_ptr<Executable>;
  using sptr_sobject = std::shared_ptr<AST::ASTNode>;
  using sptr_native_obj = std::shared_ptr<NativeObjectBase>;

  struct Value
  {
    enum class Type : uint8_t
    {
      ANY = 0x00,
      NIL = 0x01,
      NUMBER = 0x02,
      STRING = 0x03,
      CHAR = 0x04,
      BOOL = 0x05,
      SYMBOL = 0x06,
      KEYWORD = 0x07,
      LIST = 0x08,
      VECTOR = 0x09,
      MAP = 0x0a,
      FUNCTION = 0x0b,
      OBJECT = 0x0c,
      NATIVE_OBJECT = 0x0d
    };

    enum class NumberType : uint8_t
    {
      INT,
      LONG,
      FLOAT
    };

    struct Number
    {
      NumberType num_type;

      union
      {
        int32_t int_value;
        int64_t long_value;
        double float_value;
      };

      unsigned short get_unsigned_short() const;
      int get_int() const;
      long get_long() const;
      float get_float() const;
      double get_double() const;

      Number operator+(const Number& other) const;
      Number operator-(const Number& other) const;
      Number operator*(const Number& other) const;
      Number operator/(const Number& other) const;
    };

    using Data = std::variant<sptr_executable,
                              sptr_sobject,
                              std::string,
                              const Value::Number,
                              sptr_val_v,
                              sptr_native_obj,
                              bool,
                              char,
                              std::monostate>;

    Value() = default;
    explicit Value(int);
    Value(const Value::Number&);
    explicit Value(bool);
    Value(const std::string&, Type type);
    Value(std::monostate);

    Value::Data value;
    Value::Type type;

    bool operator==(const Value& other) const;

    static sptr_val boolean(bool);
    static sptr_val number(int);
    static sptr_val number(long);
    static sptr_val number(double);
    static sptr_val number(const Value::Number&);
    static sptr_val string(const std::string&);
    static sptr_val character(char);
    static sptr_val keyword(const std::string&);
    static sptr_val symbol(const std::string&);
    static sptr_val list(const sptr_val_v&);
    static sptr_val vector(const sptr_val_v&);
    static sptr_val map(const sptr_val_v&);
    static sptr_val object(const sptr_sobject&);
    static sptr_val native_object(const sptr_native_obj&);
    static sptr_val executable(const sptr_executable&);

    static std::string to_string(const sptr_val_v&);
    std::string to_string() const;

    uint8_t ui8() const;
    unsigned short ui16() const;
    int i32() const;
    long i64() const;
    float f32() const;
    double f64() const;
    char ch() const;
    const Value::Number& num() const;
    const std::string& str() const;
    sptr_sobject obj() const;
    sptr_native_obj nobj() const;
    Executable& exec() const;
    const sptr_val_v& elements() const;
    sptr_val_v& mut_elements();
    template <typename T> T& adapter()
    {
      return *dynamic_cast<T*>(std::get<sptr_native_obj>(value).get());
    }
    std::pair<std::string, std::string> qual() const;

    bool is_number(int) const;
    bool is_number(long) const;
    bool is_number(double) const;
    bool is_string(const std::string&) const;
  };

  namespace Constant
  {
    inline const sptr_val BOOL_TRUE = std::make_shared<Value>(true);
    inline const sptr_val BOOL_FALSE = std::make_shared<Value>(false);
    inline const sptr_val NIL = std::make_shared<Value>(std::monostate());
  } // namespace Constant

  sptr_val to_rt_value(sptr_sobject& obj);
  sptr_val to_rt_value(const AST::ASTNode& obj);
  sptr_sobject to_AST(Value& val);
  bool is_truthy(const Value& val);

} // namespace Lisple

#endif /* LISPLE__VALUE_H */
