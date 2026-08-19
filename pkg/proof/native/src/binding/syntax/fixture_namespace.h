#ifndef PROOF__BINDING__SYNTAX__FIXTURE_NAMESPACE_H
#define PROOF__BINDING__SYNTAX__FIXTURE_NAMESPACE_H

#include <string>
#include <string_view>

#include <roo/exec.h>
#include <roo/namespace.h>

namespace Roo::Proof
{
  inline constexpr std::string_view NS__PROOF__SYNTAX__FIXTURE = "proof.syntax.fixture";

  namespace Function
  {
    /*!
     * @brief Declare a named fixture provider.
     *
     * The body becomes a zero-argument generator registered with its options.
     * The public `proof.fixture/deffixture` binding re-exports this form.
     *
     * @code
     * (deffixture account
     *   {:storage :cache}
     *   (create-account))
     * @endcode
     *
     * | Arg     | Description                                  |
     * | ------- | -------------------------------------------- |
     * | name    | Symbol identifying the fixture.              |
     * | options | Fixture storage and projection options map.  |
     * | body... | Forms that generate the fixture value.       |
     *
     * @return The fixture name.
     * @see proof.fixture/using-fixtures
     * @since 0.1.0
     */
    SPECIAL_FORM_DECL(DefFixtureForm, deffixture)

    class FixtureBindingForm : public Roo::SpecialForm
    {
     public:
      FixtureBindingForm(std::string form_name, std::string value_function);
      EXECNODE_DECL(execnode_fixture)
      SFORM_LOWER_DECL

     private:
      std::string form_name;
      std::string value_function;
    };

    class UsingCacheFixtureForm : public FixtureBindingForm
    {
     public:
      UsingCacheFixtureForm();
      FUNC_MAKE(UsingCacheFixtureForm)
    };

    class UsingPersistentFixtureForm : public FixtureBindingForm
    {
     public:
      UsingPersistentFixtureForm();
      FUNC_MAKE(UsingPersistentFixtureForm)
    };

    /*!
     * @brief Bind declared fixture values while evaluating a body.
     *
     * Each fixture spec names a declared fixture and may provide a local
     * binding and per-use options.
     *
     * @code
     * (using-fixtures [[account account-fixture]
     *                  settings-fixture]
     *   (is account))
     * @endcode
     *
     * | Arg      | Description                                        |
     * | -------- | -------------------------------------------------- |
     * | fixtures | Vector of fixture symbols or fixture spec vectors. |
     * | body...  | Forms evaluated with the fixture bindings.         |
     *
     * @return The value of the final body form.
     * @see proof.fixture/deffixture
     * @since 0.1.0
     */
    SPECIAL_FORM_DECL(UsingFixturesForm, using_fixtures)
  } // namespace Function

  class FixtureNamespace : public Roo::Namespace
  {
   public:
    FixtureNamespace();
  };
} // namespace Roo::Proof

#endif /* PROOF__BINDING__SYNTAX__FIXTURE_NAMESPACE_H */
