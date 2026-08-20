#include "roo/exception.h"

#include "roo/runtime/value.h"

#include <algorithm>
#include <limits>
#include <sstream>
#include <utility>

#include <roo/exec.h>
#include <roo/runtime/pretty_print.h>

namespace Roo
{
  namespace
  {
    Diagnostic message_diagnostic(ErrorCategory category, const std::string& reason)
    {
      Diagnostic diagnostic;
      diagnostic.category = category;
      diagnostic.facts.detail = reason;
      return diagnostic;
    }

    std::string render_arguments(const std::vector<std::shared_ptr<Value>>& arguments)
    {
      return Pretty::print(*Value::vector(arguments), invocation_args_print_options());
    }

    std::string binding_name_for(const Diagnostic& diagnostic)
    {
      for (const auto& frame : diagnostic.frames)
      {
        if (frame.kind == DiagnosticFrameKind::CALL)
        {
          return frame.subject;
        }
      }
      return "";
    }

    std::string render_failure(const Diagnostic& diagnostic)
    {
      const auto& facts = diagnostic.facts;
      std::ostringstream out;

      switch (diagnostic.condition)
      {
      case ErrorCondition::NOT_CALLABLE:
        if (facts.target && facts.target->type == Value::Type::NIL)
        {
          out << "Cannot invoke nil with arguments: ";
        }
        else
        {
          out << (facts.target ? facts.target->to_string() : "<unknown>")
              << " is not callable. Arguments: ";
        }
        out << render_arguments(facts.arguments);
        return out.str();

      case ErrorCondition::ARGUMENT_MISMATCH:
        if (facts.target && facts.target->type == Value::Type::KEYWORD)
        {
          out << "Keyword " << facts.target->to_string();
        }
        else
        {
          out << "Invocation";
        }
        if (facts.expected_arity)
        {
          out << " expects exactly " << *facts.expected_arity << " argument";
          if (*facts.expected_arity != 1) out << "s";
        }
        out << ", got " << facts.arguments.size() << ": "
            << render_arguments(facts.arguments);
        return out.str();

      case ErrorCondition::NO_MATCHING_SIGNATURE:
      {
        std::string callee = facts.callee;
        if (auto binding_name = binding_name_for(diagnostic); !binding_name.empty())
        {
          callee = std::move(binding_name);
        }

        out << "No matching signature";
        if (!callee.empty()) out << " for " << callee;
        out << ": " << render_arguments(facts.arguments) << "\n\n";
        out << (facts.expected_signatures.size() > 1 ? "Expected one of:" : "Expected:");
        for (const auto& signature : facts.expected_signatures)
        {
          out << "\n  " << signature;
        }
        out << "\nReceived:\n  [";
        for (std::size_t i = 0; i < facts.arguments.size(); i++)
        {
          if (i > 0) out << " ";
          out << "<" << type_string(*facts.arguments[i]) << ">";
        }
        out << "]\n";
        return out.str();
      }

      case ErrorCondition::MESSAGE:
        return facts.detail;
      }

      return facts.detail;
    }

    bool visible_frame(const Diagnostic& diagnostic,
                       std::size_t frame_index,
                       std::optional<std::size_t> innermost_diagnostic_call)
    {
      const auto& frame = diagnostic.frames[frame_index];
      if (!frame.controlled_by_diagnostics) return true;
      if (!diagnostic.source_diagnostics) return false;
      return diagnostic.call_stack_diagnostics || innermost_diagnostic_call == frame_index;
    }

    bool failure_renders_arguments(ErrorCondition condition)
    {
      return condition == ErrorCondition::NOT_CALLABLE ||
             condition == ErrorCondition::ARGUMENT_MISMATCH ||
             condition == ErrorCondition::NO_MATCHING_SIGNATURE;
    }

