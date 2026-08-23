#ifndef ROO__BENCHMARK__COUNTERS_H
#define ROO__BENCHMARK__COUNTERS_H

#include <roo/export.h>

namespace Roo::Benchmark
{
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

#ifdef ROO_ENABLE_BENCHMARK_COUNTERS
  extern ROO_API Counters counters;
#endif
} // namespace Roo::Benchmark

#ifdef ROO_ENABLE_BENCHMARK_COUNTERS
#define ROO_BENCHMARK_INC(counter) (++::Roo::Benchmark::counters.counter)
#else
#define ROO_BENCHMARK_INC(counter) ((void)0)
#endif

#endif /* ROO__BENCHMARK__COUNTERS_H */
