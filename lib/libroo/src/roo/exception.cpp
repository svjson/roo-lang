
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

  InvocationException::InvocationException(const std::string& message,
                                           std::string callee,
                                           std::vector<std::string> expected_signatures,
                                           std::vector<std::shared_ptr<Value>> args)
    : RooException(message)
    , callee(std::move(callee))
    , expected_signatures(std::move(expected_signatures))
    , args(std::move(args))
  {
  }

  InvocationException InvocationException::no_matching_signature(
    const Executable& callee,
    const std::vector<std::shared_ptr<Value>>& args)
  {
    std::vector<std::string> expected_signatures;
    for (auto& signature : callee.get_signatures())
    {
      expected_signatures.push_back(signature->to_string());
    }

    std::ostringstream out;
    out << "No matching signature for " << callee.to_string() << ": ";
    out << Value::vector(args)->to_string() << "\n\n";

    if (expected_signatures.size() > 1)
    {
      out << "Expected one of:";
    }
    else
    {
      out << "Expected:";
    }
    for (auto& signature : expected_signatures)
    {
      out << "\n  " << signature;
    }
    out << "\n";

    out << "Received:";
    out << "\n  [";
    for (size_t n = 0; n < args.size(); n++)
    {
      if (n > 0) out << " ";
      out << "<" << Roo::type_string(*args[n]) << ">";
    }
    out << "]\n";

    return InvocationException(out.str(),
                               callee.to_string(),
                               std::move(expected_signatures),
                               args);
  }

  const std::string& InvocationException::get_callee() const
  {
    return callee;
  }

  const std::vector<std::string>& InvocationException::get_expected_signatures() const
  {
    return expected_signatures;
  }

  const std::vector<std::shared_ptr<Value>>& InvocationException::get_args() const
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
