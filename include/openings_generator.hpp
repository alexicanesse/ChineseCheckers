/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file openings_generator.hpp
 * \brief
 *
 *
 *
 */

#ifndef CHINESECHECKERS_OPENINGS_GENERATOR_HPP
#define CHINESECHECKERS_OPENINGS_GENERATOR_HPP

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

class OpeningsGenerator : public AlphaBeta {
  private:

  public:
    void generateOpenings(int depth);
    void saveOpenings();
};


#endif //CHINESECHECKERS_OPENINGS_GENERATOR_HPP