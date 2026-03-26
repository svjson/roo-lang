#include "benchmark.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/runtime.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>

#include "lisple/form.h"

namespace LispleTest
{
  Counters counter_snapshot;

  void print_counters()
  {
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "EVAL path executions: " << Lisple::eval_executions << std::endl;
    std::cout << "EXEC path executions: " << Lisple::exec_executions << std::endl;
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
  }

  void snapshot_counters()
  {
    counter_snapshot.eval_executions = Lisple::eval_executions;
    counter_snapshot.exec_executions = Lisple::exec_executions;
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
  }

  void reset_counters()
  {
    Lisple::eval_executions = 0;
    Lisple::exec_executions = 0;
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
  }

  void restore_counter_snapshot()
  {
    Lisple::eval_executions = counter_snapshot.eval_executions;
    Lisple::exec_executions = counter_snapshot.exec_executions;
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
  }

  void apply_counter_snapshot()
  {
    Lisple::eval_executions += counter_snapshot.eval_executions;
    Lisple::exec_executions += counter_snapshot.exec_executions;
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
  }

  long now()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
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
    if (skip_benchmark_tests) return Lisple::Constant::NIL;
    Lisple::Runtime runtime(native_namespaces);

    start_time = now();

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

    Lisple::Context ctx(runtime);
    lower_start_time = now();
    auto node = Lisple::lower_expr(parse_result[0]);
    lower_end_time = now();
    lower_time = lower_end_time - lower_start_time;

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
    print_counters();
  }

  void SnippetBenchmark::log_result()
  {
    const std::string CHANGE_ME = "022_arg_coercion_on_exec_path_45ad9eb";
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
             "unaccounted_time_ms,total_time_ms"
          << std::endl;
    }

    out << timestamp << "," << case_name << "," << parse_time << "," << lower_time << ","
        << exec_time << "," << unacc_time << "," << total_time << std::endl;

    out.close();
  }

} // namespace LispleTest
