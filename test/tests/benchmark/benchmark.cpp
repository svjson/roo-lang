#include "benchmark.h"

#include "lisple/form.h"
#include "lisple/runtime/node.h"
#include "lisple/runtime/value.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <lisple/context.h>
#include <lisple/debug.h>
#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/runtime.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>

#include <gtest/gtest.h>

namespace LispleTest
{
  Counters counter_snapshot;

  SetupCounters setup_counters;

  void print_counters(bool include_setup_phase)
  {
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "EVAL path executions: " << Lisple::eval_executions << std::endl;
    std::cout << "EXEC path executions: " << Lisple::exec_executions << std::endl;

    if (include_setup_phase)
    {
      std::cout << "-----------------------------------------------" << std::endl;
      std::cout << "SETUP PHASE - Lowered expressions: "
                << setup_counters.lowered_expressions << std::endl;
      std::cout << "SETUP PHASE - Lowered literals: " << setup_counters.lowered_literals
                << std::endl;
      std::cout << "SETUP PHASE - Callees resolved at lower time: "
                << setup_counters.lower_time_exec_resolutions << std::endl;
      std::cout << "SETUP PHASE - Callees unresolved at lower time: "
                << setup_counters.lower_time_exec_unresolved << std::endl;
    }

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Lowered expressions: " << Lisple::lowered_expressions << std::endl;
    std::cout << "Lowered literals: " << Lisple::lowered_literals << std::endl;
    std::cout << "Callees resolved at lower time: " << Lisple::lower_time_exec_resolutions
              << std::endl;
    std::cout << "Callees unresolved at lower time: " << Lisple::lower_time_exec_unresolved
              << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    std::cout << "RTValue constructions: " << Lisple::rtvalues_constructed << std::endl;
    std::cout << "RTValueWrapper constructions: " << Lisple::rtvalue_wrappers_constructed
              << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "ExecNode constructions: " << Lisple::exec_nodes_constructed << std::endl;
    std::cout << "CallNode constructions: " << Lisple::call_nodes_constructed << std::endl;
    std::cout << "LiteralNode constructions: " << Lisple::literal_nodes_constructed
              << std::endl;
    std::cout << "LookupNode constructions: " << Lisple::lookup_nodes_constructed
              << std::endl;

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "to_AST conversions: " << Lisple::to_ast_conversions << std::endl;
    std::cout << "to_RTValue conversions: " << Lisple::to_rtvalue_conversions << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "UserFunction constructions: " << Lisple::user_functions_created
              << std::endl;
    std::cout << "UserFunction(RTValue body) constructions: "
              << Lisple::user_functions_rtval_created << std::endl;
    std::cout << "UserFunction(RTValue body) invocations: "
              << Lisple::user_function_rtval_invocations << std::endl;
    std::cout << "UserFunction(AST Body) constructions: "
              << Lisple::user_functions_ast_created << std::endl;
    std::cout << "UserFunction(AST Body) invocations: "
              << Lisple::user_function_ast_invocations << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Invocations of legacy special form exec: "
              << Lisple::deprecated_special_form_invocations << std::endl;
    if (include_setup_phase)
    {
      std::cout << "SETUP PHASE - Invocations of legacy special form exec: "
                << Lisple::deprecated_special_form_invocations << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
  }

  void snapshot_counters()
  {
    counter_snapshot.eval_executions = Lisple::eval_executions;
    counter_snapshot.exec_executions = Lisple::exec_executions;
    counter_snapshot.lowered_literals = Lisple::lowered_literals;
    counter_snapshot.lowered_expressions = Lisple::lowered_expressions;
    counter_snapshot.lower_time_exec_resolutions = Lisple::lower_time_exec_resolutions;
    counter_snapshot.lower_time_exec_unresolved = Lisple::lower_time_exec_unresolved;
    counter_snapshot.rtvalues_constructed = Lisple::rtvalues_constructed;
    counter_snapshot.rtvalue_wrappers_constructed = Lisple::rtvalue_wrappers_constructed;
    counter_snapshot.to_ast_conversions = Lisple::to_ast_conversions;
    counter_snapshot.to_rtvalue_conversions = Lisple::to_rtvalue_conversions;
    counter_snapshot.exec_nodes_constructed = Lisple::exec_nodes_constructed;
    counter_snapshot.call_nodes_constructed = Lisple::call_nodes_constructed;
    counter_snapshot.literal_nodes_constructed = Lisple::literal_nodes_constructed;
    counter_snapshot.lookup_nodes_constructed = Lisple::lookup_nodes_constructed;
    counter_snapshot.user_functions_created = Lisple::user_functions_created;
    counter_snapshot.user_functions_ast_created = Lisple::user_functions_ast_created;
    counter_snapshot.user_functions_rtval_created = Lisple::user_functions_rtval_created;
    counter_snapshot.user_function_ast_invocations = Lisple::user_function_ast_invocations;
    counter_snapshot.user_function_rtval_invocations =
      Lisple::user_function_rtval_invocations;
    counter_snapshot.user_function_wrong_path_invocations =
      Lisple::user_function_wrong_path_invocations;
    counter_snapshot.deprecated_special_form_invocations =
      Lisple::deprecated_special_form_invocations;
  }

  void reset_counters()
  {
    Lisple::eval_executions = 0;
    Lisple::exec_executions = 0;
    Lisple::lowered_expressions = 0;
    Lisple::lowered_literals = 0;
    Lisple::lower_time_exec_resolutions = 0;
    Lisple::lower_time_exec_unresolved = 0;
    Lisple::rtvalues_constructed = 0;
    Lisple::rtvalue_wrappers_constructed = 0;
    Lisple::to_ast_conversions = 0;
    Lisple::to_rtvalue_conversions = 0;
    Lisple::exec_nodes_constructed = 0;
    Lisple::call_nodes_constructed = 0;
    Lisple::literal_nodes_constructed = 0;
    Lisple::lookup_nodes_constructed = 0;
    Lisple::user_functions_created = 0;
    Lisple::user_functions_ast_created = 0;
    Lisple::user_functions_rtval_created = 0;
    Lisple::user_function_ast_invocations = 0;
    Lisple::user_function_rtval_invocations = 0;
    Lisple::user_function_wrong_path_invocations = 0;
    Lisple::deprecated_special_form_invocations = 0;
  }

  void restore_counter_snapshot()
  {
    Lisple::eval_executions = counter_snapshot.eval_executions;
    Lisple::exec_executions = counter_snapshot.exec_executions;
    Lisple::lowered_expressions = counter_snapshot.lowered_expressions;
    Lisple::lowered_literals = counter_snapshot.lowered_literals;
    Lisple::lower_time_exec_resolutions = counter_snapshot.lower_time_exec_resolutions;
    Lisple::lower_time_exec_unresolved = counter_snapshot.lower_time_exec_unresolved;
    Lisple::rtvalues_constructed = counter_snapshot.rtvalues_constructed;
    Lisple::rtvalue_wrappers_constructed = counter_snapshot.rtvalue_wrappers_constructed;
    Lisple::to_ast_conversions = counter_snapshot.to_ast_conversions;
    Lisple::to_rtvalue_conversions = counter_snapshot.to_rtvalue_conversions;
    Lisple::exec_nodes_constructed = counter_snapshot.exec_nodes_constructed;
    Lisple::call_nodes_constructed = counter_snapshot.call_nodes_constructed;
    Lisple::literal_nodes_constructed = counter_snapshot.literal_nodes_constructed;
    Lisple::lookup_nodes_constructed = counter_snapshot.lookup_nodes_constructed;
    Lisple::user_functions_created = counter_snapshot.user_functions_created;
    Lisple::user_functions_ast_created = counter_snapshot.user_functions_ast_created;
    Lisple::user_functions_rtval_created = counter_snapshot.user_functions_rtval_created;
    Lisple::user_function_ast_invocations = counter_snapshot.user_function_ast_invocations;
    Lisple::user_function_rtval_invocations =
      counter_snapshot.user_function_rtval_invocations;
    Lisple::user_function_wrong_path_invocations =
      counter_snapshot.user_function_wrong_path_invocations;
    Lisple::deprecated_special_form_invocations =
      counter_snapshot.deprecated_special_form_invocations;
  }

  void apply_counter_snapshot()
  {
    Lisple::eval_executions += counter_snapshot.eval_executions;
    Lisple::exec_executions += counter_snapshot.exec_executions;
    Lisple::lowered_expressions += counter_snapshot.lowered_expressions;
    Lisple::lowered_literals += counter_snapshot.lowered_literals;
    Lisple::lower_time_exec_resolutions += counter_snapshot.lower_time_exec_resolutions;
    Lisple::lower_time_exec_unresolved += counter_snapshot.lower_time_exec_unresolved;
    Lisple::rtvalues_constructed += counter_snapshot.rtvalues_constructed;
    Lisple::rtvalue_wrappers_constructed += counter_snapshot.rtvalue_wrappers_constructed;
    Lisple::to_ast_conversions += counter_snapshot.to_ast_conversions;
    Lisple::to_rtvalue_conversions += counter_snapshot.to_rtvalue_conversions;
    Lisple::exec_nodes_constructed += counter_snapshot.exec_nodes_constructed;
    Lisple::call_nodes_constructed += counter_snapshot.call_nodes_constructed;
    Lisple::literal_nodes_constructed += counter_snapshot.literal_nodes_constructed;
    Lisple::lookup_nodes_constructed += counter_snapshot.lookup_nodes_constructed;
    Lisple::user_functions_created += counter_snapshot.user_functions_created;
    Lisple::user_functions_ast_created += counter_snapshot.user_functions_ast_created;
    Lisple::user_functions_rtval_created += counter_snapshot.user_functions_rtval_created;
    Lisple::user_function_ast_invocations += counter_snapshot.user_function_ast_invocations;
    Lisple::user_function_rtval_invocations +=
      counter_snapshot.user_function_rtval_invocations;
    Lisple::user_function_wrong_path_invocations +=
      counter_snapshot.user_function_wrong_path_invocations;
    Lisple::deprecated_special_form_invocations +=
      counter_snapshot.deprecated_special_form_invocations;
  }

  long now()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
  }

