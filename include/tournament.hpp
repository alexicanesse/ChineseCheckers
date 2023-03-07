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

#ifndef INCLUDE_TOURNAMENT_HPP_
#define INCLUDE_TOURNAMENT_HPP_

/* C libraries */
#include <numbers>

/* C++ libraries */
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <limits>
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
void print_matrix(const std::vector<double> &matrix);

/*!
 * @brief This class is designed to represent individuals in a solver population
 * and contains various methods for manipulating and printing their characteristics.
 */
class SolversIndividuals {
 private:
    /*!
     * @brief The matrix used to compute the heuristic value for the player we are playing.
     * @sa lose
     * @sa set_win
     * @sa set_lose
     * @sa get_win
     * @sa get_lose
     * */
    std::vector<double> win;
    /*!
     * @brief The matrix used to compute the heuristic value for the adversary
     * of the player we are playing.
     * @sa win
     * @sa set_win
     * @sa set_lose
     * @sa get_win
     * @sa get_lose
     */
    std::vector<double> lose;
    /*!
     * @brief The score the individual got at its last game.
     * @sa set_score
     */
    double score;

 public:
    /*! @brief Default constructor */
    SolversIndividuals();
    /*! @brief Constructor used to set @ref win and @ref lose. */
    SolversIndividuals(std::vector<double> &win_, std::vector<double> &lose_);

    /*!
     * @brief Operator used to compare two @ref SolversIndividuals
     * @sa operator=
     */
    friend bool operator<(SolversIndividuals const &s1,
                          SolversIndividuals const &s2);
    /*!
     * @brief Operator used to compare two @ref SolversIndividuals
     * @sa operator<
     */
    SolversIndividuals& operator=(const SolversIndividuals & other);

    /*! @brief
     * Returns @ref win.
     * @return @ref win
     * @sa win
     * @sa lose
     * @sa set_win
     * @sa set_lose
     * @sa get_lose
     */
    std::vector<double> get_win();
    /*! @brief
     * Returns @ref lose.
     * @return @ref lose
     * @sa win
     * @sa lose
     * @sa set_win
     * @sa set_lose
     * @sa get_win
     */
    std::vector<double> get_lose();
    /*! @brief
     * Returns @ref score.
     * @return @ref score
     */
    double get_score();

    /*!
     * @brief Inits @ref win and @ref lose at random.
     * @sa set_win
     * @sa set_lose
     * @sa get_win
     * @sa get_lose
     */
    void init_at_random();
    /*!
     * @brief Sets @ref win with the parameter.
     * @param win_ The matrix we want to set.
     * @sa init_at_random
     * @sa win
     * @sa lose
     * @sa set_lose
     * @sa get_win
     * @sa get_lose
     */
    void set_win(const std::vector<double> &win_);
    /*!
     * @brief Sets @ref lose with the parameter.
     * @param lose_ The matrix we want to set.
     * @sa init_at_random
     * @sa win
     * @sa lose
     * @sa set_win
     * @sa get_win
     * @sa get_lose
     */
    void set_lose(const std::vector<double> &lose_);
    /*!
     * @brief Sets @ref score with the parameter.
     * @param score_ The score we want to set.
     * @sa score
     * @sa get_score
     */
    void set_score(const double &score_);

    /*! @brief Mutates a solver. */
    void mutate();

    /*!
     * @brief Print information about the @ref SolversIndividuals.
     * @sa print_info_as_matrix
     * @sa print_info_as_matrix_to_file
     */
    void print_info();
    /*! @brief Prints information on @ref win and @ref lose.
     * @sa print_info
     * @sa print_info_as_matrix_to_file
     */
    void print_info_as_matrix();
    /*!
     * @brief Prints information on @ref win and @ref lose to a file given
     * as a parameter.
     * @param file A pointer to the file we want to print information to.
     * @sa print_info
     * @sa print_info_as_matrix
     */
    void print_info_as_matrix_to_file(std::ofstream &file);
};

/*! @brief This class simulates a game between two solvers. */
class GamePlayer {
 private:
    /*! @brief A solver using @ref AlphaBeta. */
    AlphaBeta white_player;
    /*! @brief A solver using @ref AlphaBeta. */
    AlphaBeta black_player;
    /*! @brief The depth @ref AlphaBeta will use. */
    int depth;

 public:
    /*! @brief Default constructor. */
    GamePlayer();
    /*! @brief A constructor used to set a custom @ref depth. */
    GamePlayer(int depth_);
    /*! @brief A constructor used to set custom solvers and a custom depth. */
    GamePlayer(SolversIndividuals &solver1,
               SolversIndividuals &solver2,
               int depth_ = 1);

    /*!
     * @brief Member used to simulate a game between
     * @ref white_player and @ref black_player.
     */
    double playGame();

    /*!
     * @brief Sets @ref white_player.
     * @param solver The solver to set.
     * @sa set_black_player
     * @sa set_depth
     */
    void set_white_player(SolversIndividuals &solver);
    /*!
     * @brief Sets @ref black_player.
     * @param solver The solver to set.
     * @sa set_white_player
     * @sa set_depth
     */
    void set_black_player(SolversIndividuals &solver);
    /*!
     * @brief Sets @ref depth.
     * @param depth_ The depth to set.
     * @sa set_white_player
     * @sa set_black_player
     */
    void set_depth(const int &depth_);

    /*! @brief Prints information on the players. */
    void print_players_info();

    /*! @brief Debug function used to test the constructors. */
    int constructor_test();
};

void write_scores(std::ofstream &file,
                  std::vector<SolversIndividuals> &population,
                  SolversIndividuals &best_to_write,
                  const int &gen);

void evol(GamePlayer *gp,
          std::vector<SolversIndividuals> &population,
          SolversIndividuals *best_player,
          bool is_white_evolving);

#endif  // INCLUDE_TOURNAMENT_HPP_
