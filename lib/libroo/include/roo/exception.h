
#ifndef __ROO_EXCEPTION_H_
#define __ROO_EXCEPTION_H_

#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Roo
{
  class Executable;
  class Signature;
  struct Value;

  class RooException : public std::exception
  {
    const std::string reason;

   public:
    RooException(const std::string& reason);

    virtual const char* what() const throw() override;

    /**
     * @brief Gives this exception a chance to rebuild itself now that more
     * context (the callee being invoked) is available at a catch site. The
     * default returns an empty exception_ptr, meaning "propagate me
     * unchanged" - only subclasses that know how to use a callee need to
     * override this.
     */
    virtual std::exception_ptr with_callee(const Executable& callee) const;
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

  /**
   * @brief Thrown when a call matched none of a callee's signatures.
   * Message text is assembled lazily on first what()/get_* access rather
   * than at construction, since it is only ever needed once the exception
   * actually surfaces.
   */
  class NoMatchingSignatureException : public InvocationException
  {
   public:
    /**
     * @brief Builds the exception with full context: the callee, all of its
     * accepted signatures, and the received arguments.
     */
    static NoMatchingSignatureException no_matching_signature(
      const Executable& callee, const std::vector<std::shared_ptr<Value>>& args);

    /**
     * @brief Builds the exception from a single Signature that failed to
     * accept `args`, without knowledge of the owning callee. Used where only
     * the Signature itself is in scope; prefer the Executable-based overload
     * above when the callee is available. A caller that later learns the
     * callee can enrich this via with_callee().
     */
    static NoMatchingSignatureException no_matching_signature(
      const Signature& signature, const std::vector<std::shared_ptr<Value>>& args);

    const char* what() const throw() override;

    std::exception_ptr with_callee(const Executable& callee) const override;

    const std::string& get_callee() const;
    const std::vector<std::string>& get_expected_signatures() const;
    const std::vector<std::shared_ptr<Value>>& get_args() const;

   private:
    NoMatchingSignatureException(std::string callee,
                                 std::vector<std::string> expected_signatures,
                                 std::vector<std::shared_ptr<Value>> args);

    std::string render_message() const;

    const std::string callee;
    const std::vector<std::string> expected_signatures;
    const std::vector<std::shared_ptr<Value>> args;
    mutable std::optional<std::string> message;
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
