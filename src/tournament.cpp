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

#warning TODO normaliser les matrices



/* Probability of mutation of an element */
#define P_MUTATION 1
/* variability of a mutation */
#define SIGMA_MUTATION  2.5
//Creating distribution generators
const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 generator(seed);/*Use std::random_device generator; for true randomness*/
std::bernoulli_distribution b_distrib(P_MUTATION);
std::normal_distribution<double> n_distrib(0,SIGMA_MUTATION);
auto mutates = std::bind(b_distrib ,generator);
auto variation = std::bind(n_distrib,generator);


int main() {
    int population_size = 10;
    std::cout << "Current seed: "<< seed << std::endl;
    std::vector< SolversIndividuals > population(population_size ,SolversIndividuals());


    for (int i = 0; i != 10; ++i) {
        double rnd_score = variation();
        population[i].set_score(rnd_score);
        std::cout << population[i].get_score() << std::endl;
    }

    std::sort(population.begin(),population.end());
    std::cout << "After sorting:"<<std::endl;
    for (int i = 0; i != 10; ++i) {
        std::cout << population[i].get_score() << std::endl;
    }

    return 0;
}

//writing SolversIndividuals class 
SolversIndividuals::SolversIndividuals() : 
win(  std::vector<double> ({0,   1,  4,  9, 16, 25, 36, 49, 
                            1,   2,  5, 10, 17, 26, 37, 50,
                            4,   5,  8, 13, 20, 29, 40, 53,
                            9,  10, 13, 18, 25, 34, 45, 58,
                            16, 17, 20, 25, 32, 41, 52, 65,
                            25, 26, 29, 34, 41, 50, 62, 74,
                            36, 37, 40, 45, 52, 62, 72, 85,
                            49, 50, 53, 58, 65, 74, 85, 98})),
loose(std::vector<double> ({0,   1,  4,  9, 16, 25, 36, 49, 
                            1,   2,  5, 10, 17, 26, 37, 50,
                            4,   5,  8, 13, 20, 29, 40, 53,
                            9,  10, 13, 18, 25, 34, 45, 58,
                            16, 17, 20, 25, 32, 41, 52, 65,
                            25, 26, 29, 34, 41, 50, 62, 74,
                            36, 37, 40, 45, 52, 62, 72, 85,
                            49, 50, 53, 58, 65, 74, 85, 98})),
score(0) {}

SolversIndividuals::SolversIndividuals(std::vector<double> & win_, std::vector<double> & loose_) : win(win_) , loose(loose_), score(0) {}


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
    for (int i = 0; i != 64; ++i) {
        if (mutates())
            win[i] += variation();
        if (mutates())
            loose[i] += variation();
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

Result GamePlayer::playGame() {
    int remaining_moves = 100;
    while((this->white_player.state_of_game() == NotFinished) && (remaining_moves > 0)) {
        auto move = this->white_player.getMove(this->depth, -100000, 100000);
        this->white_player.move(0, move);
        this->black_player.move(0, move);

        //this->white_player.print_grid_();
        //std::cout << "\n";

        if(this->white_player.state_of_game() != NotFinished)
            return this->white_player.state_of_game();

        move = this->black_player.getMove(this->depth, -100000, 100000);
        this->white_player.move(1, move);
        this->black_player.move(1, move);

        //this->white_player.print_grid_();
        //std::cout << "\n";
        --remaining_moves;
    }
    if (remaining_moves == 0)
        std::cout << "No moves remaining" << std::endl;
    return this->white_player.state_of_game();
}
