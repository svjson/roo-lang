#include "benchmark.h"

#include "lisple/form.h"
#include "lisple/runtime/node.h"
#include "lisple/runtime/value.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

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
  SetupCounters input_lower_counters;

  void clear_setup_counters()
  {
    setup_counters = SetupCounters{};
    input_lower_counters = SetupCounters{};
  }

  void snapshot_setup_counters(SetupCounters& counters)
  {
    counters.lowered_literals = Lisple::lowered_literals;
    counters.lowered_expressions = Lisple::lowered_expressions;
    counters.lower_time_exec_resolutions = Lisple::lower_time_exec_resolutions;
    counters.lower_time_exec_unresolved = Lisple::lower_time_exec_unresolved;
    counters.rtvalues_constructed = Lisple::rtvalues_constructed;
    counters.exec_nodes_constructed = Lisple::exec_nodes_constructed;
    counters.call_nodes_constructed = Lisple::call_nodes_constructed;
    counters.literal_nodes_constructed = Lisple::literal_nodes_constructed;
    counters.lookup_nodes_constructed = Lisple::lookup_nodes_constructed;
    counters.user_functions_rtval_created = Lisple::user_functions_rtval_created;
    counters.deprecated_special_form_invocations =
      Lisple::deprecated_special_form_invocations;
  }

  void snapshot_setup_counters()
  {
    snapshot_setup_counters(setup_counters);
  }

  void snapshot_input_lower_counters()
  {
    snapshot_setup_counters(input_lower_counters);
  }

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
      std::cout << "SETUP PHASE - Value constructions: "
                << setup_counters.rtvalues_constructed << std::endl;
      std::cout << "SETUP PHASE - ExecNode constructions: "
                << setup_counters.exec_nodes_constructed << std::endl;
      std::cout << "SETUP PHASE - CallNode constructions: "
                << setup_counters.call_nodes_constructed << std::endl;
      std::cout << "SETUP PHASE - LiteralNode constructions: "
                << setup_counters.literal_nodes_constructed << std::endl;
      std::cout << "SETUP PHASE - LookupNode constructions: "
                << setup_counters.lookup_nodes_constructed << std::endl;
      std::cout << "SETUP PHASE - UserFunction(Value body) constructions: "
                << setup_counters.user_functions_rtval_created << std::endl;
      std::cout << "-----------------------------------------------" << std::endl;
      std::cout << "INPUT LOWER PHASE - Lowered expressions: "
                << input_lower_counters.lowered_expressions << std::endl;
      std::cout << "INPUT LOWER PHASE - Lowered literals: "
                << input_lower_counters.lowered_literals << std::endl;
      std::cout << "INPUT LOWER PHASE - Callees resolved at lower time: "
                << input_lower_counters.lower_time_exec_resolutions << std::endl;
      std::cout << "INPUT LOWER PHASE - Callees unresolved at lower time: "
                << input_lower_counters.lower_time_exec_unresolved << std::endl;
      std::cout << "INPUT LOWER PHASE - Value constructions: "
                << input_lower_counters.rtvalues_constructed << std::endl;
      std::cout << "INPUT LOWER PHASE - ExecNode constructions: "
                << input_lower_counters.exec_nodes_constructed << std::endl;
      std::cout << "INPUT LOWER PHASE - CallNode constructions: "
                << input_lower_counters.call_nodes_constructed << std::endl;
      std::cout << "INPUT LOWER PHASE - LiteralNode constructions: "
                << input_lower_counters.literal_nodes_constructed << std::endl;
      std::cout << "INPUT LOWER PHASE - LookupNode constructions: "
                << input_lower_counters.lookup_nodes_constructed << std::endl;
      std::cout << "INPUT LOWER PHASE - UserFunction(Value body) constructions: "
                << input_lower_counters.user_functions_rtval_created << std::endl;
    }

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Lowered expressions: " << Lisple::lowered_expressions << std::endl;
    std::cout << "Lowered literals: " << Lisple::lowered_literals << std::endl;
    std::cout << "Callees resolved at lower time: " << Lisple::lower_time_exec_resolutions
              << std::endl;
    std::cout << "Callees unresolved at lower time: " << Lisple::lower_time_exec_unresolved
              << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    std::cout << "Value constructions: " << Lisple::rtvalues_constructed << std::endl;
    std::cout << "RuntimeValueWrapper constructions: "
              << Lisple::rtvalue_wrappers_constructed << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "ExecNode constructions: " << Lisple::exec_nodes_constructed << std::endl;
    std::cout << "CallNode constructions: " << Lisple::call_nodes_constructed << std::endl;
    std::cout << "LiteralNode constructions: " << Lisple::literal_nodes_constructed
              << std::endl;
    std::cout << "LookupNode constructions: " << Lisple::lookup_nodes_constructed
              << std::endl;

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "to_AST conversions: " << Lisple::to_ast_conversions << std::endl;
    std::cout << "to_Value conversions: " << Lisple::to_rtvalue_conversions << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "UserFunction constructions: " << Lisple::user_functions_created
              << std::endl;
    std::cout << "UserFunction(Value body) constructions: "
              << Lisple::user_functions_rtval_created << std::endl;
    std::cout << "UserFunction(Value body) invocations: "
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

  SnippetBenchmark& SnippetBenchmark::with_lower_iterations(size_t iterations)
  {
    if (iterations == 0)
    {
      throw Lisple::LispleException("SnippetBenchmark requires at least one lowering");
    }
    lower_iterations = iterations;
    return *this;
  }

  SnippetBenchmark& SnippetBenchmark::without_execution()
  {
    execute = false;
    return *this;
  }

  Lisple::sptr_val SnippetBenchmark::run()
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
    snapshot_setup_counters();

    runtime.switch_namespace(ns);

    parse_start_time = now();
    auto parse_result = reader.read_sexps(input);
    parse_end_time = now();
    parse_time = parse_end_time - parse_start_time;

    if (parse_result.size() != 1)
    {
      throw Lisple::LispleException("SnippetBenchmark expects a single root form. Got" +
                                    Lisple::AST::Vector::make(parse_result)->to_string());
    }

    Lisple::execution_started = true;

    Lisple::Context ctx(runtime);
    Lisple::LowerContext lctx{&ctx};
    std::vector<Lisple::uptr_exec_node> lowered_nodes(lower_iterations);

    reset_counters();
    lower_start_time = now();
    for (size_t i = 0; i < lower_iterations; i++)
    {
      lowered_nodes[i] = Lisple::lower_expr(lctx, parse_result[0]);
    }
    lower_end_time = now();
    lower_time = lower_end_time - lower_start_time;

    snapshot_input_lower_counters();

    lower_cleanup_start_time = now();
    auto node = std::move(lowered_nodes.back());
    lowered_nodes.pop_back();
    lowered_nodes.clear();
    lower_cleanup_end_time = now();
    lower_cleanup_time = lower_cleanup_end_time - lower_cleanup_start_time;

    apply_counter_snapshot();

    snapshot_counters();
    reset_counters();

    auto result = Lisple::Constant::NIL;
    if (execute)
    {
      exec_start_time = now();
      result = Lisple::exec(ctx, *node);
      exec_end_time = now();
      exec_time = exec_end_time - exec_start_time;
    }
    else
    {
      exec_start_time = now();
      exec_end_time = exec_start_time;
      exec_time = 0;
    }

    end_time = now();
    total_time = end_time - start_time;
    single_lowering_total_time =
      static_cast<double>(total_time - lower_time - lower_cleanup_time) +
      (static_cast<double>(lower_time) / static_cast<double>(lower_iterations));

    print_result();
    if (logging_enabled)
    {
      log_result();
    }
    apply_counter_snapshot();

    Lisple::execution_started = false;

    clear_setup_counters();

    return result;
  }

  void SnippetBenchmark::print_result()
  {
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << input << std::endl;
    std::cout << "+---------------------------------------------------" << std::endl;
    std::cout << "| * Total time: " << total_time << " ms" << std::endl;
    std::cout << "| * Single-lowering total time: " << single_lowering_total_time << " ms"
              << std::endl;
    std::cout << "| * Parse time: " << parse_time << " ms" << std::endl;
    std::cout << "| * Lower time: " << lower_time << " ms" << std::endl;
    std::cout << "| * Lower iterations: " << lower_iterations << std::endl;
    std::cout << "| * Lower time / iteration: "
              << static_cast<double>(lower_time) / static_cast<double>(lower_iterations)
              << " ms" << std::endl;
    std::cout << "| *  Exec time: " << exec_time << " ms" << std::endl;
    std::cout << "| *  Exec enabled: " << (execute ? "yes" : "no") << std::endl;
    std::cout << "+---------------------------------------------------" << std::endl;
    print_counters(true);
  }

  void SnippetBenchmark::log_result()
  {
    const std::string CHANGE_ME = "059_pre_lowering_optimization_604162a";
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
      out << "timestamp,benchmark,parse_time_ms,lower_time_ms,exec_time_ms,"
             "unaccounted_time_ms,total_time_ms,single_lowering_total_time_ms,eval_path,"
             "exec_path,rtval_cons,rtvw_cons,"
             "lit_cons,look_cons,to_ast,to_rt,uf_cons,uf_rt_cons,uf_ast_cons,uf_rt_inv,uf_"
             "ast_inv,lowered_expr,lowered_lit,lower_exec_res,lower_exec_unres,"
             "setup_lowered_expr,setup_lowered_lit,setup_lower_exec_res,"
             "setup_lower_exec_unres,setup_rtval_cons,setup_execnode_cons,"
             "setup_call_cons,setup_lit_cons,setup_look_cons,setup_uf_rt_cons,"
             "input_lowered_expr,input_lowered_lit,input_lower_exec_res,"
             "input_lower_exec_unres,input_rtval_cons,input_execnode_cons,"
             "input_call_cons,input_lit_cons,input_look_cons,input_uf_rt_cons,"
             "lower_iterations,lower_time_per_iter_ms,exec_enabled"
          << std::endl;
    }

    const double lower_time_per_iter =
      static_cast<double>(lower_time) / static_cast<double>(lower_iterations);
    const auto with_one_input_lowering = [this](int setup_count, int lower_count) {
      return setup_count + (lower_count / static_cast<int>(lower_iterations));
    };

    out << timestamp << "," << case_name << "," << parse_time << "," << lower_time << ","
        << exec_time << "," << unacc_time << "," << total_time << ","
        << single_lowering_total_time << "," << Lisple::eval_executions << ","
        << Lisple::exec_executions << "," << Lisple::rtvalues_constructed << ","
        << Lisple::rtvalue_wrappers_constructed << "," << Lisple::literal_nodes_constructed
        << "," << Lisple::lookup_nodes_constructed << "," << Lisple::to_ast_conversions
        << "," << Lisple::to_rtvalue_conversions << "," << Lisple::user_functions_created
        << "," << Lisple::user_functions_rtval_created << ","
        << Lisple::user_functions_ast_created << ","
        << Lisple::user_function_rtval_invocations << ","
        << Lisple::user_function_ast_invocations << ", " << Lisple::lowered_expressions
        << "," << Lisple::lowered_literals << "," << Lisple::lower_time_exec_resolutions
        << "," << Lisple::lower_time_exec_unresolved << ", "
        << with_one_input_lowering(setup_counters.lowered_expressions,
                                   input_lower_counters.lowered_expressions)
        << ","
        << with_one_input_lowering(setup_counters.lowered_literals,
                                   input_lower_counters.lowered_literals)
        << ","
        << with_one_input_lowering(setup_counters.lower_time_exec_resolutions,
                                   input_lower_counters.lower_time_exec_resolutions)
        << ","
        << with_one_input_lowering(setup_counters.lower_time_exec_unresolved,
                                   input_lower_counters.lower_time_exec_unresolved)
        << ","
        << with_one_input_lowering(setup_counters.rtvalues_constructed,
                                   input_lower_counters.rtvalues_constructed)
        << ","
        << with_one_input_lowering(setup_counters.exec_nodes_constructed,
                                   input_lower_counters.exec_nodes_constructed)
        << ","
        << with_one_input_lowering(setup_counters.call_nodes_constructed,
                                   input_lower_counters.call_nodes_constructed)
        << ","
        << with_one_input_lowering(setup_counters.literal_nodes_constructed,
                                   input_lower_counters.literal_nodes_constructed)
        << ","
        << with_one_input_lowering(setup_counters.lookup_nodes_constructed,
                                   input_lower_counters.lookup_nodes_constructed)
        << ","
        << with_one_input_lowering(setup_counters.user_functions_rtval_created,
                                   input_lower_counters.user_functions_rtval_created)
        << ","
        << input_lower_counters.lowered_expressions << ","
        << input_lower_counters.lowered_literals << ","
        << input_lower_counters.lower_time_exec_resolutions << ","
        << input_lower_counters.lower_time_exec_unresolved << ","
        << input_lower_counters.rtvalues_constructed << ","
        << input_lower_counters.exec_nodes_constructed << ","
        << input_lower_counters.call_nodes_constructed << ","
        << input_lower_counters.literal_nodes_constructed << ","
        << input_lower_counters.lookup_nodes_constructed << ","
        << input_lower_counters.user_functions_rtval_created << "," << lower_iterations << ","
        << lower_time_per_iter << "," << (execute ? 1 : 0)

        << std::endl;

    out.close();
  }

} // namespace LispleTest
