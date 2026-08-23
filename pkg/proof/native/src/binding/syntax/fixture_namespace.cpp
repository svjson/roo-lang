#include <utility>
#include <variant>
#include <vector>

#include <roo/bind.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/form.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/lower.h>

#include <binding/syntax/fixture_namespace.h>

namespace Roo::Proof
{
  namespace
  {
    sptr_ast_node fixture_spec_binding(const sptr_ast_node& spec)
    {
      if (spec->get_type() == Form::SYMBOL)
      {
        return spec;
      }

      if (spec->get_type() != Form::VECTOR)
      {
        throw RooException("Invalid fixture binding spec: " + spec->to_string());
      }

      auto& children = spec->get_children();
      if (children.size() < 2 || children.size() > 3)
      {
        throw RooException("Invalid fixture binding spec: " + spec->to_string());
      }

      return children[0];
    }

    sptr_ast_node fixture_spec_name(const sptr_ast_node& spec)
    {
      if (spec->get_type() == Form::SYMBOL)
      {
        return spec;
      }

      return spec->get_children()[1];
    }

    sptr_ast_node fixture_spec_options(const sptr_ast_node& spec)
    {
      if (spec->get_type() == Form::VECTOR && spec->get_children().size() == 3)
      {
        return spec->get_children()[2];
      }

      return AST::NIL;
    }
  } // namespace

