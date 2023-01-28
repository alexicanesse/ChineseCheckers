/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file intuition_data_generator.hpp
 * \brief
 *
 *
 *
 */

#ifndef CHINESECHECKERS_INTUITION_DATA_GENERATOR_HPP
#define CHINESECHECKERS_INTUITION_DATA_GENERATOR_HPP

/* C++ libraries */
#include <vector>
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

class IntuitionDataGenerator : public AlphaBeta {
  public:
    std::pair<std::vector<std::vector<int>>, std::vector<double>> evalAllMoves(int depth);

    template<typename T>
    void saveVectorToFile(const std::vector<T> &input,
                                                  const std::string &outputFileName);
    template<typename T>
    void saveVectorOfVectorToFile(const std::vector<std::vector<T>> &input, const std::string &outputFileName);

    void fillTransTable();
};

#endif //CHINESECHECKERS_INTUITION_DATA_GENERATOR_HPP
