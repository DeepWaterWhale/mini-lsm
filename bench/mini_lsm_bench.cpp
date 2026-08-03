#include <benchmark/benchmark.h>

#include "mini_lsm_app.hpp"

static void BM_Run(benchmark::State &state) {
  for (auto iteration : state) {
    benchmark::DoNotOptimize(iteration);
    benchmark::DoNotOptimize(mini_lsm::run());
  }
}

BENCHMARK(BM_Run);
BENCHMARK_MAIN();
