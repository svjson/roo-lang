#ifndef ROO__RUNTIME__NUMBER_H
#define ROO__RUNTIME__NUMBER_H

#include <roo/runtime/value.h>

namespace Roo
{
  sptr_val convert_to_int(const sptr_val& value);
  sptr_val convert_to_float(const sptr_val& value);
  sptr_val convert_to_number(const sptr_val& value);
  bool is_digit(const Value& value);
} // namespace Roo

#endif /* ROO__RUNTIME__NUMBER_H */
