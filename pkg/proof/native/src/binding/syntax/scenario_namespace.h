#ifndef PROOF__BINDING__SYNTAX__SCENARIO_NAMESPACE_H
#define PROOF__BINDING__SYNTAX__SCENARIO_NAMESPACE_H

#include <string>
#include <string_view>

#include <roo/exec.h>
#include <roo/namespace.h>

namespace Roo::Proof
{
  inline constexpr std::string_view NS__PROOF__SYNTAX__SCENARIO = "proof.syntax.scenario";

  namespace Function
  {
    class PhaseForm : public Roo::SpecialForm
    {
     public:
      explicit PhaseForm(std::string form_name);
      EXECNODE_DECL(execnode_phase)
      SFORM_LOWER_DECL

     private:
      std::string form_name;
    };

    class GivenForm : public PhaseForm
    {
     public:
      GivenForm();
      FUNC_MAKE(GivenForm)
    };

    class WhenForm : public PhaseForm
    {
     public:
      WhenForm();
      FUNC_MAKE(WhenForm)
    };

    class ThenForm : public PhaseForm
    {
     public:
      ThenForm();
      FUNC_MAKE(ThenForm)
    };
  } // namespace Function

  class ScenarioNamespace : public Roo::Namespace
  {
   public:
    ScenarioNamespace();
  };
} // namespace Roo::Proof

#endif /* PROOF__BINDING__SYNTAX__SCENARIO_NAMESPACE_H */
