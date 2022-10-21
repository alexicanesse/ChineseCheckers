/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file Types.hpp
 * \brief Types definition
 *
 * Definition of some types. This files should be included in any file that attend to implement the game or a solver.
 *
 */

#ifndef INCLUDE_TYPES_HPP_
#define INCLUDE_TYPES_HPP_


#include <vector>

/*! \enum Color
    \brief Used to denote the colors.
*/
enum Color {Empty, White, Black};

/*! \enum Movetype
    \brief Used to denote the move types (Illegal, Jump and notJump).
*/

enum MoveType { Illegal, Jump, notJump};

/*! \typedef Player
    \brief Used to denote the players.
*/
typedef int Player;

/*! \typedef GridType
    \brief Used to denote the grid.
*/
typedef std::vector<std::vector<Color>> GridType;

/*! \typedef PositionType
    \brief Used to denote a position.
*/
typedef std::vector<int> PositionType;

/*! \typedef ListOfPositionType
    \brief Used to denote a vector of positions.
 */
typedef std::vector<PositionType> ListOfPositionType;



#endif /* INCLUDE_TYPES_HPP_ */
