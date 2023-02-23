/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file openings_generator.cpp
 * \brief
 *
 * This class is used in order to generate the openings
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

#define MAX_TREE_DEPTH 2
#define DEPTH_ALPHABETA 5


int main() {
    OpeningsGenerator og;
    og.loadOpenings();

    std::ofstream outFile("./raw_data/openings.dat", std::ios_base::app);
    // og.generateOpeningsWhite(MAX_TREE_DEPTH, &outFile);
    og.generateOpeningsBlack(MAX_TREE_DEPTH + 1, &outFile);

    outFile.close();
    return 0;
}

void OpeningsGenerator::generateOpeningsWhite(int depth, std::ofstream *outFile) {
    if (depth == 0)
        return;

    const auto move_0 = this->getMove(DEPTH_ALPHABETA, -20, 20);
    if (!this->opening.contains(this->hashGrid())) {
        this->opening[this->hashGrid()] = move_0;

        *outFile << this->hashGrid();
        for (const auto &val : move_0)
            *outFile << " " << val[0] << " " << val[1];
        *outFile << std::endl;
    }

    this->moveWithoutVerification(0, move_0);

    std::cout << this->opening.size() << "\n";
    ListOfMoves moves_1 = this->availableMoves(1, true);
    for (const auto &move_1 : moves_1) {
        this->moveWithoutVerification(1, move_1);
        this->generateOpeningsWhite(depth - 1, outFile);
        this->reverseMove(move_1);
    }

    this->reverseMove(move_0);
}

void OpeningsGenerator::generateOpeningsBlack(int depth, std::ofstream *outFile) {
    if (depth == 0)
        return;

    ListOfPositionType move_1;
    if (depth != MAX_TREE_DEPTH + 1) {
        move_1 = this->getMove(DEPTH_ALPHABETA, -20, 20);

        if (!this->opening.contains(this->hashGrid())) {
            this->opening[this->hashGrid()] = move_1;

            *outFile << this->hashGrid();
            for (const auto &val: move_1)
                *outFile << " " << val[0] << " " << val[1];
            *outFile << std::endl;
        }

        this->moveWithoutVerification(1, move_1);
    }

    std::cout << this->opening.size() << "\n";

    ListOfMoves moves_0 = this->availableMoves(0, true);
    for (const auto &move_0 : moves_0) {
        this->moveWithoutVerification(0, move_0);

        if (!this->isPositionIllegal())
            this->generateOpeningsBlack(depth - 1, outFile);

        this->reverseMove(move_0);
    }

    if (depth != MAX_TREE_DEPTH + 1) this->reverseMove(move_1);
}

