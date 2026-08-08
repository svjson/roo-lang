
#ifndef __ROO_EXCEPTION_H_
#define __ROO_EXCEPTION_H_

#include <exception>
#include <memory>
#include <string>
#include <vector>

namespace Roo
{
  class Executable;
  struct Value;

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

    /**
     * @brief Builds an InvocationException for a call that matched none of
     * `callee`'s signatures, keeping the callee, its accepted signatures and
     * the received arguments as inspectable data alongside the rendered
     * message. Only constructed from an already-failed call path, so it adds
     * no cost on a successful call.
     */
    static InvocationException no_matching_signature(
      const Executable& callee, const std::vector<std::shared_ptr<Value>>& args);

    const std::string& get_callee() const;
    const std::vector<std::string>& get_expected_signatures() const;
    const std::vector<std::shared_ptr<Value>>& get_args() const;

   private:
    InvocationException(const std::string& message,
                        std::string callee,
                        std::vector<std::string> expected_signatures,
                        std::vector<std::shared_ptr<Value>> args);

    const std::string callee;
    const std::vector<std::string> expected_signatures;
    const std::vector<std::shared_ptr<Value>> args;
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
