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

#define DEPTH (3)

typedef std::vector<std::pair<std::string, tensorflow::Tensor>> tensor_dict;

int main() {
    for (int i = 0; i < 100; ++i) {
        std::cout << i << "\n";
        IntuitionDataGenerator generator;
        generator.fillTransTable();
        srand(time(NULL));
        while (generator.state_of_game() == NotFinished) {
            auto result = generator.evalAllMoves(3);

            generator.saveVectorToFile(result.second, "values.txt");
            generator.saveVectorOfVectorToFile(result.first, "grids.txt");


            if (rand() % 10 <= 7) {
                generator.moveWithoutVerification(generator.getMove64(DEPTH));
            } else {
                std::vector<uint_fast64_t> moves;
                generator.availableMoves(moves);
                generator.move(0, moves[rand()
                        % static_cast<int>(moves.size())]);
            }

            if (rand() % 10 <= 7) {
                generator.move(1, generator.getMove(3, -100000, 100000));
            } else {
                ListOfMoves moves = generator.availableMoves(1, false);
                generator.move(1, moves[rand()
                        % static_cast<int>(moves.size())]);
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

template<typename T>
void IntuitionDataGenerator::saveVectorOfVectorToFile(
        const std::vector<std::vector<T>> &input,
        const std::string &outputFileName) {
    std::ofstream output_file(outputFileName, std::ios_base::app);

    for (const auto &line : input) {
        std::ostream_iterator <T> output_iterator(output_file, " ");
        std::copy(std::begin(line), std::end(line), output_iterator);
        output_file << "\n";
    }
    output_file.close();
}

std::pair<std::vector<std::vector<int>>, std::vector<double>>
                IntuitionDataGenerator::evalAllMoves(int depth) {
    this->maximizing_player_ = this->who_is_to_play_;
    ListOfMoves moves = this->availableMoves(this->who_is_to_play_, false);
    std::vector<double> evals;
    std::vector<std::vector<int>> flatten_grids;

    for (const ListOfPositionType &move : moves) {
        /* Apply the move */
        this->moveWithoutVerification(this->who_is_to_play_, move);

        if (this->transTable.contains(this->hashMatrix(this->grid_, 1))) {
            this->reverseMove(move);
            continue;
        }

        /* evaluate the move */
        double buff = AlphaBetaEval(depth - 1,
                                 -100000,
                                 100000,
                                 true,
                                 true);

        std::vector<int> flatten_grid;
        for (const auto &line : this->get_grid_()) {
            for (const auto &val : line)
                flatten_grid.push_back(val);
        }
        flatten_grids.push_back(flatten_grid);
        evals.push_back(buff);

        std::cout << buff << "\n";

        /* cancel the move */
        this->reverseMove(move);
    }

    return std::make_pair(flatten_grids, evals);
}

void IntuitionDataGenerator::fillTransTable() {
    std::ifstream is_w("white.dat");
    std::istream_iterator<uint_fast64_t> start_w(is_w), end_w;
    std::vector<uint_fast64_t> white(start_w, end_w);
    is_w.close();

    std::ifstream is_b("black.dat");
    std::istream_iterator<uint_fast64_t> start_b(is_b), end_b;
    std::vector<uint_fast64_t> black(start_b, end_b);
    is_b.close();

    std::ifstream is("values.dat");
    std::istream_iterator<double> start(is), end;
    std::vector<double> values(start, end);
    is.close();

    for (int i = 0; i < white.size(); ++i)
        transposition_table_[{white[i], black[i]}] = {values[i], DEPTH - 1};
}
