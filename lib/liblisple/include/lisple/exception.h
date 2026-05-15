
#ifndef __LISPLE_EXCEPTION_H_
#define __LISPLE_EXCEPTION_H_

#include <exception>
#include <string>

namespace Lisple
{
  class LispleException : public std::exception
  {
    const std::string reason;
   public:
    LispleException(const std::string& reason);

    virtual const char* what() const throw() override;
  };

  class ParseException : public LispleException
  {
   public:
    ParseException(const std::string& message);
  };

  class IdentifierException : public LispleException
  {
   public:
    IdentifierException(const std::string& message);
  };

  class OutOfBoundsException : public LispleException
  {
   public:
    OutOfBoundsException(const std::string& message);
  };

  class InvocationException : public LispleException
  {
   public:
    InvocationException(const std::string& message);
  };

  class NamespaceException : public LispleException
  {
   public:
    NamespaceException(const std::string& message);
  };

  class CyclicNamespaceException : public NamespaceException
  {
   public:
    CyclicNamespaceException(const std::string& message);
  };

  class TypeError : public LispleException
  {
  public:
    TypeError(const std::string& message);
  };
}

#endif
