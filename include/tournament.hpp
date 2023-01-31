/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file tournament.hpp
 * \brief
 *
 *
 *
 */

#ifndef CHINESECHECKERS_TOURNAMENT_H
#define CHINESECHECKERS_TOURNAMENT_H

/* C++ libraries */
#include <vector>
/* The following pragma are used to removed depraction warning from boost
 * header files. Using them avoid to remove this warning from the entire project.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/python.hpp>
#pragma GCC diagnostic pop

/* Other */
#include "Types.hpp"
#include "AlphaBeta.hpp"

Result playGame(AlphaBeta &player0, AlphaBeta &player1, const int &depth);
void print_matrix(const std::vector< std::vector<double> > &matrix);

class SolversIndividuals {
    private:
    std::vector<double> win;
    std::vector<double> loose;
    double score;

    public:
    SolversIndividuals();
    SolversIndividuals(std::vector<double> & win_, std::vector<double> & loose_);

    friend bool operator<(SolversIndividuals const & s1, SolversIndividuals const & s2);

    std::vector<double> get_win();
    std::vector<double> get_loose();
    double get_score();

    void set_win(std::vector<double> & win_);
    void set_loose(std::vector<double> & lose_);
    void set_score(double & score_);

    void mutate();

    void print_info();
};



class GamePlayer {
    private:
    AlphaBeta white_player;
    AlphaBeta black_player;
    int depth;

    public:
    GamePlayer();
    GamePlayer(int & depth_);
    GamePlayer(SolversIndividuals & solver1, SolversIndividuals & solver2,int depth_ = 1);

    void set_white_player(SolversIndividuals &solver);
    void set_black_player(SolversIndividuals &solver);
    void set_depth(int & depth_);
    Result playGame();
};

#endif //CHINESECHECKERS_TOURNAMENT_HPP
