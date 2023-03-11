/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file AlphaBeta.cpp
 * \brief Alpha-Beta algorithm implementation.
 *
 * Implementation of the AlphaBeta Class which is an implementation of the alpha-beta pruning algorithm for the chinese checkers.
 *
 */

#define PLUS_INFTY (20)
#define MINUS_INFTY (-20)
#define DRAW_VALUE (10)


/* AlphaBeta.hpp */
#include "AlphaBeta.hpp"

/* C Libraries */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cppflow/cppflow.h>

/* C++ Libraries */
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <fstream>
#include <iomanip>
#include <boost/unordered_map.hpp>

/* Other */
#include "Types.hpp"
#include "ChineseCheckers.hpp"

AlphaBeta::AlphaBeta() {
    /* This constructor initializes the AlphaBeta class with pre-computed values that can be used
     * to determine the value of a given chess position in terms of whether the current player is
     * likely to win or lose. These values are stored in the following two vectors: */

    /* player_to_win_value_: a vector of doubles that contains the "win value" for each of the
     * possible chess positions that can occur. The values in this vector are scaled so that they
     * range from 0 to 1. This vector is meant to be seen from the perspective of the black player. */
    player_to_win_value_ = std::vector<double>({
        00.0/98,  1.0/98,  4.0/98,  9.0/98, 16.0/98, 25.0/98, 36.0/98, 49.0/98,
        01.0/98,  2.0/98,  5.0/98, 10.0/98, 17.0/98, 26.0/98, 37.0/98, 50.0/98,
        04.0/98,  5.0/98,  8.0/98, 13.0/98, 20.0/98, 29.0/98, 40.0/98, 53.0/98,
        09.0/98, 10.0/98, 13.0/98, 18.0/98, 25.0/98, 34.0/98, 45.0/98, 58.0/98,
        16.0/98, 17.0/98, 20.0/98, 25.0/98, 32.0/98, 41.0/98, 52.0/98, 65.0/98,
        25.0/98, 26.0/98, 29.0/98, 34.0/98, 41.0/98, 50.0/98, 62.0/98, 74.0/98,
        36.0/98, 37.0/98, 40.0/98, 45.0/98, 52.0/98, 62.0/98, 72.0/98, 85.0/98,
        49.0/98, 50.0/98, 53.0/98, 58.0/98, 65.0/98, 74.0/98, 85.0/98, 98.0/98});

    /* player_to_lose_value_: a vector of doubles that contains the "lose value" for each of the
     * possible chess positions that can occur. The values in this vector are scaled so that they
     * range from 0 to 1. This vector is meant to be seen from the perspective of the black player. */
    player_to_lose_value_ = std::vector<double>({
        00.0/588,  1.0/588,  4.0/588,  9.0/588, 16.0/588, 25.0/588, 36.0/588, 49.0/588,
        01.0/588,  2.0/588,  5.0/588, 10.0/588, 17.0/588, 26.0/588, 37.0/588, 50.0/588,
        04.0/588,  5.0/588,  8.0/588, 13.0/588, 20.0/588, 29.0/588, 40.0/588, 53.0/588,
        09.0/588, 10.0/588, 13.0/588, 18.0/588, 25.0/588, 34.0/588, 45.0/588, 58.0/588,
        16.0/588, 17.0/588, 20.0/588, 25.0/588, 32.0/588, 41.0/588, 52.0/588, 65.0/588,
        25.0/588, 26.0/588, 29.0/588, 34.0/588, 41.0/588, 50.0/588, 62.0/588, 74.0/588,
        36.0/588, 37.0/588, 40.0/588, 45.0/588, 52.0/588, 62.0/588, 72.0/588, 85.0/588,
        49.0/588, 50.0/588, 53.0/588, 58.0/588, 65.0/588, 74.0/588, 85.0/588, 98.0/588});

    /* Initialize possible_elementary_move_ array to empty vectors for each board position. */
    for (int i = 0; i < 64; ++i)
        possible_elementary_move_[un_64_ << i] = std::vector<uint_fast64_t>(0);

    loadOpenings();
}

