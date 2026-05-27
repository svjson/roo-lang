#include <algorithm>
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
#include <lisple/io/file_system.h>
#include <lisple/reader.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>
#include <lisple/runtime/node.h>
#include <lisple/runtime/value.h>

#include <lisple-package/native_abi.h>
#include <proof/native.h>

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

    std::string declared_namespace_name(const std::string& source,
                                        const std::string& source_name)
    {
      Reader reader;
      sptr_ast_node_v forms;
      try
      {
        forms = reader.read_sexps(source);
      }
      catch (const ParseException& e)
      {
        throw ParseException("Error parsing '" + source_name + "': " + e.what());
      }

      for (const auto& form : forms)
      {
        if (form->get_type() != Form::LIST)
        {
          continue;
        }

        const auto& children = form->get_children();
        if (children.size() < 2 || !is_symbol_named(children[0], "ns"))
        {
          continue;
        }

        if (children[1]->get_type() != Form::SYMBOL)
        {
          throw NamespaceException("Invalid ns form in '" + source_name +
                                   "', expected namespace symbol: " + form->to_string());
        }

        return children[1]->as<AST::Symbol>().value;
      }

      throw NamespaceException("No ns declaration found in '" + source_name + "'.");
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

    sptr_ast_node symbol_node(const std::string& name)
    {
      return AST::Symbol::make(name);
    }

    sptr_ast_node vector_node(const sptr_ast_node_v& children)
    {
      return AST::Vector::make(children);
    }

    sptr_ast_node list_node(const sptr_ast_node_v& children)
    {
      return AST::List::make(children);
    }

    bool is_rest_parameter(const sptr_ast_node& node)
    {
      if (node->get_type() != Form::SYMBOL)
      {
        return false;
      }

      const std::string& sym = node->as<AST::Symbol>().get_identifier();
      return sym.size() > 1 && sym[0] == '&';
    }

    bool is_optional_marker(const sptr_ast_node& node)
    {
      return is_symbol_named(node, "&");
    }

    size_t phase_arg_count(AST::Vector& arg_vec,
                           size_t available_count,
                           const std::string& phase_name)
    {
      size_t required_count = 0;
      size_t total_count = 0;
      bool in_optional = false;

      for (auto& child : arg_vec.get_children())
      {
        if (is_rest_parameter(child))
        {
          return available_count;
        }
        if (is_optional_marker(child))
        {
          in_optional = true;
          continue;
        }

        total_count++;
        if (!in_optional)
        {
          required_count++;
        }
      }

      if (required_count > available_count)
      {
        throw LispleException(
          "Invalid " + phase_name + " argument vector, expected at most " +
          std::to_string(available_count) + " required arguments: " + arg_vec.to_string());
      }

      return std::min(total_count, available_count);
    }

    std::string phase_name(const sptr_ast_node& node)
    {
      if (node->get_type() != Form::LIST)
      {
        return "";
      }

      auto& children = node->get_children();
      if (children.empty() || children[0]->get_type() != Form::SYMBOL)
      {
        return "";
      }

      const std::string& name = children[0]->as<AST::Symbol>().get_identifier();
      if (name == "given" || name == "when" || name == "then")
      {
        return name;
      }
      return "";
    }

    bool is_phase_form(const sptr_ast_node& node)
    {
      return !phase_name(node).empty();
    }

    sptr_ast_node fn_node(const sptr_ast_node& arg_vec, const sptr_ast_node_v& body)
    {
      sptr_ast_node_v children;
      children.reserve(body.size() + 2);
      children.push_back(symbol_node("fn"));
      children.push_back(arg_vec);
      children.insert(children.end(), body.begin(), body.end());
      return list_node(children);
    }

    sptr_ast_node apply_node(const sptr_ast_node& fn, const sptr_ast_node_v& args)
    {
      return list_node({symbol_node("apply"), fn, vector_node(args)});
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
          throw LispleException("Invalid " + form_name + " form: " + ast_node->to_string());
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
          message =
            "Expected " + expected->to_string() + ", got " + actual->to_string() + ".";
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

      uptr_exec_node lower_form(LowerContext&, const sptr_ast_node& ast_node) override
      {
        throw LispleException(
          form_name +
          " may only be used as a top-level deftest phase: " + ast_node->to_string());
      }

      sptr_val execnode_phase(Context&, SpecialFormNode&)
      {
        throw InvocationException("Invalid " + form_name + " execution node.");
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

    class DeclaredNamespaceFunction : public Function
    {
     public:
      DeclaredNamespaceFunction()
        : Function(SIG((FN_ARGS((&Type::STRING)),
                        EXEC_DISPATCH(&DeclaredNamespaceFunction::exec_declared_namespace))))
      {
      }

      static sptr_val make()
      {
        return Value::executable(std::make_shared<DeclaredNamespaceFunction>());
      }

      sptr_val exec_declared_namespace(Context& ctx, sptr_val_v& args)
      {
        const std::string& path = args[0]->str();
        return Value::string(declared_namespace_name(ctx.file_system().read(path), path));
      }
    };

    struct ScenarioPhase
    {
      std::string name;
      sptr_ast_node form;
      sptr_ast_node_v after;
    };

    sptr_ast_node phase_arg_vector(const ScenarioPhase& phase)
    {
      auto& elements = phase.form->get_children();
      if (phase.name == "given")
      {
        return vector_node({});
      }

      if (elements.size() < 2 || elements[1]->get_type() != Form::VECTOR)
      {
        throw LispleException("Invalid " + phase.name +
                              " form, expected argument vector: " + phase.form->to_string());
      }

      return elements[1];
    }

    sptr_ast_node_v phase_body(const ScenarioPhase& phase)
    {
      auto& elements = phase.form->get_children();
      const size_t body_start = phase.name == "given" ? 1 : 2;
      sptr_ast_node_v body;
      body.reserve(elements.size() - body_start);
      for (size_t i = body_start; i < elements.size(); i++)
      {
        body.push_back(elements[i]);
      }
      return body;
    }

    sptr_ast_node_v phase_args(const ScenarioPhase& phase,
                               const std::vector<std::string>& available_symbols)
    {
      auto arg_vec = phase_arg_vector(phase);
      const size_t arg_count =
        phase_arg_count(arg_vec->as<AST::Vector>(), available_symbols.size(), phase.name);

      sptr_ast_node_v args;
      args.reserve(arg_count);
      for (size_t i = 0; i < arg_count; i++)
      {
        args.push_back(symbol_node(available_symbols[i]));
      }
      return args;
    }

    void append_all(sptr_ast_node_v& target, const sptr_ast_node_v& source)
    {
      target.insert(target.end(), source.begin(), source.end());
    }

    sptr_ast_node then_expr(const ScenarioPhase& phase)
    {
      return apply_node(fn_node(phase_arg_vector(phase), phase_body(phase)),
                        phase_args(phase, {"__proof-when-result", "__proof-given-result"}));
    }

    sptr_ast_node when_expr(const ScenarioPhase& phase,
                            const std::vector<ScenarioPhase>& then_phases)
    {
      sptr_ast_node_v bindings{
        symbol_node("__proof-when-fn"),
        fn_node(phase_arg_vector(phase), phase_body(phase)),
        symbol_node("__proof-when-result"),
        apply_node(symbol_node("__proof-when-fn"),
                   phase_args(phase, {"__proof-given-result"})),
      };

      sptr_ast_node_v body = phase.after;
      for (auto& then_phase : then_phases)
      {
        body.push_back(then_expr(then_phase));
        append_all(body, then_phase.after);
      }
      if (body.empty())
      {
        body.push_back(symbol_node("__proof-when-result"));
      }

      sptr_ast_node_v children{symbol_node("let"), vector_node(bindings)};
      append_all(children, body);
      return list_node(children);
    }

    sptr_ast_node scenario_expr(const std::vector<ScenarioPhase>& phases)
    {
      const ScenarioPhase* given_phase = nullptr;
      const ScenarioPhase* when_phase = nullptr;
      std::vector<ScenarioPhase> then_phases;

      for (auto& phase : phases)
      {
        if (phase.name == "given")
        {
          given_phase = &phase;
        }
        else if (phase.name == "when")
        {
          when_phase = &phase;
        }
        else if (phase.name == "then")
        {
          then_phases.push_back(phase);
        }
      }

      if (!when_phase && !then_phases.empty())
      {
        throw LispleException("Invalid deftest scenario, then requires a preceding when.");
      }

      sptr_ast_node_v bindings{
        symbol_node("__proof-given-result"),
        given_phase
          ? apply_node(fn_node(phase_arg_vector(*given_phase), phase_body(*given_phase)), {})
          : AST::NIL,
      };

      sptr_ast_node_v body;
      if (given_phase)
      {
        append_all(body, given_phase->after);
      }
      if (when_phase)
      {
        body.push_back(when_expr(*when_phase, then_phases));
      }
      if (body.empty())
      {
        body.push_back(symbol_node("__proof-given-result"));
      }

      sptr_ast_node_v children{symbol_node("let"), vector_node(bindings)};
      append_all(children, body);
      return list_node(children);
    }

    std::vector<ScenarioPhase> parse_scenario_phases(const sptr_ast_node_v& body,
                                                     size_t start,
                                                     size_t end)
    {
      std::vector<ScenarioPhase> phases;
      int order = -1;
      bool seen_given = false;
      bool seen_when = false;

      for (size_t i = start; i <= end; i++)
      {
        const std::string name = phase_name(body[i]);
        if (name.empty())
        {
          phases.back().after.push_back(body[i]);
          continue;
        }

        const int next_order = name == "given" ? 0 : name == "when" ? 1 : 2;
        if (next_order < order)
        {
          throw LispleException("Invalid deftest scenario phase order: " +
                                body[i]->to_string());
        }
        order = next_order;

        if (name == "given")
        {
          if (seen_given)
          {
            throw LispleException("Invalid deftest scenario, duplicate given.");
          }
          seen_given = true;
        }
        else if (name == "when")
        {
          if (seen_when)
          {
            throw LispleException("Invalid deftest scenario, duplicate when.");
          }
          seen_when = true;
        }

        phases.push_back({name, body[i], {}});
      }

      return phases;
    }

    sptr_ast_node_v rewrite_scenario_body(const sptr_ast_node_v& body)
    {
      size_t start = body.size();
      size_t end = 0;

      for (size_t i = 0; i < body.size(); i++)
      {
        if (is_phase_form(body[i]))
        {
          start = std::min(start, i);
          end = i;
        }
      }

      if (start == body.size())
      {
        return body;
      }

      sptr_ast_node_v rewritten;
      rewritten.reserve(body.size() - (end - start));
      for (size_t i = 0; i < start; i++)
      {
        rewritten.push_back(body[i]);
      }
      rewritten.push_back(scenario_expr(parse_scenario_phases(body, start, end)));
      for (size_t i = end + 1; i < body.size(); i++)
      {
        rewritten.push_back(body[i]);
      }

      return rewritten;
    }

    class DeftestForm : public SpecialForm
    {
     public:
      DeftestForm()
        : SpecialForm(SIG((FN_ARGS((&Type::ANY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                           EXEC_DISPATCH(&DeftestForm::execnode_deftest))))
      {
      }

      static sptr_val make() { return Value::executable(std::make_shared<DeftestForm>()); }

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
        body = rewrite_scenario_body(body);

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
    ns->store("declared-namespace", DeclaredNamespaceFunction::make());
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

  void unload_proof_native() {}

  const char* proof_native_last_error()
  {
    return "";
  }
} // namespace

extern "C" LISPLE_NATIVE_EXPORT const LispleNativePackageV1* lisple_native_package_v1()
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
