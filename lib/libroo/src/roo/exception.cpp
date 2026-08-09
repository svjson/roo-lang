
#include "roo/exception.h"

#include "roo/runtime/value.h"

#include <sstream>
#include <utility>

#include <roo/exec.h>

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

  std::exception_ptr RooException::with_callee(const Executable&) const
  {
    return nullptr;
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

  /** InvocationException */

  InvocationException::InvocationException(const std::string& message)
    : RooException(message)
  {
  }

  /** NoMatchingSignatureException */

  NoMatchingSignatureException::NoMatchingSignatureException(
    std::string callee,
    std::vector<std::string> expected_signatures,
    std::vector<std::shared_ptr<Value>> args)
    : InvocationException("")
    , callee(std::move(callee))
    , expected_signatures(std::move(expected_signatures))
    , args(std::move(args))
  {
  }

  NoMatchingSignatureException NoMatchingSignatureException::no_matching_signature(
    const Executable& callee,
    const std::vector<std::shared_ptr<Value>>& args)
  {
    std::vector<std::string> expected_signatures;
    for (auto& signature : callee.get_signatures())
    {
      expected_signatures.push_back(signature->to_string());
    }

    return NoMatchingSignatureException(callee.to_string(), std::move(expected_signatures), args);
  }

  NoMatchingSignatureException NoMatchingSignatureException::no_matching_signature(
    const Signature& signature,
    const std::vector<std::shared_ptr<Value>>& args)
  {
    return NoMatchingSignatureException("", {signature.to_string()}, args);
  }

  std::string NoMatchingSignatureException::render_message() const
  {
    std::ostringstream out;
    out << "No matching signature";
    if (!callee.empty())
    {
      out << " for " << callee;
    }
    out << ": " << Value::vector(args)->to_string() << "\n\n";

    out << (expected_signatures.size() > 1 ? "Expected one of:" : "Expected:");
    for (auto& signature : expected_signatures)
    {
      out << "\n  " << signature;
    }
    out << "\n";

    out << "Received:\n  [";
    for (size_t n = 0; n < args.size(); n++)
    {
      if (n > 0) out << " ";
      out << "<" << type_string(*args[n]) << ">";
    }
    out << "]\n";

    return out.str();
  }

  const char* NoMatchingSignatureException::what() const throw()
  {
    if (!message)
    {
      message = render_message();
    }
    return message->c_str();
  }

  std::exception_ptr NoMatchingSignatureException::with_callee(const Executable& callee) const
  {
    return std::make_exception_ptr(no_matching_signature(callee, args));
  }

  const std::string& NoMatchingSignatureException::get_callee() const
  {
    return callee;
  }

  const std::vector<std::string>& NoMatchingSignatureException::get_expected_signatures() const
  {
    return expected_signatures;
  }

  const std::vector<std::shared_ptr<Value>>& NoMatchingSignatureException::get_args() const
  {
    return args;
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
