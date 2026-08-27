#include <scenario.h>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/form.h>
#include <roo/io/file_system.h>
#include <roo/reader.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/lower.h>

#include <binding/syntax_namespace.h>

namespace Roo::Proof
{
  namespace
  {
    bool is_equality_form(const sptr_ast_node& node)
    {
      if (node->get_type() != Form::LIST)
      {
        return false;
      }

      auto& children = node->get_children();
      return children.size() == 3 && children[0]->get_type() == Form::SYMBOL &&
             children[0]->as<AST::Symbol>().get_identifier() == "=";
    }

    sptr_val record_failure(Context& ctx, const std::string& message)
    {
      return ctx.call("proof.core/record-failure!", Value::string(message));
    }

    std::string assertion_failure_message(Context& ctx,
                                          const std::string& kind,
                                          const std::string& expr,
                                          const sptr_val& expected,
                                          const sptr_val& actual)
    {
      sptr_val message = ctx.call("proof.assertion/format-failure-message",
                                  {
                                    Value::keyword(kind),
                                    Value::string(expr),
                                    expected,
                                    actual,
                                  });
      if (message->type != Value::Type::STRING)
      {
        throw InvocationException(
          "proof.assertion/format-failure-message must return a string.");
      }
      return message->str();
    }
  } // namespace

