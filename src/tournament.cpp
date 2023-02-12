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
#include <algorithm>
#include <iomanip>
#include <limits>
#include <numbers>
#include <fstream>
 
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
#define P_MUTATION 0.1
/* variability of a mutation */
#define SIGMA_MUTATION  0.1
/*Number of solvers in the evolution*/
#define POP_SIZE 50
/*Maximum number of moves authorized in a evolution game*/
#define MAX_NUM_MOVES 100
/*Depth for AlphaBeta*/
#define AB_DEPTH 1
/*Number of generations in the evolution*/
#define NUM_GENERATION 100
/*Number of generation training white or black players*/
#define ROUND_LENGTH 200

//Creating distribution generators
const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 generator(seed);/*Use std::random_device generator; for true randomness*/
std::bernoulli_distribution b_distrib(P_MUTATION);
std::normal_distribution<double> n_distrib(0,SIGMA_MUTATION);
auto mutates = std::bind(b_distrib ,generator);
auto variation = std::bind(n_distrib,generator);


#warning TODO: Mutate que dans [0,1]0.0); ?
#warning TODO: écrire qu'a la fin dans les fichiers pour gagner du temps

int main() {
    /*Gives the seed to reproduce results*/
    std::cout << "Current seed: "<< seed << std::endl;

    /*Files to write in data of evolution*/
    std::ofstream white_evol;
    std::ofstream black_evol;
    std::ofstream white_best_players;
    std::ofstream black_best_players;
    white_evol.open ("White_evol.txt");
    black_evol.open("Black_evol.txt");
    white_best_players.open ("White_best_players.txt");
    black_best_players.open("Black_best_players.txt");
    white_evol << seed << ","<< P_MUTATION << "," << SIGMA_MUTATION << "," << POP_SIZE << "," << AB_DEPTH  << "," << NUM_GENERATION << "," << ROUND_LENGTH << "\n";
    black_evol << seed << ","<< P_MUTATION << "," << SIGMA_MUTATION << "," << POP_SIZE << "," << AB_DEPTH  << "," << NUM_GENERATION << "," << ROUND_LENGTH << "\n";
    white_best_players << seed << ","<< P_MUTATION << "," << SIGMA_MUTATION << "," << POP_SIZE << "," << AB_DEPTH  << "," << NUM_GENERATION << "," << ROUND_LENGTH << "\n";
    black_best_players << seed << ","<< P_MUTATION << "," << SIGMA_MUTATION << "," << POP_SIZE << "," << AB_DEPTH  << "," << NUM_GENERATION << "," << ROUND_LENGTH << "\n";


    GamePlayer gp(AB_DEPTH);

    std::vector< SolversIndividuals > pop_white(500);
    std::vector< SolversIndividuals > pop_black(500);

    /*Initialisation of the population*/
    std::vector< double > zeros(64,0);
    #warning Improve initialisation in constructor ?
    for (int i = 0; i != POP_SIZE; ++i) {
        //pop_white[i].set_win(zeros);
        //pop_white[i].set_loose(zeros);
        //pop_white[i].mutate(); /*to mutate at start*/
        //pop_black[i].set_win(zeros);
        //pop_black[i].set_loose(zeros);
        //pop_black[i].mutate(); /*to mutate at start*/
    }


    //initialisation of working variables
    SolversIndividuals best_white;
    SolversIndividuals best_black;
    gp.set_black_player(best_black);//Evolving white player at first
    double score = -2;//Minimum possible score
    best_white.set_score(score);
    best_black.set_score(score);
    bool is_white_evolving = true;
    int count = 0;

    for (int gen = 0; gen != NUM_GENERATION ; ++gen) {
        if (count == ROUND_LENGTH) {//swaping who is evolving
            is_white_evolving = !is_white_evolving;
            count = 0;
            is_white_evolving ? gp.set_white_player(best_white): gp.set_black_player(best_black);
            is_white_evolving ? best_white.print_info_as_matrix_to_file(white_best_players): best_black.print_info_as_matrix_to_file(black_best_players);
        }

        is_white_evolving ? evol(&gp, pop_white, &best_white, is_white_evolving) : evol(&gp, pop_black, &best_black, is_white_evolving);

        is_white_evolving ? write_scores(white_evol, pop_white, best_white, gen) : write_scores(black_evol, pop_black, best_black, gen);
        std::cout << "Gen n° " << gen << ", white is evolving: " << is_white_evolving << ", Best so far: ";
        if (is_white_evolving) {
            std::cout << best_white.get_score() << " Best score of the generation: " << pop_white[0].get_score() << std::endl;
        } else {
            std::cout << best_black.get_score() << " Best score of the generation: " << pop_black[0].get_score() << std::endl;
        }
        
        ++count;
    }


    std::cout <<"Best white so far:" << std::endl;
    best_white.print_info();

    std::cout <<"Best black so far:" << std::endl;
    best_black.print_info();


    white_evol.close();
    black_evol.close();
    white_best_players.close();
    black_best_players.close();

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

SolversIndividuals& SolversIndividuals::operator=(const SolversIndividuals & other) {
    if (this == &other)
        return *this;

    this->win = other.win;
    this->loose = other.loose;
    this->score = other.score;
    return *this;
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
    constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1};
    std::cout << "Win data:" << std::endl;
    std::cout << "{";
    for (int i = 0; i != 64; ++i) {
        std::cout << std::setprecision(max_precision) << this->win[i];
        if (i != 63)
            std::cout << ", ";
    }
    std::cout << "}";
    std::cout << std::endl << "Loose data:" << std::endl;
    std::cout << "{";
    for (int i = 0; i != 64; ++i) {
        std::cout << std::setprecision(max_precision) << this->loose[i];
        if (i != 63)
            std::cout << ", ";
    }
    std::cout << "}";
    std::cout << std::endl;
}