AlphaBeta::AlphaBeta(const std::vector<double> &player_to_win_value_,
                     const std::vector<double> &player_to_lose_value_) {
    /* Set member variables equal to the passed in values */
    this->player_to_win_value_ = player_to_win_value_;
    this->player_to_lose_value_ = player_to_lose_value_;

    /* Initialize possible_elementary_move_ array to empty vectors for each board position. */
    for (int i = 0; i < 64; ++i)
        possible_elementary_move_[un_64_ << i] = std::vector<uint_fast64_t>(0);

    loadOpenings();
}

void AlphaBeta::availableMoves(std::vector<uint_fast64_t> &result) {
    /* This function calculates all available moves for the current player
     * and stores them in the result vector.
     * The result vector is passed as a reference so that it can be modified
     * inside the function. */

    /* Reserve enough memory for 50 moves to avoid reallocations */
    result.reserve(50);

    /* A bitboard that keeps track of whether we have computed the possible elementary
     * moves for each position or not. */
    uint_fast64_t computed_possible_elementary_move = 0;
    /* A bitboard that represents all the pawns on the board (both black and white). */
    const uint_fast64_t bit_boards_all  = (bit_boards_.White | bit_boards_.Black);
    /* A bitboard that represents the current player's pawns. */
    const uint_fast64_t currentBitBoard = who_is_to_play_ ? bit_boards_.Black : bit_boards_.White;



    /* This part of the code handles the case of jump moves. */


    /*
     * Do a BFS to list all possible jumps.
     * Each pown is a root
     */

    /* A bitboard that stores the nodes to be visited during the BFS. */
    uint_fast64_t queue;
    /* A bitboard that stores the nodes that have been visited. */
    uint_fast64_t explored = 0;
    /* The current node being visited. */
    uint_fast64_t v;

    /* Integer coordinates of the nodes. */
    int i, j, i_neig, j_neig;
    /* Integer coordinates of the root node multiplied by 2 (to avoid floating point calculations). */
    int i_root_times_2, j_root_times_2;

    /* The current root node. */
    uint_fast64_t root;
    /* A bitboard that represents the remaining pawns to be processed. */
    uint_fast64_t pawnPositionMask = currentBitBoard;

    /* A vector that stores the possible elementary moves for each position. */
    std::vector<uint_fast64_t> temp_elementary_move;
    /* Reserve enough memory for 12 moves to avoid reallocations. */
    temp_elementary_move.reserve(12);

    /* Loop over all pawns of the current player. */
    for (root = pawnPositionMask & -pawnPositionMask;
                pawnPositionMask & -pawnPositionMask;
         root = pawnPositionMask & -pawnPositionMask) {
        /* Remove the current root from the mask of pawn positions. */
        pawnPositionMask ^= root;

        /* Initialize the queue and explored set with the current root. */
        queue    = root;
        explored = root;

        /* Get the coordinates of the root. */
        std::tie(i_root_times_2, j_root_times_2) = uint64_to_pair_[root];
        /* Multiply them by two to use them in averages. */
        i_root_times_2 <<= 1;
        j_root_times_2 <<= 1;

        /* BFS to find all possible jumps. */
        while (queue) {
            /* Get the next node to explore from the queue. */
            v = queue & -queue;
            queue ^= v;

            /* Get the coordinates of the current node. */
            std::tie(i, j) = uint64_to_pair_[v];

            /* If the possible elementary moves for this node have not been computed yet,
             * compute them and store them in the possible_elementary_move map. */
            if (!(computed_possible_elementary_move & v)) {
                /* Clear the temporary storage vector for possible moves. */
                temp_elementary_move.clear();

                /* Loop over all possible jumps for the current node. */
                for (const auto &possibleJumps : k_neighbours_[v]) {
                    for (const auto &possibleJump : possibleJumps) {

                        /* Check if there is a pawn to jump over and if the jump is valid. */
                        if ((bit_boards_all & possibleJump.first.first)
                            && ! (bit_boards_all
                                  & (possibleJump.second | possibleJump.first.second))) {
                            /* Store the possible elementary moves for the current node. */
                            temp_elementary_move.emplace_back(possibleJump.first.second);
                            break;
                        }
                    }
                }
                /* Store the possible elementary moves for the current node. */
                possible_elementary_move_[v] = std::move(temp_elementary_move);
                /* Mark that the possible elementary moves have been computed for this node. */
                computed_possible_elementary_move |= v;
            }

            /* Loop over all possible elementary moves for the current node. */
            for (uint_fast64_t neig : possible_elementary_move_[v]) {
                /* Get the coordinates of the current neighbour. */
                std::tie(i_neig, j_neig) = uint64_to_pair_[neig];
                /* Check that we haven't already explored this node and
                 * that we're not jumping over the root. */
                if (!(neig & explored)
                    && !((i + i_neig) == i_root_times_2
                         &&   (j + j_neig) == j_root_times_2)) {
                    /* Add the node to the queue and mark it as explored. */
                    queue    |= neig;
                    explored |= neig;

                    /* Add the move to the result. */
                    result.push_back(root | neig);
                }
            }
        }
    }


    /* This part of the code handles the case of not jump moves. */


    uint_fast64_t pawnPosition;
    pawnPositionMask = currentBitBoard;
    for (pawnPosition = pawnPositionMask & -pawnPositionMask;
                        pawnPositionMask & -pawnPositionMask;
         pawnPosition = pawnPositionMask & -pawnPositionMask) {
        /* This ensures that each pawn is visited only once. */
        pawnPositionMask ^= pawnPosition;

        /* Iterates over each of the direct neighbors of the pawn
         * using the direct_neighbours_ data structure. */
        for (const auto &neig : direct_neighbours_[pawnPosition]) {
            /* If the neighbor position is not occupied by any pawn (White or Black),
             * then the move is valid and is added to the result vector. */
            if (!((bit_boards_.White | bit_boards_.Black) & neig))
                result.push_back(pawnPosition | neig);
        }
    }
}

