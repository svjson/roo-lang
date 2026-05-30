
#include "runtime_fixture.h"

namespace Roo
{
  void PrintTo(const Roo::AST::ASTNode& obj, std::ostream* os)
  {
    *os << obj.to_string();
  }
} // namespace Roo
