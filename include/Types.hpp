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

/* The number of time a grid state can be seen before settling for a draw */
#define MAX_NUMBER_OF_CYCLES_FOR_DRAW_ 3

/*! \enum Color
    \brief Used to denote the colors.
*/
enum Color {Empty, White, Black};

/*! \enum Result
    \brief Used to return the current state of a game
 */
enum Result { NotFinished, Draw, WhiteWon, BlackWon};

/*! \enum MoveType
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

/*! \typedef ListOfMoves
    \brief Used to denote a list of moves
 */
typedef std::vector<ListOfPositionType> ListOfMoves;




#endif /* INCLUDE_TYPES_HPP_ */