ListOfPositionType AlphaBeta::getMove(const int &depth, const double &alpha, const double &beta) {
    /* Checks if the current state of the game is in the opening book. */
    if (opening_[who_is_to_play_].find(bit_boards_) != opening_[who_is_to_play_].end())
        return retrieveMoves(opening_[who_is_to_play_][bit_boards_]);

    /* If the current state is not in the opening book, the getMove64 function
     * is called with the same inputs to search for the best move. */
    getMove64(depth);
    return retrieveMoves(best_move_);
}

uint_fast64_t AlphaBeta::getMove64(const int &depth) {
    /* Set the maximizing player to be the one who is to play,
     * i.e., the player who is currently making a move. */
    maximizing_player_ = who_is_to_play_;
    /* Compute the heuristic value of the current game state. */
    heuristic_value_   = heuristicValue();
    /* Set the maximum search depth to the given depth parameter. */
    fullDepth_         = depth;

    /* Compute the Zobrist hash of the current game state
     * and store it in the zobrist_hash_ variable. */
    computeAndSetZobristHash();
    uint_fast64_t hash = zobrist_hash_;

    /* Clear the transposition table. */
    transposition_table_.clear();
    /* Reset the best_move_ variable to zero. */
    best_move_ = 0;

    int d = 1;
    /* Start a loop that will execute at least once and will continue
     * until the game is won or the maximum search depth is reached.
     * This is a use of zero width windows heuristic. It makes the end-game
     * faster and improves a lot the quality of the player. */
    do {
         /* If the returned value is -infinity, then the game is won,
          * and the function returns the best_move_ found so far. */
        if (AlphaBetaEval(d,
                          MINUS_INFTY,
                          MINUS_INFTY,
                          false,
                          true,
                          hash) == MINUS_INFTY)
            return best_move_;
        /* Increase the search depth by 2 because an adversary move should
         * not be the reason of a win. */
        d += 2;
    }  while (won_[maximizing_player_] && (d <= depth + 2));

    /* Do a full search */
    double val = AlphaBetaEval(depth,
                               MINUS_INFTY,
                               PLUS_INFTY,
                               false,
                               true,
                               hash);

    /* Update the won_ array to reflect whether the maximizing
     * player won the game in the current search. */
    won_[maximizing_player_] = (!won_[maximizing_player_]
                                && (val == MINUS_INFTY));
    return best_move_;
}

