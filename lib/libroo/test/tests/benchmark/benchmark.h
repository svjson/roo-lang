
#ifndef ROO_TEST_NOW_H
#define ROO_TEST_NOW_H

#include <cstddef>

#include <roo/namespace.h>
#include <roo/reader.h>

namespace RooTest
{
  inline bool logging_enabled = false;
  inline bool skip_benchmark_tests = false;

  long now();

  struct Counters
  {
    int eval_executions = 0;
    int exec_executions = 0;

    int lowered_expressions = 0;
    int lowered_literals = 0;
    int lower_time_exec_resolutions = 0;
    int lower_time_exec_unresolved = 0;

    int rtvalues_constructed = 0;
    int rtvalue_wrappers_constructed = 0;
    int to_ast_conversions = 0;
    int to_rtvalue_conversions = 0;

    int exec_nodes_constructed = 0;
    int call_nodes_constructed = 0;
    int literal_nodes_constructed = 0;
    int lookup_nodes_constructed = 0;

    int user_functions_created = 0;
    int user_functions_ast_created = 0;
    int user_functions_rtval_created = 0;
    int user_function_ast_invocations = 0;
    int user_function_rtval_invocations = 0;
    int user_function_wrong_path_invocations = 0;

    int deprecated_special_form_invocations = 0;
  };

  struct SetupCounters
  {
    int lowered_expressions = 0;
    int lowered_literals = 0;
    int lower_time_exec_resolutions = 0;
    int lower_time_exec_unresolved = 0;

    int rtvalues_constructed = 0;

    int exec_nodes_constructed = 0;
    int call_nodes_constructed = 0;
    int literal_nodes_constructed = 0;
    int lookup_nodes_constructed = 0;

    int user_functions_rtval_created = 0;

    int deprecated_special_form_invocations = 0;
  };

  extern Counters counter_snapshot;
  extern SetupCounters setup_counters;
  extern SetupCounters input_lower_counters;

  void print_counters(bool include_setup_phase = false);
  void snapshot_counters();
  void restore_counter_snapshot();

  class SnippetBenchmark
  {
    Roo::Reader reader;

   public:
    SnippetBenchmark(const std::string& input);
    SnippetBenchmark(const std::string& case_name, const std::string& input);
    SnippetBenchmark(std::map<const std::string, Roo::Namespace> native_namespaces,
                     const std::vector<std::string>& pre_evaluated,
                     const std::string& ns,
                     const std::string& input);
    SnippetBenchmark(const std::string& case_name,
                     std::map<const std::string, Roo::Namespace> native_namespaces,
                     const std::vector<std::string>& pre_evaluated,
                     const std::string& ns,
                     const std::string& input);
    SnippetBenchmark(const std::vector<std::string>& pre_evaluated,
                     const std::string& ns,
                     const std::string& input);
    SnippetBenchmark(const std::string& case_name,
                     const std::vector<std::string>& pre_evaluated,
                     const std::string& ns,
                     const std::string& input);

    Roo::sptr_val run();
    SnippetBenchmark& with_lower_iterations(size_t iterations);
    SnippetBenchmark& without_execution();
    void print_result();
    void log_result();

    std::string case_name;
    std::map<const std::string, Roo::Namespace> native_namespaces;
    std::vector<std::string> pre_evaluated;
    std::string ns;
    std::string input;

    size_t lower_iterations = 1;
    bool execute = true;

    long start_time = 0;
    long end_time = 0;
    long total_time = 0;
    double single_lowering_total_time = 0;

    long parse_start_time = 0;
    long parse_end_time = 0;
    long parse_time = 0;

    long lower_start_time = 0;
    long lower_end_time = 0;
    long lower_time = 0;
    long lower_cleanup_start_time = 0;
    long lower_cleanup_end_time = 0;
    long lower_cleanup_time = 0;

    long exec_start_time = 0;
    long exec_end_time = 0;
    long exec_time = 0;
  };
} // namespace RooTest

#endif /* NOW_H */
