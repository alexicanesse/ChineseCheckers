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

/* C libraries */
#include <numbers>

/* C++ libraries */
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <limits>
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


/*
 * Global parameters
 */

/*
 * Genetic evolution parameters
 */

/* Probability of mutation of an element */
#define P_MUTATION 0.1
/* Variability of a mutation */
#define SIGMA_MUTATION  0.1
/*Ratio of cumulated scores taken in deciding the choices of parents*/
#define SELECTION_RATIO 1

/* Number of solvers in the evolution */
#define POP_SIZE 10
/* Number of generations in the evolution */
#define NUM_GENERATION 10
/* Number of generation training white or black players */
#define ROUND_LENGTH 2000
/* Maximum number of moves authorized in a evolution game */
#define MAX_NUM_MOVES 100


/*
 * Solvers' parameters
 */

/* Depth for AlphaBeta */
#define AB_DEPTH 1

/* Mean for initialisation */
#define MEAN_INIT 0
/* Mean for initialisation */
#define SIGMA_INIT 0.1


/* Creating distribution generators */
//const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
const unsigned seed = 818934826;
/* Use std::random_device generator; for true randomness */
std::mt19937 generator(seed);
std::bernoulli_distribution b_distrib(P_MUTATION);
std::normal_distribution<double> n_distrib(0, SIGMA_MUTATION);
std::normal_distribution<double> n_distrib_for_initialisation(MEAN_INIT, SIGMA_INIT);
auto mutates   = std::bind(b_distrib, generator);
auto variation = std::bind(n_distrib, generator);
auto initiator_ = std::bind(n_distrib_for_initialisation,generator);


#warning TODO: Do cross over
#warning TODO: écrire qu'a la fin dans les fichiers pour gagner du temps

int main() {
    /* Gives the seed to reproduce results */
    std::cout << "Current seed: "<< seed << std::endl;

    /* Files to write in data of evolution */
    std::ofstream white_evol;
    std::ofstream black_evol;
    std::ofstream white_best_players;
    std::ofstream black_best_players;
    white_evol.open("White_evol.txt");
    black_evol.open("Black_evol.txt");
    white_best_players.open("White_best_players.txt");
    black_best_players.open("Black_best_players.txt");
    white_evol         << seed << ","
                       << P_MUTATION << ","
                       << SIGMA_MUTATION << ","
                       << POP_SIZE << ","
                       << AB_DEPTH  << ","
                       << NUM_GENERATION << ","
                       << ROUND_LENGTH << "\n";
    black_evol         << seed << ","
                       << P_MUTATION << ","
                       << SIGMA_MUTATION << ","
                       << POP_SIZE << ","
                       << AB_DEPTH  << ","
                       << NUM_GENERATION << ","
                       << ROUND_LENGTH << "\n";
    white_best_players << seed << ","
                       << P_MUTATION << ","
                       << SIGMA_MUTATION << ","
                       << POP_SIZE << ","
                       << AB_DEPTH << ","
                       << NUM_GENERATION << ","
                       << ROUND_LENGTH << "\n";
    black_best_players << seed << ","
                       << P_MUTATION << ","
                       << SIGMA_MUTATION << ","
                       << POP_SIZE << ","
                       << AB_DEPTH  << ","
                       << NUM_GENERATION << ","
                       << ROUND_LENGTH << "\n";

    GamePlayer gp(AB_DEPTH);

    std::vector<SolversIndividuals> pop_white(POP_SIZE);
    std::vector<SolversIndividuals> pop_black(POP_SIZE);

    /* Initialisation of the population */
    std::vector<double> zeros(64, 0);
    for (int i = 0; i != POP_SIZE; ++i) {
        pop_white[i].init_at_random();
        pop_black[i].init_at_random();
    }

    /* Initialisation of working variables */
    SolversIndividuals best_white;
    SolversIndividuals best_black;

    /* Evolving white player at first */
    gp.set_black_player(best_black);

    double score = -2;  /* Minimum possible score */
    best_white.set_score(score);
    best_black.set_score(score);

    bool is_white_evolving = true;
    int count = 0;
    for (int gen = 0; gen != NUM_GENERATION ; ++gen) {
        if (count == ROUND_LENGTH) {  /* Swaping who is evolving */
            is_white_evolving = !is_white_evolving;
            count = 0;
            if (is_white_evolving) {
                gp.set_white_player(best_white);
                best_white.print_info_as_matrix_to_file(white_best_players);
            } else {
                gp.set_black_player(best_black);
                best_black.print_info_as_matrix_to_file(black_best_players);
            }
        }


        if (is_white_evolving) {
            evol(&gp, pop_white, &best_white, is_white_evolving);
            write_scores(white_evol, pop_white, best_white, gen);
        } else {
            evol(&gp, pop_black, &best_black, is_white_evolving);
            write_scores(black_evol, pop_black, best_black, gen);
        }

        std::cout << "Gen n° "
                  << gen
                  << ", white is evolving: "
                  << is_white_evolving
                  << ", Best so far: ";
        if (is_white_evolving) {
            std::cout << best_white.get_score()
                      << " Best score of the generation: "
                      << pop_white[0].get_score()
                      << std::endl;
        } else {
            std::cout << best_black.get_score()
                      << " Best score of the generation: "
                      << pop_black[0].get_score()
                      << std::endl;
        }

        ++count;
    }

    std::cout <<"Best white so far:\n";
    best_white.print_info();

    std::cout <<"Best black so far:\n";
    best_black.print_info();

    if (!is_white_evolving) {
        gp.set_white_player(best_white);
        best_white.print_info_as_matrix_to_file(white_best_players);
    } else {
        gp.set_black_player(best_black);
        best_black.print_info_as_matrix_to_file(black_best_players);
    }

    white_evol.close();
    black_evol.close();
    white_best_players.close();
    black_best_players.close();

    return 0;
}

