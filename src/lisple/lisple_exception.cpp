
#include "lisple_exception.h"

namespace Lisple
{
  LispleException::LispleException(const std::string& reason)
    : reason(reason)
  {
  }

  const char* LispleException::what() const throw()
  {
    return reason.c_str();
  }

  ParseException::ParseException(const std::string& message)
    : LispleException(message)
  {
  }

  IdentifierException::IdentifierException(const std::string& message)
    : LispleException(message)
  {
  }

  InvocationException::InvocationException(const std::string& message)
    : LispleException(message)
  {
  }

  NamespaceException::NamespaceException(const std::string& message)
    : LispleException(message)
  {
  }

  TypeError::TypeError(const std::string& message)
    : LispleException(message)
  {
  }

}
