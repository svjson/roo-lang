
#ifndef __ROO_EXCEPTION_H_
#define __ROO_EXCEPTION_H_

#include <exception>
#include <string>

namespace Roo
{
  class RooException : public std::exception
  {
    const std::string reason;

   public:
    RooException(const std::string& reason);

    virtual const char* what() const throw() override;
  };

  class ParseException : public RooException
  {
   public:
    ParseException(const std::string& message);
  };

  class IdentifierException : public RooException
  {
   public:
    IdentifierException(const std::string& message);
  };

  class OutOfBoundsException : public RooException
  {
   public:
    OutOfBoundsException(const std::string& message);
  };

  class InvocationException : public RooException
  {
   public:
    InvocationException(const std::string& message);
  };

  class NamespaceException : public RooException
  {
   public:
    NamespaceException(const std::string& message);
  };

  class CyclicNamespaceException : public NamespaceException
  {
   public:
    CyclicNamespaceException(const std::string& message);
  };

  class TypeError : public RooException
  {
   public:
    TypeError(const std::string& message);
  };
} // namespace Roo

#endif