/* Defining SolversIndividuals class */
SolversIndividuals::SolversIndividuals() :
win(std::vector<double> ({
     0.0/98,  1.0/98,  4.0/98,  9.0/98, 16.0/98, 25.0/98, 36.0/98, 49.0/98,
     1.0/98,  2.0/98,  5.0/98, 10.0/98, 17.0/98, 26.0/98, 37.0/98, 50.0/98,
     4.0/98,  5.0/98,  8.0/98, 13.0/98, 20.0/98, 29.0/98, 40.0/98, 53.0/98,
     9.0/98, 10.0/98, 13.0/98, 18.0/98, 25.0/98, 34.0/98, 45.0/98, 58.0/98,
    16.0/98, 17.0/98, 20.0/98, 25.0/98, 32.0/98, 41.0/98, 52.0/98, 65.0/98,
    25.0/98, 26.0/98, 29.0/98, 34.0/98, 41.0/98, 50.0/98, 62.0/98, 74.0/98,
    36.0/98, 37.0/98, 40.0/98, 45.0/98, 52.0/98, 62.0/98, 72.0/98, 85.0/98,
    49.0/98, 50.0/98, 53.0/98, 58.0/98, 65.0/98, 74.0/98, 85.0/98, 98.0/98  })),
lose(std::vector<double> ({
     0.0/98,  1.0/98,  4.0/98,  9.0/98, 16.0/98, 25.0/98, 36.0/98, 49.0/98,
     1.0/98,  2.0/98,  5.0/98, 10.0/98, 17.0/98, 26.0/98, 37.0/98, 50.0/98,
     4.0/98,  5.0/98,  8.0/98, 13.0/98, 20.0/98, 29.0/98, 40.0/98, 53.0/98,
     9.0/98, 10.0/98, 13.0/98, 18.0/98, 25.0/98, 34.0/98, 45.0/98, 58.0/98,
    16.0/98, 17.0/98, 20.0/98, 25.0/98, 32.0/98, 41.0/98, 52.0/98, 65.0/98,
    25.0/98, 26.0/98, 29.0/98, 34.0/98, 41.0/98, 50.0/98, 62.0/98, 74.0/98,
    36.0/98, 37.0/98, 40.0/98, 45.0/98, 52.0/98, 62.0/98, 72.0/98, 85.0/98,
    49.0/98, 50.0/98, 53.0/98, 58.0/98, 65.0/98, 74.0/98, 85.0/98, 98.0/98  })),
