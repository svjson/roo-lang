#include <utility>

#include <roo/exception.h>

#include <binding/syntax/scenario_namespace.h>

namespace Roo::Proof
{
  namespace Function
  {
    PhaseForm::PhaseForm(std::string form_name)
      : SpecialForm(SIG((FN_ARGS((VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&PhaseForm::execnode_phase))))
      , form_name(std::move(form_name))
    {
    }

    GivenForm::GivenForm()
      : PhaseForm("given")
    {
    }

    WhenForm::WhenForm()
      : PhaseForm("when")
    {
    }

    ThenForm::ThenForm()
      : PhaseForm("then")
    {
    }

    SFORM_LOWER_IMPL(PhaseForm)
    {
      throw RooException(form_name + " may only be used as a top-level deftest phase: " +
                         ast_node->to_string());
    }

    EXECNODE_BODY(PhaseForm, execnode_phase)
    {
      throw InvocationException("Invalid " + form_name + " execution node.");
    }
  } // namespace Function

  ScenarioNamespace::ScenarioNamespace()
    : Roo::Namespace(std::string(NS__PROOF__SYNTAX__SCENARIO))
  {
    set_origin(Namespace::Origin::native());

    /*!
     * @roo.symbol proof.syntax.scenario/given
     * @roo.kind special-form
     * @brief Seed the current state of a test scenario.
     *
     * `given` may only appear as a top-level `deftest` phase. Its final body
     * value becomes both the original scenario value and the current state.
     *
     * @code
     * (given
     *   {:revision 1})
     * @endcode
     *
     * | Arg     | Description                                                |
     * | ------- | ---------------------------------------------------------- |
     * | body... | Forms whose final value initializes the scenario state.    |
     *
     * @return The initial scenario state.
     * @see proof.core/deftest
     * @see proof.scenario/when
     * @see proof.scenario/then
     * @since 0.1.0
     */
    store("given", Function::GivenForm::make());

    /*!
     * @roo.symbol proof.syntax.scenario/when
     * @roo.kind special-form
     * @brief Transform the current state of a test scenario.
     *
     * `when` may only appear as a top-level `deftest` phase. Its parameter
     * vector receives the current state using normal Roo function binding. The
     * final body value becomes the current state for later phases.
     *
     * @code
     * (when [{:keys [revision]}]
     *   {:revision (+ revision 1)})
     * @endcode
     *
     * | Arg     | Description                                               |
     * | ------- | --------------------------------------------------------- |
     * | params  | Argument vector binding the current scenario state.       |
     * | body... | Forms whose final value becomes the next scenario state.  |
     *
     * @return The updated scenario state.
     * @see proof.scenario/given
     * @see proof.scenario/then
     * @since 0.1.0
     */
    store("when", Function::WhenForm::make());

    /*!
     * @roo.symbol proof.syntax.scenario/then
     * @roo.kind special-form
     * @brief Observe the current state of a test scenario.
     *
     * `then` may only appear as a top-level `deftest` phase. Its parameter
     * vector receives the current state and may also bind the original `given`
     * value. It does not replace the state used by subsequent phases.
     *
     * @code
     * (then [{:keys [revision]} original]
     *   (is (= revision 2))
     *   (is (= original {:revision 1})))
     * @endcode
     *
     * | Arg     | Description                                                |
     * | ------- | ---------------------------------------------------------- |
     * | params  | Argument vector binding current and optional original state. |
     * | body... | Observation and assertion forms.                           |
     *
     * @return The value of the final body form without changing scenario state.
     * @see proof.scenario/given
     * @see proof.scenario/when
     * @since 0.1.0
     */
    store("then", Function::ThenForm::make());
  }
} // namespace Roo::Proof
