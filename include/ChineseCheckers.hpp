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
/* The following pragma are used to removed depraction warning from boost
 * header files. Using them avoid to remove this warning from the entire project.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/python.hpp>
#pragma GCC diagnostic pop

/* Other */
#include "Types.hpp"
class ChineseCheckers {
 protected:
    /*! A data structure containing the grid */
    GridType grid_;
    /*! The variable `position_colors_players_.at(player).at(i)` contains the position
     *  (of type `PositionType`) of the player `player` */
    std::vector<std::vector<PositionType>> position_colors_players_;
    /*! Indicate which is the next player to play */
    Player who_is_to_play_ = 0;
    /*! Indicates the number of times a position has been seen */
    std::unordered_map<unsigned long long, int> number_of_times_seen;
    /*! Keeps the positions  of the winning zone */
    const std::vector<std::pair<int, int>> winning_positions_ = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0},
        {1, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 0}};
    /*!
      \brief
     * A member returning the type of an elementary move
     * (not a succession of jumps).
     */
    /*!
      \param original_position The position.
      \param arrival_position The arrival position.
      \retval Illegal if the elementary move realized was of illegal.
      \retval Jump if the elementary move realized was a jump.
      \retval notJump if the elementary move realized was not a jump.
      \sa move(Player player, const ListOfPositionType &list_moves).
    */
    MoveType elementaryMove(PositionType original_position,
                            PositionType arrival_position);
    /*! \brief
     * A member used to indicate that a pawn should not be considered until further notice.
     * \details
     * To indicates that the pawn should currently not be considered anymore,
     * its position is set to (-1, -1).
     */
    /*!
      \param player The player to which belongs the pawn.
      \param position The position of the pawn.
      \sa reset_pawn(Player player, PositionType position)
    */
    void remove_pawn(Player player, PositionType position);
    /*! A member setting the position of a pawn which has been removed
     */
    /*!
      \param player The player to which belongs the pawn.
      \param position The position of the pawn.
      \sa remove_pawn(Player player, PositionType position)
    */
    void reset_pawn(Player player, PositionType position);
    /*! \brief
     * This member execute a move without checking if the move is legal.
     * \details
     * This function is not made to be used by the end user. It has been engineered to
     * be used by solvers. It bypasses usual verifications and hence allows illegal moves to be
     * played. This function should be used with great care.
     */
    /*!
      \param player indicates which player made the move.
      \param list_moves contains the list of intermediate positions.
                                of a move (including the starting point
                                and the arrival point).
      \sa elementaryMove(PositionType original_position, PositionType arrival_position), move(Player player,
              const ListOfPositionType &list_moves).
    */
    void moveWithoutVerification(Player player,
              const ListOfPositionType &list_moves);

    /* functions used for the transposition tables */
    /* FNV-1a hash function */
    inline uint64_t fnv1aColor(uint64_t h, const Color &x);
    inline uint64_t fnv1a(uint64_t h, const int &x);
    uint64_t hashMatrix(const GridType &matrix, const int &player);
    uint64_t hashMove(const ListOfPositionType &move);
    uint64_t hashPosition(const PositionType &move);

 public:
    /*! \brief
     * This member checks if a move is legal and executes it if it is legal.
     */
    /*!
      \param player indicates which player made the move.
      \param list_moves contains the list of intermediate positions.
                                of a move (including the starting point
                                and the arrival point).
      \retval true if the move was legal.
      \retval false if the move was not legal.
      \sa elementaryMove(PositionType original_position, PositionType arrival_position).
    */
    bool move(Player player,
              const ListOfPositionType &list_moves);
    /*! \brief
     * Indicates the current state of the game (ie, not finished, black won, white won or draw).
     */
    /*!
      \retval notFinished if the game is not finished.
      \retval whiteWon if white won the game.
      \retval blackWon if black won the game.
      \retval draw if a draw happened.
      \sa new_game().
    */
    Result state_of_game();
    /*! \brief
     * Creates a new game.
     */
    void new_game();
    /*! \brief
     * Prints \ref grid_.
     */
    void print_grid_();
    /*! \brief
     * Returns \ref grid_.
     */
    GridType get_grid_() const;
    /*! \brief
     * Prints \ref position_colors_players_.
     */
    void print_position_colors_players_();
    /*! \brief
     * Returns the \ref position_colors_players_.
     */
    std::vector<std::vector<PositionType>> get_position_colors_players_() const;
    /*! \brief
     * Prints \ref who_is_to_play_.
     */
    void print_who_is_to_play_();
    /*! \brief
     * Returns \ref who_is_to_play_.
     */
    Player get_who_is_to_play_() const;

    uint64_t hashGrid() const;

    std::unordered_map<uint64_t, int> get_number_of_times_seen() const;
    ChineseCheckers();
};



#endif /* INCLUDE_CHINESECHECKERS_HPP_ */
