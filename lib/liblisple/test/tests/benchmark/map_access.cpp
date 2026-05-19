#include <cstddef>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

#include <lisple/adapter.h>
#include <lisple/namespace.h>
#include <lisple/runtime/value.h>

#include "benchmark.h"
#include <gtest/gtest.h>

namespace
{
  constexpr size_t MAP_SMALL_SIZE = 16;
  constexpr size_t MAP_LARGE_SIZE = 16 * 1024;
  constexpr int MAP_EARLY_KEY = 0;
  constexpr int MAP_EARLY_MISS_KEY = -1;
  constexpr int MAP_SMALL_LATE_KEY = static_cast<int>(MAP_SMALL_SIZE) - 1;
  constexpr int MAP_LARGE_LATE_KEY = static_cast<int>(MAP_LARGE_SIZE) - 1;
  constexpr int MAP_SMALL_LATE_MISS_KEY = static_cast<int>(MAP_SMALL_SIZE);
  constexpr int MAP_LARGE_LATE_MISS_KEY = static_cast<int>(MAP_LARGE_SIZE);
  constexpr int MAP_NESTED_KEY = 0;

  struct MapAccessBinding
  {
    const char* name;
    Lisple::sptr_val (*make_value)();
  };

  Lisple::sptr_val make_lisple_map(size_t size)
  {
    Lisple::sptr_val_v entries;
    entries.reserve(size * 2);

    for (int i = 0; i < static_cast<int>(size); i++)
    {
      entries.push_back(Lisple::Value::number(i));
      entries.push_back(Lisple::Value::string(std::string("value_") + std::to_string(i)));
    }

    return Lisple::Value::map(std::move(entries));
  }

  std::map<int, std::string> make_native_map(size_t size)
  {
    std::map<int, std::string> values;
    for (int i = 0; i < static_cast<int>(size); i++)
    {
      values.emplace(i, std::string("value_") + std::to_string(i));
    }

    return values;
  }

  Lisple::sptr_val make_lisple_nested_map(size_t size)
  {
    Lisple::sptr_val_v entries;
    entries.reserve(size * 2);

    for (int i = 0; i < static_cast<int>(size); i++)
    {
      Lisple::sptr_val_v nested_entry;
      nested_entry.reserve(2);
      nested_entry.push_back(Lisple::Value::number(MAP_NESTED_KEY));
      nested_entry.push_back(Lisple::Value::string(std::to_string(i)));

      entries.push_back(Lisple::Value::number(i));
      entries.push_back(Lisple::Value::map(std::move(nested_entry)));
    }

    return Lisple::Value::map(std::move(entries));
  }

  Lisple::sptr_val make_lisple_mutation_maps(size_t size, size_t count)
  {
    Lisple::sptr_val_v maps;
    maps.reserve(count);
    for (size_t i = 0; i < count; i++)
    {
      maps.push_back(make_lisple_map(size));
    }

    return Lisple::Value::vector(std::move(maps));
  }

  Lisple::sptr_val make_lisple_nested_mutation_maps(size_t size, size_t count)
  {
    Lisple::sptr_val_v maps;
    maps.reserve(count);
    for (size_t i = 0; i < count; i++)
    {
      maps.push_back(make_lisple_nested_map(size));
    }

    return Lisple::Value::vector(std::move(maps));
  }

  Lisple::sptr_val make_native_mutation_maps(size_t size, size_t count)
  {
    const std::map<int, std::string> source = make_native_map(size);

    Lisple::sptr_val_v maps;
    maps.reserve(count);
    for (size_t i = 0; i < count; i++)
    {
      maps.push_back(Lisple::NativeStdMapAdapter<int, std::string>::make_unique(source));
    }

    return Lisple::Value::vector(std::move(maps));
  }

  Lisple::sptr_val lisple_small_map()
  {
    return make_lisple_map(MAP_SMALL_SIZE);
  }

  Lisple::sptr_val lisple_large_map()
  {
    return make_lisple_map(MAP_LARGE_SIZE);
  }

  Lisple::sptr_val native_small_map()
  {
    return Lisple::NativeStdMapAdapter<int, std::string>::make_unique(
      make_native_map(MAP_SMALL_SIZE));
  }

  Lisple::sptr_val native_large_map()
  {
    return Lisple::NativeStdMapAdapter<int, std::string>::make_unique(
      make_native_map(MAP_LARGE_SIZE));
  }

  Lisple::sptr_val lisple_small_mutation_maps_1500()
  {
    return make_lisple_mutation_maps(MAP_SMALL_SIZE, 1500);
  }

