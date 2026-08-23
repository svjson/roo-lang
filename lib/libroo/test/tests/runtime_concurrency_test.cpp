#include <array>
#include <barrier>
#include <exception>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <roo/form.h>
#include <roo/runtime.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

#include <gtest/gtest.h>

namespace
{
  constexpr size_t WORKER_COUNT = 4;

  struct ConcurrentRuntimeResult
  {
    Roo::sptr_val value;
    Roo::sptr_val keyword;
    Roo::sptr_val repeated_keyword;
    std::shared_ptr<Roo::AST::Keyword> ast_keyword;
    std::shared_ptr<Roo::AST::Keyword> repeated_ast_keyword;
    std::string source;
    std::vector<int> random_values;
    std::exception_ptr error;
  };
} // namespace

TEST(RuntimeConcurrency, independent_runtimes_evaluate_concurrently)
{
  std::barrier start(WORKER_COUNT);
  std::array<ConcurrentRuntimeResult, WORKER_COUNT> results;
  std::vector<std::thread> threads;
  threads.reserve(WORKER_COUNT);

  for (size_t i = 0; i < WORKER_COUNT; ++i)
  {
    threads.emplace_back(
      [i, &start, &results]
      {
        start.arrive_and_wait();

        try
        {
          Roo::Runtime runtime;
          runtime.set_source_diagnostics(true);

          const std::string source = "concurrent-runtime-" + std::to_string(i) + ".roo";
          runtime.eval("(def worker-value " + std::to_string(i) + ")", source);

          for (size_t repetition = 0; repetition < 20; ++repetition)
          {
            results[i].value =
              runtime.eval("[worker-value :shared :shared (+ worker-value 1)]", source);
          }

          results[i].keyword = runtime.eval(":shared", source);
          results[i].repeated_keyword = runtime.eval("(keyword \"shared\")", source);
          results[i].ast_keyword = Roo::AST::Keyword::make("shared", runtime.ast_pool());
          results[i].repeated_ast_keyword =
            Roo::AST::Keyword::make("shared", runtime.ast_pool());
          results[i].source =
            runtime.describe_source(Roo::SourceRef{1, Roo::SourceSpan{{1, 1}, {1, 1}}});

          runtime.seed_random(1234);
          for (size_t sample = 0; sample < 32; ++sample)
          {
            results[i].random_values.push_back(runtime.random_int(0, 1000000));
          }
        }
        catch (...)
        {
          results[i].error = std::current_exception();
        }
      });
  }

  for (auto& thread : threads)
  {
    thread.join();
  }

  for (const auto& result : results)
  {
    if (result.error) std::rethrow_exception(result.error);
  }

  for (size_t i = 0; i < WORKER_COUNT; ++i)
  {
    EXPECT_EQ(results[i].value->to_string(),
              "[" + std::to_string(i) + " :shared :shared " + std::to_string(i + 1) + "]");
    EXPECT_EQ(results[i].keyword.get(), results[i].repeated_keyword.get());
    EXPECT_EQ(results[i].ast_keyword.get(), results[i].repeated_ast_keyword.get());
    EXPECT_EQ(results[i].source, "concurrent-runtime-" + std::to_string(i) + ".roo:1:1");
    EXPECT_EQ(results[i].random_values, results[0].random_values);

    for (size_t other = i + 1; other < WORKER_COUNT; ++other)
    {
      EXPECT_NE(results[i].keyword.get(), results[other].keyword.get());
      EXPECT_NE(results[i].ast_keyword.get(), results[other].ast_keyword.get());
    }
  }
}
