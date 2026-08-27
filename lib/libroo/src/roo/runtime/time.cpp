#include "roo/runtime/time.h"

#include <thread>

#include <roo/exception.h>

namespace Roo::RuntimeTiming
{
  std::chrono::milliseconds read_milliseconds(const sptr_val& value,
                                              const std::string& operation)
  {
    const Value::Number& number = value->num();
    long long milliseconds = 0;
    switch (number.num_type)
    {
    case Value::NumberType::INT:
      milliseconds = number.int_value;
      break;
    case Value::NumberType::LONG:
      milliseconds = number.long_value;
      break;
    case Value::NumberType::FLOAT:
      throw RooException(operation + " milliseconds must be an integer.");
    }

    if (milliseconds < 0)
    {
      throw RooException(operation + " milliseconds must not be negative.");
    }
    return std::chrono::milliseconds(milliseconds);
  }

  void sleep_for(std::chrono::milliseconds duration)
  {
    std::this_thread::sleep_for(duration);
  }
} // namespace Roo::RuntimeTiming
