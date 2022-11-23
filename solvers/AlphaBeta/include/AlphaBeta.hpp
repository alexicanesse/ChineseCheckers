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

/* C++ libraries */
#include <utility>
#include <map>
#include <vector>

/* Other */
#include "Types.hpp"
#include "ChineseCheckers.hpp"

class AlphaBeta : public ChineseCheckers{
 protected:
    int AlphaBetaEval(const int depth,
                      double alpha,
                      double beta,
                      const bool maximizingPlayer,
                      const bool keepMove);
    int heuristicValue();

    /* this is meant to be seen from black perspective: white should
 * use symmetries to use this matrix. */
    std::vector<std::vector<double> > player_to_win_value_;

    /* this is meant to be seen from black perspective: white should
 * use symmetries to use this matrix. */
    std::vector<std::vector<double> > player_to_loose_value_;

    int evaluate(Player player);

    Player maximizing_player_;
    ListOfPositionType best_move_;

 public:
    ListOfMoves availableMoves(Player player);
    ListOfPositionType getMove(int depth, double alpha, double beta);
    bool isHuman() {return false; }
    AlphaBeta();
};

#endif /* SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_ */