const double AlphaBeta::AlphaBetaEval(const int &depth,
                             double alpha,
                             double beta,
                             const bool &maximizingPlayer,
                             const bool &keepMove,
                             uint_fast64_t hash) {
    /* Check if the current node is a terminating node, i.e., if the game has been won by one of the players.
     * For the player who is to play, check if they have won the game by occupying all the winning positions for their color.
     * If so, return the maximum score (PLUS_INFINITY) if the player is the maximizing player, and the minimum score
     * (MINUS_INFINITY) otherwise.
     * For the opponent player, check if they have won the game by occupying all the winning positions for their color.
     * If so, return the minimum score (MINUS_INFINITY) if the player is the maximizing player, and the maximum score
     * (PLUS_INFINITY) otherwise.
     */
    if (who_is_to_play_) {
        if ((bit_boards_.White & winning_positions_white_) /* Did white win ? */
            && ((bit_boards_.White | bit_boards_.Black) & winning_positions_white_)
                    == winning_positions_white_) {
            /* The game has been won by White. */
            return maximizing_player_ ? PLUS_INFTY : MINUS_INFTY;
        }
    } else if (     (bit_boards_.Black & winning_positions_black_) /* Did black win ? */
                && ((bit_boards_.White | bit_boards_.Black) & winning_positions_black_)
                        == winning_positions_black_) {
        /* The game has been won by Black. */
        return maximizing_player_ ? MINUS_INFTY : PLUS_INFTY;
    }

    if (number_of_times_seen_[hash] == MAX_NUMBER_OF_CYCLES_FOR_DRAW_) { /* Is there a draw ? */
        /* The game ended in a draw. */
        return DRAW_VALUE;
    } else { /* the game is not over. */
        if (depth == 0)
            return heuristic_value_;

        /* Use a transposition table to avoid redundant computation. */
        it_transposition_table_ = transposition_table_.find(hash);
        if (it_transposition_table_ != transposition_table_.end()
            && it_transposition_table_->second.second == depth) {
            /* Return the stored value from the transposition table if available. */
            return it_transposition_table_->second.first;
        }
    }

    /* Retrieve the possibles moves. */
    std::vector<uint_fast64_t> possible_moves;
    availableMoves(possible_moves);

    /* Sort according to the value of the move in order to increase the number of cut-offs. */
    if (0 && keepMove) tensorflowSortMoves(possible_moves);
    else std::sort(possible_moves.begin(), possible_moves.end(), comp_move_);

    /* We do not consider all moves in order to have a speed up */
    //if (MINUS_INFTY != beta) possible_moves.resize(std::min(15UL, possible_moves.size()));

    /* Initialize the value we will return. */
    double value = maximizingPlayer ? MINUS_INFTY - 1 : PLUS_INFTY + 1;
    /* Create a buff used to keep the result of the recursive call. */
    double buff;

    for (const auto &move:possible_moves) {
        /* Update the heuristic value with the given move. */
        updateHeuristicValue(move);
        /* Update the hash for the current position. */
        hash ^= zobrist_keys_moves_[who_is_to_play_][move];

        /* Apply the move to the current position. */
        who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;
        /* Apply the move to the current position. */
        who_is_to_play_ ^= 1;

        /* Indicates that this position has been seen another time. */
        ++number_of_times_seen_[hash];


        /* Checks for an illegal position. */
        if (isPositionIllegal()) {
            /* Undo the move and continue to the next move. */
            --number_of_times_seen_[hash];
            who_is_to_play_ ^= 1;
            who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;
            updateHeuristicValueBack(move);
            hash ^= zobrist_keys_moves_[who_is_to_play_][move];
            continue;
        }

        /* Recursively evaluate the next position with the negamax algorithm. */
        buff = AlphaBetaEval(depth - 1,
                             alpha,
                             beta,
                             !maximizingPlayer,
                             false,
                             hash);

        /* Undo the move to backtrack to the current position. */
        --number_of_times_seen_[hash];
        who_is_to_play_ ^= 1;
        who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;
        updateHeuristicValueBack(move);
        hash ^= zobrist_keys_moves_[who_is_to_play_][move];

        if (maximizingPlayer && buff > value) {
            /* We are maximizing the score and the current move's heuristic value
             * is greater than the current best value. */
            alpha = std::max(buff, alpha); /* Update alpha. */
            value = buff;                  /* Update the current best value. */
            if (value >= beta)
                break; /* Beta cutoff. */
        } else if (buff < value) {
            /* We are minimizing the score and the current move's heuristic value
             * is less than the current best value. */
            beta  = std::min(buff, beta); /* Update beta. */
            value = buff;                 /* Update the current best value. */
            if (keepMove)
                best_move_ = move;
            if (value <= alpha)
                break; /* Alpha cutoff. */
        }
    }

    /* Store the value in the transposition table when it makes sense. */
    if (   (depth < fullDepth_ - 1)
           && (value > alpha)
           && (value < beta)) {
        transposition_table_.emplace(hash, std::make_pair(value, depth));
    }

    /* Return computed value. */
    return value;
}

