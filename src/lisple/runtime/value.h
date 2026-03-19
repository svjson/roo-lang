#ifndef LISPLE__VALUE_H
#define LISPLE__VALUE_H

#include "../type.h"
#include <variant>

namespace Lisple
{
  class Function;

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
    };

    using Data = std::variant<Object*,
                              Function*,
                              std::string,
                              RTValue::Number,
                              std::vector<RTValue>,
                              bool,
                              char,
                              std::monostate>;

    RTValue::Data value;
    RTValue::Type type;

    bool operator==(const RTValue& other) const;

    static RTValue nil();
    static RTValue boolean(bool);
    static RTValue number(int);
    static RTValue number(long);
    static RTValue number(double);
    static RTValue string(const std::string&);
    static RTValue character(char);
    static RTValue keyword(const std::string&);
    static RTValue symbol(const std::string&);
    static RTValue list(std::vector<RTValue>&);
    static RTValue vector(std::vector<RTValue>&);
    static RTValue map(std::vector<RTValue>&);
    static RTValue object(Object*);
    static RTValue function(Function*);
  };

  RTValue to_rt_value(const Object& obj);

  const std::vector<const RTValue*> map_keys(const std::vector<RTValue>& map_data);

  std::pair<const RTValue*, const RTValue*> map_entry(const std::vector<RTValue>& map_data,
                                                      const RTValue& key);
  std::pair<RTValue*, RTValue*> map_entry(std::vector<RTValue>& map_data,
                                          const RTValue& key);

} // namespace Lisple

#endif /* LISPLE__VALUE_H */
