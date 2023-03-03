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

/*! @brief
 * Core implementation of the game
 */
class ChineseCheckers {
 protected:
    uint_fast64_t un_64 = static_cast<uint_fast64_t>(1);

    /*! Indicate which is the next player to play */
    Player who_is_to_play_ = 0;

    /*! A data structure containing the grid */
    uint_fast64_t bitBoardWhite;

    /*! A data structure containing the grid */
    uint_fast64_t bitBoardBlack;

    /*! Indicates the number of times a position has been seen */
    boost::unordered_map<uint64_t , int> number_of_times_seen;

    /*! Keeps the positions  of the winning zone */
    const uint_fast64_t winning_positions_white_ =
            0b1111000011100000110000001000000000000000000000000000000000000000;
    const uint_fast64_t winning_positions_black_ =
            0b0000000000000000000000000000000000000001000000110000011100001111;

    /*! Stores the illegal positions */
    boost::unordered_map<uint32_t, bool> illegalPositions;
    std::array<std::array<uint32_t, 8>, 8> cantorPairing_;

    /*! @brief
     * A member returning the type of an elementary move
     * (not a succession of jumps).
     */
    /*!
     * @param original_position The position.
     * @param arrival_position The arrival position.
     * @retval Illegal if the elementary move realized was of illegal.
     * @retval Jump if the elementary move realized was a jump.
     * @retval notJump if the elementary move realized was not a jump.
     * @sa move(Player player, const ListOfPositionType &list_moves).
     */
    MoveType elementaryMove(const PositionType &original_position,
                            const PositionType &arrival_position);

    /*!
     * @brief Cantor's pairing function
     * @param x first argument
     * @param y second argument
     * @return cantor's pairing function applied to @ref x and @ref y
     */
    int cantorPairingFunction(const int &x, const int &y);

    /*!
     * @brief Loads illegal positions
     */
    void loadIllegalPositions();

    /*!
     * @breif Indicates whether the position is illegal or not
     * @return Returns true iff the position is illegal
     */
    bool isPositionIllegal();

    std::vector<std::vector<int>> valid_lines = {{-1,  0},
                                                 {-1,  1},
                                                 {0 , -1},
                                                 {0 ,  1},
                                                 {1 , -1},
                                                 {1 ,  0}};

    std::vector<std::vector<int>> valid_lines_illegal = {{-1,  0},
                                                         {-1,  1},
                                                         {0 , -1},
                                                         {0 ,  1},
                                                         {1 , -1},
                                                         {1 ,  0},
                                                         {-2,  0},
                                                         {-2,  2},
                                                         {0 , -2},
                                                         {0 ,  2},
                                                         {2 , -2},
                                                         {2 ,  0}};
 public:
    /*! @brief
     * A simple constructor
     */
    ChineseCheckers();

    /*! @brief
     * This member checks if a move is legal and executes it if it is legal.
     */
    /*!
     * @param player indicates which player made the move.
     * @param list_moves contains the list of intermediate positions.
                                of a move (including the starting point
                                and the arrival point).
     * @retval true if the move was legal.
     * @retval false if the move was not legal.
     * @sa elementaryMove(PositionType original_position, PositionType arrival_position).
     */
    bool move(const Player &player,
              const ListOfPositionType &list_moves);

    /*! @brief
     * Indicates the current state of the game (ie, not finished, black won, white won or draw).
     */
    /*!
     * @retval notFinished if the game is not finished.
     * @retval whiteWon if white won the game.
     * @retval blackWon if black won the game.
     * @retval draw if a draw happened.
     * @sa new_game().
     */
    Result state_of_game();

    /*! @brief
     * This member execute a move without checking if the move is legal.
     * @details
     * This function is not made to be used by the end user. It has been engineered to
     * be used by solvers. It bypasses usual verifications and hence allows illegal moves to be
     * played. This function should be used with great care.
     */
    /*!
     * @param player indicates which player made the move.
     * @param list_moves contains the list of intermediate positions.
                                of a move (including the starting point
                                and the arrival point).
     * @sa elementaryMove(PositionType original_position
     * @sa PositionType arrival_position)
     * @sa move(Player player, const ListOfPositionType &list_moves).
     */
    void moveWithoutVerification(const uint_fast64_t &move);

    /*! @brief
     * Creates a new game.
     */
    void new_game();

    /*! @brief
     * Prints \ref grid_.
     */
    void print_grid_();

    /*! @brief
     * Prints @ref who_is_to_play_.
     */
    void print_who_is_to_play_();

    /*! @brief
     * Returns @ref who_is_to_play_.
     */
    /*!
     * @return @ref who_is_to_play_.
     */
    Player get_who_is_to_play_() const;

    /*! @brief
     * Returns @ref number_of_times_seen/
     */
    /*!
     * @return @ref number_of_times_seen
     */
    boost::unordered_map<uint64_t, int> get_number_of_times_seen() const;

    uint_fast64_t get_bitBoardWhite();
    uint_fast64_t get_bitBoardBlack();
    boost::unordered_map<uint_fast64_t, std::pair<int, int>> uint64_to_pair_;
    std::array<std::array<uint_fast64_t, 8>, 8> int_to_uint64_;

    /*! @brief
     * Hashes @ref grid_ according to the fnv1a scheme
     */
    /*!
     * @return A hash of @ref grid_
     */
    inline uint64_t hashGrid();

    std::unordered_map<uint_fast64_t, std::vector<uint_fast64_t>> direct_neighbours_;
    std::unordered_map<uint_fast64_t,
        std::vector<std::vector<std::pair<
                                    std::pair<uint_fast64_t, uint_fast64_t>,
                                    uint_fast64_t> > > > k_neighbours_;
};



#endif /* INCLUDE_CHINESECHECKERS_HPP_ */
