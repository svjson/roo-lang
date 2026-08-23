#include "roo/exception.h"

#include "roo/runtime/value.h"

#include <algorithm>
#include <array>
#include <limits>
#include <sstream>
#include <unordered_set>
#include <utility>

#include <roo/exec.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/pretty_print.h>

namespace Roo
{
  namespace
  {
    void append_map_field(sptr_val_v& fields, const std::string& key, const sptr_val& value)
    {
      fields.push_back(Value::keyword(key));
      fields.push_back(value);
    }

    bool reserved_error_key(const Value& key)
    {
      if (key.type != Value::Type::KEYWORD) return false;

      static const std::unordered_set<std::string> keys{
        "type",
        "parent-type",
        "parent-types",
        "message",
        "detail",
        "site",
        "frames",
        "cause",
      };
      return keys.contains(key.str());
    }

    bool qualified_error_type(const std::string& type)
    {
      const std::size_t separator = type.find('/');
      return separator != std::string::npos && separator != 0 && separator + 1 < type.size();
    }

    sptr_val strings_to_keywords(const std::vector<std::string>& values)
    {
      sptr_val_v result;
      result.reserve(values.size());
      for (const auto& value : values)
      {
        result.push_back(Value::keyword(value));
      }
      return Value::vector(result);
    }

    sptr_val strings_to_values(const std::vector<std::string>& values)
    {
      sptr_val_v result;
      result.reserve(values.size());
      for (const auto& value : values)
      {
        result.push_back(Value::string(value));
      }
      return Value::vector(result);
    }

    sptr_val site_to_value(const std::optional<DiagnosticSite>& site)
    {
      if (!site) return Constant::NIL;

      sptr_val_v fields;
      append_map_field(fields, "subject", Value::string(site->subject));
      append_map_field(fields, "source", Value::string(site->source));
      return Value::map(fields);
    }

    sptr_val frames_to_value(const std::vector<DiagnosticFrame>& frames)
    {
      sptr_val_v result;
      result.reserve(frames.size());
      for (const auto& frame : frames)
      {
        sptr_val_v fields;
        append_map_field(
          fields,
          "kind",
          Value::keyword(frame.kind == DiagnosticFrameKind::CALL ? "call" : "resource"));
        append_map_field(fields, "operation", Value::string(frame.operation));
        append_map_field(fields, "subject", Value::string(frame.subject));
        append_map_field(fields, "source", Value::string(frame.source));
        if (frame.target) append_map_field(fields, "target", frame.target);
        if (frame.arguments)
        {
          append_map_field(fields, "arguments", Value::vector(*frame.arguments));
        }
        result.push_back(Value::map(fields));
      }
      return Value::vector(result);
    }

    sptr_val foreign_error_map(const std::string& message)
    {
      sptr_val_v fields;
      append_map_field(fields, "type", Value::keyword("roo/foreign-error"));
      append_map_field(fields,
                       "parent-types",
                       strings_to_keywords(std::vector<std::string>{"roo/error"}));
      append_map_field(fields, "message", Value::string(message));
      append_map_field(fields, "detail", Value::string(message));
      append_map_field(fields, "site", Constant::NIL);
      append_map_field(fields, "frames", Value::vector({}));
      append_map_field(fields, "cause", Constant::NIL);
      return Value::map(fields);
    }

    sptr_val cause_to_value(const std::exception_ptr& cause)
    {
      if (!cause) return Constant::NIL;
      try
      {
        std::rethrow_exception(cause);
      }
      catch (const RooException& error)
      {
        return error.to_error_map();
      }
      catch (const std::exception& error)
      {
        return foreign_error_map(error.what());
      }
      catch (...)
      {
        return foreign_error_map("Unknown foreign exception");
      }
    }

    Diagnostic message_diagnostic(ErrorCategory category, const std::string& reason)
    {
      Diagnostic diagnostic;
      diagnostic.category = category;
      diagnostic.facts.detail = reason;
      return diagnostic;
    }

