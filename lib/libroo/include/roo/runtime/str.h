#ifndef ROO__RUNTIME_STR_H
#define ROO__RUNTIME_STR_H

#include <string>
#include <vector>

namespace Roo
{
  std::string escaped_string_literal(const std::string& value);
  std::vector<std::string> utf8_scalars(const std::string& value);
} // namespace Roo

#endif /* ROO__RUNTIME_STR_H */