double AlphaBeta::heuristicValue() {
    /* Initialize the heuristic value to 0. */
    double result = 0;
    /* Loops over all possible positions. */
    for (uint_fast64_t pawnPosition = un_64_; pawnPosition; pawnPosition <<= 1) {
        /* Checks if there is a White pawn at the current position. */
        if (pawnPosition & bit_boards_.White) {
            if (maximizing_player_)
                result -= player_to_lose_value_[63 - __builtin_ctzll(pawnPosition)];
            else
                result += player_to_win_value_[63 - __builtin_ctzll(pawnPosition)];
        }
        /* Checks if there is a Black pawn at the current position. */
        else if (pawnPosition & bit_boards_.Black) {
            if (maximizing_player_)
                result += player_to_win_value_[__builtin_ctzll(pawnPosition)];
            else
                result -= player_to_lose_value_[__builtin_ctzll(pawnPosition)];
        }
    }
    return result;
}

inline void AlphaBeta::updateHeuristicValue(const uint_fast64_t &move) {
    /* This function updates the heuristic value of the current game state
     * by adding or subtracting the value of the pawn moved in the last move. */
    if (who_is_to_play_) {
        if (maximizing_player_) {
            heuristic_value_ += player_to_win_value_[__builtin_ctzll(move & ~bit_boards_.Black)]
                                - player_to_win_value_[__builtin_ctzll(move & bit_boards_.Black)];
        } else {
            heuristic_value_ += player_to_lose_value_[__builtin_ctzll(move & bit_boards_.Black)]
                               - player_to_lose_value_[__builtin_ctzll(move & ~bit_boards_.Black)];
        }
    } else {
        if (maximizing_player_) {
            heuristic_value_ += player_to_lose_value_[63 - __builtin_ctzll(move & bit_boards_.White)]
                                - player_to_lose_value_[63 - __builtin_ctzll(move & ~bit_boards_.White)];
        } else {
            heuristic_value_ += player_to_win_value_[63 - __builtin_ctzll(move & ~bit_boards_.White)]
                                - player_to_win_value_[63 - __builtin_ctzll(move & bit_boards_.White)];
        }
    }
}

inline void AlphaBeta::updateHeuristicValueBack(const uint_fast64_t &move) {
    /* This function updates the heuristic value of the current game state
     * by adding or subtracting the value of the pawn moved in the last move. */
    if (who_is_to_play_) {
        if (maximizing_player_) {
            heuristic_value_ += player_to_win_value_[__builtin_ctzll(move & bit_boards_.Black)]
                                - player_to_win_value_[__builtin_ctzll(move & ~bit_boards_.Black)];
        } else {
            heuristic_value_ += player_to_lose_value_[__builtin_ctzll(move & ~bit_boards_.Black)]
                                - player_to_lose_value_[__builtin_ctzll(move & bit_boards_.Black)];
        }
    } else {
        if (maximizing_player_) {
            heuristic_value_ += player_to_lose_value_[63 - __builtin_ctzll(move & ~bit_boards_.White)]
                                - player_to_lose_value_[63 - __builtin_ctzll(move & bit_boards_.White)];
        } else {
            heuristic_value_ += player_to_win_value_[63 - __builtin_ctzll(move & bit_boards_.White)]
                                - player_to_win_value_[63 - __builtin_ctzll(move & ~bit_boards_.White)];
        }
    }
}

Player AlphaBeta::getMaximizingPlayer() const {
    return maximizing_player_;
}

std::vector<double> AlphaBeta::getPlayerToLoseValue() {
    return player_to_lose_value_;
}

std::vector<double> AlphaBeta::getPlayerToWinValue() {
    return player_to_win_value_;
}

void AlphaBeta::setPlayerToLoseValue(const std::vector<double> &player_to_lose_value_) {
    this->player_to_lose_value_ = player_to_lose_value_;
    transposition_table_.clear();
}

