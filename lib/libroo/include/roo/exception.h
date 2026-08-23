#ifndef __ROO_EXCEPTION_H_
#define __ROO_EXCEPTION_H_

#include <cstddef>
#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <roo/runtime/pretty_print.h>

namespace Roo
{
  class Executable;
  class Signature;
  struct Value;

  class ErrorMapBuilder
  {
   public:
    void add(const std::string& key, const std::shared_ptr<Value>& value);

   private:
    std::vector<std::pair<std::string, std::shared_ptr<Value>>> fields;

    friend class RooException;
  };

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

    std::shared_ptr<Value> to_error_map() const;
    std::string roo_error_type() const;
    std::vector<std::string> roo_parent_error_types() const;

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
    virtual void append_roo_error_types(std::vector<std::string>& types) const;
    virtual void append_error_fields(ErrorMapBuilder& builder) const;

   private:
    RooException(const std::string& reason, std::shared_ptr<Value> error_map_seed);

    Diagnostic diagnostic;
    mutable std::optional<std::string> message;
    std::shared_ptr<Value> seeded_error_map;

    friend class RaisedError;
  };

  class ParseException : public RooException
  {
   public:
    explicit ParseException(const std::string& message);

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class InvalidFormException : public RooException
  {
   public:
    explicit InvalidFormException(const std::string& message);

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class IdentifierException : public RooException
  {
   public:
    explicit IdentifierException(const std::string& message);

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class InvocationException : public RooException
  {
   public:
    explicit InvocationException(const std::string& message);

   protected:
    explicit InvocationException(Diagnostic diagnostic);
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class NotCallableException : public InvocationException
  {
   public:
    NotCallableException(const std::shared_ptr<Value>& target,
                         const std::vector<std::shared_ptr<Value>>& arguments);

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class ArgumentMismatchException : public InvocationException
  {
   public:
    ArgumentMismatchException(const std::shared_ptr<Value>& target,
                              const std::vector<std::shared_ptr<Value>>& arguments,
                              std::size_t expected_arity);

   protected:
    explicit ArgumentMismatchException(Diagnostic diagnostic);
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class NoMatchingSignatureException : public ArgumentMismatchException
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

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class NamespaceException : public RooException
  {
   public:
    explicit NamespaceException(const std::string& message);

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class CyclicNamespaceException : public NamespaceException
  {
   public:
    explicit CyclicNamespaceException(const std::string& message);

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class TypeError : public RooException
  {
   public:
    explicit TypeError(const std::string& message);

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  class IOException : public RooException
  {
   public:
    explicit IOException(const std::string& message);

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;
  };

  /*!
   * @brief Map-backed native carrier for an error created by Roo `raise`.
   * @since 0.1.0
   *
   * The error map is normalized and owned by the exception. `source_error`
   * identifies the original Roo map only while `GuardForm` determines whether
   * a clause is reraising its unchanged caught error.
   */
  class RaisedError : public RooException
  {
   public:
    explicit RaisedError(const std::shared_ptr<Value>& error_map,
                         std::shared_ptr<Value> source_error = nullptr);

    const std::shared_ptr<Value>& source_error() const noexcept;

   protected:
    void append_roo_error_types(std::vector<std::string>& types) const override;

   private:
    struct Normalized
    {
      std::shared_ptr<Value> error_map;
      std::string error_type;
      std::optional<std::string> parent_type;
      std::string message;
    };

    static Normalized normalize(const std::shared_ptr<Value>& error_map);
    RaisedError(Normalized normalized, std::shared_ptr<Value> source_error);

    std::shared_ptr<Value> raised_source_error;
    std::string raised_error_type;
    std::optional<std::string> raised_parent_type;
  };
} // namespace Roo

#endif
