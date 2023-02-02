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
#include <random>
#include <chrono>
#include<algorithm>
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


//Global parameters
/* Probability of mutation of an element */
#define P_MUTATION 0.05
/* variability of a mutation */
#define SIGMA_MUTATION  0.05
/*Number of solvers in the evolution*/
#define POP_SIZE 10

//Creating distribution generators
const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 generator(seed);/*Use std::random_device generator; for true randomness*/
std::bernoulli_distribution b_distrib(P_MUTATION);
std::normal_distribution<double> n_distrib(0,SIGMA_MUTATION);
auto mutates = std::bind(b_distrib ,generator);
auto variation = std::bind(n_distrib,generator);


#warning Mutate que dans [0,1]0.0); ?

int main() {
    std::cout << "Current seed: "<< seed << std::endl;
    std::vector< SolversIndividuals > population(POP_SIZE ,SolversIndividuals());

    SolversIndividuals sol1;
    SolversIndividuals sol2;
    GamePlayer gp1;
    GamePlayer gp2(sol1,sol2);
    std::cout << "A game " << gp1.playGame() << std::endl;
    std::cout << "A game " << gp2.playGame() << std::endl;



    

    /*
    
    std::vector<double> test(POP_SIZE);
    for (int i = 0; i != POP_SIZE; ++i) {
        population[i].mutate();
        auto win__ = population[i].get_win();
        auto loose__ = population[i].get_loose();
        std::cout << *std::max_element(win__.begin(),win__.end()) << " "<< *std::min_element(win__.begin(),win__.end()) << " " <<*std::max_element(loose__.begin(),loose__.end()) << " "<< *std::min_element(loose__.begin(),loose__.end()) << std::endl;
    }
    
    */

    /*
    for (int i = 0; i != POP_SIZE; ++i) {
        double rnd_score = variation();
        population[i].set_score(rnd_score);
        std::cout << population[i].get_score() << std::endl;
    }

    std::sort(population.begin(),population.end());
    std::cout << "After sorting:"<<std::endl;
    for (int i = 0; i != POP_SIZE; ++i) {
        std::cout << population[i].get_score() << std::endl;
    }
    */
    return 0;
}

//writing SolversIndividuals class 
SolversIndividuals::SolversIndividuals() : 
win(  std::vector<double> ({0         ,   0.0102041, 0.0408163,   0.0918366,   0.163265,  0.255103, 0.367347,        0.5,
                            0.0102041 ,   0.0204082, 0.0510203,    0.102041,   0.173469,  0.265306,  0.37755,   0.510203, 
                             0.0408163,   0.0510203, 0.0816325,    0.132653,   0.204082,  0.295919, 0.408163,   0.540816,
                             0.0918366,    0.102041,  0.132653,    0.183673,   0.255103,  0.346938, 0.459184,   0.591837, 
                              0.163265,    0.173469,  0.204082,    0.255103,   0.326531,  0.418366, 0.530612,   0.663266, 
                              0.255103,    0.265306,  0.295919,    0.346938,   0.418366,  0.510203, 0.632653,   0.755101, 
                              0.367347,     0.37755,  0.408163,    0.459184,   0.530612,  0.632653, 0.734694,   0.867347,
                                   0.5,    0.510203,  0.540816,    0.591837,   0.663266,  0.755101, 0.867347,          1})),
loose(std::vector<double> ({0         ,   0.0102041, 0.0408163,   0.0918366,   0.163265,  0.255103, 0.367347,        0.5,
                            0.0102041 ,   0.0204082, 0.0510203,    0.102041,   0.173469,  0.265306,  0.37755,   0.510203, 
                             0.0408163,   0.0510203, 0.0816325,    0.132653,   0.204082,  0.295919, 0.408163,   0.540816,
                             0.0918366,    0.102041,  0.132653,    0.183673,   0.255103,  0.346938, 0.459184,   0.591837, 
                              0.163265,    0.173469,  0.204082,    0.255103,   0.326531,  0.418366, 0.530612,   0.663266, 
                              0.255103,    0.265306,  0.295919,    0.346938,   0.418366,  0.510203, 0.632653,   0.755101, 
                              0.367347,     0.37755,  0.408163,    0.459184,   0.530612,  0.632653, 0.734694,   0.867347,
                                   0.5,    0.510203,  0.540816,    0.591837,   0.663266,  0.755101, 0.867347,          1})),
score(0) {}

SolversIndividuals::SolversIndividuals(std::vector<double> & win_, std::vector<double> & loose_) : win(win_), loose(loose_), score(0) {}


bool operator<(SolversIndividuals const& s1, SolversIndividuals const& s2) {
    //We sort by the probability of dying i.e we want to keep solvers with the highest score
    return(s1.score > s2.score);
}


std::vector<double> SolversIndividuals::get_win() {
    return(this->win);
}

std::vector<double> SolversIndividuals::get_loose() {
    return(this->loose);
}

