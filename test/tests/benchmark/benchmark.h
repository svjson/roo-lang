
#ifndef LISPLE_TEST_NOW_H
#define LISPLE_TEST_NOW_H

#include "lisple/reader.h"
namespace LispleTest
{
  inline bool logging_enabled = false;

  long now();

  class SnippetBenchmark
  {
    Lisple::Reader reader;

   public:
    SnippetBenchmark(const std::string& case_name, const std::string& input);
    SnippetBenchmark(const std::string& case_name,
                     const std::vector<std::string>& pre_evaluated,
                     const std::string& ns,
                     const std::string& input);

    void run();
    void print_result();
    void log_result();

    std::string case_name;
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
