#include <proof/native.h>

#include <lisple-package/native_abi.h>

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>
#include <lisple/runtime/node.h>
#include <lisple/runtime/value.h>

namespace Lisple::Proof
{
  namespace
  {
    class AssertionAbort : public std::exception
    {
     public:
      const char* what() const noexcept override
      {
        return "proof assertion aborted test body";
      }
    };

    bool is_symbol_named(const sptr_ast_node& node, const std::string& name)
    {
      return node->get_type() == Form::SYMBOL &&
             node->as<AST::Symbol>().get_identifier() == name;
    }

    bool is_equality_form(const sptr_ast_node& node)
    {
      if (node->get_type() != Form::LIST)
      {
        return false;
      }

      auto& children = node->get_children();
      return children.size() == 3 && is_symbol_named(children[0], "=");
    }

    sptr_val record_failure(Context& ctx, const std::string& message)
    {
      return ctx.call("proof.core/record-failure!", Value::string(message));
    }

    class AssertionForm : public SpecialForm
    {
      std::string form_name;
      bool abort_on_failure;

     public:
      AssertionForm(std::string form_name, bool abort_on_failure)
        : SpecialForm(SIG((FN_ARGS((&Type::ANY, NO_EVAL)),
                           EXEC_DISPATCH(&AssertionForm::execnode_assertion))))
        , form_name(std::move(form_name))
        , abort_on_failure(abort_on_failure)
      {
      }

      static sptr_val make(const std::string& form_name, bool abort_on_failure)
      {
        return Value::executable(
          std::make_shared<AssertionForm>(form_name, abort_on_failure));
      }

      uptr_exec_node lower_form(LowerContext& ctx, const sptr_ast_node& ast_node) override
      {
        auto& elements = ast_node->get_children();
        if (elements.size() != 2)
        {
          throw LispleException("Invalid " + form_name + " form: " +
                                ast_node->to_string());
        }

        sptr_ast_node expr = elements[1];
        sptr_val_v values{
          Value::boolean(abort_on_failure),
          Value::string(expr->to_string()),
          Value::keyword(is_equality_form(expr) ? "equality" : "truthy"),
        };

        uptr_exec_node_v exec_nodes;
        if (is_equality_form(expr))
        {
          auto& equality_children = expr->get_children();
          exec_nodes.push_back(lower_expr(ctx, equality_children[1]));
          exec_nodes.push_back(lower_expr(ctx, equality_children[2]));
        }
        else
        {
          exec_nodes.push_back(lower_expr(ctx, expr));
        }

        return std::make_unique<ExecNode>(
          SpecialFormNode(this, values, std::move(exec_nodes)));
      }

      sptr_val execnode_assertion(Context& ctx, SpecialFormNode& snode)
      {
        if (snode.values.size() != 3)
        {
          throw InvocationException("Invalid " + form_name + " execution node.");
        }

        const bool should_abort = std::get<bool>(snode.values[0]->value);
        const std::string& expr = snode.values[1]->str();
        const std::string& assertion_kind = snode.values[2]->str();

        bool passed = false;
        std::string message;

        if (assertion_kind == "equality")
        {
          if (snode.exec_nodes.size() != 2)
          {
            throw InvocationException("Invalid equality assertion execution node.");
          }
          sptr_val expected = exec(ctx, *snode.exec_nodes[0]);
          sptr_val actual = exec(ctx, *snode.exec_nodes[1]);
          passed = *expected == *actual;
          message = "Expected " + expected->to_string() + ", got " +
                    actual->to_string() + ".";
        }
        else
        {
          if (snode.exec_nodes.size() != 1)
          {
            throw InvocationException("Invalid truthy assertion execution node.");
          }
          sptr_val value = exec(ctx, *snode.exec_nodes[0]);
          passed = is_truthy(*value);
          message = "Expected truthy expression: " + expr + ".";
        }

        if (passed)
        {
          return Constant::BOOL_TRUE;
        }

        sptr_val failure = record_failure(ctx, message);
        if (should_abort)
        {
          throw AssertionAbort();
        }
        return failure;
      }
    };

    class PhaseForm : public SpecialForm
    {
      std::string form_name;

     public:
      explicit PhaseForm(std::string form_name)
        : SpecialForm(SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                           EXEC_DISPATCH(&PhaseForm::execnode_phase))))
        , form_name(std::move(form_name))
      {
      }

      static sptr_val make(const std::string& form_name)
      {
        return Value::executable(std::make_shared<PhaseForm>(form_name));
      }

