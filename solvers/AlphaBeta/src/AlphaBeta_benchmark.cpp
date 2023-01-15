/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file AlphaBeta_benchmark.cpp
 * \brief Benchmarks of AlphaBeta
 */


/* C libraries */
#include <benchmark/benchmark.h>

/* C++ libraries */
#include <vector>
#include <algorithm>

/* Other */
#include "ChineseCheckers.hpp"
#include "AlphaBeta.hpp"
#include "Types.hpp"


static void BM_GetMoveD3(benchmark::State &state) {
    // Perform setup here
    AlphaBeta ab;
    for (int i = 0; i < state.range(0); ++i)
        ab.move(i & 0x1, ab.getMove(3, -1000000, 1000000));


    for (auto _ : state) {
        // This code gets timed
        ab.getMove(3, -1000000, 1000000);
    }
}

// Register the function as a benchmark
BENCHMARK(BM_GetMoveD3)->Arg(0)->Arg(1)->Arg(2)->Arg(5)->Arg(10)->Arg(15)->Arg(20)->Unit(benchmark::kMillisecond);

// Run the benchmark
BENCHMARK_MAIN();
