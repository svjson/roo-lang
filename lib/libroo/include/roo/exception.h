#ifndef __ROO_EXCEPTION_H_
#define __ROO_EXCEPTION_H_

#include <cstddef>
#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <roo/runtime/pretty_print.h>

namespace Roo
{
  class Executable;
  class Signature;
  struct Value;

  const Pretty::PrintOptions& invocation_args_print_options();

  enum class ErrorCategory
  {
    GENERAL,
    PARSE,
    IDENTIFIER,
    INVOCATION,
    NAMESPACE,
    TYPE,
    IO,
    FORM,
  };

  enum class ErrorCondition
  {
    MESSAGE,
    NOT_CALLABLE,
    ARGUMENT_MISMATCH,
    NO_MATCHING_SIGNATURE,
  };

  enum class DiagnosticFrameKind
  {
    CALL,
    RESOURCE,
  };

  struct DiagnosticFacts
  {
    std::string detail;
    std::shared_ptr<Value> target;
    std::vector<std::shared_ptr<Value>> arguments;
    std::optional<std::size_t> expected_arity;
    std::vector<std::string> expected_signatures;
    std::string callee;
  };

  struct DiagnosticFrame
  {
    DiagnosticFrameKind kind = DiagnosticFrameKind::CALL;
    std::string operation;
    std::string subject;
    std::string source;
    bool controlled_by_diagnostics = false;
    std::shared_ptr<Value> target;
    std::optional<std::vector<std::shared_ptr<Value>>> arguments;
  };

  struct DiagnosticSite
  {
    std::string subject;
    std::string source;
  };

  struct Diagnostic
  {
    ErrorCategory category = ErrorCategory::GENERAL;
    ErrorCondition condition = ErrorCondition::MESSAGE;
    DiagnosticFacts facts;
    std::optional<DiagnosticSite> site;
    std::vector<DiagnosticFrame> frames;
    std::exception_ptr cause;
    bool source_diagnostics = true;
    bool call_stack_diagnostics = true;
  };

  class RooException : public std::exception
  {
   public:
    explicit RooException(const std::string& reason);

    const char* what() const noexcept override;

    const Diagnostic& get_diagnostic() const;

    void add_context(DiagnosticFrame frame);
    void add_call_context(const std::string& operation,
                          const std::string& binding_name,
                          const std::string& source = "",
                          bool controlled_by_diagnostics = false);
    void add_call_context(const std::string& operation,
                          const std::string& binding_name,
                          const std::vector<std::shared_ptr<Value>>& arguments,
                          const std::string& source = "",
                          bool controlled_by_diagnostics = false);
    void add_indirect_call_context(const std::string& operation,
                                   const std::shared_ptr<Value>& target,
                                   const std::vector<std::shared_ptr<Value>>& arguments,
                                   bool controlled_by_diagnostics = false);
    void add_resource_context(const std::string& operation, const std::string& resource);
    bool has_resource_context(const std::string& operation) const;
    void set_form_site(const std::string& form_name, const std::string& source = "");
    void set_diagnostic_options(bool source_diagnostics, bool call_stack_diagnostics);
    void set_cause(std::exception_ptr cause);

   protected:
    RooException(ErrorCategory category, const std::string& reason);
    explicit RooException(Diagnostic diagnostic);

   private:
    Diagnostic diagnostic;
    mutable std::optional<std::string> message;
  };

  class ParseException : public RooException
  {
   public:
    explicit ParseException(const std::string& message);
  };

  class InvalidFormException : public RooException
  {
   public:
    explicit InvalidFormException(const std::string& message);
  };

  class IdentifierException : public RooException
  {
   public:
    explicit IdentifierException(const std::string& message);
  };

  class InvocationException : public RooException
  {
   public:
    explicit InvocationException(const std::string& message);

    static InvocationException not_callable(
      const std::shared_ptr<Value>& target,
      const std::vector<std::shared_ptr<Value>>& arguments);
    static InvocationException argument_mismatch(
      const std::shared_ptr<Value>& target,
      const std::vector<std::shared_ptr<Value>>& arguments,
      std::size_t expected_arity);

   protected:
    explicit InvocationException(Diagnostic diagnostic);
  };

  class NoMatchingSignatureException : public InvocationException
  {
   public:
    static NoMatchingSignatureException no_matching_signature(
      const Executable& callee,
      const std::vector<std::shared_ptr<Value>>& args);
    static NoMatchingSignatureException no_matching_signature(
      const Signature& signature,
      const std::vector<std::shared_ptr<Value>>& args);

    const std::string& get_callee() const;
    const std::vector<std::string>& get_expected_signatures() const;
    const std::vector<std::shared_ptr<Value>>& get_args() const;

   private:
    explicit NoMatchingSignatureException(Diagnostic diagnostic);
  };

  class NamespaceException : public RooException
  {
   public:
    explicit NamespaceException(const std::string& message);
  };

  class CyclicNamespaceException : public NamespaceException
  {
   public:
    explicit CyclicNamespaceException(const std::string& message);
  };

  class TypeError : public RooException
  {
   public:
    explicit TypeError(const std::string& message);
  };

  class IOException : public RooException
  {
   public:
    explicit IOException(const std::string& message);
  };
} // namespace Roo

#endif