    Diagnostic not_callable_diagnostic(const std::shared_ptr<Value>& target,
                                       const std::vector<std::shared_ptr<Value>>& arguments)
    {
      Diagnostic diagnostic;
      diagnostic.category = ErrorCategory::INVOCATION;
      diagnostic.condition = ErrorCondition::NOT_CALLABLE;
      diagnostic.facts.target = target;
      diagnostic.facts.arguments = arguments;
      return diagnostic;
    }

    Diagnostic argument_mismatch_diagnostic(
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

    std::string render_failure(const Diagnostic& diagnostic, bool render_received_arguments)
    {
      const auto& facts = diagnostic.facts;
      std::ostringstream out;

      switch (diagnostic.condition)
      {
      case ErrorCondition::NOT_CALLABLE:
        if (facts.target && facts.target->type == Value::Type::NIL)
        {
          out << "Cannot invoke nil";
        }
        else
        {
          out << (facts.target ? facts.target->to_string() : "<unknown>")
              << " is not callable";
        }
        if (render_received_arguments)
        {
          out << " with arguments: " << render_arguments(facts.arguments);
        }
        out << ".";
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
        out << ", got " << facts.arguments.size();
        if (render_received_arguments)
        {
          out << ": " << render_arguments(facts.arguments);
        }
        else
        {
          out << ".";
        }
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
        if (render_received_arguments)
        {
          out << ": " << render_arguments(facts.arguments);
        }
        else
        {
          out << ".";
        }
        out << "\n\n";
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

      bool invocation_frame_renders_arguments = false;
      for (std::size_t frame_index = 0; frame_index < diagnostic.frames.size();
           frame_index++)
      {
        if (!visible_frame(diagnostic, frame_index, visible_innermost_call)) continue;
        const auto& frame = diagnostic.frames[frame_index];
        if (frame.kind == DiagnosticFrameKind::CALL && frame.arguments)
        {
          invocation_frame_renders_arguments = true;
          break;
        }
      }

      const bool failure_has_invocation_arguments =
        failure_renders_arguments(diagnostic.condition);
      std::string message = render_failure(
        diagnostic,
        failure_has_invocation_arguments && !invocation_frame_renders_arguments);
      bool invocation_arguments_rendered =
        failure_has_invocation_arguments && !invocation_frame_renders_arguments;
      bool call_frame_rendered = false;
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

          if (message.empty() || message.back() != '\n') message += "\n";
          message += call_frame_rendered ? "  from " : "  in ";
          message += subject;
          if (!frame.source.empty()) message += " at " + frame.source;
          if (frame.arguments && !invocation_arguments_rendered)
          {
            message += " - " + render_arguments(*frame.arguments);
            invocation_arguments_rendered = true;
          }
          call_frame_rendered = true;
        }
        else
        {
          if (message.empty() || message.back() != '\n') message += "\n";
          message += "  while " + frame.operation + " '" + frame.subject + "'";
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

  void ErrorMapBuilder::add(const std::string& key, const std::shared_ptr<Value>& value)
  {
    fields.emplace_back(key, value);
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

  RooException::RooException(const std::string& reason,
                             std::shared_ptr<Value> error_map_seed)
    : RooException(reason)
  {
    seeded_error_map = std::move(error_map_seed);
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

  std::shared_ptr<Value> RooException::to_error_map() const
  {
    const std::string error_type = roo_error_type();
    std::vector<std::string> parent_types;
    std::unordered_set<std::string> seen_parent_types;
    for (const auto& parent : roo_parent_error_types())
    {
      if (parent != error_type && seen_parent_types.insert(parent).second)
      {
        parent_types.push_back(parent);
      }
    }

    const sptr_val& seed = seeded_error_map;
    auto seeded_value = [&](const std::string& key, const sptr_val& fallback)
    {
      if (!seed) return fallback;
      const auto [found, value] = Dict::find_property(seed, *Value::keyword(key));
      return found ? value : fallback;
    };

    sptr_val_v combined_frames;
    if (seed)
    {
      const sptr_val seeded_frames = seeded_value("frames", Value::vector({}));
      combined_frames = seeded_frames->elements();
    }
    const sptr_val diagnostic_frames = frames_to_value(diagnostic.frames);
    combined_frames.insert(combined_frames.end(),
                           diagnostic_frames->elements().begin(),
                           diagnostic_frames->elements().end());

    sptr_val site = site_to_value(diagnostic.site);
    if (seed)
    {
      const sptr_val seeded_site = seeded_value("site", Constant::NIL);
      if (seeded_site->type != Value::Type::NIL) site = seeded_site;
    }

    sptr_val cause = cause_to_value(diagnostic.cause);
    if (seed)
    {
      const sptr_val seeded_cause = seeded_value("cause", Constant::NIL);
      if (seeded_cause->type != Value::Type::NIL) cause = seeded_cause;
    }

    sptr_val_v fields;
    append_map_field(fields, "type", Value::keyword(error_type));
    append_map_field(fields, "parent-types", strings_to_keywords(parent_types));
    append_map_field(fields, "message", Value::string(what()));
    append_map_field(
      fields,
      "detail",
      seeded_value("detail", Value::string(render_failure(diagnostic, true))));
    append_map_field(fields, "site", site);
    append_map_field(fields, "frames", Value::vector(combined_frames));
    append_map_field(fields, "cause", cause);

    std::unordered_set<std::string>
      keys{"type", "parent-types", "message", "detail", "site", "frames", "cause"};

    if (seed)
    {
      const sptr_val_v& seeded_fields = seed->elements();
      for (std::size_t i = 0; i < seeded_fields.size(); i += 2)
      {
        if (reserved_error_key(*seeded_fields[i])) continue;
        fields.push_back(seeded_fields[i]);
        fields.push_back(seeded_fields[i + 1]);
        if (seeded_fields[i]->type == Value::Type::KEYWORD)
        {
          keys.insert(seeded_fields[i]->str());
        }
      }
    }

    const auto& facts = diagnostic.facts;
    if (facts.target)
    {
      append_map_field(fields, "target", facts.target);
      keys.insert("target");
    }
    if (!facts.arguments.empty() || diagnostic.condition != ErrorCondition::MESSAGE)
    {
      append_map_field(fields, "arguments", Value::vector(facts.arguments));
      keys.insert("arguments");
    }
    if (facts.expected_arity)
    {
      append_map_field(fields,
                       "expected-arity",
                       Value::number(static_cast<long>(*facts.expected_arity)));
      keys.insert("expected-arity");
    }
    if (!facts.expected_signatures.empty())
    {
      append_map_field(fields,
                       "expected-signatures",
                       strings_to_values(facts.expected_signatures));
      keys.insert("expected-signatures");
    }
    if (!facts.callee.empty())
    {
      append_map_field(fields, "callee", Value::string(facts.callee));
      keys.insert("callee");
    }

    ErrorMapBuilder builder;
    append_error_fields(builder);
    for (const auto& [key, value] : builder.fields)
    {
      if (value && keys.insert(key).second) append_map_field(fields, key, value);
    }
    return Value::map(fields);
  }

  std::string RooException::roo_error_type() const
  {
    std::vector<std::string> types;
    append_roo_error_types(types);
    return types.back();
  }

  std::vector<std::string> RooException::roo_parent_error_types() const
  {
    std::vector<std::string> types;
    append_roo_error_types(types);
    types.pop_back();
    std::reverse(types.begin(), types.end());
    return types;
  }

  void RooException::append_roo_error_types(std::vector<std::string>& types) const
  {
    types.push_back("roo/error");
  }

  void RooException::append_error_fields(ErrorMapBuilder&) const {}

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

  void ParseException::append_roo_error_types(std::vector<std::string>& types) const
  {
    RooException::append_roo_error_types(types);
    types.push_back("roo/parse-error");
  }

  InvalidFormException::InvalidFormException(const std::string& message)
    : RooException(ErrorCategory::FORM, message)
  {
  }

  void InvalidFormException::append_roo_error_types(std::vector<std::string>& types) const
  {
    RooException::append_roo_error_types(types);
    types.push_back("roo/invalid-form-error");
  }

  IdentifierException::IdentifierException(const std::string& message)
    : RooException(ErrorCategory::IDENTIFIER, message)
  {
  }

  void IdentifierException::append_roo_error_types(std::vector<std::string>& types) const
  {
    RooException::append_roo_error_types(types);
    types.push_back("roo/identifier-error");
  }

  InvocationException::InvocationException(const std::string& message)
    : RooException(ErrorCategory::INVOCATION, message)
  {
  }

  InvocationException::InvocationException(Diagnostic diagnostic)
    : RooException(std::move(diagnostic))
  {
  }

  void InvocationException::append_roo_error_types(std::vector<std::string>& types) const
  {
    RooException::append_roo_error_types(types);
    types.push_back("roo/invocation-error");
  }

  NotCallableException::NotCallableException(
    const std::shared_ptr<Value>& target,
    const std::vector<std::shared_ptr<Value>>& arguments)
    : InvocationException(not_callable_diagnostic(target, arguments))
  {
  }

  void NotCallableException::append_roo_error_types(std::vector<std::string>& types) const
  {
    InvocationException::append_roo_error_types(types);
    types.push_back("roo/not-callable-error");
  }

  ArgumentMismatchException::ArgumentMismatchException(
    const std::shared_ptr<Value>& target,
    const std::vector<std::shared_ptr<Value>>& arguments,
    std::size_t expected_arity)
    : InvocationException(argument_mismatch_diagnostic(target, arguments, expected_arity))
  {
  }

  ArgumentMismatchException::ArgumentMismatchException(Diagnostic diagnostic)
    : InvocationException(std::move(diagnostic))
  {
  }

  void ArgumentMismatchException::append_roo_error_types(
    std::vector<std::string>& types) const
  {
    InvocationException::append_roo_error_types(types);
    types.push_back("roo/argument-mismatch-error");
  }

  NoMatchingSignatureException::NoMatchingSignatureException(Diagnostic diagnostic)
    : ArgumentMismatchException(std::move(diagnostic))
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

  void NoMatchingSignatureException::append_roo_error_types(
    std::vector<std::string>& types) const
  {
    ArgumentMismatchException::append_roo_error_types(types);
    types.push_back("roo/no-matching-signature-error");
  }

  NamespaceException::NamespaceException(const std::string& message)
    : RooException(ErrorCategory::NAMESPACE, message)
  {
  }

  void NamespaceException::append_roo_error_types(std::vector<std::string>& types) const
  {
    RooException::append_roo_error_types(types);
    types.push_back("roo/namespace-error");
  }

  CyclicNamespaceException::CyclicNamespaceException(const std::string& message)
    : NamespaceException(message)
  {
  }

  void CyclicNamespaceException::append_roo_error_types(
    std::vector<std::string>& types) const
  {
    NamespaceException::append_roo_error_types(types);
    types.push_back("roo/require-cycle-error");
  }

  TypeError::TypeError(const std::string& message)
    : RooException(ErrorCategory::TYPE, message)
  {
  }

  void TypeError::append_roo_error_types(std::vector<std::string>& types) const
  {
    RooException::append_roo_error_types(types);
    types.push_back("roo/type-error");
  }

  IOException::IOException(const std::string& message)
    : RooException(ErrorCategory::IO, message)
  {
  }

  void IOException::append_roo_error_types(std::vector<std::string>& types) const
  {
    RooException::append_roo_error_types(types);
    types.push_back("roo.io/error");
  }

  RaisedError::Normalized RaisedError::normalize(const std::shared_ptr<Value>& error_map)
  {
    if (!error_map || error_map->type != Value::Type::MAP)
    {
      throw TypeError("RaisedError requires an error map.");
    }

    auto find = [&](const std::string& key)
    {
      return Dict::find_property(error_map, *Value::keyword(key));
    };

    std::string error_type = "roo/error";
    if (const auto [found, value] = find("type"); found)
    {
      if (value->type != Value::Type::KEYWORD || !qualified_error_type(value->str()))
      {
        throw TypeError("Raised error :type must be a qualified keyword.");
      }
      error_type = value->str();
    }

    std::optional<std::string> parent_type;
    if (const auto [found, value] = find("parent-type");
        found && value->type != Value::Type::NIL)
    {
      if (value->type != Value::Type::KEYWORD || !qualified_error_type(value->str()))
      {
        throw TypeError("Raised error :parent-type must be a qualified keyword or nil.");
      }
      if (value->str() == error_type)
      {
        throw TypeError("Raised error :parent-type cannot equal :type.");
      }
      if (error_type == "roo/error")
      {
        throw TypeError(":roo/error cannot declare a parent error type.");
      }
      parent_type = value->str();
    }

    std::string message = "Raised :" + error_type;
    if (const auto [found, value] = find("message"); found)
    {
      if (value->type != Value::Type::STRING)
      {
        throw TypeError("Raised error :message must be a string.");
      }
      message = value->str();
    }

    sptr_val detail = Value::string(message);
    if (const auto [found, value] = find("detail"); found)
    {
      if (value->type != Value::Type::STRING)
      {
        throw TypeError("Raised error :detail must be a string.");
      }
      detail = value;
    }

    sptr_val site = Constant::NIL;
    if (const auto [found, value] = find("site"); found)
    {
      if (value->type != Value::Type::MAP && value->type != Value::Type::NIL)
      {
        throw TypeError("Raised error :site must be a map or nil.");
      }
      site = value;
    }

    sptr_val frames = Value::vector({});
    if (const auto [found, value] = find("frames"); found)
    {
      if (value->type != Value::Type::VECTOR)
      {
        throw TypeError("Raised error :frames must be a vector.");
      }
      frames = value;
    }

    sptr_val cause = Constant::NIL;
    if (const auto [found, value] = find("cause"); found)
    {
      if (value->type != Value::Type::MAP && value->type != Value::Type::NIL)
      {
        throw TypeError("Raised error :cause must be an error map or nil.");
      }
      cause = value;
    }

    std::vector<std::string> parent_types;
    if (parent_type && *parent_type != "roo/error") parent_types.push_back(*parent_type);
    if (error_type != "roo/error") parent_types.push_back("roo/error");

    sptr_val_v normalized_fields;
    append_map_field(normalized_fields, "type", Value::keyword(error_type));
    append_map_field(normalized_fields, "parent-types", strings_to_keywords(parent_types));
    append_map_field(normalized_fields, "message", Value::string(message));
    append_map_field(normalized_fields, "detail", detail);
    append_map_field(normalized_fields, "site", site);
    append_map_field(normalized_fields, "frames", frames);
    append_map_field(normalized_fields, "cause", cause);

    const sptr_val_v& input_fields = error_map->elements();
    for (std::size_t i = 0; i < input_fields.size(); i += 2)
    {
      if (reserved_error_key(*input_fields[i])) continue;
      normalized_fields.push_back(input_fields[i]);
      normalized_fields.push_back(input_fields[i + 1]);
    }

    return Normalized{Value::map(normalized_fields),
                      error_type,
                      parent_type,
                      std::move(message)};
  }

  RaisedError::RaisedError(const std::shared_ptr<Value>& error_map,
                           std::shared_ptr<Value> source_error)
    : RaisedError(normalize(error_map), std::move(source_error))
  {
  }

  RaisedError::RaisedError(Normalized normalized, std::shared_ptr<Value> source_error)
    : RooException(normalized.message, std::move(normalized.error_map))
    , raised_source_error(std::move(source_error))
    , raised_error_type(std::move(normalized.error_type))
    , raised_parent_type(std::move(normalized.parent_type))
  {
  }

  const std::shared_ptr<Value>& RaisedError::source_error() const noexcept
  {
    return raised_source_error;
  }

  void RaisedError::append_roo_error_types(std::vector<std::string>& types) const
  {
    RooException::append_roo_error_types(types);
    if (raised_parent_type && *raised_parent_type != "roo/error")
    {
      types.push_back(*raised_parent_type);
    }
    if (raised_error_type != "roo/error") types.push_back(raised_error_type);
  }

} // namespace Roo
