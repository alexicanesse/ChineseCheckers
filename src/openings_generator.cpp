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

#define MAX_TREE_DEPTH 2
#define DEPTH_ALPHABETA 5
#define TREE_WIDTH (10UL)


int main() {
    auto black_openings = []() {
        OpeningsGenerator og_black;
        og_black.loadOpenings();

        std::ofstream outFile("./raw_data/openings_black.dat", std::ios_base::app);
        og_black.generateOpeningsBlack(MAX_TREE_DEPTH + 1, &outFile);
        outFile.close();
    };

    std::thread black(black_openings);

    int depth = MAX_TREE_DEPTH;
    while (black.joinable()) {
        auto white_openings = [depth]() {
            OpeningsGenerator og_white;
            og_white.loadOpenings();

            std::ofstream outFile("./raw_data/openings_white.dat", std::ios_base::app);
            og_white.generateOpeningsWhite(depth, &outFile);
            outFile.close();
        };
        std::thread white(white_openings);
        white.join();
        depth += 1;
    }
    black.join();

    return 0;
}

void OpeningsGenerator::generateOpeningsWhite(int depth, std::ofstream *outFile) {
    if (depth == 0)
        return;

    uint_fast64_t move_0;
    if (!(this->opening_[0].find(bit_boards_) != opening_[0].end())) {
        move_0 = getMove64(DEPTH_ALPHABETA);
        opening_[0][bit_boards_] = move_0;
        *outFile << std::hex
                 << bit_boards_.White
                 << " "
                 << std::hex
                 << bit_boards_.Black
                 << " "
                 << std::hex
                 << move_0
                 << std::endl;
    } else {
        move_0 = opening_[0][bit_boards_];
    }
    moveWithoutVerification(move_0);

    std::cout << "White : " << this->opening_[0].size() << "\n";
    std::vector<uint_fast64_t> moves_1;
    availableMoves(moves_1);
    std::sort(moves_1.begin(), moves_1.end(), comp_move_);
    moves_1.resize(std::min(TREE_WIDTH, moves_1.size()));

    for (const auto &move_1 : moves_1) {
        moveWithoutVerification(move_1);
        if (!this->isPositionIllegal())
            generateOpeningsWhite(depth - 1, outFile);

        --number_of_times_seen_[zobrist_hash_];
        who_is_to_play_ ^= 1;
        who_is_to_play_ ? bit_boards_.Black ^= move_1 : bit_boards_.White ^= move_1;
        zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][move_1];
    }

    --number_of_times_seen_[zobrist_hash_];
    who_is_to_play_ ^= 1;
    who_is_to_play_ ? bit_boards_.Black ^= move_0 : bit_boards_.White ^= move_0;
    zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][move_0];
}

void OpeningsGenerator::generateOpeningsBlack(int depth, std::ofstream *outFile) {
    if (depth == 0)
        return;

    uint_fast64_t move_1;
    if (depth != MAX_TREE_DEPTH + 1) {
        if (!(this->opening_[1].find(bit_boards_) != opening_[0].end())) {
            move_1 = this->getMove64(DEPTH_ALPHABETA);
            this->opening_[1][bit_boards_] = move_1;
            *outFile << std::hex
                     << bit_boards_.White
                     << " "
                     << std::hex
                     << bit_boards_.Black
                     << " "
                     << std::hex
                     << move_1
                     << std::endl;
        } else {
            move_1 = this->opening_[1][bit_boards_];
        }

        this->moveWithoutVerification(move_1);
    }

    std::cout << "Black : " << this->opening_[1].size() << "\n";

    std::vector<uint_fast64_t> moves_0;
    this->availableMoves(moves_0);
    std::sort(moves_0.begin(), moves_0.end(), comp_move_);
    moves_0.resize(std::min(TREE_WIDTH, moves_0.size()));

    for (const auto &move_0 : moves_0) {
        this->moveWithoutVerification(move_0);

        if (!this->isPositionIllegal())
            this->generateOpeningsBlack(depth - 1, outFile);

        --number_of_times_seen_[zobrist_hash_];
        who_is_to_play_ ^= 1;
        who_is_to_play_ ? bit_boards_.Black ^= move_0 : bit_boards_.White ^= move_0;
        zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][move_0];
    }

    if (depth != MAX_TREE_DEPTH + 1) {
        --number_of_times_seen_[zobrist_hash_];
        who_is_to_play_ ^= 1;
        who_is_to_play_ ? bit_boards_.Black ^= move_1 : bit_boards_.White ^= move_1;
        zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][move_1];
    }
}