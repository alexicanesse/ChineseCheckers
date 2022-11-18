/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file ChineseCheckers_benchmark.cpp
 * \brief Benchmarks of the core implementation of the game
 */

/* ChineseCheckers.hpp */
#include "ChineseCheckers.hpp"

/* C libraries */
#include <benchmark/benchmark.h>

/* C++ libraries */
#include <vector>
#include <algorithm>

/* Other */
#include <Types.hpp>


static void BM_SomeFunction(benchmark::State& state) {
    // Perform setup here
    std::vector<int> test(10000, 0);


    for (auto _ : state) {
        // This code gets timed
        for (int i = 0; i < 10000; ++i) {
            test[i] = 1 - i;
        }
    }
}

// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();