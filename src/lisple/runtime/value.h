#ifndef LISPLE__VALUE_H
#define LISPLE__VALUE_H

#include <memory>
#include <variant>
#include <vector>

namespace Lisple
{
  extern int rtvalues_constructed;
  extern int rtvalue_wrappers_constructed;
  extern int to_ast_conversions;
  extern int to_rtvalue_conversions;

  class Object;
  class Executable;
  struct RTValue;

  using sptr_rtval = std::shared_ptr<RTValue>;
  using sptr_rtval_v = std::vector<std::shared_ptr<RTValue>>;
  using sptr_sobject = std::shared_ptr<Object>;

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
      OBJECT = 0x0c
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

      int get_int() const;
      long get_long() const;
      double get_double() const;
      float get_float() const;
    };

    using Data = std::variant<sptr_sobject,
                              std::string,
                              RTValue::Number,
                              sptr_rtval_v,
                              bool,
                              char,
                              std::monostate>;

    RTValue() = default;
    explicit RTValue(int);
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
    static sptr_rtval string(const std::string&);
    static sptr_rtval character(char);
    static sptr_rtval keyword(const std::string&);
    static sptr_rtval symbol(const std::string&);
    static sptr_rtval list(sptr_rtval_v&);
    static sptr_rtval vector(const sptr_rtval_v&);
    static sptr_rtval map(const sptr_rtval_v&);
    static sptr_rtval object(sptr_sobject&);
    static sptr_rtval executable(const sptr_sobject&);

    static std::string to_string(const sptr_rtval_v&);
    std::string to_string() const;
  };

  namespace Constant
  {
    inline const sptr_rtval TRUE = std::make_shared<RTValue>(true);
    inline const sptr_rtval FALSE = std::make_shared<RTValue>(false);
    inline const sptr_rtval NIL = std::make_shared<RTValue>(std::monostate());
  } // namespace Constant

  sptr_rtval to_rt_value(sptr_sobject& obj);
  sptr_sobject to_AST(RTValue& val);
  bool is_truthy(RTValue& val);

} // namespace Lisple

#endif /* LISPLE__VALUE_H */
