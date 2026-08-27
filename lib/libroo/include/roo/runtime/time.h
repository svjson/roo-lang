#ifndef ROO__RUNTIME__TIME_H
#define ROO__RUNTIME__TIME_H

#include <chrono>
#include <string>

#include <roo/runtime/value.h>

namespace Roo::RuntimeTiming
{
  std::chrono::milliseconds read_milliseconds(const sptr_val& value,
                                              const std::string& operation);

  void sleep_for(std::chrono::milliseconds duration);
} // namespace Roo::RuntimeTiming

#endif