score(0) {}

void SolversIndividuals::init_at_random() {
    for (int i = 0; i != 64; ++i) {
        this->win[i] = std::min(1.0, std::max(0.0, initiator_()));
        this->lose[i] = std::min(1.0, std::max(0.0, initiator_()));
    }
}

SolversIndividuals::SolversIndividuals(std::vector<double> &win_,
                                       std::vector<double> &lose_) :
                                       win(win_),
                                       lose(lose_),
                                       score(0) {}


bool operator<(SolversIndividuals const& s1, SolversIndividuals const& s2) {
    /*
     * We sort by the probability of dying i.e
     * we want to keep solvers with the highest score
     */
    return (s1.score > s2.score);
}

SolversIndividuals& SolversIndividuals::operator=(
        const SolversIndividuals &other) {
    if (this == &other)
        return *this;

    win = other.win;
    lose = other.lose;
    score = other.score;
    return *this;
}

std::vector<double> SolversIndividuals::get_win() {
    return this->win;
}

std::vector<double> SolversIndividuals::get_lose() {
    return this->lose;
}

void SolversIndividuals::set_win(const std::vector<double> &win_) {
    this->win = win_;
}

void SolversIndividuals::set_lose(const std::vector<double> &lose_) {
    this->lose = lose_;
}

void SolversIndividuals::set_score(const double &score_) {
    this->score = score_;
}

double SolversIndividuals::get_score() {
    return this->score;
}


void SolversIndividuals::mutate() {
    double normalisation_factor = 0;
    for (int i = 0; i != 64; ++i) {
        if (mutates()) win[i]  = std::min(1.0, std::max(0.0, win[i]  + variation()));
        if (mutates()) lose[i] = std::min(1.0, std::max(0.0, lose[i] + variation()));
    }


    /*
    const auto [min0, max0] = std::minmax_element(this->win.begin(),
                                                  this->win.end());
    const auto [min1, max1] = std::minmax_element(this->lose.begin(),
                                                  this->lose.end());
    normalisation_factor = std::max(std::abs(*min0),
                                    std::abs(*max0));
    normalisation_factor = std::max(normalisation_factor,
                                    std::max(std::abs(*min1),
                                             std::abs(*max1)));

    if (normalisation_factor != 0) {
        for (int i = 0; i != 64; ++i) {
            win[i]   /= normalisation_factor;
            lose[i] /= normalisation_factor;
        }
    }
    */
}


void SolversIndividuals::print_info() {
    constexpr auto max_precision {
        std::numeric_limits<long double>::digits10 + 1
    };

    std::cout << "Win data:\n{";
    for (int i = 0; i != 64; ++i) {
        std::cout << std::setprecision(max_precision) << win[i];
        if (i != 63)
            std::cout << ", ";
    }

    std::cout << "}\nlose data:\n{";
    for (int i = 0; i != 64; ++i) {
        std::cout << std::setprecision(max_precision) << lose[i];
        if (i != 63)
            std::cout << ", ";
    }
    std::cout << "}\n";
}

void SolversIndividuals::print_info_as_matrix() {
    constexpr auto max_precision {
        std::numeric_limits<long double>::digits10 + 2
    };

    std::cout << "Win data:\n{";
    for (int i = 0; i != 8; ++i) {
        std::cout << "{";
        for (auto j = 0; j != 8; ++j) {
            std::cout << std::setprecision(max_precision) << this->win[i*8 + j];
            if (j != 7)
                std::cout << ", ";
        }
        if (i != 7)
            std::cout << "},\n";
        else
            std::cout << "}";
    }

    std::cout << "}\nlose data:\n{";
    for (int i = 0; i != 8; ++i) {
        std::cout << "{";
        for (auto j = 0; j != 8; ++j) {
            std::cout << std::setprecision(max_precision)
                      << this->lose[i*8 + j];
            if (j != 7)
                std::cout << ", ";
        }
        if (i != 7)
            std::cout << "},\n";
        else
            std::cout << "}";
    }
    std::cout << "}";
}

