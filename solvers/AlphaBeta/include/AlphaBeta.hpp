/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file AlphaBeta.hpp
 * \brief Alpha-Beta algorithm declaration.
 *
 * Declaration of the AlphaBeta Class which is an implementation of the alpha-beta pruning algorithm for the chinese checkers.
 *
 */

#ifndef SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_
#define SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_

/* C Libraries */
#include <stdint.h>

/* C++ libraries */
#include <utility>
#include <map>
#include <vector>
#include <unordered_map>

/* Other */
#include "Types.hpp"
#include "ChineseCheckers.hpp"

class AlphaBeta : public ChineseCheckers{
 protected:
    double heuristicValue();

    /* functions used for the transposition tables */
    /* FNV-1a hash function */
    inline uint64_t fnv1aColor(uint64_t h, const Color &x);
    inline uint64_t fnv1a(uint64_t h, const int &x);
    uint64_t hashMatrix(const GridType &matrix, const int &player);
    inline uint64_t hashMove(const ListOfPositionType &move);
    inline uint64_t hashPosition(const PositionType &move);

    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    std::vector<std::vector<double> > player_to_win_value_;

    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    std::vector<std::vector<double> > player_to_loose_value_;

    double evaluate(const Player &player);

    void reverseMove(const ListOfPositionType &move);

    Player maximizing_player_;
    ListOfPositionType best_move_;

    /* transposition table to store the results of previous searches */
    std::unordered_map<unsigned long long, std::pair<int, int>> transTable;

 public:
    double AlphaBetaEval(const int &depth,
                      double alpha,
                      double beta,
                      const bool &maximizingPlayer,
                      const bool &keepMove);

    ListOfMoves availableMoves(const Player &player, const bool &full);
    ListOfPositionType getMove(const int &depth, const double &alpha, const double &beta);
    bool isHuman() {return false; }
    AlphaBeta();
    AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
                         const std::vector< std::vector<double> > &player_to_loose_value_);

    Player get_maximizing_player_() const;
    std::vector<std::vector<double> > get_player_to_loose_value_();
    std::vector<std::vector<double> > get_player_to_win_value_();
    void set_player_to_loose_value_(std::vector< std::vector<double> > &player_to_loose_value_);
    void set_player_to_win_value_(  std::vector< std::vector<double> > &player_to_win_value_  );
};

#endif  // SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_
