//
//  ChineseCheckers.cpp
//  Chinese Checkers
//
//  Created on 06/10/2022.
//

#include "../include/ChineseCheckers.hpp"

#include <vector>

MoveType ChineseCheckers::elementaryMove(PositionType original_position, PositionType arrival_position){
#warning TODO
    return Illegal; //This is just to remove the error while this function is not implemented
};

ChineseCheckers::ChineseCheckers(){
    this->new_game();
};

bool ChineseCheckers::move(Player player, std::vector<PositionType> list_moves){
#warning TODO
    return true; //This is just to remove the error while this function is not implemented
}

bool ChineseCheckers::is_finished(){ /* returns true or false to indicate if the current position is a winning position */
#warning TODO
    return true; //This is just to remove the error while this function is not implemented
}

void ChineseCheckers::new_game(){
    /* Initialize the grid */
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            if(i + j < 4){
                this->grid_.at(i).at(j) = White;
                this->grid_.at(7-i).at(7-j) = Black;

                /*
                 * (i, j) |-> 3*i + j is a bijection from [|0, 3|]^2 to [|0, 9|]
                 */
                this->position_colors_players_.at(0).at(i*3 + j) = {i, j};
                this->position_colors_players_.at(1).at(i*3 + j) = {7-i, 7-j};
                std::cout << std::endl;
            }
            else{
                this->grid_.at(i).at(j) = Empty;
                this->grid_.at(7-i).at(7-j) = Empty;
            }
        }
    }
    
    this->who_is_to_play_ = 0;
}

void ChineseCheckers::get_grid(){
    for(auto x : this->grid_){
        for(auto y : x)
            std::cout << y << " ";
        std::cout << "\n";
    }
};
