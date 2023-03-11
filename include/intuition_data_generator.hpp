/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * @file intuition_data_generator.hpp
 * @brief
 *
 * This class is used to generate the data required for the intuition learning process
 *
 */

#ifndef INCLUDE_INTUITION_DATA_GENERATOR_HPP_
#define INCLUDE_INTUITION_DATA_GENERATOR_HPP_

/* C++ libraries */
#include <vector>
#include <string>
#include <utility>
#include <boost/unordered_map.hpp>
/* The following pragma are used to removed depraction warning from boost
 * header files. Using them avoid to remove this warning from the entire project.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/python.hpp>
#pragma GCC diagnostic pop

/* Other */
#include "Types.hpp"
#include "AlphaBeta.hpp"

/*!
 * @brief This class is used to generate the data required for the intuition learning process.
 */
class IntuitionDataGenerator : public AlphaBeta {
 protected:
    boost::unordered_map<bitBoards_t, uint_fast64_t, bitBoardsHasher, bitBoardsEqual> transposition_table_permanent_;
 public:
    /*!
     * @details this function is used to eval all possible moves.
     * @param depth The depth used by the alpha beta algorithm.
     * @return The evaluation associated with each move and the grid obtained after each move.
     */
    std::pair<std::vector<bitBoards_t>, std::vector<double>>
                                                evalAllMoves(int depth);

    /*!
     * @details Save a vector to a file.
     * @tparam T
     * @param input The vector to save.
     * @param outputFileName The name of the file we need to save the data to.
     */
    template<typename T>
    void saveVectorToFile(const std::vector<T> &input,
                          const std::string &outputFileName);

    /*!
     * @details Save a matrix to a file.
     * @param input The matrix to save.
     * @param outputFileName The name of the file we need to save the data to.
     */
    void saveVectorOfBitBoardsToFiles(const std::vector<bitBoards_t> &input,
                                      const std::string &outputFileNameW,
                                      const std::string &outputFileNameB);

    /*!
     * @details Fill the @ref transposition_table_ with the data we already calculated.
     */
    void fillTransTable();
};

#endif  // INCLUDE_INTUITION_DATA_GENERATOR_HPP_
