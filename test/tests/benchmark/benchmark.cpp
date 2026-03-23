#include "benchmark.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/runtime.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>

namespace LispleTest
{
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

  void SnippetBenchmark::run()
  {
    if (skip_benchmark_tests) return;
    Lisple::Runtime runtime;

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
  }

  void SnippetBenchmark::log_result()
  {
    const std::string CHANGE_ME = "014_rtval_execution_of_user_functions_ded3388";
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
