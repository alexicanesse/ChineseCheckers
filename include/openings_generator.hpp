/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file openings_generator.hpp
 * \brief
 *
 * This file is used for the declarations needed to generate the openings
 *
 */

#ifndef INCLUDE_OPENINGS_GENERATOR_HPP_
#define INCLUDE_OPENINGS_GENERATOR_HPP_

/* C++ libraries */
#include <unordered_map>
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
 * @brief
 * This class is used in order to generate the openings
 */
class OpeningsGenerator : public AlphaBeta {
 private:
 public:
    /*!
     * @brief Generates the openings up to a given depth
     * @param depth Defines the length of the openings we will compute
     * @sa saveOpenings()
     */
    void generateOpenings(int depth);

    /*!
     * @brief Saves the openings generated using @ref generateOpenings(int depth)
     * @sa generateOpenings(int depth)
     */
    void saveOpenings();
};


#endif  // INCLUDE_OPENINGS_GENERATOR_HPP_