  Lisple::sptr_val lisple_large_mutation_maps_200()
  {
    return make_lisple_mutation_maps(MAP_LARGE_SIZE, 200);
  }

  Lisple::sptr_val lisple_nested_small_mutation_maps_1500()
  {
    return make_lisple_nested_mutation_maps(MAP_SMALL_SIZE, 1500);
  }

  Lisple::sptr_val lisple_nested_large_mutation_maps_200()
  {
    return make_lisple_nested_mutation_maps(MAP_LARGE_SIZE, 200);
  }

  Lisple::sptr_val native_small_mutation_maps_1500()
  {
    return make_native_mutation_maps(MAP_SMALL_SIZE, 1500);
  }

  Lisple::sptr_val native_large_mutation_maps_200()
  {
    return make_native_mutation_maps(MAP_LARGE_SIZE, 200);
  }

  Lisple::sptr_val hit_key()
  {
    return Lisple::Value::number(MAP_EARLY_KEY);
  }

  Lisple::sptr_val miss_key()
  {
    return Lisple::Value::number(MAP_EARLY_MISS_KEY);
  }

  Lisple::sptr_val small_late_key()
  {
    return Lisple::Value::number(MAP_SMALL_LATE_KEY);
  }

  Lisple::sptr_val small_late_miss_key()
  {
    return Lisple::Value::number(MAP_SMALL_LATE_MISS_KEY);
  }

  Lisple::sptr_val large_late_key()
  {
    return Lisple::Value::number(MAP_LARGE_LATE_KEY);
  }

  Lisple::sptr_val large_late_miss_key()
  {
    return Lisple::Value::number(MAP_LARGE_LATE_MISS_KEY);
  }

  Lisple::sptr_val nested_key()
  {
    return Lisple::Value::number(MAP_NESTED_KEY);
  }

  Lisple::sptr_val assoc_value()
  {
    return Lisple::Value::string("updated");
  }

  Lisple::sptr_val assoc_in_value()
  {
    return Lisple::Value::string("assoc-in-updated");
  }

  std::map<const std::string, Lisple::Namespace> map_access_namespaces(
    std::initializer_list<MapAccessBinding> bindings)
  {
    Lisple::Namespace ns("benchmark.map-access");
    for (const auto& binding : bindings)
    {
      ns.store(binding.name, binding.make_value());
    }

    return {{"benchmark.map-access", std::move(ns)}};
  }

  LispleTest::SnippetBenchmark map_access_benchmark(
    const std::string& case_name,
    std::initializer_list<MapAccessBinding> bindings,
    const std::string& input)
  {
    return LispleTest::SnippetBenchmark(case_name,
                                        map_access_namespaces(bindings),
                                        {},
                                        "benchmark.map-access",
                                        input);
  }
} // namespace

