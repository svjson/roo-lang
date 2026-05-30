#ifndef ROOTEST__HOST__OPTIONAL_IMPL_H
#define ROOTEST__HOST__OPTIONAL_IMPL_H

#include <optional>

namespace RooTest
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
} // namespace RooTest

#endif /* ROOTEST__HOST__OPTIONAL_IMPL_H */
