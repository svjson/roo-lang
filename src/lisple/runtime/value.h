#ifndef LISPLE__VALUE_H
#define LISPLE__VALUE_H

#include "../exec.h"
#include "../type.h"

namespace Lisple
{
  struct RTValue
  {
    Form type;

    union
    {
      struct
      {
        NumberType num_type;

        union
        {
          int32_t int_value;
          int64_t long_value;
          double float_value;
        };
      } number_value;

      bool bool_value;
      Object* object_value;
      Function* function_value;
    };
    static RTValue nil();
    static RTValue number(int);
    static RTValue number(long);
    static RTValue number(double);
    static RTValue object(Object*);
    static RTValue function(Function*);
  };

  RTValue to_rt_value(const Object& obj);

} // namespace Lisple

#endif /* LISPLE__VALUE_H */
