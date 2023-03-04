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
#include <boost/functional/hash.hpp>

/* The number of time a grid state can be seen before settling for a draw */
#define MAX_NUMBER_OF_CYCLES_FOR_DRAW_ 3

/*! \cond DO_NOT_DOCUMENT */
struct bitBoards_s {
    uint_fast64_t White;
    uint_fast64_t Black;
};
typedef struct bitBoards_s bitBoards_t;

struct bitBoardsHasher {
    std::size_t operator()(const bitBoards_t& b) const {
        return boost::hash<uint_fast64_t>{}(b.White) ^ (std::hash<uint_fast64_t>{}(b.Black) << 1);
    }
};

struct bitBoardsEqual {
    bool operator()(const bitBoards_t& b1, const bitBoards_t& b2) const {
        return (b1.White == b2.White) && (b1.Black == b2.Black);
    }
};
/*! \endcond */

/*! \enum Color
    \brief Used to denote the colors.
*/
enum Color {Empty = 0, White = 1, Black = -1};

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
