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
 * An implementation of the alpha-beta pruning algorithm for the chinese checkers.
 */
class AlphaBeta : public ChineseCheckers{
 protected:
    /*! @brief
     * A member used to estimate the value of a position
     * @details
     * This heuristic value is computed using a linear function generated
     * by genetic evolution
     */
    /*!
     * @sa evaluate(const Player &player)
     * @sa tensorflowOrderMoves(ListOfMoves &possible_moves)
     * @sa sortDepth1(ListOfMoves &possible_moves)
     * @return The heuristic value of the current position
     */
    double heuristicValue();
    inline void updateHeuristicValue(const uint_fast64_t &move);
    inline void updateHeuristicValueBack(const uint_fast64_t &move);

    /*! @brief
     * A member used to order the moves before exploring them
     * @details
     * The moves are ordered using a tensorflow model that estimates
     * the value that the alpha-beta algorithm would assign to each move
     */
    /*!
     * @param possible_moves The list of moves we could play
     * @sa heuristicValue()
     * @sa evaluate(const Player &player)
     * @sa sortDepth1(ListOfMoves &possible_moves)
     * @sa availableMoves(const Player &player, const bool &full)
     */
    void tensorflowOrderMoves(ListOfMoves &possible_moves);

    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    /*! This matrix indicates the values we will assign to each
     * pawns when we compute the heuristic value for the player
     * we are playing for */
    std::vector<std::vector<double> > player_to_win_value_;
    boost::unordered_map<uint_fast64_t, double> player_to_win_value_map_white_;
    boost::unordered_map<uint_fast64_t, double> player_to_win_value_map_black_;

    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    /*! This matrix indicates the values we will assign to each
     * pawns when we compute the heuristic value for the player
     * we are playing against */
    std::vector<std::vector<double> > player_to_lose_value_;
    boost::unordered_map<uint_fast64_t, double> player_to_lose_value_map_white_;
    boost::unordered_map<uint_fast64_t, double> player_to_lose_value_map_black_;

    /*! Indicates which plyaer we are playing for */
    Player maximizing_player_;

    /*! Contains the best move we found so far */
    uint_fast64_t best_move_;

    /*! Transposition table used to store the results of previous searches */
    boost::unordered_map<std::pair<uint_fast64_t, uint_fast64_t>, std::pair<double, int>> transTable;
    boost::unordered_map<std::pair<uint_fast64_t, uint_fast64_t>, std::pair<double, int>>::iterator it;

    /*! Map of pre-computed optimal openings */
    boost::unordered_map<uint_fast64_t, uint_fast64_t> opening;

    /*! Tensorflow model used by @ref tensorflowOrderMoves(ListOfMoves &possible_moves) */
    cppflow::model *model = new cppflow::model("./raw_data/model");

    boost::unordered_map<uint_fast64_t, int> ctz;

    double heuristic_value_;
    int fullDepth_;

    /*! Indicates if there is a jump from (i, j) to (k, l) */
    boost::unordered_map<uint_fast64_t, std::vector<uint_fast64_t>> possible_elementary_move;

    std::function<bool(const uint_fast64_t&, const uint_fast64_t&)> compMoveVect =
            [this](const uint_fast64_t &a,
                   const uint_fast64_t &b){
                if (who_is_to_play_) {
                    return player_to_win_value_map_black_[a & ~bitBoardBlack]
                           + player_to_win_value_map_black_[b & bitBoardBlack]
                           < player_to_win_value_map_black_[b & ~bitBoardBlack]
                             + player_to_win_value_map_black_[a & bitBoardBlack];
                } else {
                    return player_to_win_value_map_white_[a & ~bitBoardWhite]
                           + player_to_win_value_map_white_[b & bitBoardWhite]
                           < player_to_win_value_map_white_[b & ~bitBoardWhite]
                             + player_to_win_value_map_white_[a & bitBoardWhite];
                }
            };
 public:
    /* Constructors */
    /*! @brief
     * A simple constructor
     */
    /*!
     * @sa  AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
             const std::vector< std::vector<double> > &player_to_lose_value_)
     */
    AlphaBeta();

