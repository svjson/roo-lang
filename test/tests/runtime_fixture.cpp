
#include "runtime_fixture.h"

namespace Lisple
{
  void PrintTo(const Lisple::AST::Object& obj, std::ostream* os)
  {
    *os << obj.to_string();
  }
} // namespace Lisple
