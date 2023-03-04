/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file ChineseCheckers.hpp
 * \brief Core implementation of the game
 *
 * Declaration of the ChineseCheckers Class which is the core of the game
 *
 */

#ifndef INCLUDE_CHINESECHECKERS_HPP_
#define INCLUDE_CHINESECHECKERS_HPP_

/* C++ libraries */
#include <vector>
#include <array>
#include <iostream>
#include <utility>
#include <map>
#include <unordered_map>
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

/*!
 * @class ChineseCheckers
 * @brief This class represents a Chinese Checkers game
 * The ChineseCheckers class is responsible for representing the game of Chinese Checkers
 * and for providing functionalities such as checking if a move is legal, executing moves,
 * printing the game state, returning the current state of the game, and creating a new game.
 * It also stores information about the grid, the number of times a position has been seen,
 * the winning positions, and the illegal positions.
*/
class ChineseCheckers {
 protected:
    /*! Basic object used to create bitmasks efficiently. */
    uint_fast64_t un_64_ = static_cast<uint_fast64_t>(1);
    /*! Indicate which is the next player to play. */
    Player who_is_to_play_ = 0;
    /*! A data structure containing the grid. */
    bitBoards_t bit_boards_;
    /*!
     * Indicates the number of times a position has been seen.
     * It is used to check for draws.
     * */
    boost::unordered_map<uint64_t , int> number_of_times_seen_;
    /*! Keeps the positions of the white winning zone. */
    const uint_fast64_t winning_positions_white_ = 0xF0E0C08000000000;
    /*! Keeps the positions of the black winning zone. */
    const uint_fast64_t winning_positions_black_ = 0x000000000103070F;
    /*! Stores the illegal positions. */
    boost::unordered_map<uint32_t, bool> illegal_positions_;
    /*! Stores results of cantor pairing to make the check for illegal positions faster. */
    std::array<std::array<uint32_t, 8>, 8> cantor_pairing_;
    /* Stores the valid directions to find if a move is valid faster. */
    std::vector<std::vector<int>> valid_lines = {{-1,  0}, {-1,  1}, {0 , -1},
                                                 {0 ,  1}, {1 , -1}, {1 ,  0}};
    /* Stores the valid directions to find if a position is legal faster. */
    std::vector<std::vector<int>> valid_lines_illegal = {{-1,  0}, {-1,  1}, {0 , -1},
                                                         {0 ,  1}, {1 , -1}, {1 ,  0},
                                                         {-2,  0}, {-2,  2}, {0 , -2},
                                                         {0 ,  2}, {2 , -2}, {2 ,  0}};
    /*! Stores the conversion of a bit-wise position to a pair of index. */
    boost::unordered_map<uint_fast64_t, std::pair<int, int>> uint64_to_pair_;
    /* !Stores the conversion of a pair of index to a bit-wise position. */
    std::array<std::array<uint_fast64_t, 8>, 8> int_to_uint64_;
    /*! Stores the neighbours' position of all pawns. */
    std::unordered_map<uint_fast64_t, std::vector<uint_fast64_t>> direct_neighbours_;
    /*! Stores all required bitMasks to find all jumps from a given pawn. */
    std::unordered_map<uint_fast64_t, std::vector<
        std::vector<std::pair<std::pair<uint_fast64_t,
        uint_fast64_t>, uint_fast64_t> > > > k_neighbours_;

    /*! @brief
     * A member returning the type of an elementary move
     * (not a succession of jumps).
     * @param original_position The position.
     * @param arrival_position The arrival position.
     * @retval Illegal if the elementary move realized was of illegal.
     * @retval Jump if the elementary move realized was a jump.
     * @retval notJump if the elementary move realized was not a jump.
     * @sa move
     */
    MoveType elementaryMove(const PositionType &original_position,
                            const PositionType &arrival_position);
    /*!
     * @brief Indicates whether the position is illegal or not.
     * @return Returns true iff the position is illegal.
     * @sa cantorPairingFunction
     * @sa loadIllegalPositions
     */
    bool isPositionIllegal();

    /*!
     * @brief Cantor's pairing function.
     * @param x first argument.
     * @param y second argument.
     * @return cantor's pairing function applied to `x` and `y`.
     * @sa isPositionIllegal
     * @sa loadIllegalPositions
     */
    int cantorPairingFunction(const int &x, const int &y);

    /*!
     * @brief Loads illegal positions.
     * @sa isPositionIllegal
     * @sa cantorPairingFunction
     */
    void loadIllegalPositions();
 public:
    /*! @brief
     * A simple constructor.
     * @sa newGame
     */
    ChineseCheckers();
    /*! @brief
    * Creates a new game.
    */
    void newGame();

    /*! @brief
     * This member checks if a move is legal and executes it if it is legal.
     * @param player indicates which player made the move.
     * @param list_moves contains the list of intermediate positions.
                                of a move (including the starting point
                                and the arrival point).
     * @retval true if the move was legal.
     * @retval false if the move was not legal.
     * @sa MoveType elementaryMove
     */
    bool move(const Player &player,
              const ListOfPositionType &list_moves);
    /*! @brief
     * Indicates the current state of the game
     * (ie, not finished, black won, white won or draw).
     * @retval notFinished if the game is not finished.
     * @retval whiteWon if white won the game.
     * @retval blackWon if black won the game.
     * @retval draw if a draw happened.
     * @sa newGame
     */
    Result stateOfGame();
    /*! @brief
     * This member execute a move without checking if the move is legal.
     * @details
     * This function is not made to be used by the end user. It has been engineered to
     * be used by solvers. It bypasses usual verifications and hence allows illegal moves to be
     * played. This function should be used with great care.
     * @param player indicates which player made the move.
     * @param list_moves contains the list of intermediate positions.
                                of a move (including the starting point
                                and the arrival point).
     * @sa elementaryMove
     * @sa move
     */
    void moveWithoutVerification(const uint_fast64_t &move);
    /*! @brief
     * Hashes @ref the grid.
     * @return A hash of @ref grid_
     */
    inline uint64_t hashGrid();

    /*! @brief
     * Returns @ref who_is_to_play_
     * @return @ref who_is_to_play_
     */
    Player getWhoIsToPlay() const;
    /*! @brief
     * Returns @ref number_of_times_seen
     * @return @ref number_of_times_seen
     */
    boost::unordered_map<uint64_t, int> getNumberOfTimesSeen() const;
    /*! @brief
     * Returns @ref bitBoards.White
     * @return @ref bitBoards.White
     */
    uint_fast64_t getBitBoardWhite();
    /*! @brief
     * Returns @ref bitBoards.Black
     * @return @ref bitBoards.Black
     */
    uint_fast64_t getBitBoardBlack();

    /*! @brief Prints the grid. */
    void printGrid();
    /*! @brief Prints @ref who_is_to_play_ */
    void printWhoIsToPlay();
};



#endif /* INCLUDE_CHINESECHECKERS_HPP_ */
