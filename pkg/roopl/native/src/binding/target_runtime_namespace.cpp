#include <string>
#include <vector>

#include <roo/exception.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>

#include <binding/target/dedicated_adapter.h>
#include <binding/target_runtime_namespace.h>

namespace Roo::Roopl
{
  namespace
  {
    std::string required_string(const sptr_val& value,
                                const std::string& field,
                                const std::string& operation)
    {
      if (value->type != Value::Type::STRING)
      {
        throw TypeError(operation + " :" + field + " must be a string.");
      }
      return value->str();
    }

    Target::DedicatedOptions target_options(const sptr_val& options)
    {
      Target::DedicatedOptions result;
      const auto load_paths = Dict::get_property(options, *Value::keyword("load-paths"));
      if (load_paths->type != Value::Type::NIL)
      {
        if (load_paths->type != Value::Type::VECTOR)
        {
          throw TypeError("open! :load-paths must be a vector of strings.");
        }
        for (const auto& path : load_paths->elements())
        {
          result.load_paths.push_back(required_string(path, "load-paths", "open!"));
        }
      }

      const auto package_root = Dict::get_property(options, *Value::keyword("package-root"));
      if (package_root->type != Value::Type::NIL)
      {
        result.package_root = required_string(package_root, "package-root", "open!");
      }
      return result;
    }

    Target::EvaluationRequest evaluation_request(const sptr_val& request)
    {
      Target::EvaluationRequest result{
        required_string(Dict::get_property(request, *Value::keyword("source")),
                        "source",
                        "eval!"),
        required_string(Dict::get_property(request, *Value::keyword("namespace")),
                        "namespace",
                        "eval!"),
        required_string(Dict::get_property(request, *Value::keyword("source-name")),
                        "source-name",
                        "eval!"),
        std::nullopt,
      };

      const auto print_width = Dict::get_property(request, *Value::keyword("print-width"));
      if (print_width->type != Value::Type::NIL)
      {
        if (print_width->type != Value::Type::NUMBER)
        {
          throw TypeError("eval! :print-width must be a number.");
        }
        const long width = print_width->i64();
        if (width < 0)
        {
          throw TypeError("eval! :print-width must not be negative.");
        }
        result.print_width = static_cast<std::size_t>(width);
      }
      return result;
    }
  } // namespace

  namespace Function
  {
    FUNC_IMPL(OpenTargetFunction,
              SIG((FN_ARGS((&Type::MAP)),
                   EXEC_DISPATCH(&OpenTargetFunction::exec_open_target))))

    EXEC_BODY(OpenTargetFunction, exec_open_target)
    {
      return DedicatedTargetAdapter::make_unique(target_options(args[0]));
    }

    FUNC_IMPL(EvalTargetFunction,
              SIG((FN_ARGS((&ROOPL_DEDICATED_TARGET_TYPE), (&Type::MAP)),
                   EXEC_DISPATCH(&EvalTargetFunction::exec_eval_target))))

    EXEC_BODY(EvalTargetFunction, exec_eval_target)
    {
      return args[0]->adapter<DedicatedTargetAdapter>().get_object().evaluate(
        evaluation_request(args[1]));
    }

    FUNC_IMPL(CloseTargetFunction,
              SIG((FN_ARGS((&ROOPL_DEDICATED_TARGET_TYPE)),
                   EXEC_DISPATCH(&CloseTargetFunction::exec_close_target))))

    EXEC_BODY(CloseTargetFunction, exec_close_target)
    {
      args[0]->adapter<DedicatedTargetAdapter>().get_object().close();
      return Constant::NIL;
    }
  } // namespace Function

  TargetRuntimeNamespace::TargetRuntimeNamespace()
    : Roo::Namespace(std::string(NS__ROOPL__TARGET__RUNTIME))
  {
    set_origin(Namespace::Origin::native());
    store("open!", Function::OpenTargetFunction::make());
    store("eval!", Function::EvalTargetFunction::make());
    store("close!", Function::CloseTargetFunction::make());
  }
} // namespace Roo::Roopl