void SolversIndividuals::set_win(std::vector<double> & win_) {
    this->win = win_;
}

void SolversIndividuals::set_loose(std::vector<double> & loose_) {
    this->loose = loose_;
}

void SolversIndividuals::set_score(double & score_) {
    this->score = score_;
}

double SolversIndividuals::get_score() {
    return(this->score);
}


void SolversIndividuals::mutate() {
    double normalisation_factor = 0;
    for (int i = 0; i != 64; ++i) {
        if (mutates()) 
            this->win[i] += variation();
        if (mutates())
            this->loose[i] += variation();
    }

    const auto [min0, max0] = std::minmax_element(this->win.begin(), this->win.end());
    const auto [min1, max1] = std::minmax_element(this->loose.begin(), this->loose.end());
    normalisation_factor = std::max(std::abs(*min0), std::abs(*max0));
    normalisation_factor = std::max(normalisation_factor, std::max(std::abs(*min1), std::abs(*max1)));

    if (normalisation_factor != 0) {
        for (int i = 0; i != 64; ++i) {
            win[i]   /= normalisation_factor;
            loose[i] /= normalisation_factor;
        }
    }

}


void SolversIndividuals::print_info() {
    std::cout << "Win data:" << std::endl;
    for (auto x : win) 
        std::cout << x << " ";
    std::cout << std::endl << "Loose data:" << std::endl;
    for (auto x : loose) 
        std::cout << x << " ";
    std::cout << std::endl;
}

void print_matrix(const std::vector< std::vector<double> > &matrix) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            std::cout << matrix[i][j] << " ";
        std::cout << "\n";
    }
}


//Writing GamePlayer class

GamePlayer::GamePlayer() : white_player(AlphaBeta()), black_player(AlphaBeta()), depth(1) {}
GamePlayer::GamePlayer(int & depth_) : white_player(AlphaBeta()), black_player(AlphaBeta()), depth(depth_) {}
GamePlayer::GamePlayer(SolversIndividuals & solver1, SolversIndividuals & solver2,int depth_) {
    this->set_white_player(solver1);
    this->set_black_player(solver2);
    this->depth = depth_;
}

void GamePlayer::set_white_player(SolversIndividuals &solver) {
    std::vector<double> win_ = solver.get_win();
    std::vector<double> loose_ = solver.get_loose();
    std::vector< std::vector<double> > matrix1(8, std::vector<double>(8));
    std::vector< std::vector<double> > matrix2(8, std::vector<double>(8));
    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 8; ++j) {
            matrix1[i][j] = win_[i*8 + j];
            matrix2[i][j] =loose_[i*i +j];
        }
    }
    this->white_player.set_player_to_win_value_(matrix1);
    this->white_player.set_player_to_loose_value_(matrix2);
}

void GamePlayer::set_black_player(SolversIndividuals &solver) {
    std::vector<double> win_ = solver.get_win();
    std::vector<double> loose_ = solver.get_loose();
    std::vector< std::vector<double> > matrix1(8, std::vector<double>(8));
    std::vector< std::vector<double> > matrix2(8, std::vector<double>(8));
    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 8; ++j) {
            matrix1[i][j] = win_[i*8 + j];
            matrix2[i][j] =loose_[i*i +j];
        }
    }
    this->black_player.set_player_to_win_value_(matrix1);
    this->black_player.set_player_to_loose_value_(matrix2);
}

void GamePlayer::set_depth(int & depth_) {
    this->depth = depth_;
}

double GamePlayer::playGame() {
    int remaining_moves = 100;
    while((this->white_player.state_of_game() == NotFinished) && (remaining_moves > 0)) {
        auto move = this->white_player.getMove(this->depth, -100000, 100000);
        this->white_player.move(0, move);
        this->black_player.move(0, move);

        if(this->white_player.state_of_game() != NotFinished)
            return this->white_player.state_of_game();

        move = this->black_player.getMove(this->depth, -100000, 100000);
        this->white_player.move(1, move);
        this->black_player.move(1, move);
        --remaining_moves;
    }

    Result result = this->white_player.state_of_game();
    std::vector<std::vector<Color> > end_grid;
    double score = 0.0;
    switch(result) {
        case NotFinished:
            end_grid = this->white_player.get_grid_();
            for (auto x: this->white_triangle) {
                if (end_grid[x[0]][x[1]] != Empty) {
                    score -= 1;
                }
            }
            for (auto x: this->black_triangle) {
                if (end_grid[x[0]][x[1]] != Empty) {
                    score += 1;
                }
            }
            return(score/20);
            break;

        case Draw:
            return(0.0);
            break;

        case WhiteWon:
            return(1.0 + remaining_moves/100);
            break;

        case BlackWon:
            return(-1.0 - remaining_moves/100);
            break;

        default:
            std::cout<<"Error in the switch of GamePlayer::playGame(), return 0.0 by default";
            return(0.0);
            break;
    }
}