void SolversIndividuals::print_info_as_matrix_to_file(std::ofstream & file) {
    constexpr auto max_precision {
        std::numeric_limits<long double>::digits10 + 2
    };

    file << "Win data:\n{";
    for (int i = 0; i != 8; ++i) {
        file << "{";
        for (auto j = 0; j != 8; ++j) {
            file << std::setprecision(max_precision) << this->win[i*8 + j];
            if (j != 7)
                file << ", ";
        }
        if (i != 7)
            file << "},\n";
        else
            file << "}";
    }

    file << "}\nlose data:\n{";
    for (int i = 0; i != 8; ++i) {
        file << "{";
        for (auto j = 0; j != 8; ++j) {
            file << std::setprecision(max_precision) << this->lose[i*8 + j];
            if (j != 7)
                file << ", ";
        }
        if (i != 7)
            file << "},\n";
        else
            file << "}";
    }
    file << "}\n\n";
}

void print_matrix(const std::vector<double> &matrix) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            std::cout << matrix[i*8 + j] << " ";
        std::cout << "\n";
    }
}


/* Defining GamePlayer class */

GamePlayer::GamePlayer() : white_player(AlphaBeta()),
                           black_player(AlphaBeta()),
                           depth(1) {}

GamePlayer::GamePlayer(int depth_) : white_player(AlphaBeta()),
                                     black_player(AlphaBeta()),
                                     depth(depth_) {}

GamePlayer::GamePlayer(SolversIndividuals &solver1,
                       SolversIndividuals &solver2,
                       int depth_) {
    set_white_player(solver1);
    set_black_player(solver2);
    depth = depth_;
}

void GamePlayer::set_white_player(SolversIndividuals &solver) {
    white_player.setPlayerToWinValue(solver.get_win());
    white_player.setPlayerToLoseValue(solver.get_lose());
}

void GamePlayer::set_black_player(SolversIndividuals &solver) {
    black_player.setPlayerToWinValue(solver.get_win());
    black_player.setPlayerToLoseValue(solver.get_lose());
}

void GamePlayer::set_depth(const int &depth_) {
    this->depth = depth_;
}

double GamePlayer::playGame() {
    int remaining_moves = MAX_NUM_MOVES;

    white_player.newGame();
    black_player.newGame();

    uint_fast64_t move;
    while ((this->white_player.stateOfGame() == NotFinished)
            && (remaining_moves > 0)) {
        move = white_player.getMove64(this->depth);
        white_player.moveWithoutVerification(move);
        black_player.moveWithoutVerification(move);
        --remaining_moves;

        if (white_player.stateOfGame() != NotFinished)
            break;

        move = black_player.getMove64(this->depth);
        this->white_player.moveWithoutVerification(move);
        this->black_player.moveWithoutVerification(move);
        --remaining_moves;
    }

    double score = 0.0;
    // std::cout << " remaining moves: " << remaining_moves<< std::endl;
    switch (white_player.stateOfGame()) {
        case NotFinished:
            score -= std::bitset<64>((white_player.getBitBoardWhite() | white_player.getBitBoardBlack()) & 0x000000000103070F).count();
            score += std::bitset<64>((white_player.getBitBoardWhite() | white_player.getBitBoardBlack()) & 0xF0E0C08000000000).count();
            return score/20;
        break;

        case Draw:
            return(0.0);
            break;

        case WhiteWon:
            return(1.0 + static_cast<double>(remaining_moves)/MAX_NUM_MOVES);
            break;

        case BlackWon:
            return(-1.0 - static_cast<double>(remaining_moves)/MAX_NUM_MOVES);
            break;

        default:
            std::cout << "Error in the switch of GamePlayer::playGame(),"
                      << " return 0.0 by default";
            return(0.0);
            break;
    }
}

