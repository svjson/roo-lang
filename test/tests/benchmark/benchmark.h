
#ifndef LISPLE_TEST_NOW_H
#define LISPLE_TEST_NOW_H

#include <lisple/namespace.h>
#include <lisple/reader.h>

namespace LispleTest
{
  inline bool logging_enabled = false;
  inline bool skip_benchmark_tests = false;

  long now();

  struct Counters
  {
    int eval_executions = 0;
    int exec_executions = 0;

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
  };

  extern Counters counter_snapshot;

  void print_counters();
  void snapshot_counters();
  void restore_counter_snapshot();

  class SnippetBenchmark
  {
    Lisple::Reader reader;

   public:
    SnippetBenchmark(const std::string& input);
    SnippetBenchmark(const std::string& case_name, const std::string& input);
    SnippetBenchmark(std::map<const std::string, Lisple::Namespace> native_namespaces,
                     const std::vector<std::string>& pre_evaluated,
                     const std::string& ns,
                     const std::string& input);
    SnippetBenchmark(const std::string& case_name,
                     std::map<const std::string, Lisple::Namespace> native_namespaces,
                     const std::vector<std::string>& pre_evaluated,
                     const std::string& ns,
                     const std::string& input);
    SnippetBenchmark(const std::string& case_name,
                     const std::vector<std::string>& pre_evaluated,
                     const std::string& ns,
                     const std::string& input);

    Lisple::sptr_rtval run();
    void print_result();
    void log_result();

    std::string case_name;
    std::map<const std::string, Lisple::Namespace> native_namespaces;
    std::vector<std::string> pre_evaluated;
    std::string ns;
    std::string input;

    long start_time;
    long end_time;
    long total_time;

    long parse_start_time;
    long parse_end_time;
    long parse_time;

    long lower_start_time;
    long lower_end_time;
    long lower_time;

    long exec_start_time;
    long exec_end_time;
    long exec_time;
  };
} // namespace LispleTest

#endif /* NOW_H */
