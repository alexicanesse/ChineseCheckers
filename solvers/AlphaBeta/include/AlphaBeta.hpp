/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * @file AlphaBeta.hpp
 * @brief Alpha-Beta algorithm declaration.
 *
 * Declaration of the AlphaBeta Class which is an implementation of the alpha-beta pruning algorithm for the chinese checkers.
 *
 */

#ifndef SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_
#define SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_

#define TF_CPP_MIN_LOG_LEVEL 3

/* C Libraries */
#include <stdint.h>
#include <cppflow/cppflow.h>

/* C++ libraries */
#include <utility>
#include <map>
#include <vector>
#include <unordered_map>
#include <boost/unordered_map.hpp>

/* Other */
#include "Types.hpp"
#include "ChineseCheckers.hpp"

/*!
 * @brief
 * The AlphaBeta class inherits from the ChineseCheckers class and provides an implementation of the alpha-beta
 * algorithm with various optimizations for playing Chinese Checkers. The class provides several methods for
 * evaluating the heuristic value of a position, ordering moves, and storing the results of previous searches
 * in a transposition table.
 */
class AlphaBeta : public ChineseCheckers{
 protected:
    /*!
     * @brief
     * This matrix indicates the values we will assign to each
     * pawns when we compute the heuristic value for the player
     * we are playing for.
     */
    std::vector<double> player_to_win_value_;
    /*!
     * @brief
     * This matrix indicates the values we will assign to each
     * pawns when we compute the heuristic value for the player
     * we are playing for.
     */
    std::vector<double> player_to_lose_value_;
    /*! @brief
     * This map is used to get values without having to get indexes from bitmasks
     * on \ref player_to_win_value_.
     */
    boost::unordered_map<uint_fast64_t, double> player_to_win_value_map_white_;
    /*! @brief
     * This map is used to get values without having to get indexes from bitmasks
     * on \ref player_to_win_value_.
     */
    boost::unordered_map<uint_fast64_t, double> player_to_win_value_map_black_;
    /*! @brief
     * This map is used to get values without having to get indexes from bitmasks
     * on \ref player_to_win_value_.
     */
    boost::unordered_map<uint_fast64_t, double> player_to_lose_value_map_white_;
    /*! @brief
     * This map is used to get values without having to get indexes from bitmasks
     * on \ref player_to_win_value_.
     */
    boost::unordered_map<uint_fast64_t, double> player_to_lose_value_map_black_;

    /*! Indicates which player we are playing for. */
    Player maximizing_player_;
    /*! Contains the best move we found so far. */
    uint_fast64_t best_move_;

    /*! Transposition table used to store the results of previous searches. */
    boost::unordered_map<uint_fast64_t, std::pair<double, int>> transposition_table_;
    /*! Iterator used to find elements through the transposition table. */
    boost::unordered_map<uint_fast64_t, std::pair<double, int>>::iterator it_transposition_table_;
    /*! Map of pre-computed optimal openings. */
    boost::unordered_map<bitBoards_t, uint_fast64_t , bitBoardsHasher, bitBoardsEqual> opening_;
    /*! Tensorflow model used by @ref tensorflowOrderMoves. */
    cppflow::model *model = new cppflow::model("model");

    /*! A map used to get the number of trailing zeroes of uint_64. It is used to get index from bit masks. */
    boost::unordered_map<uint_fast64_t, int> ctz_;

    /*! The current heuristic value. It avoids to compute it from scratch at each terminating node. */
    double heuristic_value_;
    /*! The depth asked for. */
    int fullDepth_;
    /*!
     * Indicates if according to previous searches, a player can be sure to win.
     * It is used to compute shortest path to win in late game.
     */
    std::array<bool, 2> won_ = {false, false};
    /*! Indicates if there is a jump from (i, j) to (k, l). */
    boost::unordered_map<uint_fast64_t, std::vector<uint_fast64_t>> possible_elementary_move_;
    /*! Function used to compare moves for sorting. */
    std::function<bool(const uint_fast64_t&, const uint_fast64_t&)> comp_move_ =
            [this](const uint_fast64_t &a,
                   const uint_fast64_t &b){
                if (who_is_to_play_) {
                    return player_to_win_value_map_black_[a & ~bit_boards_.Black]
                           + player_to_win_value_map_black_[b & bit_boards_.Black]
                           < player_to_win_value_map_black_[b & ~bit_boards_.Black]
                             + player_to_win_value_map_black_[a & bit_boards_.Black];
                } else {
                    return player_to_win_value_map_white_[a & ~bit_boards_.White]
                           + player_to_win_value_map_white_[b & bit_boards_.White]
                           < player_to_win_value_map_white_[b & ~bit_boards_.White]
                             + player_to_win_value_map_white_[a & bit_boards_.White];
                }
            };


