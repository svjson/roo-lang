
#include "roo/exception.h"

namespace Roo
{
  RooException::RooException(const std::string& reason)
    : reason(reason)
  {
  }

  const char* RooException::what() const throw()
  {
    return reason.c_str();
  }

  ParseException::ParseException(const std::string& message)
    : RooException(message)
  {
  }

  IdentifierException::IdentifierException(const std::string& message)
    : RooException(message)
  {
  }

  OutOfBoundsException::OutOfBoundsException(const std::string& message)
    : RooException(message)
  {
  }

  InvocationException::InvocationException(const std::string& message)
    : RooException(message)
  {
  }

  NamespaceException::NamespaceException(const std::string& message)
    : RooException(message)
  {
  }

  CyclicNamespaceException::CyclicNamespaceException(const std::string& message)
    : NamespaceException(message)
  {
  }

  TypeError::TypeError(const std::string& message)
    : RooException(message)
  {
  }

} // namespace Roo