  SnippetBenchmark::SnippetBenchmark(const std::string& input)
    : case_name(::testing::UnitTest::GetInstance()->current_test_info()->name())
    , ns("user")
    , input(input)
  {
  }

  SnippetBenchmark::SnippetBenchmark(const std::string& case_name, const std::string& input)
    : case_name(case_name)
    , ns("user")
    , input(input)
  {
  }

  SnippetBenchmark::SnippetBenchmark(
    const std::string& case_name,
    std::map<const std::string, Lisple::Namespace> native_namespaces,
    const std::vector<std::string>& pre_evaluated,
    const std::string& ns,
    const std::string& input)
    : case_name(case_name)
    , native_namespaces(native_namespaces)
    , pre_evaluated(pre_evaluated)
    , ns(ns)
    , input(input)
  {
  }

  SnippetBenchmark::SnippetBenchmark(
    std::map<const std::string, Lisple::Namespace> native_namespaces,
    const std::vector<std::string>& pre_evaluated,
    const std::string& ns,
    const std::string& input)
    : SnippetBenchmark(::testing::UnitTest::GetInstance()->current_test_info()->name(),
                       native_namespaces,
                       pre_evaluated,
                       ns,
                       input)
  {
  }

  SnippetBenchmark::SnippetBenchmark(const std::vector<std::string>& pre_evaluated,
                                     const std::string& ns,
                                     const std::string& input)
    : case_name(::testing::UnitTest::GetInstance()->current_test_info()->name())
    , pre_evaluated(pre_evaluated)
    , ns(ns)
    , input(input)
  {
  }