  namespace Function
  {
    /** DefFixtureForm - proof.syntax.fixture/deffixture */
    SPECIAL_FORM_IMPL(DefFixtureForm,
                      SIG((FN_ARGS((&Type::SYMBOL, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                           EXEC_DISPATCH(&DefFixtureForm::execnode_deffixture))))

    SFORM_LOWER_IMPL(DefFixtureForm)
    {
      auto& elements = ast_node->get_children();
      if (elements.size() < 4 || elements[1]->get_type() != Form::SYMBOL ||
          elements[2]->get_type() != Form::MAP)
      {
        throw RooException("Invalid deffixture form: " + ast_node->to_string());
      }

      if (!ctx.ctx)
      {
        throw RooException("deffixture requires an active lowering context.");
      }

      const size_t body_start = 3;
      sptr_val name = std::get<LiteralNode>(lower_literal(ctx, elements[1])->data).value;
      sptr_val options = std::get<LiteralNode>(lower_literal(ctx, elements[2])->data).value;

      sptr_ast_node_v body;
      body.reserve(elements.size() - body_start);
      for (size_t i = body_start; i < elements.size(); i++)
      {
        body.push_back(elements[i]);
      }

      auto arg_vec = std::make_shared<AST::Vector>();
      std::shared_ptr<UserFunction> generate_fn =
        create_function("fixture:" + name->to_string(),
                        *ctx.ctx,
                        ctx.ctx->get_current_namespace(),
                        *arg_vec,
                        body);

      return std::make_unique<ExecNode>(
        SpecialFormNode(this, {name, Value::executable(generate_fn), options}, {}));
    }

    EXECNODE_BODY(DefFixtureForm, execnode_deffixture)
    {
      if (snode.values.size() != 3)
      {
        throw InvocationException("Invalid deffixture execution node.");
      }

      ctx.call("proof.fixture/register-fixture!", snode.values);
      return snode.values.front();
    }

    FixtureBindingForm::FixtureBindingForm(std::string form_name, std::string value_function)
      : SpecialForm(SIG((FN_ARGS((&Type::VECTOR, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&FixtureBindingForm::execnode_fixture))))
      , form_name(std::move(form_name))
      , value_function(std::move(value_function))
    {
    }

    UsingCacheFixtureForm::UsingCacheFixtureForm()
      : FixtureBindingForm("using-cache-fixture", "proof.fixture/cache-fixture-value")
    {
    }

    UsingPersistentFixtureForm::UsingPersistentFixtureForm()
      : FixtureBindingForm("using-persistent-fixture",
                           "proof.fixture/persistent-fixture-value")
    {
    }

    SFORM_LOWER_IMPL(FixtureBindingForm)
    {
      auto& elements = ast_node->get_children();
      if (elements.size() < 3 || elements[1]->get_type() != Form::VECTOR)
      {
        throw RooException("Invalid " + form_name + " form: " + ast_node->to_string());
      }

      auto& bind_forms = elements[1]->get_children();
      if (bind_forms.size() < 3 || bind_forms.size() > 4)
      {
        throw RooException("Invalid " + form_name +
                           " binding vector, expected [binding key generator] or "
                           "[binding key generator options]: " +
                           elements[1]->to_string());
      }

      uptr_exec_node_v exec_nodes;
      exec_nodes.reserve(elements.size() + 1);
      exec_nodes.push_back(lower_expr(ctx, bind_forms[1]));
      exec_nodes.push_back(lower_expr(ctx, bind_forms[2]));
      if (bind_forms.size() == 4)
      {
        exec_nodes.push_back(lower_expr(ctx, bind_forms[3]));
      }
      else
      {
        exec_nodes.push_back(lower_literal(ctx, AST::NIL));
      }

      auto bind_node = lower_literal(ctx, bind_forms[0]);
      std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
      bindings.push_back(
        std::make_pair(LexicalBinding::create(std::get<LiteralNode>(bind_node->data)),
                       lower_literal(ctx, AST::NIL)));

      ctx.push({});
      ctx.add_lexical_binding(*bindings.back().first);
      for (size_t i = 2; i < elements.size(); i++)
      {
        exec_nodes.push_back(lower_expr(ctx, elements[i]));
      }
      ctx.pop();

      return std::make_unique<ExecNode>(
        SpecialFormNode(this, std::move(bindings), std::move(exec_nodes)));
    }

    EXECNODE_BODY(FixtureBindingForm, execnode_fixture)
    {
      if (snode.bind_forms.size() != 1)
      {
        throw InvocationException("Invalid " + form_name + " execution node.");
      }

      const size_t body_start = 3;
      if (snode.exec_nodes.size() < body_start)
      {
        throw InvocationException("Invalid " + form_name + " execution body.");
      }

      sptr_val_v fixture_args;
      fixture_args.reserve(body_start);
      for (size_t i = 0; i < body_start; i++)
      {
        fixture_args.push_back(exec(ctx, *snode.exec_nodes[i]));
      }

      Scope bind_scope;
      snode.bind_forms.front().first->apply(bind_scope,
                                            ctx.call(value_function, fixture_args));
      ctx.push_context(true, bind_scope);

      sptr_val result = Constant::NIL;
      for (size_t i = body_start; i < snode.exec_nodes.size(); i++)
      {
        result = exec(ctx, *snode.exec_nodes[i]);
      }

      ctx.pop_context();
      return result;
    }

    /** UsingFixturesForm - proof.syntax.fixture/using-fixtures */
    SPECIAL_FORM_IMPL(UsingFixturesForm,
                      SIG((FN_ARGS((&Type::VECTOR, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                           EXEC_DISPATCH(&UsingFixturesForm::execnode_using_fixtures))))

    SFORM_LOWER_IMPL(UsingFixturesForm)
    {
      auto& elements = ast_node->get_children();
      if (elements.size() < 3 || elements[1]->get_type() != Form::VECTOR)
      {
        throw RooException("Invalid using-fixtures form: " + ast_node->to_string());
      }

      auto& fixture_specs = elements[1]->get_children();
      uptr_exec_node_v exec_nodes;
      exec_nodes.reserve((fixture_specs.size() * 2) + elements.size() - 2);

      std::vector<std::pair<std::unique_ptr<LexicalBinding>, uptr_exec_node>> bindings;
      bindings.reserve(fixture_specs.size());

      ctx.push({});
      for (auto& spec : fixture_specs)
      {
        auto bind_node = lower_literal(ctx, fixture_spec_binding(spec));
        bindings.push_back(
          std::make_pair(LexicalBinding::create(std::get<LiteralNode>(bind_node->data)),
                         lower_literal(ctx, AST::NIL)));
        exec_nodes.push_back(lower_literal(ctx, fixture_spec_name(spec)));
        exec_nodes.push_back(lower_expr(ctx, fixture_spec_options(spec)));
        ctx.add_lexical_binding(*bindings.back().first);
      }

      for (size_t i = 2; i < elements.size(); i++)
      {
        exec_nodes.push_back(lower_expr(ctx, elements[i]));
      }
      ctx.pop();

      return std::make_unique<ExecNode>(
        SpecialFormNode(this, std::move(bindings), std::move(exec_nodes)));
    }

    EXECNODE_BODY(UsingFixturesForm, execnode_using_fixtures)
    {
      const size_t body_start = snode.bind_forms.size() * 2;
      if (snode.exec_nodes.size() < body_start)
      {
        throw InvocationException("Invalid using-fixtures execution node.");
      }

      Scope bind_scope;
      ctx.push_context(true, bind_scope);

      for (size_t i = 0; i < snode.bind_forms.size(); i++)
      {
        sptr_val_v fixture_args{
          exec(ctx, *snode.exec_nodes[i * 2]),
          exec(ctx, *snode.exec_nodes[(i * 2) + 1]),
        };
        snode.bind_forms[i].first->apply(
          ctx.current_scope(),
          ctx.call("proof.fixture/fixture-value", fixture_args));
      }

      sptr_val result = Constant::NIL;
      for (size_t i = body_start; i < snode.exec_nodes.size(); i++)
      {
        result = exec(ctx, *snode.exec_nodes[i]);
      }

      ctx.pop_context();
      return result;
    }
  } // namespace Function

  FixtureNamespace::FixtureNamespace()
    : Roo::Namespace(std::string(NS__PROOF__SYNTAX__FIXTURE))
  {
    set_origin(Namespace::Origin::native());
    store("deffixture", Function::DefFixtureForm::make());
    store("using-fixtures", Function::UsingFixturesForm::make());

    /*!
     * @roo.symbol proof.syntax.fixture/using-cache-fixture
     * @roo.kind special-form
     * @brief Bind an in-process cached fixture value while evaluating a body.
     *
     * The generator runs only when the key is absent from the fixture cache.
     * Options may select stable paths from the generated value.
     *
     * @code
     * (using-cache-fixture [account "account/default" create-account]
     *   (is account))
     * @endcode
     *
     * | Arg     | Description                                                |
     * | ------- | ---------------------------------------------------------- |
     * | binding | Vector of binding, cache key, generator, and optional opts. |
     * | body... | Forms evaluated with the generated value bound.            |
     *
     * @return The value of the final body form.
     * @see proof.fixture/using-persistent-fixture
     * @since 0.1.0
     */
    store("using-cache-fixture", Function::UsingCacheFixtureForm::make());

    /*!
     * @roo.symbol proof.syntax.fixture/using-persistent-fixture
     * @roo.kind special-form
     * @brief Bind a filesystem-backed fixture value while evaluating a body.
     *
     * The generator creates or refreshes a persisted fixture document. Options
     * may control versioning, refresh behavior, and projected paths.
     *
     * @code
     * (using-persistent-fixture
     *   [world "world/default.edn" create-world {:version 1}]
     *   (is world))
     * @endcode
     *
     * | Arg     | Description                                                   |
     * | ------- | ------------------------------------------------------------- |
     * | binding | Vector of binding, relative path, generator, and optional opts. |
     * | body... | Forms evaluated with the persisted value bound.               |
     *
     * @return The value of the final body form.
     * @see proof.fixture/using-cache-fixture
     * @since 0.1.0
     */
    store("using-persistent-fixture", Function::UsingPersistentFixtureForm::make());
  }
} // namespace Roo::Proof