void AlphaBeta::setPlayerToWinValue(const std::vector<double> &player_to_win_value_) {
    this->player_to_win_value_ = player_to_win_value_;

    transposition_table_.clear();
}

void AlphaBeta::loadOpenings() {
    std::array<std::string, 2> files =
            {"./raw_data/openings_white.dat",
             "./raw_data/openings_black.dat"};

    for (int i = 0; i < 2; ++i) {
        /* Open the file containing the pre-calculated opening moves. */
        std::ifstream inFile(files[i]);

        /* Iterate through each line in the file and load the bitboards and move. */
        std::string line;
        uint_fast64_t move;
        bitBoards_t bb;
        while (std::getline(inFile, line)) {
            std::istringstream ss(line);
            ss >> std::hex >> bb.White >> std::hex >> bb.Black >> std::hex >> move;

            /* Store the opening move in the 'opening_' map, indexed by the bitboards. */
            opening_[i][bb] = move;
        }

        /* Close the file */
        inFile.close();
    }
}

ListOfPositionType AlphaBeta::retrieveMoves(const uint_fast64_t &move) {
    /* A bitboard that keeps track of whether we have computed the possible elementary
     * moves for each position or not. */
    uint_fast64_t computed_possible_elementary_move_ = 0;

    for (auto &x : possible_elementary_move_)
        x.second.clear();

    /* A bitboard that represents the current player's pawns. */
    uint_fast64_t currentBitBoard = who_is_to_play_ ? bit_boards_.Black : bit_boards_.White;


    /* This part of the code handles the case of not jump moves. */

    /* Iterates over each of the direct neighbors of the pawn
     * using the direct_neighbours_ data structure. */
    for (const auto &neig : direct_neighbours_[currentBitBoard & move]) {
        if (neig & move) {
            /* If the neighbor position is not occupied by any pawn (White or Black),
             * then the move is valid and returned. */
            return {{uint64_to_pair_[currentBitBoard & move].first, uint64_to_pair_[currentBitBoard & move].second},
                    {uint64_to_pair_[neig].first,                   uint64_to_pair_[neig].second}};
        }
    }


    /* This part of the code handles the case of jump moves. */


    /*
     * Do a BFS to list all possible jumps.
     */
    /* Stores the nodes to be visited during the BFS. */
    std::queue<uint_fast64_t> queue;
    /* Stores the paths */
    boost::unordered_map<uint64_t, std::vector<uint_fast64_t> > paths;
    /* A bitboard that stores the nodes that have been visited. */
    uint_fast64_t explored = 0;
    /* The current node being visited. */
    uint_fast64_t v;
    /* Integer coordinates of the nodes. */
    int i, j, i_neig, j_neig, i_root, j_root;

    /* The root node : first position of the move. */
    uint_fast64_t root = currentBitBoard & move;
    queue.push(root);
    explored = root;

    paths[root] = {root};

    /* A vector to store the possible elementary moves from the current node. */
    std::vector<uint_fast64_t> temp_elementary_move;
    temp_elementary_move.reserve(20);

    /* Get the coordinates of the root. */
    std::tie(i_root, j_root) = uint64_to_pair_[root];

    /* Start the BFS algorithm. */
    while (!queue.empty()) {
        /* Take the first node in the queue */
        v = queue.front();
        queue.pop();

        /* Get the coordinates of the node. */
        std::tie(i, j) = uint64_to_pair_[v];

        /* If the possible elementary moves for this node have not been computed yet,
         * compute them and store them in the possible_elementary_move map. */
        if (!(computed_possible_elementary_move_ & v)) {
            /* Clear the temporary storage vector for possible moves. */
            temp_elementary_move.clear();

            /* Loop over all possible jumps for the current node. */
            for (const auto &possibleJumps : k_neighbours_[v]) {
                for (const auto &possibleJump : possibleJumps) {

                    /* Check if there is a pawn to jump over and if the jump is valid. */
                    if (((bit_boards_.White | bit_boards_.Black) & possibleJump.first.first)
                        && ! ((bit_boards_.White   | bit_boards_.Black        )
                            & (possibleJump.second | possibleJump.first.second))) {
                        /* Store the possible elementary moves for the current node. */
                        temp_elementary_move.push_back(possibleJump.first.second);
                        break;
                    }
                }
            }
            /* Store the possible elementary moves for the current node. */
            possible_elementary_move_[v] = temp_elementary_move;
            /* Mark that the possible elementary moves have been computed for this node. */
            computed_possible_elementary_move_ |= v;
        }

        /* Loop over all possible elementary moves for the current node. */
        for (uint_fast64_t neig : possible_elementary_move_[v]) {
            /* Get the coordinates of the current neighbour. */
            std::tie(i_neig, j_neig) = uint64_to_pair_[neig];
            if (!(neig & explored)
                /* Check that we haven't already explored this node and
                 * that we're not jumping over the root. */
                && !((i + i_neig) / 2 == i_root
                     &&   (j + j_neig)/ 2 == j_root)) {
                /* Add the node to the queue and mark it as explored. */
                queue.push(neig);
                explored |= neig;

                /* Add the move to the paths. */
                paths[neig].reserve(paths[v].size() + 1);
                paths[neig] = paths[v];
                paths[neig].push_back(neig);
            }
        }

        paths.extract(root);

        /* we add to result all the paths we found from this root */
        ListOfPositionType result;
        for (const auto &m : paths) {
            if (m.second[0] == root && m.second.back() & move) {
                for (const auto &pos : m.second)
                    result.push_back({uint64_to_pair_[pos].first, uint64_to_pair_[pos].second});
                return result;
            }
        }
    }

    return {};
}