      uptr_exec_node lower_form(LowerContext& ctx, const sptr_ast_node& ast_node) override
      {
        auto& elements = ast_node->get_children();
        uptr_exec_node_v body;
        body.reserve(elements.size() - 1);
        for (size_t i = 1; i < elements.size(); i++)
        {
          body.push_back(lower_expr(ctx, elements[i]));
        }

        return std::make_unique<ExecNode>(
          SpecialFormNode(this, {Value::keyword(form_name)}, std::move(body)));
      }

      sptr_val execnode_phase(Context& ctx, SpecialFormNode& snode)
      {
        sptr_val result = Constant::NIL;
        for (auto& node : snode.exec_nodes)
        {
          result = exec(ctx, *node);
        }
        return result;
      }
    };

    class RunTestBodyFunction : public Function
    {
     public:
      RunTestBodyFunction()
        : Function(SIG((FN_ARGS((&Type::EXEC)),
                        EXEC_DISPATCH(&RunTestBodyFunction::exec_run_test_body))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<RunTestBodyFunction>());
      }

      sptr_val exec_run_test_body(Context& ctx, sptr_val_v& args)
      {
        sptr_val_v body_args;
        try
        {
          return args[0]->exec().execute(ctx, body_args);
        }
        catch (const AssertionAbort&)
        {
          return Constant::NIL;
        }
      }
    };

    class DeftestForm : public SpecialForm
    {
     public:
      DeftestForm()
        : SpecialForm(SIG((FN_ARGS((&Type::ANY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                           EXEC_DISPATCH(&DeftestForm::execnode_deftest))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<DeftestForm>());
      }

      uptr_exec_node lower_form(LowerContext& ctx, const sptr_ast_node& ast_node) override
      {
        auto& elements = ast_node->get_children();
        if (elements.size() < 3)
        {
          throw LispleException("Invalid deftest form: " + ast_node->to_string());
        }

        if (!ctx.ctx)
        {
          throw LispleException("deftest requires an active lowering context.");
        }

        sptr_val name = std::get<LiteralNode>(lower_literal(elements[1])->data).value;
        auto arg_vec = std::make_shared<AST::Vector>();

        sptr_ast_node_v body;
        body.reserve(elements.size() - 2);
        for (size_t i = 2; i < elements.size(); i++)
        {
          body.push_back(elements[i]);
        }

        std::shared_ptr<UserFunction> body_fn =
          create_function("test:" + name->to_string(),
                          *ctx.ctx,
                          ctx.ctx->get_current_namespace(),
                          *arg_vec,
                          body);

        return std::make_unique<ExecNode>(
          SpecialFormNode(this, {name, Value::executable(body_fn)}, {}));
      }

      sptr_val execnode_deftest(Context& ctx, SpecialFormNode& snode)
      {
        if (snode.values.size() != 2)
        {
          throw InvocationException("Invalid deftest execution node.");
        }

        ctx.call("proof.core/register-test!", snode.values);
        return snode.values.front();
      }
    };
  } // namespace

  std::unique_ptr<Namespace> make_native_namespace()
  {
    auto ns = std::make_unique<Namespace>("proof.syntax");
    ns->set_origin(Namespace::Origin::native());
    ns->store("deftest", DeftestForm::make());
    ns->store("is", AssertionForm::make("is", true));
    ns->store("should", AssertionForm::make("should", true));
    ns->store("assert", AssertionForm::make("assert", true));
    ns->store("expect", AssertionForm::make("expect", false));
    ns->store("given", PhaseForm::make("given"));
    ns->store("when", PhaseForm::make("when"));
    ns->store("then", PhaseForm::make("then"));
    ns->store("run-test-body", RunTestBodyFunction::make());
    return ns;
  }

  std::vector<std::unique_ptr<Namespace>> make_native_namespaces()
  {
    std::vector<std::unique_ptr<Namespace>> namespaces;
    namespaces.push_back(make_native_namespace());
    return namespaces;
  }
} // namespace Lisple::Proof

namespace
{
  int load_proof_native(const LispleNativeHostV1* host)
  {
    auto ns = Lisple::Proof::make_native_namespace();
    return host->register_namespace(host->user, ns.release());
  }

  void unload_proof_native()
  {
  }

  const char* proof_native_last_error()
  {
    return "";
  }
} // namespace

extern "C" LISPLE_NATIVE_EXPORT const LispleNativePackageV1*
lisple_native_package_v1()
{
  static const LispleNativePackageV1 package{
    LISPLE_NATIVE_ABI_VERSION,
    sizeof(LispleNativePackageV1),
    "proof-native",
    "0.1.0",
    LISPLE_NATIVE_CXX_ABI,
    load_proof_native,
    unload_proof_native,
    proof_native_last_error,
  };
  return &package;
}
