#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <roo/exception.h>
#include <roo/io/dir_root_file_system.h>
#include <roo/runtime.h>
#include <roo/runtime/pretty_print.h>
#include <roo/runtime/value.h>

#include <roo-package/manifest.h>
#include <roo-package/runtime_environment.h>
#include <target/dedicated.h>

namespace Roo::Roopl::Target
{
  namespace
  {
    std::string error_kind(ErrorCategory category)
    {
      switch (category)
      {
      case ErrorCategory::PARSE:
        return "parse";
      case ErrorCategory::FORM:
        return "form";
      case ErrorCategory::IDENTIFIER:
        return "identifier";
      case ErrorCategory::INVOCATION:
        return "invocation";
      case ErrorCategory::NAMESPACE:
        return "namespace";
      case ErrorCategory::TYPE:
        return "type";
      case ErrorCategory::IO:
        return "io";
      case ErrorCategory::GENERAL:
        return "general";
      }
      return "general";
    }

    sptr_val success(const std::string& namespace_name, const std::string& display)
    {
      return Value::map({
        Value::keyword("status"),
        Value::keyword("ok"),
        Value::keyword("namespace"),
        Value::string(namespace_name),
        Value::keyword("display"),
        Value::string(display),
      });
    }

    sptr_val failure(const std::string& namespace_name,
                     const std::string& kind,
                     const std::string& message)
    {
      return Value::map({
        Value::keyword("status"),
        Value::keyword("error"),
        Value::keyword("namespace"),
        Value::string(namespace_name),
        Value::keyword("kind"),
        Value::keyword(kind),
        Value::keyword("message"),
        Value::string(message),
      });
    }

    std::optional<Package::LoadPlan> load_plan(const DedicatedOptions& options)
    {
      DirRootFileSystem manifest_fs("/");
      std::optional<std::string> package_root = options.package_root;
      if (!package_root)
      {
        package_root =
          Package::find_package_root(manifest_fs, std::filesystem::current_path().string());
      }
      if (!package_root)
      {
        return std::nullopt;
      }
      return Package::resolve_load_plan(manifest_fs, *package_root);
    }
  } // namespace

  struct Dedicated::State
  {
    Package::ApplicationRuntime application;

    explicit State(const Package::ApplicationRuntimeSpec& spec)
      : application(spec)
    {
    }
  };

  Dedicated::Dedicated(DedicatedOptions options)
  {
    auto package_plan = load_plan(options);
    if (options.load_paths.empty())
    {
      options.load_paths = {std::filesystem::current_path().string(), "/"};
    }

    Package::ApplicationRuntimeSpec runtime_spec =
      Package::make_directory_application_runtime_spec(
        package_plan.value_or(Package::LoadPlan{}),
        options.load_paths);
    state = std::make_unique<State>(runtime_spec);
  }

  Dedicated::~Dedicated() = default;

  sptr_val Dedicated::evaluate(const EvaluationRequest& request)
  {
    if (!state)
    {
      throw RooException("The dedicated Roo target is closed.");
    }

    try
    {
      Runtime& runtime = state->application.runtime();
      runtime.switch_namespace(request.namespace_name);
      const auto value = runtime.eval(request.source, request.source_name);
      Pretty::PrintOptions print_options;
      if (request.print_width)
      {
        print_options.width = *request.print_width;
      }
      return success(runtime.get_current_namespace().get_name(),
                     Pretty::print(*value, print_options));
    }
    catch (const RooException& e)
    {
      return failure(state->application.runtime().get_current_namespace().get_name(),
                     error_kind(e.get_diagnostic().category),
                     e.what());
    }
    catch (const std::exception& e)
    {
      return failure(state->application.runtime().get_current_namespace().get_name(),
                     "general",
                     e.what());
    }
  }

  void Dedicated::close()
  {
    state.reset();
  }
} // namespace Roo::Roopl::Target
