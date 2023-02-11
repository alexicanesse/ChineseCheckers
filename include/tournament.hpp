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
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <numbers>
#include <fstream>
 
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
    SolversIndividuals& operator=(const SolversIndividuals & other);

    std::vector<double> get_win();
    std::vector<double> get_loose();
    double get_score();

    void set_win(std::vector<double> & win_);
    void set_loose(std::vector<double> & lose_);
    void set_score(double & score_);

    void mutate();

    void print_info();
    void print_info_as_matrix();
    void print_info_as_matrix_to_file(std::ofstream & file);

};



class GamePlayer {
    private:
    AlphaBeta white_player;
    AlphaBeta black_player;
    std::vector< std::vector<int> > white_triangle = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 0}};
    std::vector< std::vector<int> > black_triangle = {{7, 7}, {7, 6}, {7, 5}, {7, 4}, {6, 7}, {6, 6}, {6, 5}, {5, 7}, {5, 6}, {4, 7}};
    int depth;

    public:
    GamePlayer();
    GamePlayer(int depth_);
    GamePlayer(SolversIndividuals & solver1, SolversIndividuals & solver2,int depth_ = 1);

    void set_white_player(SolversIndividuals &solver);
    void set_black_player(SolversIndividuals &solver);
    void set_depth(int & depth_);
    double playGame();
    void print_players_info();
    int constructor_test();
};

void write_scores(std::ofstream & file,std::vector< SolversIndividuals > & population, SolversIndividuals & best_to_write, int& gen);
void evol(GamePlayer  *gp,std::vector< SolversIndividuals >& population,  SolversIndividuals *best_player, bool is_white_evolving);

#endif //CHINESECHECKERS_TOURNAMENT_HPP