    std::string render_diagnostic(const Diagnostic& diagnostic)
    {
      std::optional<std::size_t> innermost_diagnostic_call;
      std::optional<std::size_t> innermost_diagnostic_source_call;
      for (std::size_t i = 0; i < diagnostic.frames.size(); i++)
      {
        const auto& frame = diagnostic.frames[i];
        if (frame.kind == DiagnosticFrameKind::CALL && frame.controlled_by_diagnostics)
        {
          if (!innermost_diagnostic_call) innermost_diagnostic_call = i;
          if (!innermost_diagnostic_source_call && !frame.source.empty())
          {
            innermost_diagnostic_source_call = i;
          }
        }
      }
      const auto visible_innermost_call = innermost_diagnostic_source_call
                                            ? innermost_diagnostic_source_call
                                            : innermost_diagnostic_call;

      std::string message = render_failure(diagnostic);
      bool invocation_arguments_rendered = failure_renders_arguments(diagnostic.condition);
      if (diagnostic.category == ErrorCategory::FORM)
      {
        std::string prefix = "Invalid";
        if (diagnostic.site && !diagnostic.site->subject.empty())
        {
          prefix += " " + diagnostic.site->subject;
        }
        prefix += " form";
        if (diagnostic.source_diagnostics && diagnostic.site &&
            !diagnostic.site->source.empty())
        {
          prefix += " at " + diagnostic.site->source;
        }
        message = prefix + ":\n" + message;
      }
      for (std::size_t frame_index = 0; frame_index < diagnostic.frames.size();
           frame_index++)
      {
        if (!visible_frame(diagnostic, frame_index, visible_innermost_call)) continue;

        const auto& frame = diagnostic.frames[frame_index];
        if (frame.kind == DiagnosticFrameKind::CALL)
        {
          std::string subject = frame.subject;
          if (subject.empty() && frame.target) subject = frame.target->to_string();
          if (subject.empty()) subject = "<anonymous>";

          std::string prefix = "Error while " + frame.operation + " " + subject;
          if (frame.arguments && !invocation_arguments_rendered)
          {
            prefix += " with arguments " + render_arguments(*frame.arguments);
            invocation_arguments_rendered = true;
          }
          if (!frame.source.empty()) prefix += " at " + frame.source;
          message = prefix + ":\n" + message;
        }
        else
        {
          message = "Error " + frame.operation + " '" + frame.subject + "': " + message;
        }
      }
      return message;
    }
  } // namespace

  const Pretty::PrintOptions& invocation_args_print_options()
  {
    static const Pretty::PrintOptions options{
      .indent_width = 2,
      .width = std::numeric_limits<std::size_t>::max(),
      .max_depth = 3,
      .max_elements = 10,
    };
    return options;
  }

  RooException::RooException(const std::string& reason)
    : RooException(ErrorCategory::GENERAL, reason)
  {
  }

  RooException::RooException(ErrorCategory category, const std::string& reason)
    : diagnostic(message_diagnostic(category, reason))
  {
  }

  RooException::RooException(Diagnostic diagnostic)
    : diagnostic(std::move(diagnostic))
  {
  }

  const char* RooException::what() const noexcept
  {
    try
    {
      if (!message) message = render_diagnostic(diagnostic);
      return message->c_str();
    }
    catch (...)
    {
      return "Roo diagnostic rendering failed";
    }
  }

  const Diagnostic& RooException::get_diagnostic() const
  {
    return diagnostic;
  }

  void RooException::add_context(DiagnosticFrame frame)
  {
    diagnostic.frames.push_back(std::move(frame));
    message.reset();
  }

  void RooException::add_call_context(const std::string& operation,
                                      const std::string& binding_name,
                                      const std::string& source,
                                      bool controlled_by_diagnostics)
  {
    DiagnosticFrame frame;
    frame.kind = DiagnosticFrameKind::CALL;
    frame.operation = operation;
    frame.subject = binding_name.empty() ? "<anonymous>" : binding_name;
    frame.source = source;
    frame.controlled_by_diagnostics = controlled_by_diagnostics;
    add_context(std::move(frame));
  }

  void RooException::add_call_context(const std::string& operation,
                                      const std::string& binding_name,
                                      const std::vector<std::shared_ptr<Value>>& arguments,
                                      const std::string& source,
                                      bool controlled_by_diagnostics)
  {
    DiagnosticFrame frame;
    frame.kind = DiagnosticFrameKind::CALL;
    frame.operation = operation;
    frame.subject = binding_name.empty() ? "<anonymous>" : binding_name;
    frame.source = source;
    frame.controlled_by_diagnostics = controlled_by_diagnostics;
    frame.arguments = arguments;
    add_context(std::move(frame));
  }

  void RooException::add_indirect_call_context(
    const std::string& operation,
    const std::shared_ptr<Value>& target,
    const std::vector<std::shared_ptr<Value>>& arguments,
    bool controlled_by_diagnostics)
  {
    DiagnosticFrame frame;
    frame.kind = DiagnosticFrameKind::CALL;
    frame.operation = operation;
    frame.controlled_by_diagnostics = controlled_by_diagnostics;
    frame.target = target;
    frame.arguments = arguments;
    add_context(std::move(frame));
  }

  void RooException::add_resource_context(const std::string& operation,
                                          const std::string& resource)
  {
    DiagnosticFrame frame;
    frame.kind = DiagnosticFrameKind::RESOURCE;
    frame.operation = operation;
    frame.subject = resource;
    add_context(std::move(frame));
  }

  bool RooException::has_resource_context(const std::string& operation) const
  {
    return std::any_of(diagnostic.frames.begin(),
                       diagnostic.frames.end(),
                       [&](const DiagnosticFrame& frame)
                       {
                         return frame.kind == DiagnosticFrameKind::RESOURCE &&
                                frame.operation == operation;
                       });
  }

