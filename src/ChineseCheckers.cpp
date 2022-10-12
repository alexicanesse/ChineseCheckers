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

bool ChineseCheckers::no_pawns_between(PositionType original_position, PositionType arrival_position, int which_case, int middle_x, int middle_y)
    {
        int i = 0;
        size_t like_x;
        size_t like_y;
        int smaller_no_change_coordinate;
        int smaller_change_coordinate;
        int bigger_no_change_coordinate;
        int bigger_change_coordinate;

        if(which_case != 2){
            like_x = which_case;
            like_y = (1 + which_case) % 2;

            if(original_position[like_y] < arrival_position[like_y]){
                smaller_no_change_coordinate = original_position[like_x];
                smaller_change_coordinate = original_position[like_y];
                bigger_no_change_coordinate = arrival_position[like_x];
                bigger_change_coordinate = arrival_position[like_y];
            }
            else{
                smaller_no_change_coordinate = arrival_position[like_x];
                smaller_change_coordinate = arrival_position[like_y];
                bigger_no_change_coordinate = original_position[like_x];
                bigger_change_coordinate = original_position[like_y];
            }

        }

        if(which_case == 0){
            while(smaller_change_coordinate + i <= smaller_change_coordinate){
                if((ChineseCheckers::grid_.at(smaller_no_change_coordinate)).at(smaller_change_coordinate + i) != Empty){ // There is a pawn there
                    if((smaller_change_coordinate + i) != middle_y){ // We are not in the middle of the two positions
                        return false;
                    }
                }
                i = i + 1;
            }
            return true;
        }
        if(which_case == 1){
            while(smaller_change_coordinate + i <= bigger_change_coordinate){
                if((ChineseCheckers::grid_.at(smaller_change_coordinate + i)).at(smaller_no_change_coordinate) != Empty){
                    if((smaller_change_coordinate + i) != middle_x){
                        return false;
                    }
                }
                i = i + 1;
            }
            return true;
        }

        if(which_case == 2){
            while(original_position[0] - i > arrival_position[0]){
                if((ChineseCheckers::grid_.at(original_position[0] - i)).at(original_position[1] + i) != Empty){
                    if((original_position[0] - i) != middle_x){
                        return false;
                    }
                }
                i = i + 1;
            }
            i = 0;
            while(original_position[1] - i > arrival_position[1]){
                if((ChineseCheckers::grid_.at(original_position[0] + i)).at(original_position[1] - i) != Empty){
                    if((original_position[0] + i) != middle_x){
                        return false;
                    }
                }
                i = i + 1;
            }
            return true;
        }
        return false;
    }


MoveType ChineseCheckers::elementaryMove(PositionType original_position, PositionType arrival_position){
    // Goal: to decide which move was realized

    // Wathever happens, if the arrival_position is already occupied, then the move is not valid
    if((ChineseCheckers::grid_.at(arrival_position[0]).at(arrival_position[1])) != Empty){
        return Illegal;
    }
    

    // Then we can decide if it looks like we will have a jump or not with the distance between
    // the two positions
    int distance = (original_position[0]-arrival_position[0])*(original_position[0]-arrival_position[0]) + (original_position[1]-arrival_position[1])*(original_position[1]-arrival_position[1]);

    if(distance <= 2){
        // Case where it resembles a notJump

        // There are two cases where it is not notJump ; then it is illegal
        if(original_position[1] == arrival_position[1] - 1 && original_position[0] == arrival_position[0] - 1){
            return Illegal;
        }
        if(original_position[1] == arrival_position[1] + 1 && original_position[0] == arrival_position[0] + 1){
            return Illegal;
        }
        // Else it is a notJump
        return notJump;
    }


    // Else, we are in the case where it resembles a jump

    // There needs to be a pawn in the middle of the originial position and the arrival position
    // but no other pawn between them.

    int which_case;

    if(original_position[0] == arrival_position[0]){
        which_case = 0;
    } else {
    if(original_position[1] == arrival_position[1]){
        which_case = 1;
    } else {
    // Else we are on a "diagonal" (and the good one)
    if(original_position[0] + original_position[1] == arrival_position[0] + arrival_position[1]){
        which_case = 2;
    } else {
        return Illegal;
        // It is not even possible that the move is right.
    } } }

    // If there is a place between the two positions, it will be at position
    // ((original_position[0]+arrival_position[0])/2, (original_position[1]+arrival_position[1])/2)
    int twice_middle_x = original_position[0]+arrival_position[0];
    int twice_middle_y = original_position[1]+arrival_position[1];

    // First, we have to check if that position even exists
    // If it does not, then middle_x or middle_y will be a float of the form
    // k.5. Therefore, middle_x*2 or middle_y*2 will be odd (else it would not)
    if (twice_middle_x % 2 == 1 || twice_middle_y % 2 == 1){
        return Illegal;
    }

    // Then, we have to check is there is a pawn there
    int middle_x = twice_middle_x / 2;
    int middle_y = twice_middle_y / 2;
    if((ChineseCheckers::grid_.at(middle_x).at(middle_y)) == Empty){ // There is no pawn there
        return Illegal;
    }

    // Finally, we have to check if there is any other pawns between the two positions
    if(ChineseCheckers::no_pawns_between(original_position, arrival_position, which_case, middle_x, middle_y) == 0){
        return Illegal;
    }

    return Jump;
};

ChineseCheckers::ChineseCheckers() {
    this->new_game();
}

bool ChineseCheckers::move(Player player,
                           const ListOfPositionType &list_moves){
  int n = (int) list_moves.size();
  MoveType fst_move = elementaryMove(list_moves[0],list_moves[1]);
  if (fst_move == Illegal) return false;
  if (fst_move == notJump) return (n == 1);
  for (int i=1; i<n-1; ++i){
    if (elementaryMove(list_moves[i], list_moves[i+1]) != Jump) return false;
  }
    return true;  // This is just to remove
                  // the error while this function is not implemented
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
