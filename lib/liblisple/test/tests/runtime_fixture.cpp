
#include "runtime_fixture.h"

namespace Lisple
{
  void PrintTo(const Lisple::AST::ASTNode& obj, std::ostream* os)
  {
    *os << obj.to_string();
  }
} // namespace Lisple
