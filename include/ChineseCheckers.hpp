//
//  ChineseCheckers.hpp
//
//
//  Created on 29/09/2022.
//

#ifndef ChineseCheckers_hpp
#define ChineseCheckers_hpp


#include <vector>

enum Color {Empty, Red, Orange, Yellow, Green, Blue, Purple};
enum MoveType { Illegal, Jump, notJump};

typedef int Player;
typedef std::vector<std::vector<Color>> GridType;
typedef std::pair<int, int> PositionType;

class ChineseCheckers{
private:
    GridType grid_; /* a data structure containing the grid */
    int nbr_players_;
    int nbr_colors_;
    std::vector<std::vector<PositionType>> position_colors_players_; /* (i,j) contains the j-th color of player i */
    Player who_is_to_play_;
    MoveType elementaryMove(PositionType original_position, PositionType arrival_position);

public:
    bool move(Player player, std::vector<PositionType> list_moves);
    bool is_finished(); /* returns true or false to indicate if the current position is a winning position */
    void new_game(int nbr_players, int nbr_colors);
    ChineseCheckers();
};

class Addon : public ChineseCheckers{
private:
    std::vector<MoveType> history_;

public:
    std::vector<std::vector<PositionType>> available_moves(); /* list of all available moves */
    std::vector<PositionType> available_moves(Player player, PositionType position);  /* list of available moves from a given position */
    void cancel_moves(int number_of_moves_to_cancel);
    std::vector<std::pair<Player, GridType>> get_history();
    Addon();
};



#endif /* ChineseCheckers_hpp */