void SolversIndividuals::print_info_as_matrix() {
    constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 2};
    std::cout << "Win data:" << std::endl;
    std::cout << "{";
    for (int i = 0; i != 8; ++i) {
        std::cout << "{";
        for (auto j = 0; j != 8; ++j) {
            std::cout << std::setprecision(max_precision) << this->win[i*8 + j];
            if (j != 7)
                std::cout << ", ";
        };
        if (i != 7) {
            std::cout << "}," << std::endl;
        } else {
            std::cout << "}";
        }
    }
    std::cout << "}";
    std::cout << std::endl << "Loose data:" << std::endl;
    std::cout << "{";
    for (int i = 0; i != 8; ++i) {
        std::cout << "{";
        for (auto j = 0; j != 8; ++j) {
            std::cout << std::setprecision(max_precision) << this->loose[i*8 + j];
            if (j != 7)
                std::cout << ", ";
        };
        if (i != 7) {
            std::cout << "}," << std::endl;
        } else {
            std::cout << "}";
        }
    }
    std::cout << "}";
}

void SolversIndividuals::print_info_as_matrix_to_file(std::ofstream & file) {
    constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 2};
    file << "Win data:" << std::endl;
    file << "{";
    for (int i = 0; i != 8; ++i) {
        file << "{";
        for (auto j = 0; j != 8; ++j) {
            file << std::setprecision(max_precision) << this->win[i*8 + j];
            if (j != 7)
                file << ", ";
        };
        if (i != 7) {
            file << "}," << std::endl;
        } else {
            file << "}";
        }
    }
    file << "}";
    file << std::endl << "Loose data:" << std::endl;
    file << "{";
    for (int i = 0; i != 8; ++i) {
        file << "{";
        for (auto j = 0; j != 8; ++j) {
            file << std::setprecision(max_precision) << this->loose[i*8 + j];
            if (j != 7)
                file << ", ";
        };
        if (i != 7) {
            file << "}," << std::endl;
        } else {
            file << "}";
        }
    }
    file << "}" << std::endl<< std::endl;
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
    //std::cout << " remaining moves: " << remaining_moves<< std::endl;
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

void write_scores(std::ofstream & file,std::vector< SolversIndividuals > & population, SolversIndividuals & best_to_write, int& gen) {
    file << gen <<",";
    file << best_to_write.get_score() <<",";
    for (int i = 0; i != 10; ++i) {
        file << population[i * POP_SIZE /10].get_score();
        if (i != 9) {
            file << ",";
        } else {
            file << "," << population[POP_SIZE - 1].get_score() << "\n";
        }
    }

}

void evol(GamePlayer *gp,std::vector< SolversIndividuals >& population,  SolversIndividuals *best_player, bool is_white_evolving) {
        double score = -2;
        /*Do every games*/
        for (int i = 0; i != POP_SIZE; ++i)  {
            is_white_evolving ? gp->set_white_player(population[i]) : gp->set_black_player(population[i]);
            is_white_evolving ? score = gp->playGame():  score = - gp->playGame();
            population[i].set_score(score);
        }
        /*sort the population by their scores*/
        std::sort(population.begin(),population.end());
        /*Saving best white solver*/
        if (population[0].get_score() > best_player->get_score()) 
            *best_player = population[0];

        /*Kill and reproduce*/
        population[POP_SIZE/2-1] = *best_player;/*Best player always reproduces*/
        for (int i = 0; i < POP_SIZE/2 ; ++i) {
            population[i + POP_SIZE/2 ] = population[i];
        }
        for (int i = 0; i != POP_SIZE; ++i) {
            population[i].mutate();
        }

}