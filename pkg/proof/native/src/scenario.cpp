#include <algorithm>
#include <scenario.h>
#include <string>
#include <vector>

#include <roo/exception.h>
#include <roo/form.h>

namespace Roo::Proof
{
  namespace
  {
    struct ScenarioPhase
    {
      std::string name;
      sptr_ast_node form;
      sptr_ast_node_v after;
    };

    bool is_rest_parameter(const sptr_ast_node& node)
    {
      if (node->get_type() != Form::SYMBOL)
      {
        return false;
      }

      const std::string& identifier = node->as<AST::Symbol>().get_identifier();
      return identifier.size() > 1 && identifier[0] == '&';
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
        if (child->get_type() == Form::SYMBOL &&
            child->as<AST::Symbol>().get_identifier() == "&")
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
        throw RooException("Invalid " + phase_name + " argument vector, expected at most " +
                           std::to_string(available_count) +
                           " required arguments: " + arg_vec.to_string());
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

      const std::string& identifier = children[0]->as<AST::Symbol>().get_identifier();
      if (identifier == "given" || identifier == "when" || identifier == "then")
      {
        return identifier;
      }
      return "";
    }

    sptr_ast_node fn_node(const sptr_ast_node& arg_vec, const sptr_ast_node_v& body)
    {
      sptr_ast_node_v children{AST::Symbol::make("fn"), arg_vec};
      children.insert(children.end(), body.begin(), body.end());
      return AST::List::make(children);
    }

    sptr_ast_node apply_node(const sptr_ast_node& fn, const sptr_ast_node_v& args)
    {
      return AST::List::make({AST::Symbol::make("apply"), fn, AST::Vector::make(args)});
    }

    sptr_ast_node phase_arg_vector(const ScenarioPhase& phase)
    {
      auto& elements = phase.form->get_children();
      if (phase.name == "given")
      {
        return AST::Vector::make({});
      }

      if (elements.size() < 2 || elements[1]->get_type() != Form::VECTOR)
      {
        throw RooException("Invalid " + phase.name +
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
        args.push_back(AST::Symbol::make(available_symbols[i]));
      }
      return args;
    }

    sptr_ast_node then_expr(const ScenarioPhase& phase,
                            const std::vector<std::string>& available_symbols)
    {
      return apply_node(fn_node(phase_arg_vector(phase), phase_body(phase)),
                        phase_args(phase, available_symbols));
    }

    sptr_ast_node phase_sequence_expr(const std::vector<ScenarioPhase>& phases,
                                      size_t index,
                                      bool has_given,
                                      bool current_is_given)
    {
      const ScenarioPhase& phase = phases[index];
      const bool has_next = index + 1 < phases.size();

      if (phase.name == "when")
      {
        sptr_ast_node_v bindings{
          AST::Symbol::make("__proof-when-fn"),
          fn_node(phase_arg_vector(phase), phase_body(phase)),
          AST::Symbol::make("__proof-current-state"),
          apply_node(AST::Symbol::make("__proof-when-fn"),
                     phase_args(phase, {"__proof-current-state"})),
        };

        sptr_ast_node_v body = phase.after;
        if (has_next)
        {
          body.push_back(phase_sequence_expr(phases, index + 1, has_given, false));
        }
        if (body.empty())
        {
          body.push_back(AST::Symbol::make("__proof-current-state"));
        }

        sptr_ast_node_v children{AST::Symbol::make("let"), AST::Vector::make(bindings)};
        children.insert(children.end(), body.begin(), body.end());
        return AST::List::make(children);
      }

      std::vector<std::string> available_symbols{"__proof-current-state"};
      if (has_given && !current_is_given)
      {
        available_symbols.push_back("__proof-given-result");
      }

      sptr_ast_node_v body{then_expr(phase, available_symbols)};
      body.insert(body.end(), phase.after.begin(), phase.after.end());
      if (has_next)
      {
        body.push_back(phase_sequence_expr(phases, index + 1, has_given, current_is_given));
      }

      sptr_ast_node_v children{AST::Symbol::make("let"), AST::Vector::make({})};
      children.insert(children.end(), body.begin(), body.end());
      return AST::List::make(children);
    }

    sptr_ast_node scenario_expr(const std::vector<ScenarioPhase>& phases)
    {
      const ScenarioPhase* given_phase = nullptr;
      const bool has_given = !phases.empty() && phases.front().name == "given";
      const size_t sequence_start = has_given ? 1 : 0;
      if (has_given)
      {
        given_phase = &phases.front();
      }

      sptr_ast_node_v bindings{
        AST::Symbol::make("__proof-given-result"),
        given_phase
          ? apply_node(fn_node(phase_arg_vector(*given_phase), phase_body(*given_phase)), {})
          : AST::NIL,
        AST::Symbol::make("__proof-current-state"),
        AST::Symbol::make("__proof-given-result"),
      };

      sptr_ast_node_v body;
      if (given_phase)
      {
        body.insert(body.end(), given_phase->after.begin(), given_phase->after.end());
      }
      if (sequence_start < phases.size())
      {
        body.push_back(phase_sequence_expr(phases, sequence_start, has_given, has_given));
      }
      if (body.empty())
      {
        body.push_back(AST::Symbol::make("__proof-current-state"));
      }

      sptr_ast_node_v children{AST::Symbol::make("let"), AST::Vector::make(bindings)};
      children.insert(children.end(), body.begin(), body.end());
      return AST::List::make(children);
    }

    std::vector<ScenarioPhase> parse_scenario_phases(const sptr_ast_node_v& body,
                                                     size_t start,
                                                     size_t end)
    {
      std::vector<ScenarioPhase> phases;
      bool seen_given = false;
      bool has_state = false;

      for (size_t i = start; i <= end; i++)
      {
        const std::string name = phase_name(body[i]);
        if (name.empty())
        {
          phases.back().after.push_back(body[i]);
          continue;
        }

        if (name == "given")
        {
          if (seen_given)
          {
            throw RooException("Invalid deftest scenario, duplicate given.");
          }
          if (!phases.empty())
          {
            throw RooException("Invalid deftest scenario phase order: " +
                               body[i]->to_string());
          }
          seen_given = true;
          has_state = true;
        }
        else if (name == "when")
        {
          has_state = true;
        }
        else if (!has_state)
        {
          throw RooException(
            "Invalid deftest scenario, then requires a preceding given or when.");
        }

        phases.push_back({name, body[i], {}});
      }

      return phases;
    }
  } // namespace

  sptr_ast_node_v rewrite_scenario_body(const sptr_ast_node_v& body)
  {
    size_t start = body.size();
    size_t end = 0;

    for (size_t i = 0; i < body.size(); i++)
    {
      if (!phase_name(body[i]).empty())
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
} // namespace Roo::Proof
