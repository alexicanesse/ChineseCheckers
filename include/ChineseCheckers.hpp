/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

#ifndef INCLUDE_CHINESECHECKERS_HPP_
#define INCLUDE_CHINESECHECKERS_HPP_


#include <vector>
#include <array>
#include <iostream>
#include <utility>

enum Color {Empty, White, Black};
enum MoveType { Illegal, Jump, notJump};

typedef int Player;
typedef std::array<std::array<Color, 8>, 8> GridType;
typedef std::vector<int> PositionType;
typedef std::vector<PositionType> ListOfPositionType;

class ChineseCheckers {
 private:
    /* a data structure containing the grid */
    GridType grid_;
    /* (i,j) contains the j-th color of player i */
    std::array<std::array<PositionType, 10>, 2> position_colors_players_;
    Player who_is_to_play_;
    bool no_pawns_between(PositionType original_position, PositionType arrival_position, int which_case, int middle_x, int middle_y);
    MoveType elementaryMove(PositionType original_position,
                            PositionType arrival_position);

 public:
    bool move(Player player, const ListOfPositionType &list_moves);
    /*
     * returns true or false to indicate if
     * the current position is a winning position
     */
    bool is_finished();
    void new_game();
    ChineseCheckers();
    void get_grid();
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