void GamePlayer::print_players_info() {
    std::cout << "\n";
    auto matrix11 = this->white_player.getPlayerToWinValue();
    auto matrix12 = this->white_player.getPlayerToLoseValue();
    std::cout << "White player info:\nWin value:\n";
    print_matrix(matrix11);
    std::cout << "\nlose value:\n";
    print_matrix(matrix12);

    std::cout << "\n";
    auto matrix21 = this->black_player.getPlayerToWinValue();
    auto matrix22 = this->black_player.getPlayerToLoseValue();
    std::cout << "Black player info:\nWin value:\n";
    print_matrix(matrix21);
    std::cout << "\nlose value:\n" << std::endl;
    print_matrix(matrix22);
}

int GamePlayer::constructor_test() {
    AlphaBeta testPlayer;
    auto matrix11 = white_player.getPlayerToWinValue();
    auto matrix12 = white_player.getPlayerToLoseValue();
    auto matrix21 = testPlayer.getPlayerToWinValue();
    auto matrix22 = testPlayer.getPlayerToLoseValue();

    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 8; ++j) {
            if (matrix11[i*8 + j] != matrix21[i*8 + j]) {
                std::cout << "ko" << i << " " << j << "case 1\n";
                return(0);
            }
            if (matrix12[i*8 + j] != matrix22[i*8 + j]) {
                std::cout << "ko" << i << " " << j << "case 1\n";
                return(0);
            }
        }
    }
    std::cout << "ok ";
    return(1);
}

void write_scores(std::ofstream &file,
                  std::vector<SolversIndividuals> &population,
                  SolversIndividuals &best_to_write,
                  const int &gen) {
    file << gen << "," << best_to_write.get_score() << ",";
    for (int i = 0; i != 10; ++i) {
        file << population[i * POP_SIZE /10].get_score();
        if (i != 9)
            file << ",";
        else
            file << "," << population[POP_SIZE - 1].get_score() << "\n";
    }
}

void evol(GamePlayer *gp,
          std::vector<SolversIndividuals> &population,
          SolversIndividuals *best_player,
          bool is_white_evolving) {
    double score = -2;

    /* Do every games */
    for (int i = 0; i != POP_SIZE; ++i)  {
        if (is_white_evolving) {
            //std::cout << " -------------------------"<< std::endl;
            gp->set_white_player(population[i]);
            //std::cout << " -------------------------"<<std::endl;
            score = gp->playGame();
        } else {
            gp->set_black_player(population[i]);
            score = -gp->playGame();
        }
        population[i].set_score(score);
    }

    /* Sorts the population by their scores */
    std::sort(population.begin(), population.end());

    /* Saves the best solver */
    if (population[0].get_score() > best_player->get_score())
        *best_player = population[0];

    /* Selection */
    std::vector<double> cum_scores_normalized(POP_SIZE);
    double worst_score = population[POP_SIZE - 1].get_score();
    cum_scores_normalized[0] = population[0].get_score() - worst_score;
    for (int i = 1; i != POP_SIZE; ++i)
        cum_scores_normalized[i] = cum_scores_normalized[i-1]
                                    + population[i].get_score()
                                    - worst_score;

    double step_length = cum_scores_normalized[POP_SIZE -1]*SELECTION_RATIO /((double) POP_SIZE);
    double position    = 0.0;
    int j              = 0;  /* Current solver taken for next gen */
    /* Starts at 1 to set population[0] to be the best player so far */
    for (int i = POP_SIZE - 2; i != 0; --i) {
        if (position > cum_scores_normalized[j])
            ++j;
        population[i] = population[j];
        position     += step_length;
    }
    population[POP_SIZE - 1] = *best_player;

    /*
    population[POP_SIZE/2 -1] = *best_player;//best player always reproduce
    for (int i =  POP_SIZE/2; i != POP_SIZE; ++i) {
        population[i] = population[i - POP_SIZE/2];
    }
    */
    /*Reproduction*/

    /*Mutation*/
    for (int i = 0; i != POP_SIZE; ++i) {
        population[i].mutate();
    }
}
