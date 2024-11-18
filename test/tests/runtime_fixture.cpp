
#include "runtime_fixture.h"


namespace Lisple
{
  void PrintTo(const Lisple::Object& obj, std::ostream* os)
  {
    *os << obj.to_string();
  }
}