void AlphaBeta::tensorflowSortMoves(std::vector<uint_fast64_t> &possible_moves) {
    /* We create a tensor for tensorFlow to evaluate all moves at once. */
    std::vector<uint8_t> bb_temp;
    std::vector<float> data_;
    /* Reserve space for the data of each move. */
    data_.reserve(128 * possible_moves.size());

    bitBoards_t bb;
    /* Loop through each possible move and convert it to a bitboard representation. */
    for (const uint_fast64_t &move : possible_moves) {
        bb = bit_boards_;
        if (who_is_to_play_) bb.Black ^= move;
        else                 bb.White ^= move;

        /* Convert the bitboard representation to a vector of uint8_t values. */
        bb_temp = bitBoardsAsVector(bb);
        /* Add the vector of uint8_t values to the data vector. */
        data_.insert(data_.end(), bb_temp.begin(), bb_temp.end());
    }

    /* Create a TensorFlow tensor object to hold the data vector. */
    cppflow::tensor tensor_data_ = cppflow::tensor(data_, {static_cast<int>(possible_moves.size()), 128});

    /* Run the tensor through the TensorFlow model and get the predicted value of each move. */
    std::vector<cppflow::tensor> output = (*model)({{"serving_default_dense_input:0", tensor_data_}},
                                                    {"StatefulPartitionedCall:0"});

    /* Map each possible move to its predicted value. */
    std::unordered_map<uint_fast64_t, double> res;
    auto output_data_ = output[0].get_data<double>();
    for (int d = 0; d < possible_moves.size(); ++d)
        res[possible_moves[d]] = output_data_[d];

    /* Sort the moves. The network has been trained as white so we cannot sort
     * the moves the same way for the two players. The best move for Black is the worst
     * for white. */
    if (who_is_to_play_) {
        auto compMoveB = [&](const uint_fast64_t &a, const uint_fast64_t &b) {
            return res[a] > res[b];
        };
        std::sort(possible_moves.begin(), possible_moves.end(), compMoveB);
    } else {
        auto compMoveW = [&](const uint_fast64_t &a, const uint_fast64_t &b) {
            return res[a] < res[b];
        };
        std::sort(possible_moves.begin(), possible_moves.end(), compMoveW);
    }
}

std::vector<uint8_t> AlphaBeta::bitBoardsAsVector(const bitBoards_t &bb) {
    /* Copy the grid to a vector and reverse it if black is playing.
     * We need to reverse it if black is playing because the intuition
     * has been trained for white only. */
    std::vector<uint8_t> grid_temp(128, 0);
    int i;
    for (i = 0; i < 64; ++i) {
        if (bb.White & (un_64_ << i)) grid_temp[63  - i] = 1;
        if (bb.Black & (un_64_ << i)) grid_temp[127 - i] = 1;
    }
    return grid_temp;
}