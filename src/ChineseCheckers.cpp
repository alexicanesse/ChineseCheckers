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
#include <boost/python.hpp>


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
        /* Debug output */
        std::cout << "elementaryMove : 0" << std::endl;
        return(Illegal);
    }

    /*
     * Wathever happens, if the arrival_position is already occupied,
     * then the move is not valid
     */
    if ((this->grid_.at(arrival_position[0]).at(arrival_position[1]))
        != Empty) {
        /* Debug output */
        std::cout << "elementaryMove : 1" << std::endl;
        return Illegal;
    }

    int a = original_position[0];
    int b = original_position[1];
    int c = arrival_position[0];
    int d = arrival_position[1];
    bool rep;

    /* Checks if the direction is legal */
    if (((c - a) * (d - b) == 0) || ((d - b) / (c - a) == -1)) {
        rep = (abs(a-c+b-d) <= 1) && (abs(a-c) +abs(b-d) <= 2);
        /* Debug output */
        std::cout << "Temp cout rep before:" << rep << std::endl;
        for (int i = 0; i < 2; ++i) {
            /* Debug output */
            std::cout << "cible" << (a + c)/2 << " " << (b+d)/2 << std::endl;
            for (auto x : this->position_colors_players_.at(i)) {
                std::cout<< x.at(0) << " " << x.at(1) <<std::endl;
                if ((x.at(0) != a) || (x.at(1) != b)) {
                    if (   x.at(0) == (a + c)/2
                        && x.at(1) == (b + d)/2 ) {
                        rep = true;
                    } else if (((x.at(0) - a) * (d - b)
                                == (x.at(1) - b) * (c - a))
                            && ((x.at(0) - a) * (c - a)
                                + (x.at(1)-b) * (d - b)
                                                    >= 0)
                            && ((x.at(0) - c) * (a - c)
                                + (x.at(1) - d)*(b - d)
                                                    >= 0)
                               ) {
                        /* Debug output */
                        std::cout<< "elementaryMove : 2" << x.at(0) << " " << x.at(1) << std::endl;
                        return Illegal;
                    }
                }
            }
        }
        /* Debug output */
        std::cout << "Temp cout rep after:" << rep << std::endl;
        if (rep) {
            if ((abs(a-c+b-d) <= 1) && (abs(a-c) +abs(b-d) <= 2))
                return(notJump);
            else
                return(Jump);
        } else {
            /* Debug output */
            std::cout << "elementaryMove : 3" << std::endl;
            return Illegal;
        }
    } else {
        /* Debug output */
        std::cout << "elementaryMove : 4" << std::endl;
        return Illegal;
    }
}

ChineseCheckers::ChineseCheckers() {
    this->new_game();
}

bool ChineseCheckers::move(Player player,
                           const ListOfPositionType &list_moves) {
    if(list_moves.size() == 0)
        return false;
    
    /* Check that the right player is playing */
    if (this->grid_.at(list_moves.at(0).at(0)).at(list_moves.at(0).at(1))
                                                            != player + 1) {
        /* Debug output */
        std::cout << "move: 0" << std::endl;
        return false;
        }

    if (player != this->who_is_to_play_) {
        /* Debug output */
        std::cout << "move: 1" << std::endl;
        return false;
        }


    /* Check that every moves are legals */
    int n = static_cast<int>(list_moves.size());
    MoveType fst_move = elementaryMove(list_moves[0], list_moves[1]);
    if (fst_move == Illegal) {
        /* Debug output */
        std::cout << "move: 2" << std::endl;
        return false;
    } else if (fst_move == notJump) {
        if (n != 2) {
            /* Debug output */
            std::cout << "move: 3" << std::endl;
            return false;
        }
    } else {
        for (int i=1; i < n - 1; ++i)
            if (elementaryMove(list_moves.at(i), list_moves.at(i+1)) != Jump) {
                /* Debug output */
                std::cout << "move: 4" << std::endl;
                return false;
            }
    }

    /* Now that we know that the move is legal, we just have to apply it */
    this->grid_.at(list_moves.at(n-1).at(0)).at(list_moves.at(n-1).at(1)) =
                grid_.at(list_moves.at(0).at(0)).at(list_moves.at(0).at(1));
    this->grid_.at(list_moves.at(0).at(0)).at(list_moves.at(0).at(1)) = Empty;

    std::cout << "Actualisation of position_colors_player" << std::endl;
    for (int i = 0; i < 10; ++i) {
        if ((this->position_colors_players_.at(player).at(i).at(0)
                                            == list_moves.at(0).at(0))
           && (this->position_colors_players_.at(player).at(i).at(1)
                                            == list_moves.at(0).at(1))) {
            this->position_colors_players_.at(player).at(i).at(0)
                                                = list_moves.at(n-1).at(0);
            this->position_colors_players_.at(player).at(i).at(1)
                                                = list_moves.at(n-1).at(1);
            /* Debug output */
            std::cout << "Actualisation done !" << std::endl;
            break;
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
    this->grid_ = GridType(8, std::vector<Color>(8, Empty));
    this->position_colors_players_ =
                            std::vector<std::vector<PositionType>>(2);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i + j < 4) {
                this->grid_.at(i).at(j) = White;
                this->grid_.at(7-i).at(7-j) = Black;

                PositionType pos_temp = {i, j};
                this->position_colors_players_.at(0).push_back(pos_temp);
                pos_temp = {7-i, 7-j};
                this->position_colors_players_.at(1).push_back(pos_temp);
            }
        }
    }
    this->who_is_to_play_ = 0;
}

void ChineseCheckers::print_grid_() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            std::cout << this->grid_.at(i).at(j) << " ";
        std::cout << "\n";
    }
}

GridType ChineseCheckers::get_grid_() {
    return this->grid_;
}

void ChineseCheckers::print_position_colors_players_() {
    for (std::vector<PositionType> pawns_positions :
                        this->position_colors_players_) {
        for (PositionType position : pawns_positions)
            std::cout << "("
                      << position.at(0)
                      << ", "
                      << position.at(1)
                      << ") ";
        std::cout << std::endl;
    }
}

std::vector<std::vector<PositionType>>
            ChineseCheckers::get_position_colors_players_() {
    return this->position_colors_players_;
}

void ChineseCheckers::print_who_is_to_play_() {
    std::cout << this->who_is_to_play_ << "\n";
}

Player ChineseCheckers::get_who_is_to_play_() {
    return this->who_is_to_play_;
}