  SnippetBenchmark::SnippetBenchmark(const std::string& case_name,
                                     const std::vector<std::string>& pre_evaluated,
                                     const std::string& ns,
                                     const std::string& input)
    : case_name(case_name)
    , pre_evaluated(pre_evaluated)
    , ns(ns)
    , input(input)
  {
  }

  Lisple::sptr_rtval SnippetBenchmark::run()
  {
    Lisple::execution_started = false;
    if (skip_benchmark_tests) return Lisple::Constant::NIL;
    Lisple::Runtime runtime(native_namespaces);

    start_time = now();

    snapshot_counters();
    reset_counters();

    for (auto& pe : pre_evaluated)
    {
      runtime.eval(pe);
    }

    runtime.switch_namespace(ns);

    parse_start_time = now();
    auto parse_result = reader.read_sexps(input);
    parse_end_time = now();
    parse_time = parse_end_time - parse_start_time;

    if (parse_result.size() != 1)
    {
      throw Lisple::LispleException("SnippetBenchmark expects a single root form. Got" +
                                    Lisple::Array::make(parse_result)->to_string());
    }

    Lisple::execution_started = true;

    Lisple::Context ctx(runtime);
    Lisple::LowerContext lctx{&ctx};
    lower_start_time = now();
    auto node = Lisple::lower_expr(lctx, parse_result[0]);
    lower_end_time = now();
    lower_time = lower_end_time - lower_start_time;

    setup_counters.lowered_expressions = Lisple::lowered_expressions;
    setup_counters.lowered_literals = Lisple::lowered_literals;
    setup_counters.lower_time_exec_resolutions = Lisple::lower_time_exec_resolutions;
    setup_counters.lower_time_exec_unresolved = Lisple::lower_time_exec_unresolved;
    setup_counters.deprecated_special_form_invocations =
      Lisple::deprecated_special_form_invocations;

    apply_counter_snapshot();

    snapshot_counters();
    reset_counters();

    exec_start_time = now();
    auto result = Lisple::exec(ctx, *node);
    exec_end_time = now();
    exec_time = exec_end_time - exec_start_time;

    end_time = now();
    total_time = end_time - start_time;

    print_result();
    if (logging_enabled)
    {
      log_result();
    }
    apply_counter_snapshot();

    Lisple::execution_started = false;

    setup_counters.lowered_expressions = 0;
    setup_counters.lowered_literals = 0;
    setup_counters.lower_time_exec_resolutions = 0;
    setup_counters.lower_time_exec_unresolved = 0;
    setup_counters.deprecated_special_form_invocations = 0;

    return result;
  }

