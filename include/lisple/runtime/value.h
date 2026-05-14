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
  struct RTValue;
  template <class T> class HostObject;
  struct NativeObjectBase;

  using sptr_rtval = std::shared_ptr<RTValue>;
  using sptr_rtval_v = std::vector<std::shared_ptr<RTValue>>;
  using sptr_sobject = std::shared_ptr<AST::ASTNode>;
  using sptr_native_obj = std::shared_ptr<NativeObjectBase>;

  struct RTValue
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

    using Data = std::variant<sptr_sobject,
                              std::string,
                              const RTValue::Number,
                              sptr_rtval_v,
                              sptr_native_obj,
                              bool,
                              char,
                              std::monostate>;

    RTValue() = default;
    explicit RTValue(int);
    RTValue(const RTValue::Number&);
    explicit RTValue(bool);
    RTValue(const std::string&, Type type);
    RTValue(std::monostate);

    RTValue::Data value;
    RTValue::Type type;

    bool operator==(const RTValue& other) const;

    static sptr_rtval boolean(bool);
    static sptr_rtval number(int);
    static sptr_rtval number(long);
    static sptr_rtval number(double);
    static sptr_rtval number(const RTValue::Number&);
    static sptr_rtval string(const std::string&);
    static sptr_rtval character(char);
    static sptr_rtval keyword(const std::string&);
    static sptr_rtval symbol(const std::string&);
    static sptr_rtval list(const sptr_rtval_v&);
    static sptr_rtval vector(const sptr_rtval_v&);
    static sptr_rtval map(const sptr_rtval_v&);
    static sptr_rtval object(const sptr_sobject&);
    static sptr_rtval native_object(const sptr_native_obj&);
    static sptr_rtval executable(const sptr_sobject&);

    static std::string to_string(const sptr_rtval_v&);
    std::string to_string() const;

    uint8_t ui8() const;
    unsigned short ui16() const;
    int i32() const;
    long i64() const;
    float f32() const;
    double f64() const;
    char ch() const;
    const RTValue::Number& num() const;
    const std::string& str() const;
    sptr_sobject obj() const;
    sptr_native_obj nobj() const;
    Executable& exec() const;
    const sptr_rtval_v& elements() const;
    sptr_rtval_v& mut_elements();
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
    inline const sptr_rtval BOOL_TRUE = std::make_shared<RTValue>(true);
    inline const sptr_rtval BOOL_FALSE = std::make_shared<RTValue>(false);
    inline const sptr_rtval NIL = std::make_shared<RTValue>(std::monostate());
  } // namespace Constant

  sptr_rtval to_rt_value(sptr_sobject& obj);
  sptr_rtval to_rt_value(const AST::ASTNode& obj);
  sptr_sobject to_AST(RTValue& val);
  bool is_truthy(const RTValue& val);

} // namespace Lisple

#endif /* LISPLE__VALUE_H */
