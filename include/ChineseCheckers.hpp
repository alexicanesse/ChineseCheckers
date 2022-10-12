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


#include <vector>
#include <array>
#include <iostream>
#include <utility>

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
typedef std::array<std::array<Color, 8>, 8> GridType;
/*! \typedef PositionType
    \brief Used to denote a position.
*/
typedef std::vector<int> PositionType;
/*! \typedef ListOfPositionType
    \brief Used to denote the a vector of positions.
 */
typedef std::vector<PositionType> ListOfPositionType;

/*! \class ChineseCheckers
    \brief The core of the game.
*/

class ChineseCheckers {
 private:
    /*! A data structure containing the grid */
    GridType grid_;
    /*! The variable `position_colors_players_.at(player).at(i)` contains the position
     *  (of type `PositionType`) of the player `player` */
    std::array<std::array<PositionType, 10>, 2> position_colors_players_;
    /*! Indicate which is the next player to play */
    Player who_is_to_play_;
    /*! A member returning the type of an elementary move
     * (not a succession of jumps).
     */
    /*!
      \param original_position The position.
      \param arrival_position The arrival position.
      \return The type of elementary move (`Illegal`, `Jump` or `notJump`).
      \sa move(Player player, const ListOfPositionType &list_moves).
    */
    MoveType elementaryMove(PositionType original_position,
                            PositionType arrival_position);

 public:
    /*!
     * This member checks if a move is legal and executes it if it is legal.
     */
    /*!
      \param Player indicates which player made the move.
      \param ListOfPositionType contains the list of intermediate positions.
                                of a move (including the starting point
                                and the arrival point).
      \return The `bool true` if and only if the move was indeed legal.
      \sa elementaryMove(PositionType original_position, PositionType arrival_position).
    */
    bool move(Player player,
              const ListOfPositionType &list_moves);
    /*!
     * Indicates if someone won the game.
     */
    /*!
      \return The `bool true` if and only if someone won the game.
      \sa new_game().
    */
    bool is_finished();
    /*!
     * Creates a new game.
     * \sa new_game().
     */
    void new_game();
    /*!
     * Prints the grid.
     */
    void get_grid();
    ChineseCheckers();
};

class Addon : public ChineseCheckers{
 private:
    std::vector<MoveType> history_;

 public:
    /* list of all available moves */
    std::vector<std::vector<PositionType>> available_moves();
    /* list of available moves from a given position */
    std::vector<PositionType> available_moves(Player player,
                                              PositionType position);
    void cancel_moves(int number_of_moves_to_cancel);
    std::vector<std::pair<Player, GridType>> get_history();
    Addon();
};



#endif /* INCLUDE_CHINESECHECKERS_HPP_ */