    /*! @brief
     * A member used to estimate the value of a position.
     * @details
     * This heuristic value is computed using a linear function generated
     * by genetic evolution.
     * @sa tensorflowOrderMoves
     * @return The heuristic value of the current position.
     */
    double heuristicValue();
    /*!
     * @brief Updates \ref heuristic_value_ for a move.
     * @param move Said move.
     */
    inline void updateHeuristicValue(const uint_fast64_t &move);
    /*!
     * @brief Updates \ref heuristic_value_ when a move is canceled.
     * @param move Said move.
     */
    inline void updateHeuristicValueBack(const uint_fast64_t &move);
    /*! @brief
     * A member used to order the moves before exploring them.
     * @details
     * The moves are ordered using a tensorflow model that estimates
     * the value that the alpha-beta algorithm would assign to each move.
     * @param possible_moves The list of moves we could play.
     * @sa heuristicValue
     * @sa availableMoves
     */
    void tensorflowSortMoves(std::vector<uint_fast64_t> &possible_moves);
    /*!
     * @brief Returns a representation of a given bit board as a vector.
     * @param bb The bit boards considered.
     * @return A representation of @ref bb as a vector.
     */
    std::vector<uint8_t> bitBoardsAsVector(const bitBoards_t &bb);

    /*!
     * @brief Compute the full path of a move from a simple bit mask.
     * @param move
     * @return the full path of a move.
     */
    ListOfPositionType retrieveMoves(const uint_fast64_t &move);

 public:
    /* Constructors */
    /*! @brief
     * A simple constructor.
     * @sa  AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
             const std::vector< std::vector<double> > &player_to_lose_value_)
     */
    AlphaBeta();
    /*! @brief
     * A simple constructor.
     * @param player_to_win_value_
     * @param player_to_lose_value_
     * @sa  AlphaBeta()
     */
    AlphaBeta(const std::vector<double> &player_to_win_value_,
              const std::vector<double> &player_to_lose_value_);

    /*! @brief
     * Gives the best move according to the alpha beta algorithm.
     * @param depth Indicates how deep we should explore the tree.
     * @param alpha Check the Alpha-Beta algorithm to know what this is.
     * @param beta Check the Alpha-Beta algorithm to know what this is.
     * @sa getMove64
     * @sa AlphaBetaEval
     * @sa availableMoves
     * @sa heuristicValue
     * @sa updateHeuristicValue
     * @sa updateHeuristicValueBack
     * @return The best move according to the alpha beta algorithm.
     */
    ListOfPositionType getMove(const int &depth, const double &alpha, const double &beta);
    /*! @brief
     * Gives the best move according to the alpha beta algorithm.
     * @param depth Indicates how deep we should explore the tree.
     * @sa getMove
     * @sa AlphaBetaEval
     * @sa availableMoves
     * @sa heuristicValue
     * @sa updateHeuristicValue
     * @sa updateHeuristicValueBack
     * @return The best move according to the alpha beta algorithm.
     */
    uint_fast64_t getMove64(const int &depth);
    /*! @brief
     * Gives the list of available moves.
     * @sa getMove
     * @sa tensorflowOrderMoves
     */
    void availableMoves(std::vector<uint_fast64_t> &result);

    /*! @brief
     * Makes a step of the alpha beta algorithm.
     * @param depth Indicates how deep we should explore the tree.
     * @param alpha Check the Alpha-Beta algorithm to know what this is.
     * @param beta Check the Alpha-Beta algorithm to know what this is.
     * @param maximizingPlayer Indicates if the current player if the maximizing player.
     * @param keepMove indicates if the best move from the current depth should be kept.
     * @sa getMove
     * @sa getMove64
     * @sa availableMoves
     * @sa heuristicValue
     * @return The value computed by the alpha beta algorithm. Sets \ref best_move_ if asked to.
     */
    const double AlphaBetaEval(const int &depth,
                               double alpha,
                               double beta,
                               const bool &maximizingPlayer,
                               const bool &keepMove,
                               uint_fast64_t hash);

    /*! @brief A helper function for the python connexion */
    bool isHuman() { return false; }

    /*! @brief
     * A member loading the pre-computed optimal openings into \ref opening_.
     */
    void loadOpenings();

    /*!
     * @brief Returns \ref maximizing_player_.
     * @return @ref maximizing_player_.
     */
    Player getMaximizingPlayer() const;
    /*!
     * @brief Returns \ref player_to_lose_value_.
     * @return @ref player_to_lose_value_.
     */
    std::vector<double> getPlayerToLoseValue();
    /*!
     * @brief Returns \ref player_to_win_value_.
     * @return @ref player_to_win_value_.
     */
    std::vector<double> getPlayerToWinValue();
    /*!
     * @brief Sets \ref player_to_lose_value_.
     * @param player_to_lose_value_ The value to set.
     */
    void setPlayerToLoseValue(const std::vector<double> &player_to_lose_value_);
    /*!
     * @brief Sets \ref player_to_win_value_.
     * @param player_to_win_value_ The value to set.
     */
    void setPlayerToWinValue(const std::vector<double> &player_to_win_value_);

//temp
    int rank;
    int number_of_moves;
};

#endif  // SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_
