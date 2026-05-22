#ifndef LISPLE_PACKAGE_APPLICATION_H
#define LISPLE_PACKAGE_APPLICATION_H

#include <cstddef>
#include <string>
#include <vector>

#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/runtime.h>
#include <lisple/runtime/value.h>

namespace Lisple::Package::Application
{
  inline std::string qualifier_of(const std::string& symbol)
  {
    const auto separator = symbol.rfind('/');
    if (separator == std::string::npos || separator == 0)
    {
      return "";
    }
    return symbol.substr(0, separator);
  }

  inline sptr_val cli_args_value(const std::vector<std::string>& args)
  {
    sptr_val_v values;
    values.reserve(args.size());
    for (const auto& arg : args)
    {
      values.push_back(Value::string(arg));
    }
    return Value::vector(values);
  }

  inline sptr_val cli_args_value(int argc, char** argv)
  {
    std::vector<std::string> args;
    if (argc > 1)
    {
      args.reserve(static_cast<std::size_t>(argc - 1));
    }
    for (int i = 1; i < argc; ++i)
    {
      args.push_back(argv[i]);
    }
    return cli_args_value(args);
  }

  inline std::size_t declared_main_arity(const Executable& executable,
                                         const std::string& main_function)
  {
    const Signature* fallback = nullptr;
    for (const auto& signature : executable.get_signatures())
    {
      if (!signature->supports_rt_value())
      {
        continue;
      }
      if (!fallback)
      {
        fallback = signature.get();
      }
      if (!signature->has_rest_parameter())
      {
        return signature->get_arguments().size();
      }
    }
    if (fallback)
    {
      return fallback->get_arguments().size();
    }
    throw LispleException("Package :main is not callable as a runtime function: " +
                          main_function);
  }

  inline sptr_val_v main_args_for_arity(std::size_t arity, const sptr_val& cli_args)
  {
    sptr_val_v args;
    if (arity == 0)
    {
      return args;
    }

    args.reserve(arity);
    args.push_back(cli_args);
    while (args.size() < arity)
    {
      args.push_back(Constant::NIL);
    }
    return args;
  }

  inline sptr_val invoke_main(Runtime& runtime,
                              const std::string& main_function,
                              const sptr_val& cli_args)
  {
    if (main_function.empty())
    {
      return Constant::NIL;
    }

    const auto main_namespace = qualifier_of(main_function);
    if (main_namespace.empty())
    {
      throw LispleException("Package :main must be a qualified function.");
    }

    runtime.eval("(ns lisple.package.main (:require " + main_namespace + "))",
                 "<package-main>");

    const auto main_value = runtime.lookup(main_function);
    if (main_value->type != Value::Type::FUNCTION)
    {
      throw LispleException("Package :main is not a function: " + main_function);
    }

    sptr_val_v args =
      main_args_for_arity(declared_main_arity(main_value->exec(), main_function), cli_args);
    return runtime.invoke(main_function, args);
  }

  inline sptr_val invoke_main(Runtime& runtime,
                              const std::string& main_function,
                              const std::vector<std::string>& args)
  {
    return invoke_main(runtime, main_function, cli_args_value(args));
  }

  inline sptr_val invoke_main(Runtime& runtime,
                              const std::string& main_function,
                              int argc,
                              char** argv)
  {
    return invoke_main(runtime, main_function, cli_args_value(argc, argv));
  }
} // namespace Lisple::Package::Application

#endif
