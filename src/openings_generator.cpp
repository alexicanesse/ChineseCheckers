/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file openings_generator.cpp
 * \brief
 *
 *
 *
 */

/* openings_generator.hpp */
#include "openings_generator.hpp"

/* C++ libraries */
#include <unordered_map>
#include <fstream>
#include <sstream>
/* The following pragma are used to removed deprecation warning from boost
 * header files. Using them avoid to remove this warning from the entire project.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/python.hpp>
#pragma GCC diagnostic pop

/* Other */
#include "Types.hpp"
#include "AlphaBeta.hpp"

#define MAX_TREE_DEPTH 4
#define DEPTH_ALPHABETA 3


int main() {
    OpeningsGenerator og;
    og.loadOpenings();
    og.generateOpenings(MAX_TREE_DEPTH);
    og.saveOpenings();
    return 0;
}

void OpeningsGenerator::generateOpenings(int depth) {
    if (depth == 0)
        return;

    const auto move_0 = this->getMove(DEPTH_ALPHABETA, -20, 20);
    if (!this->opening.contains(this->hashGrid()))
        this->opening[this->hashGrid()] = move_0;
    this->moveWithoutVerification(0, move_0);

    std::cout << this->opening.size() << "\n";

    ListOfMoves moves_1 = this->availableMoves(1, true);
    for (const auto &move_1: moves_1) {
        this->moveWithoutVerification(1, move_1);
        this->generateOpenings(depth - 1);
        this->reverseMove(move_1);
    }

    this->reverseMove(move_0);
}

void OpeningsGenerator::saveOpenings() {
    std::ofstream outFile("openings.dat");

    /* Iterate through the map and save each element to the file */
    for(const auto &it : this->opening) {
        outFile << it.first;
        for (const auto &val : it.second)
            outFile << " " << val[0] << " " << val[1];
        outFile << std::endl;
    }

    /* Close the file */
    outFile.close();
}