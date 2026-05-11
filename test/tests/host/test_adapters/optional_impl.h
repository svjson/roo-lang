#ifndef LISPLETEST__HOST__OPTIONAL_IMPL_H
#define LISPLETEST__HOST__OPTIONAL_IMPL_H

#include <optional>

namespace LispleTest
{
  struct OptionalBox
  {
    std::optional<int> count = std::nullopt;
  };

  struct OptionalChild
  {
    int value = 0;
  };

  struct OptionalChildBox
  {
    std::optional<OptionalChild> child = std::nullopt;
  };

  struct OptionalChildPtrBox
  {
    OptionalChild* child = nullptr;
  };
} // namespace LispleTest

#endif /* LISPLETEST__HOST__OPTIONAL_IMPL_H */
