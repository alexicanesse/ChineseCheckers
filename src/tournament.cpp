/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file tournaments.cpp
 * \brief
 *
 *
 *
 */

/* tournament.hpp */
#include "tournament.hpp"

/* C++ libraries */
#include <vector>
/* The following pragma are used to removed deprecation warning from boost
 * header files. Using them avoid to remove this warning from the entire project.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/python.hpp>
#pragma GCC diagnostic pop


/* Other */
#include "Types.hpp"
#include "AlphaBeta.hpp"

Result playGame(AlphaBeta &player0, AlphaBeta &player1, const int &depth) {
    while(player0.state_of_game() == NotFinished) {
        auto move = player0.getMove(depth, -100000, 100000);
        player0.move(0, move);
        player1.move(0, move);

        // player0.print_grid_();
        // std::cout << "\n";

        if(player0.state_of_game() != NotFinished)
            return player0.state_of_game();

        move = player1.getMove(depth, -100000, 100000);
        player0.move(1, move);
        player1.move(1, move);

        // player0.print_grid_();
        // std::cout << "\n";
    }

    return player0.state_of_game();
}

int main() {
    AlphaBeta player0;

    auto matrix = player0.get_player_to_win_value_();

    for (int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            if (i + j < 3) matrix[i][j] = -42*(i + j);

    AlphaBeta player1(matrix, matrix);
    std::cout << playGame(player0, player1, 3) << "\n";
    return 0;
}