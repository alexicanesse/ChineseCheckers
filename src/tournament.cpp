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
#include <cmath>
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


int main() {
    std::cout << "Current seed: "<< seed << std::endl;
    std::vector< SolversIndividuals > population(POP_SIZE ,SolversIndividuals());
    std::vector<double> vect = population[0].get_win();


    double sum = std::accumulate(vect.begin(),vect.end(),0.0);
    std::cout<<sum;
    

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
win(  std::vector<double> ({0         ,  0.00044603, 0.00178412, 0.00401427, 0.00713649, 0.0111508, 0.0160571, 0.0218555,
                            0.00044603, 0.000892061, 0.00223015,  0.0044603, 0.00758252, 0.0115968, 0.0165031, 0.0223015, 
                            0.00178412,  0.00223015, 0.00356824, 0.00579839, 0.00892061, 0.0129349, 0.0178412, 0.0236396, 
                            0.00401427,   0.0044603, 0.00579839, 0.00802855,  0.0111508,  0.015165, 0.0200714, 0.0258698, 
                            0.00713649,  0.00758252, 0.00892061,  0.0111508,   0.014273, 0.0182872, 0.0231936,  0.028992, 
                             0.0111508,   0.0115968,  0.0129349,   0.015165,  0.0182872, 0.0223015, 0.0276539, 0.0330062, 
                             0.0160571,   0.0165031,  0.0178412,  0.0200714,  0.0231936, 0.0276539, 0.0321142, 0.0379126, 
                             0.0218555,   0.0223015,  0.0236396,  0.0258698,   0.028992, 0.0330062, 0.0379126, 0.043711  })),
loose(std::vector<double> ({0         ,  0.00044603, 0.00178412, 0.00401427, 0.00713649, 0.0111508, 0.0160571, 0.0218555,
                            0.00044603, 0.000892061, 0.00223015,  0.0044603, 0.00758252, 0.0115968, 0.0165031, 0.0223015, 
                            0.00178412,  0.00223015, 0.00356824, 0.00579839, 0.00892061, 0.0129349, 0.0178412, 0.0236396, 
                            0.00401427,   0.0044603, 0.00579839, 0.00802855,  0.0111508,  0.015165, 0.0200714, 0.0258698, 
                            0.00713649,  0.00758252, 0.00892061,  0.0111508,   0.014273, 0.0182872, 0.0231936,  0.028992, 
                             0.0111508,   0.0115968,  0.0129349,   0.015165,  0.0182872, 0.0223015, 0.0276539, 0.0330062, 
                             0.0160571,   0.0165031,  0.0178412,  0.0200714,  0.0231936, 0.0276539, 0.0321142, 0.0379126, 
                             0.0218555,   0.0223015,  0.0236396,  0.0258698,   0.028992, 0.0330062, 0.0379126, 0.043711   })),
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
    double normalization_factor_w = 1;
    double normalization_factor_l = 1;
    double variation_ = 0;
    for (int i = 0; i != 64; ++i) {
        if (mutates()) {
            variation_ = variation();
            win[i] += variation_;
            normalization_factor_w += std::abs(variation_);
        }
        if (mutates()) {
            variation_ = variation();
            loose[i] += variation_;
            normalization_factor_l += std::abs(variation_);
        }
    }

    for (int i = 0; i != 64; ++i) {
        win[i]   /= normalization_factor_w;
        loose[i] /= normalization_factor_l;
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
    switch(result) {
        case NotFinished:
            return(0.0);
            break;
        case Draw:
            return(0.0);
            break;
        case WhiteWon:
            return(1.0);
            break;
        case BlackWon:
            return(-1.0);
            break;
        default:
            std::cout<<"Error in the switch of GamePlayer::playGame(), return 0.0 by default";
            return(0.0);
            break;
    }
     

}