    /*! @brief
     * A simple constructor
     */
    /*!
     * @param player_to_win_value_
     * @param player_to_lose_value_
     * @sa  AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
             const std::vector< std::vector<double> > &player_to_lose_value_)
     */
    AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
              const std::vector< std::vector<double> > &player_to_lose_value_);


    /*! @brief
     * A member loading the pre-computed optimal openings into a map
     */
    void loadOpenings();

    /*! @brief
     * Gives the list of available moves
     */
    /*!
     * @sa  getMove(const int &depth, const double &alpha, const double &beta)
     * @sa tensorflowOrderMoves(ListOfMoves &possible_moves)
     * @sa sortDepth1(ListOfMoves &possible_moves)
     * @return The list of available moves
     */
    void availableMoves(std::vector<uint_fast64_t> &result);

    /*! @brief
     * Gives the list of available moves
     */
    /*!
     * @sa  getMove(const int &depth, const double &alpha, const double &beta)
     * @sa tensorflowOrderMoves(ListOfMoves &possible_moves)
     * @sa sortDepth1(ListOfMoves &possible_moves)
     * @return The list of available moves
     */
    /*! @brief
     * Gives the best move according to the alpha beta algorithm
     */
    /*!
     * @param depth Indicates how deep we should explore the tree
     * @param alpha
     * @param beta
     * @sa AlphaBetaEval(const int &depth,
                      double alpha,
                      double beta,
                      const bool &maximizingPlayer,
                      const bool &keepMove)
     * @sa availableMoves(const Player &player, const bool &full)
     * @sa heuristicValue()
     * @sa evaluate(const Player &player)
     * @return The best move according to the alpha beta algorithm
     */
    ListOfPositionType getMove(const int &depth, const double &alpha, const double &beta);
    uint_fast64_t getMove64(const int &depth);

    /*! @brief
     * Makes a step of the alpha beta algorithm
     */
    /*!
     * @param depth Indicates how deep we should explore the tree
     * @param alpha
     * @param beta
     * @param maximizingPlayer Indicates if the current player if the maximizing player
     * @param keepMove indicates if the best move from the current depth should be kept
     * @sa getMove(const int &depth, const double &alpha, const double &beta)
     * @sa availableMoves(const Player &player, const bool &full)
     * @sa heuristicValue()
     * @sa evaluate(const Player &player)
     * @return The value computed by the alpha beta algorithm. Sets the best move if asked to
     */
    const double AlphaBetaEval(const int &depth,
                      double alpha,
                      double beta,
                      const bool &maximizingPlayer,
                      const bool &keepMove);

    /*! @brief
     * Cancel the last move
     */
    /*!
     * @param move The move to cancel
     */
    void reverseMove(const uint_fast64_t &move);

    /*! @brief
     * A helper function for the python connexion
     */
    bool isHuman() { return false; }

    /*! @brief
     * A simple get
     */
    /*!
     * @return @ref maximizing_player_
     */
    Player get_maximizing_player_() const;

    /*! @brief
     * A simple get
     */
    /*!
     * @return @ref player_to_lose_value_
     */
    std::vector<std::vector<double> > get_player_to_lose_value_();

    /*! @brief
     * A simple get
     */
    /*!
     * @return @ref player_to_win_value_
     */
    std::vector<std::vector<double> > get_player_to_win_value_();

    /*! @brief
     * A simple set
     */
    /*!
     * @param player_to_lose_value_
     */
    void set_player_to_lose_value_(std::vector< std::vector<double> > &player_to_lose_value_);

    /*! @brief
     * A simple set
     */
    /*!
     * @param player_to_win_value_
     */
    void set_player_to_win_value_(  std::vector< std::vector<double> > &player_to_win_value_  );

//temp
    int rank;
    int number_of_moves;

    inline uint64_t hashGrid();
    ListOfPositionType retrieveMoves(const uint_fast64_t &move);
    std::array<bool, 2> won_ = {false, false};
};

#endif  // SOLVERS_ALPHABETA_INCLUDE_ALPHABETA_HPP_
