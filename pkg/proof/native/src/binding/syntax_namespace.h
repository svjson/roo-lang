#ifndef PROOF__BINDING__SYNTAX_NAMESPACE_H
#define PROOF__BINDING__SYNTAX_NAMESPACE_H

#include <string>
#include <string_view>

#include <roo/exec.h>
#include <roo/namespace.h>

namespace Roo::Proof
{
  inline constexpr std::string_view NS__PROOF__SYNTAX = "proof.syntax";

  namespace Function
  {
    class AssertionForm : public Roo::SpecialForm
    {
     public:
      AssertionForm(std::string form_name, bool abort_on_failure);
      EXECNODE_DECL(execnode_assertion)
      SFORM_LOWER_DECL

     private:
      std::string form_name;
      bool abort_on_failure;
    };

    class IsForm : public AssertionForm
    {
     public:
      IsForm();
      FUNC_MAKE(IsForm)
    };

    class ShouldForm : public AssertionForm
    {
     public:
      ShouldForm();
      FUNC_MAKE(ShouldForm)
    };

    class AssertForm : public AssertionForm
    {
     public:
      AssertForm();
      FUNC_MAKE(AssertForm)
    };

    class ExpectForm : public AssertionForm
    {
     public:
      ExpectForm();
      FUNC_MAKE(ExpectForm)
    };

    /*!
     * @brief Define and register a test.
     *
     * The body is retained for execution by the proof runner. An optional
     * fixture vector binds declared fixtures around the body. Scenario phases
     * from `proof.scenario` may appear as top-level body forms.
     *
     * @code
     * (deftest addition
     *   (is (= 4 (+ 2 2))))
     * @endcode
     *
     * | Arg     | Description                                    |
     * | ------- | ---------------------------------------------- |
     * | name    | Symbol identifying the test.                   |
     * | body... | Forms evaluated when the selected test is run. |
     *
     * | Arg      | Description                                    |
     * | -------- | ---------------------------------------------- |
     * | name     | Symbol identifying the test.                   |
     * | fixtures | Vector of declared fixture specs.              |
     * | body...  | Forms evaluated when the selected test is run. |
     *
     * @return The test name.
     * @see proof.fixture/using-fixtures
     * @see proof.scenario/given
     * @since 0.1.0
     */
    SPECIAL_FORM_DECL(DefTestForm, deftest)

    /*!
     * @brief Execute a registered test body.
     *
     * Assertion aborts are converted to `nil`; other exceptions become error
     * result maps for the proof runner.
     *
     * @code
     * (run-test-body test-body)
     * @endcode
     *
     * | Arg       | Description                         |
     * | --------- | ----------------------------------- |
     * | test-body | Executable registered test body.    |
     *
     * @return The body result, `nil`, or an error result map.
     * @see proof.core/deftest
     * @since 0.1.0
     */
    FUNC(RunTestBodyFunction, run_test_body)

    /*!
     * @brief Read the namespace declared by a Roo source file.
     *
     * Reads top-level forms until it finds an `ns` declaration. Parse errors
     * and missing or malformed declarations are reported with the source path.
     *
     * @code
     * (declared-namespace "test/example-test.roo")
     * => "example-test"
     * @endcode
     *
     * | Arg  | Description                         |
     * | ---- | ----------------------------------- |
     * | path | Path to the Roo source file.        |
     *
     * @return The declared namespace as a string.
     * @since 0.1.0
     */
    FUNC(DeclaredNamespaceFunction, declared_namespace)
  } // namespace Function

  class SyntaxNamespace : public Roo::Namespace
  {
   public:
    SyntaxNamespace();
  };
} // namespace Roo::Proof

#endif /* PROOF__BINDING__SYNTAX_NAMESPACE_H */
