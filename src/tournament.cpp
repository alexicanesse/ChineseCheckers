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

Result playGame(AlphaBeta &player0, AlphaBeta &player1, const int &depth) {
    int remaining_moves = 100;
    while((player0.state_of_game() == NotFinished) && (remaining_moves > 0)) {
        auto move = player0.getMove(depth, -100000, 100000);
        player0.move(0, move);
        player1.move(0, move);

        //player0.print_grid_();
        //std::cout << "\n";

        if(player0.state_of_game() != NotFinished)
            return player0.state_of_game();

        move = player1.getMove(depth, -100000, 100000);
        player0.move(1, move);
        player1.move(1, move);

        //player0.print_grid_();
        //std::cout << "\n";
        --remaining_moves;
    }
    return player0.state_of_game();
}

void print_matrix(const std::vector< std::vector<double> > &matrix) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            std::cout << matrix[i][j] << " ";
        std::cout << "\n";
    }
}



//Evolution parameters
double p_mutation = 0.05; /*Probability of mutation of an element*/
double sigma_mutation = 1; /*variability of a mutation*/
//Creating distribution generators
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//std::random_device generator;
std::mt19937 generator(seed);
std::bernoulli_distribution b_distrib(p_mutation);
std::normal_distribution<double> n_distrib(0,sigma_mutation);
auto mutates = std::bind(b_distrib ,generator);
auto variation = std::bind(n_distrib,generator);

void mutate_matrix(std::vector< std::vector<double> > &matrix) {
    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 8; ++j) {
            if (mutates()) {
                matrix[i][j] = matrix[i][j] + variation();
            }
        }
    }
}

//writing SolversIndividuals class 

SolversIndividuals::SolversIndividuals() : 
win(  std::vector<double> ({0, 1, 4, 9, 16, 25, 36, 49, 1, 2, 5, 10, 17, 26, 37, 50, 4, 5, 8, 13, 20, 29, 40, 53, 9, 10, 13, 18, 25, 34, 45, 58, 16, 17, 20, 25, 32, 41, 52, 65, 25, 26, 29, 34, 41, 50, 62, 74, 36, 37, 40, 45, 52, 62, 72, 85, 49, 50, 53, 58, 65, 74, 85, 98})) ,
loose(std::vector<double> ({0, 1, 4, 9, 16, 25, 36, 49, 1, 2, 5, 10, 17, 26, 37, 50, 4, 5, 8, 13, 20, 29, 40, 53, 9, 10, 13, 18, 25, 34, 45, 58, 16, 17, 20, 25, 32, 41, 52, 65, 25, 26, 29, 34, 41, 50, 62, 74, 36, 37, 40, 45, 52, 62, 72, 85, 49, 50, 53, 58, 65, 74, 85, 98})) {}

SolversIndividuals::SolversIndividuals(std::vector<double> & win_, std::vector<double> & loose_) : win(win_) , loose(loose_) {};

std::vector<double> SolversIndividuals::get_win() {
    return(win);
}

std::vector<double> SolversIndividuals::get_loose() {
    return(loose);
}

void SolversIndividuals::set_win(std::vector<double> & win_) {
    win = win_;
}

void SolversIndividuals::set_loose(std::vector<double> & loose_) {
    loose = loose_;
}

void SolversIndividuals::mutate() {
    for (int i = 0; i != 64; ++i) {
        if (mutates())
            win[i] = win[i] + variation();
        if (mutates())
            loose[i] = loose[i] + variation();
    }
}


void SolversIndividuals::print_info() {
    std::cout << "Win data:" << std::endl;
    for (auto x : win) {
        std::cout << x << " ";
    }
    std::cout << std::endl << "Loose data:" << std::endl;
    for (auto x : loose) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}


int main() {
    int population_size = 10;
    std::cout << "Current seed: "<< seed << std::endl;

    SolversIndividuals test;
    test.print_info();
    test.mutate();
    test.print_info();




    /*
    std::vector< std::vector<double> > matrix(8, std::vector<double>(8));
    print_matrix(matrix);

    for (int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            if (i + j < 3) matrix[i][j] = -42*(i + j);

    
    */
    // AlphaBeta player0;
    // auto matrix = player0.get_player_to_win_value_();
    // AlphaBeta player1(matrix, matrix);
    // std::cout << playGame(player0, player1, 3) << "\n";
    return 0;
}