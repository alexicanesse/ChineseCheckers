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

#define TF_CPP_MIN_LOG_LEVEL 3

/* C Libraries */
#include <stdint.h>/*
#include <tensorflow/cc/saved_model/loader.h>
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/public/session_options.h>
#include <tensorflow/core/framework/logging.h>*/
#include <cppflow/cppflow.h>

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
    std::unordered_map<unsigned long long, std::pair<double, int>> transTable;

    cppflow::model *model = new cppflow::model("./model");
    //tensorflow::SavedModelBundleLite *savedModelBundle = new tensorflow::SavedModelBundleLite;
    void tensorflowOrderMoves(ListOfMoves &possible_moves);
    void sortDepth1(ListOfMoves &possible_moves);
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

//temp
    int rank;
    int number_of_moves;
};

#endif  // SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_