TEST(Benchmark_MapAccess, value_map__small__get_hit__2000_iterations)
{
  auto bm = map_access_benchmark("value_map__small__get_hit__2000_iterations",
                                 {{"lisple-small", lisple_small_map}, {"hit-key", hit_key}},
                                 "(dotimes [n 2000] (get lisple-small hit-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__get_hit__2000_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__small__get_hit__2000_iterations",
                         {{"native-std-small", native_small_map}, {"hit-key", hit_key}},
                         "(dotimes [n 2000] (get native-std-small hit-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__get_hit__500_iterations)
{
  auto bm = map_access_benchmark("value_map__large__get_hit__500_iterations",
                                 {{"lisple-large", lisple_large_map}, {"hit-key", hit_key}},
                                 "(dotimes [n 500] (get lisple-large hit-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__get_hit__500_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__get_hit__500_iterations",
                         {{"native-std-large", native_large_map}, {"hit-key", hit_key}},
                         "(dotimes [n 500] (get native-std-large hit-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__get_hit_late__2000_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__small__get_hit_late__2000_iterations",
    {{"lisple-small", lisple_small_map}, {"small-late-key", small_late_key}},
    "(dotimes [n 2000] (get lisple-small small-late-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__get_hit_late__2000_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__get_hit_late__2000_iterations",
    {{"native-std-small", native_small_map}, {"small-late-key", small_late_key}},
    "(dotimes [n 2000] (get native-std-small small-late-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__get_hit_late__500_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__large__get_hit_late__500_iterations",
    {{"lisple-large", lisple_large_map}, {"large-late-key", large_late_key}},
    "(dotimes [n 500] (get lisple-large large-late-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__get_hit_late__500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__large__get_hit_late__500_iterations",
    {{"native-std-large", native_large_map}, {"large-late-key", large_late_key}},
    "(dotimes [n 500] (get native-std-large large-late-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__get_miss__2000_iterations)
{
  auto bm =
    map_access_benchmark("value_map__small__get_miss__2000_iterations",
                         {{"lisple-small", lisple_small_map}, {"miss-key", miss_key}},
                         "(dotimes [n 2000] (get lisple-small miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__get_miss__2000_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__small__get_miss__2000_iterations",
                         {{"native-std-small", native_small_map}, {"miss-key", miss_key}},
                         "(dotimes [n 2000] (get native-std-small miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__get_miss__500_iterations)
{
  auto bm =
    map_access_benchmark("value_map__large__get_miss__500_iterations",
                         {{"lisple-large", lisple_large_map}, {"miss-key", miss_key}},
                         "(dotimes [n 500] (get lisple-large miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__get_miss__500_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__get_miss__500_iterations",
                         {{"native-std-large", native_large_map}, {"miss-key", miss_key}},
                         "(dotimes [n 500] (get native-std-large miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__get_miss_late__2000_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__small__get_miss_late__2000_iterations",
    {{"lisple-small", lisple_small_map}, {"small-late-miss-key", small_late_miss_key}},
    "(dotimes [n 2000] (get lisple-small small-late-miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__get_miss_late__2000_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__get_miss_late__2000_iterations",
    {{"native-std-small", native_small_map}, {"small-late-miss-key", small_late_miss_key}},
    "(dotimes [n 2000] (get native-std-small small-late-miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__get_miss_late__500_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__large__get_miss_late__500_iterations",
    {{"lisple-large", lisple_large_map}, {"large-late-miss-key", large_late_miss_key}},
    "(dotimes [n 500] (get lisple-large large-late-miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__get_miss_late__500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__large__get_miss_late__500_iterations",
    {{"native-std-large", native_large_map}, {"large-late-miss-key", large_late_miss_key}},
    "(dotimes [n 500] (get native-std-large large-late-miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_hit__1000_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__small__assoc_hit__1000_iterations",
    {{"lisple-small", lisple_small_map}, {"hit-key", hit_key}, {"assoc-value", assoc_value}},
    "(dotimes [n 1000] (assoc lisple-small hit-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_hit__1000_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__assoc_hit__1000_iterations",
                                 {{"native-std-small", native_small_map},
                                  {"hit-key", hit_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 1000] (assoc native-std-small hit-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_hit__220_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__large__assoc_hit__220_iterations",
    {{"lisple-large", lisple_large_map}, {"hit-key", hit_key}, {"assoc-value", assoc_value}},
    "(dotimes [n 220] (assoc lisple-large hit-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_hit__220_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__assoc_hit__220_iterations",
                                 {{"native-std-large", native_large_map},
                                  {"hit-key", hit_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 220] (assoc native-std-large hit-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_hit_late__1000_iterations)
{
  auto bm = map_access_benchmark("value_map__small__assoc_hit_late__1000_iterations",
                                 {{"lisple-small", lisple_small_map},
                                  {"small-late-key", small_late_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 1000] (assoc lisple-small small-late-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_hit_late__1000_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__assoc_hit_late__1000_iterations",
                                 {{"native-std-small", native_small_map},
                                  {"small-late-key", small_late_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 1000] (assoc native-std-small small-late-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_hit_late__220_iterations)
{
  auto bm = map_access_benchmark("value_map__large__assoc_hit_late__220_iterations",
                                 {{"lisple-large", lisple_large_map},
                                  {"large-late-key", large_late_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 220] (assoc lisple-large large-late-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_hit_late__220_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__assoc_hit_late__220_iterations",
                                 {{"native-std-large", native_large_map},
                                  {"large-late-key", large_late_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 220] (assoc native-std-large large-late-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_miss__1000_iterations)
{
  auto bm =
    map_access_benchmark("value_map__small__assoc_miss__1000_iterations",
                         {{"lisple-small", lisple_small_map},
                          {"miss-key", miss_key},
                          {"assoc-value", assoc_value}},
                         "(dotimes [n 1000] (assoc lisple-small miss-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_miss__1000_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__assoc_miss__1000_iterations",
                                 {{"native-std-small", native_small_map},
                                  {"miss-key", miss_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 1000] (assoc native-std-small miss-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_miss__220_iterations)
{
  auto bm =
    map_access_benchmark("value_map__large__assoc_miss__220_iterations",
                         {{"lisple-large", lisple_large_map},
                          {"miss-key", miss_key},
                          {"assoc-value", assoc_value}},
                         "(dotimes [n 220] (assoc lisple-large miss-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_miss__220_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__assoc_miss__220_iterations",
                                 {{"native-std-large", native_large_map},
                                  {"miss-key", miss_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 220] (assoc native-std-large miss-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_miss_late__1000_iterations)
{
  auto bm = map_access_benchmark("value_map__small__assoc_miss_late__1000_iterations",
                                 {{"lisple-small", lisple_small_map},
                                  {"small-late-miss-key", small_late_miss_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 1000] (assoc lisple-small small-late-miss-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_miss_late__1000_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__assoc_miss_late__1000_iterations",
                                 {{"native-std-small", native_small_map},
                                  {"small-late-miss-key", small_late_miss_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 1000] (assoc native-std-small "
                                 "small-late-miss-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_miss_late__220_iterations)
{
  auto bm = map_access_benchmark("value_map__large__assoc_miss_late__220_iterations",
                                 {{"lisple-large", lisple_large_map},
                                  {"large-late-miss-key", large_late_miss_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 220] (assoc lisple-large large-late-miss-key "
                                 "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_miss_late__220_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__assoc_miss_late__220_iterations",
                                 {{"native-std-large", native_large_map},
                                  {"large-late-miss-key", large_late_miss_key},
                                  {"assoc-value", assoc_value}},
                                 "(dotimes [n 220] (assoc native-std-large "
                                 "large-late-miss-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_bang_hit__1500_iterations)
{
  auto bm =
    map_access_benchmark("value_map__small__assoc_bang_hit__1500_iterations",
                         {{"lisple-small-mutation-maps", lisple_small_mutation_maps_1500},
                          {"hit-key", hit_key},
                          {"assoc-value", assoc_value}},
                         "(dotimes [n 1500] (assoc! (get "
                         "lisple-small-mutation-maps n) hit-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_bang_hit__1500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__assoc_bang_hit__1500_iterations",
    {{"native-std-small-mutation-maps", native_small_mutation_maps_1500},
     {"hit-key", hit_key},
     {"assoc-value", assoc_value}},
    "(dotimes [n 1500] (assoc! (get "
    "native-std-small-mutation-maps n) hit-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_bang_hit__200_iterations)
{
  auto bm =
    map_access_benchmark("value_map__large__assoc_bang_hit__200_iterations",
                         {{"lisple-large-mutation-maps", lisple_large_mutation_maps_200},
                          {"hit-key", hit_key},
                          {"assoc-value", assoc_value}},
                         "(dotimes [n 200] (assoc! (get "
                         "lisple-large-mutation-maps n) hit-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_bang_hit__200_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__assoc_bang_hit__200_iterations",
                         {{"native-std-large-mutation-maps", native_large_mutation_maps_200},
                          {"hit-key", hit_key},
                          {"assoc-value", assoc_value}},
                         "(dotimes [n 200] (assoc! (get "
                         "native-std-large-mutation-maps n) hit-key assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_bang_hit_late__1500_iterations)
{
  auto bm =
    map_access_benchmark("value_map__small__assoc_bang_hit_late__1500_iterations",
                         {{"lisple-small-mutation-maps", lisple_small_mutation_maps_1500},
                          {"small-late-key", small_late_key},
                          {"assoc-value", assoc_value}},
                         "(dotimes [n 1500] (assoc! (get "
                         "lisple-small-mutation-maps n) small-late-key "
                         "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_bang_hit_late__1500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__assoc_bang_hit_late__1500_iterations",
    {{"native-std-small-mutation-maps", native_small_mutation_maps_1500},
     {"small-late-key", small_late_key},
     {"assoc-value", assoc_value}},
    "(dotimes [n 1500] (assoc! (get "
    "native-std-small-mutation-maps n) small-late-key "
    "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_bang_hit_late__200_iterations)
{
  auto bm =
    map_access_benchmark("value_map__large__assoc_bang_hit_late__200_iterations",
                         {{"lisple-large-mutation-maps", lisple_large_mutation_maps_200},
                          {"large-late-key", large_late_key},
                          {"assoc-value", assoc_value}},
                         "(dotimes [n 200] (assoc! (get "
                         "lisple-large-mutation-maps n) large-late-key "
                         "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_bang_hit_late__200_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__assoc_bang_hit_late__200_iterations",
                         {{"native-std-large-mutation-maps", native_large_mutation_maps_200},
                          {"large-late-key", large_late_key},
                          {"assoc-value", assoc_value}},
                         "(dotimes [n 200] (assoc! (get "
                         "native-std-large-mutation-maps n) large-late-key "
                         "assoc-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_in_hit__1000_iterations)
{
  auto bm = map_access_benchmark("value_map__small__assoc_in_hit__1000_iterations",
                                 {{"lisple-small", lisple_small_map},
                                  {"hit-key", hit_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 1000] (assoc-in lisple-small [hit-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_in_hit__1000_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__assoc_in_hit__1000_iterations",
                                 {{"native-std-small", native_small_map},
                                  {"hit-key", hit_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 1000] (assoc-in native-std-small [hit-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_in_hit__220_iterations)
{
  auto bm = map_access_benchmark("value_map__large__assoc_in_hit__220_iterations",
                                 {{"lisple-large", lisple_large_map},
                                  {"hit-key", hit_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 220] (assoc-in lisple-large [hit-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_in_hit__220_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__assoc_in_hit__220_iterations",
                                 {{"native-std-large", native_large_map},
                                  {"hit-key", hit_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 220] (assoc-in native-std-large [hit-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_in_hit_late__1000_iterations)
{
  auto bm = map_access_benchmark("value_map__small__assoc_in_hit_late__1000_iterations",
                                 {{"lisple-small", lisple_small_map},
                                  {"small-late-key", small_late_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 1000] (assoc-in lisple-small [small-late-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_in_hit_late__1000_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__assoc_in_hit_late__1000_iterations",
                                 {{"native-std-small", native_small_map},
                                  {"small-late-key", small_late_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 1000] (assoc-in native-std-small "
                                 "[small-late-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_in_hit_late__220_iterations)
{
  auto bm = map_access_benchmark("value_map__large__assoc_in_hit_late__220_iterations",
                                 {{"lisple-large", lisple_large_map},
                                  {"large-late-key", large_late_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 220] (assoc-in lisple-large [large-late-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_in_hit_late__220_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__assoc_in_hit_late__220_iterations",
                                 {{"native-std-large", native_large_map},
                                  {"large-late-key", large_late_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 220] (assoc-in native-std-large "
                                 "[large-late-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_in_miss__1000_iterations)
{
  auto bm = map_access_benchmark("value_map__small__assoc_in_miss__1000_iterations",
                                 {{"lisple-small", lisple_small_map},
                                  {"miss-key", miss_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 1000] (assoc-in lisple-small [miss-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_in_miss__1000_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__assoc_in_miss__1000_iterations",
                                 {{"native-std-small", native_small_map},
                                  {"miss-key", miss_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 1000] (assoc-in native-std-small [miss-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_in_miss__220_iterations)
{
  auto bm = map_access_benchmark("value_map__large__assoc_in_miss__220_iterations",
                                 {{"lisple-large", lisple_large_map},
                                  {"miss-key", miss_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 220] (assoc-in lisple-large [miss-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_in_miss__220_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__assoc_in_miss__220_iterations",
                                 {{"native-std-large", native_large_map},
                                  {"miss-key", miss_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 220] (assoc-in native-std-large [miss-key] "
                                 "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_in_miss_late__1000_iterations)
{
  auto bm = map_access_benchmark("value_map__small__assoc_in_miss_late__1000_iterations",
                                 {{"lisple-small", lisple_small_map},
                                  {"small-late-miss-key", small_late_miss_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 1000] (assoc-in lisple-small "
                                 "[small-late-miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_in_miss_late__1000_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__assoc_in_miss_late__1000_iterations",
                                 {{"native-std-small", native_small_map},
                                  {"small-late-miss-key", small_late_miss_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 1000] (assoc-in native-std-small "
                                 "[small-late-miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_in_miss_late__220_iterations)
{
  auto bm = map_access_benchmark("value_map__large__assoc_in_miss_late__220_iterations",
                                 {{"lisple-large", lisple_large_map},
                                  {"large-late-miss-key", large_late_miss_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 220] (assoc-in lisple-large "
                                 "[large-late-miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_in_miss_late__220_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__assoc_in_miss_late__220_iterations",
                                 {{"native-std-large", native_large_map},
                                  {"large-late-miss-key", large_late_miss_key},
                                  {"assoc-in-value", assoc_in_value}},
                                 "(dotimes [n 220] (assoc-in native-std-large "
                                 "[large-late-miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_in_bang_nested_hit__1500_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__small__assoc_in_bang_nested_hit__1500_iterations",
    {{"lisple-small-nested-mutation-maps", lisple_nested_small_mutation_maps_1500},
     {"hit-key", hit_key},
     {"nested-key", nested_key},
     {"assoc-in-value", assoc_in_value}},
    "(dotimes [n 1500] (assoc-in! (get "
    "lisple-small-nested-mutation-maps n) [hit-key nested-key] "
    "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_in_bang_nested_hit__200_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__large__assoc_in_bang_nested_hit__200_iterations",
    {{"lisple-large-nested-mutation-maps", lisple_nested_large_mutation_maps_200},
     {"hit-key", hit_key},
     {"nested-key", nested_key},
     {"assoc-in-value", assoc_in_value}},
    "(dotimes [n 200] (assoc-in! (get "
    "lisple-large-nested-mutation-maps n) [hit-key nested-key] "
    "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_in_bang_hit__1500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__assoc_in_bang_hit__1500_iterations",
    {{"native-std-small-mutation-maps", native_small_mutation_maps_1500},
     {"hit-key", hit_key},
     {"assoc-in-value", assoc_in_value}},
    "(dotimes [n 1500] (assoc-in! (get "
    "native-std-small-mutation-maps n) [hit-key] "
    "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_in_bang_hit__200_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__assoc_in_bang_hit__200_iterations",
                         {{"native-std-large-mutation-maps", native_large_mutation_maps_200},
                          {"hit-key", hit_key},
                          {"assoc-in-value", assoc_in_value}},
                         "(dotimes [n 200] (assoc-in! (get "
                         "native-std-large-mutation-maps n) [hit-key] "
                         "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_in_bang_nested_late_hit__1500_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__small__assoc_in_bang_nested_late_hit__1500_iterations",
    {{"lisple-small-nested-mutation-maps", lisple_nested_small_mutation_maps_1500},
     {"small-late-key", small_late_key},
     {"nested-key", nested_key},
     {"assoc-in-value", assoc_in_value}},
    "(dotimes [n 1500] (assoc-in! (get "
    "lisple-small-nested-mutation-maps n) [small-late-key nested-key] "
    "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_in_bang_nested_late_hit__200_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__large__assoc_in_bang_nested_late_hit__200_iterations",
    {{"lisple-large-nested-mutation-maps", lisple_nested_large_mutation_maps_200},
     {"large-late-key", large_late_key},
     {"nested-key", nested_key},
     {"assoc-in-value", assoc_in_value}},
    "(dotimes [n 200] (assoc-in! (get "
    "lisple-large-nested-mutation-maps n) [large-late-key nested-key] "
    "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_in_bang_late_hit__1500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__assoc_in_bang_late_hit__1500_iterations",
    {{"native-std-small-mutation-maps", native_small_mutation_maps_1500},
     {"small-late-key", small_late_key},
     {"assoc-in-value", assoc_in_value}},
    "(dotimes [n 1500] (assoc-in! (get "
    "native-std-small-mutation-maps n) [small-late-key] "
    "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_in_bang_late_hit__200_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__assoc_in_bang_late_hit__200_iterations",
                         {{"native-std-large-mutation-maps", native_large_mutation_maps_200},
                          {"large-late-key", large_late_key},
                          {"assoc-in-value", assoc_in_value}},
                         "(dotimes [n 200] (assoc-in! (get "
                         "native-std-large-mutation-maps n) [large-late-key] "
                         "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_in_bang_miss__1500_iterations)
{
  auto bm =
    map_access_benchmark("value_map__small__assoc_in_bang_miss__1500_iterations",
                         {{"lisple-small-mutation-maps", lisple_small_mutation_maps_1500},
                          {"miss-key", miss_key},
                          {"assoc-in-value", assoc_in_value}},
                         "(dotimes [n 1500] (assoc-in! (get lisple-small-mutation-maps n) "
                         "[miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_in_bang_miss__200_iterations)
{
  auto bm =
    map_access_benchmark("value_map__large__assoc_in_bang_miss__200_iterations",
                         {{"lisple-large-mutation-maps", lisple_large_mutation_maps_200},
                          {"miss-key", miss_key},
                          {"assoc-in-value", assoc_in_value}},
                         "(dotimes [n 200] (assoc-in! (get lisple-large-mutation-maps n) "
                         "[miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__assoc_in_bang_miss_late__1500_iterations)
{
  auto bm =
    map_access_benchmark("value_map__small__assoc_in_bang_miss_late__1500_iterations",
                         {{"lisple-small-mutation-maps", lisple_small_mutation_maps_1500},
                          {"small-late-miss-key", small_late_miss_key},
                          {"assoc-in-value", assoc_in_value}},
                         "(dotimes [n 1500] (assoc-in! (get lisple-small-mutation-maps n) "
                         "[small-late-miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__assoc_in_bang_miss_late__200_iterations)
{
  auto bm =
    map_access_benchmark("value_map__large__assoc_in_bang_miss_late__200_iterations",
                         {{"lisple-large-mutation-maps", lisple_large_mutation_maps_200},
                          {"large-late-miss-key", large_late_miss_key},
                          {"assoc-in-value", assoc_in_value}},
                         "(dotimes [n 200] (assoc-in! (get lisple-large-mutation-maps n) "
                         "[large-late-miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_in_bang_miss__1500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__assoc_in_bang_miss__1500_iterations",
    {{"native-std-small-mutation-maps", native_small_mutation_maps_1500},
     {"miss-key", miss_key},
     {"assoc-in-value", assoc_in_value}},
    "(dotimes [n 1500] (assoc-in! (get "
    "native-std-small-mutation-maps n) [miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_in_bang_miss__200_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__assoc_in_bang_miss__200_iterations",
                         {{"native-std-large-mutation-maps", native_large_mutation_maps_200},
                          {"miss-key", miss_key},
                          {"assoc-in-value", assoc_in_value}},
                         "(dotimes [n 200] (assoc-in! (get "
                         "native-std-large-mutation-maps n) [miss-key] assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__assoc_in_bang_miss_late__1500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__assoc_in_bang_miss_late__1500_iterations",
    {{"native-std-small-mutation-maps", native_small_mutation_maps_1500},
     {"small-late-miss-key", small_late_miss_key},
     {"assoc-in-value", assoc_in_value}},
    "(dotimes [n 1500] (assoc-in! (get "
    "native-std-small-mutation-maps n) [small-late-miss-key] "
    "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__assoc_in_bang_miss_late__200_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__assoc_in_bang_miss_late__200_iterations",
                         {{"native-std-large-mutation-maps", native_large_mutation_maps_200},
                          {"large-late-miss-key", large_late_miss_key},
                          {"assoc-in-value", assoc_in_value}},
                         "(dotimes [n 200] (assoc-in! (get "
                         "native-std-large-mutation-maps n) [large-late-miss-key] "
                         "assoc-in-value))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__contains_hit__2000_iterations)
{
  auto bm = map_access_benchmark("value_map__small__contains_hit__2000_iterations",
                                 {{"lisple-small", lisple_small_map}, {"hit-key", hit_key}},
                                 "(dotimes [n 2000] (contains? lisple-small hit-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__contains_hit__2000_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__small__contains_hit__2000_iterations",
                         {{"native-std-small", native_small_map}, {"hit-key", hit_key}},
                         "(dotimes [n 2000] (not (nil? (get native-std-small hit-key))))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__contains_hit__500_iterations)
{
  auto bm = map_access_benchmark("value_map__large__contains_hit__500_iterations",
                                 {{"lisple-large", lisple_large_map}, {"hit-key", hit_key}},
                                 "(dotimes [n 500] (contains? lisple-large hit-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__contains_hit__500_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__contains_hit__500_iterations",
                         {{"native-std-large", native_large_map}, {"hit-key", hit_key}},
                         "(dotimes [n 500] (not (nil? (get native-std-large hit-key))))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__contains_hit_late__2000_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__small__contains_hit_late__2000_iterations",
    {{"lisple-small", lisple_small_map}, {"small-late-key", small_late_key}},
    "(dotimes [n 2000] (contains? lisple-small small-late-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__contains_hit_late__2000_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__contains_hit_late__2000_iterations",
    {{"native-std-small", native_small_map}, {"small-late-key", small_late_key}},
    "(dotimes [n 2000] (not (nil? (get native-std-small "
    "small-late-key))))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__contains_hit_late__500_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__large__contains_hit_late__500_iterations",
    {{"lisple-large", lisple_large_map}, {"large-late-key", large_late_key}},
    "(dotimes [n 500] (contains? lisple-large large-late-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__contains_hit_late__500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__large__contains_hit_late__500_iterations",
    {{"native-std-large", native_large_map}, {"large-late-key", large_late_key}},
    "(dotimes [n 500] (not (nil? (get native-std-large "
    "large-late-key))))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__contains_miss__2000_iterations)
{
  auto bm =
    map_access_benchmark("value_map__small__contains_miss__2000_iterations",
                         {{"lisple-small", lisple_small_map}, {"miss-key", miss_key}},
                         "(dotimes [n 2000] (contains? lisple-small miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__contains_miss__2000_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__small__contains_miss__2000_iterations",
                         {{"native-std-small", native_small_map}, {"miss-key", miss_key}},
                         "(dotimes [n 2000] (nil? (get native-std-small miss-key)))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__contains_miss__500_iterations)
{
  auto bm =
    map_access_benchmark("value_map__large__contains_miss__500_iterations",
                         {{"lisple-large", lisple_large_map}, {"miss-key", miss_key}},
                         "(dotimes [n 500] (contains? lisple-large miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__contains_miss__500_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__contains_miss__500_iterations",
                         {{"native-std-large", native_large_map}, {"miss-key", miss_key}},
                         "(dotimes [n 500] (nil? (get native-std-large miss-key)))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__contains_miss_late__2000_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__small__contains_miss_late__2000_iterations",
    {{"lisple-small", lisple_small_map}, {"small-late-miss-key", small_late_miss_key}},
    "(dotimes [n 2000] (contains? lisple-small "
    "small-late-miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__contains_miss_late__2000_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__small__contains_miss_late__2000_iterations",
    {{"native-std-small", native_small_map}, {"small-late-miss-key", small_late_miss_key}},
    "(dotimes [n 2000] (nil? (get native-std-small "
    "small-late-miss-key)))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__contains_miss_late__500_iterations)
{
  auto bm = map_access_benchmark(
    "value_map__large__contains_miss_late__500_iterations",
    {{"lisple-large", lisple_large_map}, {"large-late-miss-key", large_late_miss_key}},
    "(dotimes [n 500] (contains? lisple-large large-late-miss-key))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__contains_miss_late__500_iterations)
{
  auto bm = map_access_benchmark(
    "native_std_map__large__contains_miss_late__500_iterations",
    {{"native-std-large", native_large_map}, {"large-late-miss-key", large_late_miss_key}},
    "(dotimes [n 500] (nil? (get native-std-large "
    "large-late-miss-key)))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__count__2500_iterations)
{
  auto bm = map_access_benchmark("value_map__small__count__2500_iterations",
                                 {{"lisple-small", lisple_small_map}},
                                 "(dotimes [n 2500] (count lisple-small))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__count__2500_iterations)
{
  auto bm = map_access_benchmark("native_std_map__small__count__2500_iterations",
                                 {{"native-std-small", native_small_map}},
                                 "(dotimes [n 2500] (count native-std-small))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__count__400_iterations)
{
  auto bm = map_access_benchmark("value_map__large__count__400_iterations",
                                 {{"lisple-large", lisple_large_map}},
                                 "(dotimes [n 400] (count lisple-large))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__count__400_iterations)
{
  auto bm = map_access_benchmark("native_std_map__large__count__400_iterations",
                                 {{"native-std-large", native_large_map}},
                                 "(dotimes [n 400] (count native-std-large))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__small__full_traversal__1200_iterations)
{
  auto bm =
    map_access_benchmark("value_map__small__full_traversal__1200_iterations",
                         {{"lisple-small", lisple_small_map}},
                         "(dotimes [n 1200] (reduce-kv lisple-small 0 (fn [acc k v] (+ "
                         "acc 1))))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__small__full_traversal__1200_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__small__full_traversal__1200_iterations",
                         {{"native-std-small", native_small_map}},
                         "(dotimes [n 1200] (reduce-kv native-std-small 0 (fn [acc k v] "
                         "(+ acc 1))))");

  bm.run();
}

TEST(Benchmark_MapAccess, value_map__large__full_traversal__80_iterations)
{
  auto bm = map_access_benchmark("value_map__large__full_traversal__80_iterations",
                                 {{"lisple-large", lisple_large_map}},
                                 "(dotimes [n 80] (reduce-kv lisple-large 0 (fn [acc k "
                                 "v] (+ acc 1))))");

  bm.run();
}

TEST(Benchmark_MapAccess, native_std_map__large__full_traversal__80_iterations)
{
  auto bm =
    map_access_benchmark("native_std_map__large__full_traversal__80_iterations",
                         {{"native-std-large", native_large_map}},
                         "(dotimes [n 80] (reduce-kv native-std-large 0 (fn [acc k v] (+ "
                         "acc 1))))");

  bm.run();
}
