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
     * @details
     * This matrix indicates the values we will assign to each
     * pawns when we compute the heuristic value for the player
     * we are playing for.
     */
    std::vector<double> player_to_win_value_;
    /*!
     * @details
     * This matrix indicates the values we will assign to each
     * pawns when we compute the heuristic value for the player
     * we are playing for.
     */
    std::vector<double> player_to_lose_value_;
    /*! @details
     * This map is used to get values without having to get indexes from bitmasks
     * on \ref player_to_win_value_.
     */
    boost::unordered_map<uint_fast64_t, double> player_to_win_value_map_white_;
    /*! @details
     * This map is used to get values without having to get indexes from bitmasks
     * on \ref player_to_win_value_.
     */
    boost::unordered_map<uint_fast64_t, double> player_to_win_value_map_black_;
    /*! @details
     * This map is used to get values without having to get indexes from bitmasks
     * on \ref player_to_win_value_.
     */
    boost::unordered_map<uint_fast64_t, double> player_to_lose_value_map_white_;
    /*! @details
     * This map is used to get values without having to get indexes from bitmasks
     * on \ref player_to_win_value_.
     */
    boost::unordered_map<uint_fast64_t, double> player_to_lose_value_map_black_;

    /*! @details Indicates which player we are playing for. */
    Player maximizing_player_;
    /*! @details Contains the best move we found so far. */
    uint_fast64_t best_move_;

    /*! @details Transposition table used to store the results of previous searches. */
    boost::unordered_map<uint_fast64_t, std::pair<double, int>> transposition_table_;
    /*! @details Iterator used to find elements through the transposition table. */
    boost::unordered_map<uint_fast64_t, std::pair<double, int>>::iterator it_transposition_table_;
    /*! @details Map of pre-computed optimal openings. */
    std::array<boost::unordered_map<bitBoards_t, uint_fast64_t , bitBoardsHasher, bitBoardsEqual>, 2> opening_;
    /*! @details Tensorflow model used by @ref tensorflowOrderMoves. */
    cppflow::model *model = new cppflow::model("model");

    /*! @details The current heuristic value. It avoids to compute it from scratch at each terminating node. */
    double heuristic_value_;
    /*!@details The depth asked for. */
    int fullDepth_;
    /*!
     * @details
     * Indicates if according to previous searches, a player can be sure to win.
     * It is used to compute shortest path to win in late game. */
    std::array<bool, 2> won_ = {false, false};
    /*! @details Indicates if there is a jump from (i, j) to (k, l). */
    boost::unordered_map<uint_fast64_t, std::vector<uint_fast64_t>> possible_elementary_move_;
    /*! @details Function used to compare moves for sorting. */
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


    /*! @details
     * The function computes a heuristic value for the current game state
     * by evaluating the positions of the pawns on the board.
     * This value is computed using a linear function generated
     * by genetic evolution.
     * @sa tensorflowOrderMoves
     * @return The heuristic value of the current position.
     */
    double heuristicValue();
    /*!
     * @details
     * This function updates the heuristic value of the current game state
     * by adding or subtracting the value of the pawn moved in the last move.
     * @param move Said move.
     */
    inline void updateHeuristicValue(const uint_fast64_t &move);
    /*!
     * @details
     * This function updates the heuristic value of the current game state
     * by adding or subtracting the value of the pawn moved in the last move.
     * @param move Said move.
     */
    inline void updateHeuristicValueBack(const uint_fast64_t &move);
    /*! @details
     * The moves are ordered using a tensorflow model that estimates
     * the value that the alpha-beta algorithm would assign to each move.
     * @param possible_moves The list of moves we could play.
     * @sa heuristicValue
     * @sa availableMoves
     */
    void tensorflowSortMoves(std::vector<uint_fast64_t> &possible_moves);
    /*!
     * @details Returns a representation of a given bit board as a vector.
     * @param bb The bit boards considered.
     * @return A representation of @ref bb as a vector.
     */
    std::vector<uint8_t> bitBoardsAsVector(const bitBoards_t &bb);

    /*!
     * @details Compute the full path of a move from a simple bit mask.
     * @param move
     * @return the full path of a move.
     */
    ListOfPositionType retrieveMoves(const uint_fast64_t &move);

 public:
    /* Constructors */
    /*! @details
     * Construct the object.
     * @sa  AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
             const std::vector< std::vector<double> > &player_to_lose_value_)
     */
    AlphaBeta();
    /*! @details
     * Construct the object.
     * @param player_to_win_value_
     * @param player_to_lose_value_
     * @sa  AlphaBeta()
     */
    AlphaBeta(const std::vector<double> &player_to_win_value_,
              const std::vector<double> &player_to_lose_value_);

    /*!
     * @details
     * This is a method that returns the best move the AI agent can make at a given
     * depth in the game tree using the alpha-beta pruning algorithm.
     * The first check in the method is to see if the current game state is part of
     * the opening book. If so, the method retrieves the pre-calculated moves for this position.
     * Otherwise, it calls the @ref getMove64 method to calculate the best move at the given
     * depth using the alpha-beta pruning algorithm. Once the best move has been determined,
     * the method calls @ref retrieveMoves to convert the bitboard representation of the move
     * into a more human-readable format before returning the result.
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
    /*! @details
     * This is a method that returns the best move the AI agent can make at a given
     * depth in the game tree using the alpha-beta pruning algorithm.
     * This function implements the Alpha-Beta search algorithm to find the best move in a game state.
     * It uses of zero width windows heuristic when a sure win has been detected which makes the end-game
     * faster and improves a lot the quality of the player.
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
    /*! @details
     * This function calculates all available moves for the current player
     * and stores them in the result vector.
     * The result vector is passed as a reference so that it can be modified
     * inside the function.
     * @sa getMove
     * @sa tensorflowOrderMoves
     */
    void availableMoves(std::vector<uint_fast64_t> &result);

    /*! @details
     * This methode performs an Alpha-Beta search in the game tree to find the best move
     * for a given game state. It evaluates each possible move and calls the @ref AlphaBetaEval
     * function recursively on the next depth level in the game tree.
     * The code updates the game state and calculates the heuristic value for the new position.
     * It also updates the Zobrist hash and checks for an illegal position.
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

    /*! @details A helper function for the python connexion */
    bool isHuman() { return false; }

    /*! @details
     * This function loads the pre-calculated opening moves from a file and
     * stores them in the @ref opening_ map for later use in the alpha-beta algorithm.
     */
    void loadOpenings();

    /*!
     * @details Returns \ref maximizing_player_.
     * @return @ref maximizing_player_.
     */
    Player getMaximizingPlayer() const;
    /*!
     * @details Returns \ref player_to_lose_value_.
     * @return @ref player_to_lose_value_.
     */
    std::vector<double> getPlayerToLoseValue();
    /*!
     * @details Returns \ref player_to_win_value_.
     * @return @ref player_to_win_value_.
     */
    std::vector<double> getPlayerToWinValue();
    /*!
     * @details Sets \ref player_to_lose_value_.
     * @param player_to_lose_value_ The value to set.
     */
    void setPlayerToLoseValue(const std::vector<double> &player_to_lose_value_);
    /*!
     * @details Sets \ref player_to_win_value_.
     * @param player_to_win_value_ The value to set.
     */
    void setPlayerToWinValue(const std::vector<double> &player_to_win_value_);
};

#endif  // SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_
