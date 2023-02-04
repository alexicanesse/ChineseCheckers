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
/*Maximum number of moves authorized in a evolution game*/
#define MAX_NUM_MOVES 100
/*Depth for AlphaBeta*/
#define AB_DEPTH 1

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
    SolversIndividuals sol1;
    SolversIndividuals sol2;
    GamePlayer gp1(AB_DEPTH);
    GamePlayer gp2(sol1,sol2,AB_DEPTH);
    //gp1.print_players_info();
    //gp2.print_players_info();

    std::cout << "Constructor test: "<< gp2.constructor_test()<<std::endl;

    std::cout << "Default constructor game: " << gp1.playGame() << std::endl;
    std::cout << "Specific constructor game: " << gp2.playGame() << std::endl;


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
win(  std::vector<double> ({  0.0/98,  1.0/98,  4.0/98,  9.0/98, 16.0/98, 25.0/98, 36.0/98, 49.0/98,
                              1.0/98,  2.0/98,  5.0/98, 10.0/98, 17.0/98, 26.0/98, 37.0/98, 50.0/98,
                              4.0/98,  5.0/98,  8.0/98, 13.0/98, 20.0/98, 29.0/98, 40.0/98, 53.0/98,
                              9.0/98, 10.0/98, 13.0/98, 18.0/98, 25.0/98, 34.0/98, 45.0/98, 58.0/98,
                             16.0/98, 17.0/98, 20.0/98, 25.0/98, 32.0/98, 41.0/98, 52.0/98, 65.0/98,
                             25.0/98, 26.0/98, 29.0/98, 34.0/98, 41.0/98, 50.0/98, 62.0/98, 74.0/98,
                             36.0/98, 37.0/98, 40.0/98, 45.0/98, 52.0/98, 62.0/98, 72.0/98, 85.0/98,
                             49.0/98, 50.0/98, 53.0/98, 58.0/98, 65.0/98, 74.0/98, 85.0/98, 98.0/98  })),
loose(std::vector<double> ({  0.0/98,  1.0/98,  4.0/98,  9.0/98, 16.0/98, 25.0/98, 36.0/98, 49.0/98,
                              1.0/98,  2.0/98,  5.0/98, 10.0/98, 17.0/98, 26.0/98, 37.0/98, 50.0/98,
                              4.0/98,  5.0/98,  8.0/98, 13.0/98, 20.0/98, 29.0/98, 40.0/98, 53.0/98,
                              9.0/98, 10.0/98, 13.0/98, 18.0/98, 25.0/98, 34.0/98, 45.0/98, 58.0/98,
                             16.0/98, 17.0/98, 20.0/98, 25.0/98, 32.0/98, 41.0/98, 52.0/98, 65.0/98,
                             25.0/98, 26.0/98, 29.0/98, 34.0/98, 41.0/98, 50.0/98, 62.0/98, 74.0/98,
                             36.0/98, 37.0/98, 40.0/98, 45.0/98, 52.0/98, 62.0/98, 72.0/98, 85.0/98,
                             49.0/98, 50.0/98, 53.0/98, 58.0/98, 65.0/98, 74.0/98, 85.0/98, 98.0/98  })),
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

GamePlayer::GamePlayer(int depth_) : white_player(AlphaBeta()), black_player(AlphaBeta()), depth(depth_) {}

GamePlayer::GamePlayer(SolversIndividuals & solver1, SolversIndividuals & solver2,int depth_) {
    this->set_white_player(solver1);
    this->set_black_player(solver2);
    this->depth = depth_;
}

void GamePlayer::set_white_player(SolversIndividuals &solver) {
    std::vector<double> win_ = solver.get_win();
    std::vector<double> loose_ = solver.get_loose();
    std::vector< std::vector<double> > matrix1(8, std::vector<double>(8,0));
    std::vector< std::vector<double> > matrix2(8, std::vector<double>(8,0));
    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 8; ++j) {
            matrix1[i][j] = win_[i*8 + j];
            matrix2[i][j] = loose_[i*8 + j];
        }
    }
    this->white_player.set_player_to_win_value_(matrix1);
    this->white_player.set_player_to_loose_value_(matrix2);
}

void GamePlayer::set_black_player(SolversIndividuals &solver) {
    std::vector<double> win_ = solver.get_win();
    std::vector<double> loose_ = solver.get_loose();
    std::vector< std::vector<double> > matrix1(8, std::vector<double>(8,0));
    std::vector< std::vector<double> > matrix2(8, std::vector<double>(8,0));
    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 8; ++j) {
            matrix1[i][j] = win_[i*8 + j];
            matrix2[i][j] = loose_[i*8 + j];
        }
    }
    this->black_player.set_player_to_win_value_(matrix1);
    this->black_player.set_player_to_loose_value_(matrix2);
}

void GamePlayer::set_depth(int & depth_) {
    this->depth = depth_;
}

double GamePlayer::playGame() {
    int remaining_moves = MAX_NUM_MOVES;
    this->white_player.new_game();
    this->black_player.new_game();
    while((this->white_player.state_of_game() == NotFinished) && (remaining_moves > 0)) {
        auto move = this->white_player.getMove(this->depth, -100000, 100000);
        this->white_player.move(0, move);
        this->black_player.move(0, move);
        --remaining_moves;

        if(this->white_player.state_of_game() != NotFinished)
            break;

        move = this->black_player.getMove(this->depth, -100000, 100000);
        this->white_player.move(1, move);
        this->black_player.move(1, move);
        --remaining_moves;
    }

    Result result = this->white_player.state_of_game();
    std::vector<std::vector<Color> > end_grid;
    double score = 0.0;
    std::cout << " remaining moves: " << remaining_moves<< std::endl;
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
            return(1.0 + ((double) remaining_moves)/MAX_NUM_MOVES);
            break;

        case BlackWon:
            return(-1.0 - ((double) remaining_moves)/MAX_NUM_MOVES);
            break;

        default:
            std::cout<<"Error in the switch of GamePlayer::playGame(), return 0.0 by default";
            return(0.0);
            break;
    }
}

void GamePlayer::print_players_info() {
    std::cout << std::endl;
    auto matrix11 = this->white_player.get_player_to_win_value_();
    auto matrix12 = this->white_player.get_player_to_loose_value_();
    std::cout << "White player info:" << std::endl << "Win value:" << std::endl;
    print_matrix(matrix11);
    std::cout << std::endl;
    std::cout << "Loose value:" << std::endl;
    print_matrix(matrix12);

    std::cout << std::endl;
    auto matrix21 = this->black_player.get_player_to_win_value_();
    auto matrix22 = this->black_player.get_player_to_loose_value_();
    std::cout << "Black player info:" << std::endl << "Win value:" << std::endl;
    print_matrix(matrix21);
    std::cout << std::endl;
    std::cout << "Loose value:" << std::endl;
    print_matrix(matrix22);
}

int GamePlayer::constructor_test() {
    AlphaBeta testPlayer;
    auto matrix11 = this->white_player.get_player_to_win_value_();
    auto matrix12 = this->white_player.get_player_to_loose_value_();
    auto matrix21 = testPlayer.get_player_to_win_value_();
    auto matrix22 = testPlayer.get_player_to_loose_value_();

    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 8; ++j) {
            if (matrix11[i][j] != matrix21[i][j]) {
                std::cout << "ko" << i << " " << j << "case 1" << std::endl;
                return(0);
            }
                
            if (matrix12[i][j] != matrix22[i][j]) {
                std::cout << "ko" << i << " " << j << "case 1" << std::endl;
                return(0);
            }
        }
    }
    std::cout << "ok " ;
    return(1);

}