  namespace Function
  {
    AssertionForm::AssertionForm(std::string form_name, bool abort_on_failure)
      : SpecialForm(SIG((FN_ARGS((&Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&AssertionForm::execnode_assertion))))
      , form_name(std::move(form_name))
      , abort_on_failure(abort_on_failure)
    {
    }

    IsForm::IsForm()
      : AssertionForm("is", true)
    {
    }

    ShouldForm::ShouldForm()
      : AssertionForm("should", true)
    {
    }

    AssertForm::AssertForm()
      : AssertionForm("assert", true)
    {
    }

    ExpectForm::ExpectForm()
      : AssertionForm("expect", false)
    {
    }

    SFORM_LOWER_IMPL(AssertionForm)
    {
      auto& elements = ast_node->get_children();
      if (elements.size() != 2)
      {
        throw RooException("Invalid " + form_name + " form: " + ast_node->to_string());
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

    EXECNODE_BODY(AssertionForm, execnode_assertion)
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
        message = assertion_failure_message(ctx, assertion_kind, expr, expected, actual);
      }
      else
      {
        if (snode.exec_nodes.size() != 1)
        {
          throw InvocationException("Invalid truthy assertion execution node.");
        }
        sptr_val value = exec(ctx, *snode.exec_nodes[0]);
        passed = is_truthy(*value);
        message = assertion_failure_message(ctx, assertion_kind, expr, Constant::NIL, value);
      }

      if (passed)
      {
        return Constant::BOOL_TRUE;
      }

      sptr_val failure = record_failure(ctx, message);
      if (should_abort)
      {
        return ctx.call("raise",
                        {
                          Value::keyword("proof/assertion-abort"),
                          Value::string(message),
                        });
      }
      return failure;
    }

    /** DefTestForm - proof.syntax/deftest */
    SPECIAL_FORM_IMPL(DefTestForm,
                      SIG((FN_ARGS((&Type::ANY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                           EXEC_DISPATCH(&DefTestForm::execnode_deftest))))

    SFORM_LOWER_IMPL(DefTestForm)
    {
      auto& elements = ast_node->get_children();
      if (elements.size() < 3)
      {
        throw RooException("Invalid deftest form: " + ast_node->to_string());
      }

      if (!ctx.ctx)
      {
        throw RooException("deftest requires an active lowering context.");
      }

      sptr_val name = std::get<LiteralNode>(lower_literal(ctx, elements[1])->data).value;
      auto arg_vec = std::make_shared<AST::Vector>();
      const bool has_directives = elements.size() >= 4 &&
                                  elements[2]->get_type() == Form::MAP &&
                                  elements[3]->get_type() == Form::VECTOR;
      const bool has_fixtures = has_directives || elements[2]->get_type() == Form::VECTOR;
      const size_t fixture_index = has_directives ? 3 : 2;
      const size_t body_start = has_directives ? 4 : has_fixtures ? 3 : 2;

      if (body_start >= elements.size())
      {
        throw RooException("Invalid deftest form: " + ast_node->to_string());
      }

      sptr_ast_node_v body;
      body.reserve(elements.size() - body_start);
      for (size_t i = body_start; i < elements.size(); i++)
      {
        body.push_back(elements[i]);
      }
      body = rewrite_scenario_body(body);

      if (has_directives)
      {
        sptr_ast_node before = AST::NIL;
        sptr_ast_node after = AST::NIL;
        auto& directive_elements = elements[2]->get_children();
        for (size_t i = 0; i < directive_elements.size(); i += 2)
        {
          const auto& key = directive_elements[i];
          if (key->get_type() != Form::KEYWORD ||
              (!key->has_value("before") && !key->has_value("after")))
          {
            throw RooException("Invalid deftest directive " + key->to_string() +
                               ", expected :before or :after.");
          }

          sptr_ast_node directive = directive_elements[i + 1];
          if (directive->get_type() == Form::SYMBOL)
          {
            directive = AST::List::make({directive});
          }

          sptr_ast_node_v function_elements{
            AST::Symbol::make("fn"),
            std::make_shared<AST::Vector>(),
            directive,
          };
          if (key->has_value("before"))
          {
            before = AST::List::make(function_elements);
          }
          else
          {
            after = AST::List::make(function_elements);
          }
        }

        sptr_ast_node_v body_function_elements{
          AST::Symbol::make("fn"),
          std::make_shared<AST::Vector>(),
        };
        body_function_elements.insert(body_function_elements.end(),
                                      body.begin(),
                                      body.end());
        body = {AST::List::make({
          AST::Symbol::make("proof.core/run-test-lifecycle"),
          before,
          AST::List::make(body_function_elements),
          after,
        })};
      }

      if (has_fixtures)
      {
        sptr_ast_node_v fixture_body{AST::Symbol::make("proof.fixture/using-fixtures"),
                                     elements[fixture_index]};
        fixture_body.insert(fixture_body.end(), body.begin(), body.end());
        body = {AST::List::make(fixture_body)};
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

    EXECNODE_BODY(DefTestForm, execnode_deftest)
    {
      if (snode.values.size() != 2)
      {
        throw InvocationException("Invalid deftest execution node.");
      }

      sptr_val_v args = snode.values;
      args.push_back(Value::string(ctx.get_current_namespace()->get_name()));
      ctx.call("proof.core/register-test!", args);
      return snode.values.front();
    }

    /** DeclaredNamespaceFunction - proof.syntax/declared-namespace */
    FUNC_IMPL(DeclaredNamespaceFunction,
              SIG((FN_ARGS((&Type::STRING)),
                   EXEC_DISPATCH(&DeclaredNamespaceFunction::exec_declared_namespace))))

    EXEC_BODY(DeclaredNamespaceFunction, exec_declared_namespace)
    {
      const std::string& path = args[0]->str();
      Reader reader;
      sptr_ast_node_v forms;
      try
      {
        forms = reader.read_sexps(ctx.file_system().read(path));
      }
      catch (const ParseException& e)
      {
        throw ParseException("Error parsing '" + path + "': " + e.what());
      }

      for (const auto& form : forms)
      {
        if (form->get_type() != Form::LIST)
        {
          continue;
        }

        const auto& children = form->get_children();
        if (children.size() < 2 || children[0]->get_type() != Form::SYMBOL ||
            children[0]->as<AST::Symbol>().get_identifier() != "ns")
        {
          continue;
        }

        if (children[1]->get_type() != Form::SYMBOL)
        {
          throw NamespaceException("Invalid ns form in '" + path +
                                   "', expected namespace symbol: " + form->to_string());
        }

        return Value::string(children[1]->as<AST::Symbol>().value);
      }

      throw NamespaceException("No ns declaration found in '" + path + "'.");
    }
  } // namespace Function

  SyntaxNamespace::SyntaxNamespace()
    : Roo::Namespace(std::string(NS__PROOF__SYNTAX))
  {
    set_origin(Namespace::Origin::native());
    store("deftest", Function::DefTestForm::make());

    /*!
     * @roo.symbol proof.syntax/is
     * @roo.kind special-form
     * @brief Assert that an expression is truthy and abort on failure.
     *
     * Equality expressions receive an expected-versus-actual failure message.
     * A failed assertion records the failure and stops the remaining test body.
     *
     * @code
     * (is (= 4 (+ 2 2)))
     * @endcode
     *
     * | Arg  | Description                              |
     * | ---- | ---------------------------------------- |
     * | expr | Expression evaluated for truthiness.     |
     *
     * @return `true` when the assertion passes.
     * @see proof.core/expect
     * @since 0.1.0
     */
    store("is", Function::IsForm::make());

    /*!
     * @roo.symbol proof.syntax/should
     * @roo.kind special-form
     * @brief Assert that an expression is truthy and abort on failure.
     *
     * `should` is the descriptive assertion spelling. A failure is recorded
     * before the remainder of the current test body is aborted.
     *
     * @code
     * (should (= (:status response) :ok))
     * @endcode
     *
     * | Arg  | Description                              |
     * | ---- | ---------------------------------------- |
     * | expr | Expression evaluated for truthiness.     |
     *
     * @return `true` when the assertion passes.
     * @see proof.core/is
     * @since 0.1.0
     */
    store("should", Function::ShouldForm::make());

    /*!
     * @roo.symbol proof.syntax/assert
     * @roo.kind special-form
     * @brief Assert that an expression is truthy and abort on failure.
     *
     * A failed assertion records a diagnostic and prevents later forms in the
     * current test body from running.
     *
     * @code
     * (assert (not-empty? results))
     * @endcode
     *
     * | Arg  | Description                              |
     * | ---- | ---------------------------------------- |
     * | expr | Expression evaluated for truthiness.     |
     *
     * @return `true` when the assertion passes.
     * @see proof.core/expect
     * @since 0.1.0
     */
    store("assert", Function::AssertForm::make());

    /*!
     * @roo.symbol proof.syntax/expect
     * @roo.kind special-form
     * @brief Assert that an expression is truthy and continue on failure.
     *
     * A failed expectation records a diagnostic but allows subsequent forms in
     * the test body to run, enabling several failures to be reported together.
     *
     * @code
     * (expect (= (:name user) "Ada"))
     * @endcode
     *
     * | Arg  | Description                              |
     * | ---- | ---------------------------------------- |
     * | expr | Expression evaluated for truthiness.     |
     *
     * @return `true` on success, otherwise the recorded failure value.
     * @see proof.core/is
     * @since 0.1.0
     */
    store("expect", Function::ExpectForm::make());
    store("declared-namespace", Function::DeclaredNamespaceFunction::make());
  }
} // namespace Roo::Proof
