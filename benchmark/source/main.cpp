#include <benchmark/benchmark.h>

#include <algorithm>
#include <limits>
#include <random>
#include <vector>

#include "fluentiter/fluentiter.h"

static std::vector<int> generateVector() {
  static int SIZE = 1000;
  static std::mt19937 generator;

  std::uniform_int_distribution<int> distribution(std::numeric_limits<int>::min(),
                                                  std::numeric_limits<int>::max());

  std::vector<int> data;
  data.reserve(SIZE);
  for (auto i = 0; i < SIZE; i++) {
    data.push_back(distribution(generator));
  }

  return data;
}

static void BM_map_ranged_for_loop(benchmark::State& state) {
  auto vec = generateVector();

  for (auto _ : state) {
    std::vector<int> output;
    output.reserve(vec.size());

    for (const auto& elem : vec) output.push_back(elem + elem);
  }
}
BENCHMARK(BM_map_ranged_for_loop);

static void BM_Map_std_transform(benchmark::State& state) {
  auto vec = generateVector();

  for (auto _ : state) {
    std::vector<int> output;
    output.reserve(vec.size());

    std::transform(vec.begin(), vec.end(), std::back_inserter(output),
                   [](const auto& elem) { return elem + elem; });
  }
}
BENCHMARK(BM_Map_std_transform);

static void BM_Map_fluent_iter(benchmark::State& state) {
  auto vec = generateVector();

  for (auto _ : state) {
    auto output = fluentiter::from(vec.begin(), vec.end())
                      .map([](const auto& elem) { return elem + elem; })
                      .collect<fluentiter::VectorCollector<int>, std::vector<int>>();
  }
}
BENCHMARK(BM_Map_fluent_iter);

BENCHMARK_MAIN();