  void SnippetBenchmark::print_result()
  {
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << input << std::endl;
    std::cout << "+---------------------------------------------------" << std::endl;
    std::cout << "| * Total time: " << total_time << " ms" << std::endl;
    std::cout << "| * Parse time: " << parse_time << " ms" << std::endl;
    std::cout << "| * Lower time: " << lower_time << " ms" << std::endl;
    std::cout << "| *  Exec time: " << exec_time << " ms" << std::endl;
    std::cout << "+---------------------------------------------------" << std::endl;
    print_counters(true);
  }

  void SnippetBenchmark::log_result()
  {
    const std::string CHANGE_ME = "038_no_sform_node_rewrap_a589aac";
    const std::string dir = "benchmarks/" + CHANGE_ME;
    const std::string file_name = dir + "/" + case_name + ".csv";

    std::filesystem::create_directories(dir);

    std::ifstream in(file_name);
    bool empty = true;
    if (in.good())
    {
      empty = in.peek() == std::ifstream::traits_type::eof();
    }
    in.close();

    std::ofstream out(file_name, std::ios::app);

    long unacc_time = total_time - parse_time - lower_time - exec_time;
    long timestamp = now();

    if (empty)
    {
      out
        << "timestamp,benchmark,parse_time_ms,lower_time_ms,exec_time_ms,"
           "unaccounted_time_ms,total_time_ms,eval_path,exec_path,rtval_cons,rtvw_cons,"
           "lit_cons,look_cons,to_ast,to_rt,uf_cons,uf_rt_cons,uf_ast_cons,uf_rt_inv,uf_"
           "ast_inv,lowered_expr,lowered_lit,lower_exec_res,lower_exec_unres,"
           "setup_lowered_expr,setup_lowered_lit,setup_lower_exec_res,setup_lower_exec_unres"
        << std::endl;
    }

    out << timestamp << "," << case_name << "," << parse_time << "," << lower_time << ","
        << exec_time << "," << unacc_time << "," << total_time << ","
        << Lisple::eval_executions << "," << Lisple::exec_executions << ","
        << Lisple::rtvalues_constructed << "," << Lisple::rtvalue_wrappers_constructed << ","
        << Lisple::literal_nodes_constructed << "," << Lisple::lookup_nodes_constructed
        << "," << Lisple::to_ast_conversions << "," << Lisple::to_rtvalue_conversions << ","
        << Lisple::user_functions_created << "," << Lisple::user_functions_rtval_created
        << "," << Lisple::user_functions_ast_created << ","
        << Lisple::user_function_rtval_invocations << ","
        << Lisple::user_function_ast_invocations << ", " << Lisple::lowered_expressions
        << "," << Lisple::lowered_literals << "," << Lisple::lower_time_exec_resolutions
        << "," << Lisple::lower_time_exec_unresolved << ", "
        << setup_counters.lowered_expressions << "," << setup_counters.lowered_literals
        << "," << setup_counters.lower_time_exec_resolutions << ","
        << setup_counters.lower_time_exec_unresolved

        << std::endl;

    out.close();
  }

} // namespace LispleTest
