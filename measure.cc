#include <benchmark/benchmark.h>

#include "MagField.h"
#include "make_VMagField.h"

static void UnorderedMap_value(benchmark::State& state) {

  auto mf = make_vmagfield();

  Point p;
  for(auto _ : state) {
    mf->valueUnorderedMap(p);
  }
}

BENCHMARK(UnorderedMap_value);

static void TLS_value(benchmark::State& state) {

  auto mf = make_vmagfield();

  Point p;
  for(auto _ : state) {
    mf->value(p);
  }
}

BENCHMARK(TLS_value);


static void Cache_value(benchmark::State& state) {

  auto mf = make_vmagfield();

  Cache c = mf->nominalCache();
  Point p;
  for(auto _ : state) {
    mf->value(p,c);
  }
}

BENCHMARK(Cache_value);

BENCHMARK_MAIN();
