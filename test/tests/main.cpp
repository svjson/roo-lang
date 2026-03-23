#include "benchmark/benchmark.h"
#include <chrono>
#include <cstdlib>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
#include <lisple/runtime/exec_node.h>

#include "lisple/runtime/exec_tree.h"
#include "lisple/runtime/value.h"

int main(int argc, char** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "--benchmark")
    {
      LispleTest::logging_enabled = true;
    }
    else if (std::string(argv[i]) == "--skip-benchmarks")
    {
      LispleTest::skip_benchmark_tests = true;
    }
  }

  std::srand(std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
               .count());
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();

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
  std::cout << "LookupNode constructions: " << Lisple::lookup_nodes_constructed << std::endl;

  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "to_AST conversions: " << Lisple::to_ast_conversions << std::endl;
  std::cout << "to_RTValue conversions: " << Lisple::to_rtvalue_conversions << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;

  return ret;
}
