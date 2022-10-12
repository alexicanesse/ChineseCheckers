/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file ChineseCheckers.cpp
 * \brief Core implementation of the game
 *
 * Implementation of the ChineseCheckers Class which is an implementation of the core of the game
 *
 */


#include "../include/ChineseCheckers.hpp"

#include <vector>


MoveType ChineseCheckers::elementaryMove(PositionType original_position,
                                         PositionType arrival_position) {
    // Goal: to decide which move was realized
    // Check if the move is in the grid
    if (!(0<= arrival_position[0]
          && arrival_position[0] <= 7
          && 0<= arrival_position[1]
          && arrival_position[1] <= 7
          && 0<= original_position[0]
          && original_position[0] <= 7
          && 0<= original_position[1]
          && original_position[1] <= 7)
        ) {
        return(Illegal);
    }

    /*
     * Wathever happens, if the arrival_position is already occupied,
     * then the move is not valid
     */
    if ((this->grid_.at(arrival_position[0]).at(arrival_position[1]))
        != Empty) {
        return Illegal;
    }

    int a = original_position[0];
    int b = original_position[1];
    int c = arrival_position[0];
    int d = arrival_position[1];
    bool rep = (abs(a-c+b-d) <= 1) && (abs(a-c) +abs(b-d) <= 2);

    /* Checks if the direction is legal */
    if (((c - a) * (d - b) == 0) || ((d - b) / (c - a) == -1)) {
        for (int i = 0; i < 2; ++i) {
            for (auto x : this->position_colors_players_.at(i)) {
                if (   x[0] == (a + c)/2
                    && x[1] == (b + d)/2 ) {
                    rep = true;
                } else if ((  x[0] - a)*(d-b) == (x[1] - b)*(c - a)
                           && (x[0] -a)*(c-a) + (x[1]-b)*(d-b) >= 0
                           && (x[0] -c)*(a-c) + (x[1]-d)*(b-d) >= 0) {
                    return Illegal;
                }
            }
        }

        if (rep) {
            if ((abs(a-c+b-d) <= 1) && (abs(a-c) +abs(b-d) <= 2))
                return(notJump);
            else
                return(Jump);
        } else {
            return Illegal;
        }
    } else {
        return Illegal;
    }
}

ChineseCheckers::ChineseCheckers() {
    this->new_game();
}

bool ChineseCheckers::move(Player player,
                           const ListOfPositionType &list_moves) {
    /* Check that the right player is playing */
    if (this->grid_.at(list_moves.at(0).at(0)).at(list_moves.at(0).at(1))
                                                            != player + 1)
        return false;

    if (player != this->who_is_to_play_)
        return false;


    /* Check that every moves are legals */
    int n = static_cast<int>(list_moves.size());
    MoveType fst_move = elementaryMove(list_moves[0], list_moves[1]);
    if (fst_move == Illegal) {
        return false;
    } else if (fst_move == notJump) {
        if (n != 2)
            return false;
    } else {
        for (int i=1; i < n - 1; ++i)
            if (elementaryMove(list_moves.at(i), list_moves.at(i+1)) != Jump)
                return false;
    }

    /* Now that we know that the move is legal, we just have to apply it */
    this->grid_.at(list_moves.at(n-1).at(0)).at(list_moves.at(n-1).at(1)) =
                grid_.at(list_moves.at(0).at(0)).at(list_moves.at(0).at(1));
    this->grid_.at(list_moves.at(0).at(0)).at(list_moves.at(0).at(1)) = Empty;

    for (int i = 0; i < 8; ++i) {
        if ((this->position_colors_players_.at(player).at(i).at(0)
                                            == list_moves.at(0).at(0))
           && (this->position_colors_players_.at(player).at(i).at(1)
                                            == list_moves.at(0).at(1))) {
            this->position_colors_players_.at(player).at(i).at(0)
                                                = list_moves.at(n-1).at(0);
            this->position_colors_players_.at(player).at(i).at(1)
                                                = list_moves.at(n-1).at(1);
        }
    }

    this->who_is_to_play_ = 1 - this->who_is_to_play_;
    return true;
}

/*
 * returns true or false to indicate if
 * the current position is a winning position
 */
bool ChineseCheckers::is_finished() {
    /* Check if player 0 won */
    bool won = true;
    for (auto x : this->position_colors_players_.at(0)) {
        if (x.at(0) < 4 || x.at(1) < 4)
            won = false;
    }
    if (won)
        return true;

    /* Check if Player 1 won */
    won = true;
    for (auto x : this->position_colors_players_.at(1)) {
        if (x.at(0) > 4 || x.at(1) > 4)
            won = false;
    }
    /*
     * This return is a shortcut for
     * if(won)
     *    return true;
     *
     * return false;
     */
    return won;
}

void ChineseCheckers::new_game() {
    /* Initialize the grid */
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i + j < 4) {
                this->grid_.at(i).at(j) = White;
                this->grid_.at(7-i).at(7-j) = Black;

                /*
                 * (i, j) |-> 3*i + j is a bijection from [|0, 3|]^2 to [|0, 9|]
                 */
                this->position_colors_players_.at(1).at(i*3 + j) = {i, j};
                this->position_colors_players_.at(0).at(i*3 + j) = {7-i, 7-j};
                std::cout << std::endl;
            } else {
                this->grid_.at(i).at(j) = Empty;
                this->grid_.at(7-i).at(7-j) = Empty;
            }
        }
    }
    this->who_is_to_play_ = 0;
}

void ChineseCheckers::get_grid() {
    for (auto x : this->grid_) {
        for (auto y : x)
            std::cout << y << " ";
        std::cout << "\n";
    }
}