  void RooException::set_form_site(const std::string& form_name, const std::string& source)
  {
    if (diagnostic.site) return;
    diagnostic.site = DiagnosticSite{form_name, source};
    message.reset();
  }

  void RooException::set_diagnostic_options(bool source_diagnostics,
                                            bool call_stack_diagnostics)
  {
    diagnostic.source_diagnostics = source_diagnostics;
    diagnostic.call_stack_diagnostics = call_stack_diagnostics;
    message.reset();
  }

  void RooException::set_cause(std::exception_ptr cause)
  {
    diagnostic.cause = std::move(cause);
  }

  ParseException::ParseException(const std::string& message)
    : RooException(ErrorCategory::PARSE, message)
  {
  }

  InvalidFormException::InvalidFormException(const std::string& message)
    : RooException(ErrorCategory::FORM, message)
  {
  }

  IdentifierException::IdentifierException(const std::string& message)
    : RooException(ErrorCategory::IDENTIFIER, message)
  {
  }

  InvocationException::InvocationException(const std::string& message)
    : RooException(ErrorCategory::INVOCATION, message)
  {
  }

  InvocationException::InvocationException(Diagnostic diagnostic)
    : RooException(std::move(diagnostic))
  {
  }

  InvocationException InvocationException::not_callable(
    const std::shared_ptr<Value>& target,
    const std::vector<std::shared_ptr<Value>>& arguments)
  {
    Diagnostic diagnostic;
    diagnostic.category = ErrorCategory::INVOCATION;
    diagnostic.condition = ErrorCondition::NOT_CALLABLE;
    diagnostic.facts.target = target;
    diagnostic.facts.arguments = arguments;
    return InvocationException(std::move(diagnostic));
  }

  InvocationException InvocationException::argument_mismatch(
    const std::shared_ptr<Value>& target,
    const std::vector<std::shared_ptr<Value>>& arguments,
    std::size_t expected_arity)
  {
    Diagnostic diagnostic;
    diagnostic.category = ErrorCategory::INVOCATION;
    diagnostic.condition = ErrorCondition::ARGUMENT_MISMATCH;
    diagnostic.facts.target = target;
    diagnostic.facts.arguments = arguments;
    diagnostic.facts.expected_arity = expected_arity;
    return InvocationException(std::move(diagnostic));
  }

  NoMatchingSignatureException::NoMatchingSignatureException(Diagnostic diagnostic)
    : InvocationException(std::move(diagnostic))
  {
  }

  NoMatchingSignatureException NoMatchingSignatureException::no_matching_signature(
    const Executable& callee,
    const std::vector<std::shared_ptr<Value>>& args)
  {
    Diagnostic diagnostic;
    diagnostic.category = ErrorCategory::INVOCATION;
    diagnostic.condition = ErrorCondition::NO_MATCHING_SIGNATURE;
    diagnostic.facts.callee = callee.to_string();
    diagnostic.facts.arguments = args;
    for (const auto& signature : callee.get_signatures())
    {
      diagnostic.facts.expected_signatures.push_back(signature->to_string());
    }
    return NoMatchingSignatureException(std::move(diagnostic));
  }

  NoMatchingSignatureException NoMatchingSignatureException::no_matching_signature(
    const Signature& signature,
    const std::vector<std::shared_ptr<Value>>& args)
  {
    Diagnostic diagnostic;
    diagnostic.category = ErrorCategory::INVOCATION;
    diagnostic.condition = ErrorCondition::NO_MATCHING_SIGNATURE;
    diagnostic.facts.arguments = args;
    diagnostic.facts.expected_signatures.push_back(signature.to_string());
    return NoMatchingSignatureException(std::move(diagnostic));
  }

  const std::string& NoMatchingSignatureException::get_callee() const
  {
    return get_diagnostic().facts.callee;
  }

  const std::vector<std::string>& NoMatchingSignatureException::get_expected_signatures()
    const
  {
    return get_diagnostic().facts.expected_signatures;
  }

  const std::vector<std::shared_ptr<Value>>& NoMatchingSignatureException::get_args() const
  {
    return get_diagnostic().facts.arguments;
  }

  NamespaceException::NamespaceException(const std::string& message)
    : RooException(ErrorCategory::NAMESPACE, message)
  {
  }

  CyclicNamespaceException::CyclicNamespaceException(const std::string& message)
    : NamespaceException(message)
  {
  }

  TypeError::TypeError(const std::string& message)
    : RooException(ErrorCategory::TYPE, message)
  {
  }

  IOException::IOException(const std::string& message)
    : RooException(ErrorCategory::IO, message)
  {
  }
} // namespace Roo
