/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file intuition_data_generator.cpp
 * \brief
 *
 * This class is used to generate the data required for the intuition learning process
 *
 */

/* intuition_data_generator.hpp */
#include "intuition_data_generator.hpp"

/* C libraries */
#include <time.h>
#include <stdlib.h>

/* C++ libraries */
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
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

#define DEPTH (1)
#define NUMBER_OF_GAMES (500)

typedef std::vector<std::pair<std::string, tensorflow::Tensor>> tensor_dict;

int main() {
    std::vector<uint_fast64_t> moves;
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    for (int i = 0; i < NUMBER_OF_GAMES; ++i) {
        std::cout << std::setw(5) << 100 * static_cast<double>(i)/NUMBER_OF_GAMES << "%\n";
        IntuitionDataGenerator generator;
        generator.fillTransTable();
        srand(time(NULL));
        while (generator.stateOfGame() == NotFinished) {
            auto result = generator.evalAllMoves(DEPTH);

            generator.saveVectorToFile(result.second, "./raw_data/values.dat");
            generator.saveVectorOfBitBoardsToFiles(result.first, "./raw_data/white.dat", "./raw_data/black.dat");


            if (rand() % 10 <= 7) {
                generator.moveWithoutVerification(generator.getMove64(DEPTH));
            } else {
                moves.clear();
                generator.availableMoves(moves);
                generator.moveWithoutVerification(
                        moves[rand() % static_cast<int>(moves.size())]);
            }

            if (rand() % 10 <= 7) {
                generator.moveWithoutVerification(generator.getMove64(DEPTH));
            } else {
                moves.clear();
                generator.availableMoves(moves);
                generator.moveWithoutVerification(
                        moves[rand() % static_cast<int>(moves.size())]);
            }
        }
    }
    return 0;
}

template<typename T>
void IntuitionDataGenerator::saveVectorToFile(
        const std::vector<T> &input,
        const std::string &outputFileName) {
    std::ofstream output_file(outputFileName, std::ios_base::app);

    std::ostream_iterator<T> output_iterator(output_file, "\n");
    std::copy(std::begin(input), std::end(input), output_iterator);

    output_file.close();
}

void IntuitionDataGenerator::saveVectorOfBitBoardsToFiles(
        const std::vector<bitBoards_t> &input,
        const std::string &outputFileNameW,
        const std::string &outputFileNameB) {
    std::ofstream output_fileW(outputFileNameW, std::ios_base::app);
    std::ofstream output_fileB(outputFileNameB, std::ios_base::app);

    for (const auto &bb : input) {
        output_fileW << std::hex << bb.White << "\n";
        output_fileB << std::hex << bb.Black << "\n";
    }

    output_fileW.close();
    output_fileB.close();
}

std::pair<std::vector<bitBoards_t>, std::vector<double>>
                IntuitionDataGenerator::evalAllMoves(int depth) {
    maximizing_player_ = who_is_to_play_;
    fullDepth_         = depth;

    std::vector<uint_fast64_t> moves;
    availableMoves(moves);

    std::vector<double> evals;
    std::vector<bitBoards_t> all_bit_boards;
    double buff;
    for (const uint_fast64_t &move : moves) {
        transposition_table_.clear();
        /* Apply the move */
        this->moveWithoutVerification(move);
        heuristic_value_   = heuristicValue();

        /* Check if we already have informations about this position */
        if (transposition_table_permanent_.find(bit_boards_)
                != transposition_table_permanent_.end()) {
            --number_of_times_seen_[zobrist_hash_];
            who_is_to_play_ ^= 1;
            who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;
            zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][move];
            continue;
        }

        if (this->isPositionIllegal()) {
            /* cancel the move */
            who_is_to_play_ ^= 1;
            who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;
            zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][move];
            continue;
        }

        /* evaluate the move */
        buff = AlphaBetaEval(depth - 1,
                             -20,
                             20,
                             true,
                             false,
                             zobrist_hash_);

        all_bit_boards.push_back(bit_boards_);
        evals.push_back(buff);

        /* cancel the move */
        who_is_to_play_ ^= 1;
        who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;
        zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][move];
    }

    return std::make_pair(all_bit_boards, evals);
}

void IntuitionDataGenerator::fillTransTable() {
    bitBoards_t bb;
    double value;

    std::ifstream is_w("./raw_data/white.dat");
    std::ifstream is_b("./raw_data/black.dat");
    std::ifstream is_v("./raw_data/values.dat");

    while(is_w >> std::hex >> bb.White
       && is_b >> std::hex >> bb.Black
       && is_v >>std::hex >>  value)
        transposition_table_permanent_[bb] = value;


    is_w.close();
    is_b.close();
    is_v.close();
}
