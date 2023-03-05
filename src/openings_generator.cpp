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
#include <thread>
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

#define MAX_TREE_DEPTH 3
#define DEPTH_ALPHABETA 7


int main() {
    bool white = true;
    bool black = true;
    if (white) {
        OpeningsGenerator og_white;
        og_white.loadOpenings();

        std::ofstream outFile("./raw_data/openings.dat", std::ios_base::app);
        og_white.generateOpeningsWhite(MAX_TREE_DEPTH, &outFile);
        outFile.close();
    };
    if (black) {
        OpeningsGenerator og_black;
        og_black.loadOpenings();

        std::ofstream outFile("./raw_data/openings.dat", std::ios_base::app);
        og_black.generateOpeningsBlack(MAX_TREE_DEPTH + 1, &outFile);
        outFile.close();
    };

    //std::thread white(white_openings);
    //std::thread black(black_openings);

    //white.join();
    //black.join();


    return 0;
}

void OpeningsGenerator::generateOpeningsWhite(int depth, std::ofstream *outFile) {
    if (depth == 0)
        return;

    uint_fast64_t move_0 = getMove64(DEPTH_ALPHABETA);
    if (!(this->opening_.find(bit_boards_) != opening_.end())) {
        opening_[bit_boards_] = move_0;
        *outFile << std::hex << bit_boards_.White << " " << std::hex << bit_boards_.Black << " " << std::hex << move_0
                 << std::endl;
    }
    moveWithoutVerification(move_0);

    std::cout << "White : " << this->opening_.size() << "\n";
    std::vector<uint_fast64_t> moves_1;
    availableMoves(moves_1);
    for (const auto &move_1 : moves_1) {
        moveWithoutVerification(move_1);
        if (!this->isPositionIllegal())
            generateOpeningsWhite(depth - 1, outFile);
        reverseMove(move_1);
    }

    reverseMove(move_0);
}

void OpeningsGenerator::generateOpeningsBlack(int depth, std::ofstream *outFile) {
    if (depth == 0)
        return;

    uint_fast64_t move_1;
    if (depth != MAX_TREE_DEPTH + 1) {
        move_1 = this->getMove64(DEPTH_ALPHABETA);

        if (!(this->opening_.find(bit_boards_) != opening_.end())) {
            this->opening_[bit_boards_] = move_1;
            *outFile << std::hex << bit_boards_.White << " " << std::hex << bit_boards_.Black << " " << std::hex << move_1
                     << std::endl;
        }

        this->moveWithoutVerification(move_1);
    }

    std::cout << "Black : " << this->opening_.size() << "\n";

    std::vector<uint_fast64_t> moves_0;
    this->availableMoves(moves_0);
    for (const auto &move_0 : moves_0) {
        this->moveWithoutVerification(move_0);

        if (!this->isPositionIllegal())
            this->generateOpeningsBlack(depth - 1, outFile);

        this->reverseMove(move_0);
    }

    if (depth != MAX_TREE_DEPTH + 1) this->reverseMove(move_1);
}