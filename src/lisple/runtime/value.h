#ifndef LISPLE__VALUE_H
#define LISPLE__VALUE_H

#include "../type.h"
#include <memory>
#include <variant>
#include <vector>

namespace Lisple
{
  class Executable;
  struct RTValue;

  using sptr_rtval = std::shared_ptr<RTValue>;
  using sptr_rtval_v = std::vector<std::shared_ptr<RTValue>>;

  struct RTValue
  {
    enum class Type : uint8_t
    {
      NIL,
      NUMBER,
      STRING,
      CHAR,
      BOOL,
      SYMBOL,
      KEYWORD,
      LIST,
      VECTOR,
      MAP,
      FUNCTION,
      OBJECT
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
                              std::shared_ptr<Executable>,
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
    static sptr_rtval vector(sptr_rtval_v&);
    static sptr_rtval map(sptr_rtval_v&);
    static sptr_rtval object(sptr_sobject&);
    static sptr_rtval executable(std::shared_ptr<Executable>&);

    static std::string to_string(sptr_rtval_v&);
    std::string to_string();
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
  void set_property(sptr_rtval& target, const sptr_rtval& property, sptr_rtval& value);

  sptr_rtval remove_property(sptr_rtval& target, const sptr_rtval& property);

  const std::vector<const RTValue*> map_keys(const std::vector<RTValue>& map_data);

  std::pair<const sptr_rtval, const sptr_rtval> map_entry(const sptr_rtval_v& map_data,
                                                          const RTValue& key);
  std::pair<sptr_rtval, sptr_rtval> map_entry(sptr_rtval_v& map_data, const RTValue& key);

} // namespace Lisple

#endif /* LISPLE__VALUE_H */
