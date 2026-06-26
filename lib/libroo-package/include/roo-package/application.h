#ifndef ROO_PACKAGE_APPLICATION_H
#define ROO_PACKAGE_APPLICATION_H

#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/runtime.h>
#include <roo/runtime/value.h>

#include <roo-package/manifest.h>

namespace Roo::Package::Application
{
  namespace Constant = Roo::Constant;
  using Roo::Executable;
  using Roo::RooException;
  using Roo::Runtime;
  using Roo::Signature;
  using Roo::sptr_val;
  using Roo::sptr_val_v;
  using Roo::Value;

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

  inline std::vector<std::string> cli_args_vector(int argc, char** argv)
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
    return args;
  }

  inline sptr_val cli_args_value(int argc, char** argv)
  {
    return cli_args_value(cli_args_vector(argc, argv));
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
    throw RooException("Package :main is not callable as a runtime function: " +
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
      throw RooException("Package :main must be a qualified function.");
    }

    runtime.eval("(ns roo.package.main (:require " + main_namespace + "))",
                 "<package-main>");

    const auto main_value = runtime.lookup(main_function);
    if (main_value->type != Value::Type::FUNCTION)
    {
      throw RooException("Package :main is not a function: " + main_function);
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

  inline const PackageInfo* find_package(const LoadPlan& plan,
                                         const std::string& package_name)
  {
    for (const auto& package : plan.packages)
    {
      if (package.name == package_name)
      {
        return &package;
      }
    }
    return nullptr;
  }

  inline const PackageInfo* root_package(const LoadPlan& plan)
  {
    for (const auto& package : plan.packages)
    {
      if (package.package_root == plan.package_root)
      {
        return &package;
      }
    }
    return nullptr;
  }

  inline std::string source_string_literal(const std::string& value)
  {
    std::string result = "\"";
    for (const char c : value)
    {
      switch (c)
      {
      case '\\':
        result += "\\\\";
        break;
      case '"':
        result += "\\\"";
        break;
      case '\n':
        result += "\\n";
        break;
      case '\r':
        result += "\\r";
        break;
      case '\t':
        result += "\\t";
        break;
      default:
        result += c;
        break;
      }
    }
    result += "\"";
    return result;
  }

  inline std::string source_string_vector_literal(const std::vector<std::string>& values)
  {
    std::ostringstream stream;
    stream << "[";
    for (std::size_t i = 0; i < values.size(); ++i)
    {
      if (i > 0)
      {
        stream << " ";
      }
      stream << source_string_literal(values[i]);
    }
    stream << "]";
    return stream.str();
  }

  inline std::string tool_invocation_context(const LoadPlan& plan,
                                             const PackageInfo& package,
                                             const PackageInfo& tool_package,
                                             const std::string& tool_name,
                                             const std::vector<std::string>& args)
  {
    const auto config_it = package.config.find(tool_package.name);
    const std::string config = config_it == package.config.end() ? "{}" : config_it->second;

    std::ostringstream stream;
    stream << "{:package-root " << source_string_literal(package.package_root)
           << " :package-name " << source_string_literal(package.name)
           << " :package-version " << source_string_literal(package.version)
           << " :package-load-roots " << source_string_vector_literal(package.load_roots)
           << " :load-paths " << source_string_vector_literal(plan.load_paths)
           << " :tool-package " << source_string_literal(tool_package.name) << " :tool-name "
           << source_string_literal(tool_name) << " :config " << config << " :args "
           << source_string_vector_literal(args) << "}";
    return stream.str();
  }

  inline sptr_val invoke_tool(Runtime& runtime,
                              const LoadPlan& plan,
                              const std::string& tool_package_name,
                              const std::string& tool_name = "run",
                              const std::vector<std::string>& args = {})
  {
    const auto* package = root_package(plan);
    if (!package)
    {
      throw RooException("Package load plan has no root package metadata.");
    }

    const auto* tool_package = find_package(plan, tool_package_name);
    if (!tool_package)
    {
      throw RooException("Package '" + package->name + "' has no dependency named '" +
                         tool_package_name + "'.");
    }

    const auto tool_it = tool_package->tools.find(tool_name);
    if (tool_it == tool_package->tools.end())
    {
      throw RooException("Package '" + tool_package_name + "' does not declare a '" +
                         tool_name + "' tool in package.edn.");
    }

    const std::string& tool_function = tool_it->second;
    const std::string tool_namespace = qualifier_of(tool_function);
    if (tool_namespace.empty())
    {
      throw RooException("Package tool '" + tool_package_name + "/" + tool_name +
                         "' must be a qualified function.");
    }

    runtime.eval("(ns roo.package.tool (:require " + tool_namespace + "))",
                 "<package-tool>");
    return runtime.eval("(" + tool_function + " " +
                          tool_invocation_context(plan,
                                                  *package,
                                                  *tool_package,
                                                  tool_name,
                                                  args) +
                          ")",
                        "<package-tool>");
  }

  inline sptr_val invoke_tool(Runtime& runtime,
                              const LoadPlan& plan,
                              const std::string& tool_package_name,
                              int argc,
                              char** argv)
  {
    return invoke_tool(runtime, plan, tool_package_name, "run", cli_args_vector(argc, argv));
  }
} // namespace Roo::Package::Application

#endif
