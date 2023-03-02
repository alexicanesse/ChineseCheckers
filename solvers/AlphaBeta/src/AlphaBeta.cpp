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
#define DRAW_VALUE (0);


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
#include <boost/unordered_map.hpp>

/* Other */
#include "Types.hpp"
#include "ChineseCheckers.hpp"

AlphaBeta::AlphaBeta() {
    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    player_to_win_value_ = std::vector< std::vector<double> >({
        {0.0/98,  1.0/98,  4.0/98,  9.0/98, 16.0/98, 25.0/98, 36.0/98, 49.0/98},
        {1.0/98,  2.0/98,  5.0/98, 10.0/98, 17.0/98, 26.0/98, 37.0/98, 50.0/98},
        {4.0/98,  5.0/98,  8.0/98, 13.0/98, 20.0/98, 29.0/98, 40.0/98, 53.0/98},
        {9.0/98, 10.0/98, 13.0/98, 18.0/98, 25.0/98, 34.0/98, 45.0/98, 58.0/98},
        {16.0/98, 17.0/98, 20.0/98, 25.0/98, 32.0/98, 41.0/98, 52.0/98, 65.0/98},
        {25.0/98, 26.0/98, 29.0/98, 34.0/98, 41.0/98, 50.0/98, 62.0/98, 74.0/98},
        {36.0/98, 37.0/98, 40.0/98, 45.0/98, 52.0/98, 62.0/98, 72.0/98, 85.0/98},
        {49.0/98, 50.0/98, 53.0/98, 58.0/98, 65.0/98, 74.0/98, 85.0/98, 98.0/98}  });

    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    player_to_lose_value_ = std::vector< std::vector<double> >({
        {0.0/588,  1.0/588,  4.0/588,  9.0/588, 16.0/588, 25.0/588, 36.0/588, 49.0/588},
        {1.0/588,  2.0/588,  5.0/588, 10.0/588, 17.0/588, 26.0/588, 37.0/588, 50.0/588},
        {4.0/588,  5.0/588,  8.0/588, 13.0/588, 20.0/588, 29.0/588, 40.0/588, 53.0/588},
        {9.0/588, 10.0/588, 13.0/588, 18.0/588, 25.0/588, 34.0/588, 45.0/588, 58.0/588},
        {16.0/588, 17.0/588, 20.0/588, 25.0/588, 32.0/588, 41.0/588, 52.0/588, 65.0/588},
        {25.0/588, 26.0/588, 29.0/588, 34.0/588, 41.0/588, 50.0/588, 62.0/588, 74.0/588},
        {36.0/588, 37.0/588, 40.0/588, 45.0/588, 52.0/588, 62.0/588, 72.0/588, 85.0/588},
        {49.0/588, 50.0/588, 53.0/588, 58.0/588, 65.0/588, 74.0/588, 85.0/588, 98.0/588}  });

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            player_to_lose_value_map_white_[int_to_uint64_[i][j]] = player_to_lose_value_[7 - i][7 - j];
            player_to_lose_value_map_black_[int_to_uint64_[i][j]] = player_to_lose_value_[i][j];
            player_to_win_value_map_white_[int_to_uint64_[i][j]]  = player_to_win_value_[7 - i][7 - j];
            player_to_win_value_map_black_[int_to_uint64_[i][j]]  = player_to_win_value_[i][j];
        }
    }

    ctz[0] = 0;
    for (uint_fast64_t i = un_64; i; i <<= 1) {
        ctz[i] = __builtin_ctzl(i);
    }

    /* Indicates if there is a jump from (i, j) to (k, l) */
    for (int i = 0; i < 64; ++i)
        possible_elementary_move[un_64 << i] = std::vector<uint_fast64_t>(0);

    loadOpenings();
}

AlphaBeta::AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
                     const std::vector< std::vector<double> > &player_to_lose_value_) {
    this->player_to_win_value_ = player_to_win_value_;
    this->player_to_lose_value_ = player_to_lose_value_;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            player_to_lose_value_map_white_[int_to_uint64_[i][j]] = this->player_to_lose_value_[7 - i][7 - j];
            player_to_lose_value_map_black_[int_to_uint64_[i][j]] = this->player_to_lose_value_[i][j];
            player_to_win_value_map_white_[int_to_uint64_[i][j]]  = this->player_to_win_value_[7 - i][7 - j];
            player_to_win_value_map_black_[int_to_uint64_[i][j]]  = this->player_to_win_value_[i][j];
        }
    }

    ctz[0] = 0;
    for (uint_fast64_t i = un_64; i; i <<= 1) {
        ctz[i] = __builtin_ctzl(i);
    }

    /* Indicates if there is a jump from (i, j) to (k, l) */
    for (int i = 0; i < 64; ++i)
        possible_elementary_move[un_64 << i] = std::vector<uint_fast64_t>(0);

    loadOpenings();
}

void AlphaBeta::availableMoves(std::vector<uint_fast64_t> &result) {
    result.reserve(50);
    uint_fast64_t computed_possible_elementary_move = 0;

    for (auto &x : possible_elementary_move)
        x.second.clear();

    uint_fast64_t currentBitBoard = who_is_to_play_ ? bitBoardBlack : bitBoardWhite;

    int i, j;
    /* Check the case of Jump moves */
    /*
     * Do a BFS to list all possible jumps.
     * Each pown is a root
     */
    uint_fast64_t queue;
    uint_fast64_t explored = 0;
    uint_fast64_t v;
    int i_neig, j_neig, i_root, j_root;
    /* When another root is chosen, the queue is already empty */
    uint_fast64_t root;
    uint_fast64_t pawnPositionMask = currentBitBoard;
    for (root = pawnPositionMask & -pawnPositionMask;
         pawnPositionMask & -pawnPositionMask;
         root = pawnPositionMask & -pawnPositionMask) {
        pawnPositionMask ^= root;

        queue    = root;
        explored = root;

        std::tie(i_root, j_root) = uint64_to_pair_[root];
        while (queue) {
            v = queue & -queue;
            queue ^= v;

            std::tie(i, j) = uint64_to_pair_[v];

            if (!(computed_possible_elementary_move & v)) {
                for (const auto &possibleJumps : k_neighbours_[v]) {
                    for (const auto &possibleJump : possibleJumps) {
                        /* Check if there is a pawn to jump over and if the jump is valid */
                        if (((bitBoardWhite | bitBoardBlack) & possibleJump.first.first)
                            && ! ((bitBoardWhite       | bitBoardBlack            )
                                  & (possibleJump.second | possibleJump.first.second))) {
                            possible_elementary_move[v].push_back(possibleJump.first.second);
                            break;
                        }
                    }
                }
                computed_possible_elementary_move |= v;
            }

            for (uint_fast64_t neig : possible_elementary_move[v]) {
                std::tie(i_neig, j_neig) = uint64_to_pair_[neig];
                if (!(neig & explored)
                    /*
                     * Checks that we are not jumping over the root
                     * (it has moved)
                     */
                    && !((i + i_neig) / 2 == i_root
                         &&   (j + j_neig)/ 2 == j_root)) {
                    queue    |= neig;
                    explored |= neig;

                    result.push_back(root | neig);
                }
            }
        }
    }

    /* Check the case of notJump moves */
    uint_fast64_t pawnPosition;
    pawnPositionMask = currentBitBoard;
    for (pawnPosition = pawnPositionMask & -pawnPositionMask;
                        pawnPositionMask & -pawnPositionMask;
         pawnPosition = pawnPositionMask & -pawnPositionMask) {
        pawnPositionMask ^= pawnPosition;

        for (const auto &neig : direct_neighbours_[pawnPosition]) {
            if (!((bitBoardWhite | bitBoardBlack) & neig))
                result.push_back(pawnPosition | neig);
        }
    }
}

ListOfPositionType AlphaBeta::getMove(const int &depth, const double &alpha, const double &beta) {
    maximizing_player_ = who_is_to_play_;
    heuristic_value_   = heuristicValue();
    fullDepth_         = depth;

    //if (opening.contains(hashGrid()))
        //return opening[hashGrid()];

    /* Aspiration window */
    best_move_ = 0;
    double heur_beta_ = heuristic_value_ * .8;
    while(!best_move_) {
        transTable.clear();
        AlphaBetaEval(depth,
                      -20,
                      heur_beta_,
                      false,
                      true);
        heur_beta_ *= 1.1;
    }
    return retrieveMoves(best_move_);
}

const double AlphaBeta::AlphaBetaEval(const int &depth,
                             double alpha,
                             double beta,
                             const bool &maximizingPlayer,
                             const bool &keepMove) {
    if (depth == 0) {
        return heuristic_value_;
    }

    /* Sort according to the value of the move in order to increase the number of cut-offs */
    std::vector<uint_fast64_t> possible_moves;

    availableMoves(possible_moves);
    if (keepMove) std::sort(possible_moves.begin(), possible_moves.end(), compMoveVect);

    // possible_moves.resize(std::min(10, std::max(1, possible_moves.size()/(1 + (fullDepth_ - depth)))));

    //if (0 && keepMove)
    //    tensorflowOrderMoves(possible_moves);
    //else
    //    sortDepth1Light(moves);

    double value = maximizingPlayer ? MINUS_INFTY - 1 : PLUS_INFTY + 1;
    double buff;

//temp
    // int r = 0;
    // number_of_moves = possible_moves.size();
    for (const auto &move:possible_moves) {
//temp
        // ++r;
        updateHeuristicValue(move);
        moveWithoutVerification(move);

        /* Checks for an illegal position */
        if (isPositionIllegal()) {
            reverseMove(move);
            updateHeuristicValueBack(move);
            continue;
        }

        /* Check if the current node is a terminating node */
        switch (state_of_game()) {
            case WhiteWon:
                if (maximizing_player_ == 0) {
                    if (keepMove) {
                        best_move_ = move;
                        reverseMove(move);
                        updateHeuristicValueBack(move);
                        return MINUS_INFTY;
                    }
                    buff = MINUS_INFTY;
                } else {
                    reverseMove(move);
                    updateHeuristicValueBack(move);
                    return PLUS_INFTY;
                }
                break;

            case BlackWon:
                if (maximizing_player_ == 1) {
                    if (keepMove) {
                        best_move_ = move;
                        reverseMove(move);
                        updateHeuristicValueBack(move);
                        return MINUS_INFTY;
                    }
                    buff = MINUS_INFTY;
                } else {
                    reverseMove(move);
                    updateHeuristicValueBack(move);
                    return PLUS_INFTY;
                }
                break;

            case Draw:
                buff = 10;
                break;

            default:
                /* the game is not over */
                if (depth == 1) {
                    buff = heuristic_value_;
                } else {
                    it = transTable.find(hashGrid());
                    if (it != transTable.end() && it->second.second >= depth - 1) {
                        /* retrieve the value from the transposition table */
                        buff = it->second.first;
                    } else {
                        buff = AlphaBetaEval(depth - 1,
                                             alpha,
                                             beta,
                                             !maximizingPlayer,
                                             false);
                    }
                }
                break;
        }

        reverseMove(move);
        updateHeuristicValueBack(move);

        if (maximizingPlayer) {
            alpha = std::max(alpha, buff);
            if (buff > value) {
                value = buff;
                if (value >= beta)
                    break; /* beta cutoff */
            }
        } else {
            beta = std::min(beta, buff);
            if (buff < value) {
                value = buff;
                if (keepMove) best_move_ = move;
                // if (keepMove) rank = r;
                if (value <= alpha)
                    break; /* alpha cutoff */
            }
        }
    }

    /* store the value in the transposition table */
    transTable[hashGrid()] = {value, depth};

    /* return value */
    return value;
}

double AlphaBeta::heuristicValue() {
    double result = 0;
    for (uint_fast64_t pawnPosition = un_64; pawnPosition; pawnPosition <<= 1) {
        if (pawnPosition & bitBoardWhite) {
            if (maximizing_player_)
                result -= player_to_lose_value_map_white_[pawnPosition];
            else
                result +=player_to_win_value_map_white_[pawnPosition];
        } else if (pawnPosition & bitBoardBlack) {
            if (maximizing_player_)
                result += player_to_win_value_map_black_[pawnPosition];
            else
                result -=player_to_lose_value_map_black_[pawnPosition];
        }
    }
    return result;
}

inline void AlphaBeta::updateHeuristicValue(const uint_fast64_t &move) {
    if (who_is_to_play_) {
        if (maximizing_player_) {
            heuristic_value_ += player_to_win_value_map_black_[move & ~bitBoardBlack]
                                - player_to_win_value_map_black_[move & bitBoardBlack];
        } else {
            heuristic_value_ += player_to_lose_value_map_black_[move & bitBoardBlack]
                               - player_to_lose_value_map_black_[move & ~bitBoardBlack];
        }
    } else {
        if (maximizing_player_) {
            heuristic_value_ += player_to_lose_value_map_white_[move & bitBoardWhite]
                                - player_to_lose_value_map_white_[move & ~bitBoardWhite];
        } else {
            heuristic_value_ += player_to_win_value_map_white_[move & ~bitBoardWhite]
                                - player_to_win_value_map_white_[move & bitBoardWhite];
        }
    }
}

inline void AlphaBeta::updateHeuristicValueBack(const uint_fast64_t &move) {
    if (who_is_to_play_) {
        if (maximizing_player_) {
            heuristic_value_ += player_to_win_value_map_black_[move & bitBoardBlack]
                                - player_to_win_value_map_black_[move & ~bitBoardBlack];
        } else {
            heuristic_value_ += player_to_lose_value_map_black_[move & ~bitBoardBlack]
                                - player_to_lose_value_map_black_[move & bitBoardBlack];
        }
    } else {
        if (maximizing_player_) {
            heuristic_value_ += player_to_lose_value_map_white_[move & ~bitBoardWhite]
                                - player_to_lose_value_map_white_[move & bitBoardWhite];
        } else {
            heuristic_value_ += player_to_win_value_map_white_[move & bitBoardWhite]
                                - player_to_win_value_map_white_[move & ~bitBoardWhite];
        }
    }
}

void AlphaBeta::reverseMove(const uint_fast64_t &move) {
    uint64_t hash = hashGrid();
    if(number_of_times_seen.contains(hash))
        --number_of_times_seen[hash];

    who_is_to_play_ ^= 1;

    if (who_is_to_play_)
        bitBoardBlack ^= move;
    else
        bitBoardWhite ^= move;
}

Player AlphaBeta::get_maximizing_player_() const {
    return maximizing_player_;
}

std::vector<std::vector<double> > AlphaBeta::get_player_to_lose_value_() {
    return player_to_lose_value_;
}

std::vector<std::vector<double> > AlphaBeta::get_player_to_win_value_() {
    return player_to_win_value_;
}

void AlphaBeta::set_player_to_lose_value_(
        std::vector< std::vector<double> > &player_to_lose_value_) {
    player_to_lose_value_ = player_to_lose_value_;
    transTable.clear();
}

void AlphaBeta::set_player_to_win_value_(
        std::vector< std::vector<double> > &player_to_win_value_  ) {
    player_to_win_value_ = player_to_win_value_;
    transTable.clear();
}

void AlphaBeta::loadOpenings() {
    std::ifstream inFile("raw_data/openings.dat");

    /* Iterate through the file and load each element through the file */
    std::string line;
    uint64_t hash;
    int position0, position1;
    ListOfPositionType move;
    while(std::getline(inFile, line)) {
        move.clear();
        std::istringstream ss(line);
        ss >> hash;
        while(ss >> position0 >> position1)
            move.push_back({position0, position1});

        opening[hash] = move;
    }

    /* Close the file */
    inFile.close();
}

/* FNV-1a hash function */
inline const uint64_t AlphaBeta::hashGrid() {
    return (0x100000001b3 * (0xcbf29ce484222325 ^ bitBoardWhite) ^ bitBoardBlack);
}

ListOfPositionType AlphaBeta::retrieveMoves(const uint_fast64_t &move) {
    uint_fast64_t computed_possible_elementary_move = 0;

    for (auto &x : possible_elementary_move)
        x.second.clear();

    uint_fast64_t currentBitBoard = who_is_to_play_ ? bitBoardBlack : bitBoardWhite;

    /* Check the case of notJump moves */
    for (const auto &neig : direct_neighbours_[currentBitBoard & move]) {
        if (neig & move)
            return {{uint64_to_pair_[currentBitBoard & move].first, uint64_to_pair_[currentBitBoard & move].second},
                    {uint64_to_pair_[neig                  ].first, uint64_to_pair_[neig                  ].second}};
    }

    /* Check the case of Jump moves */
    /*
     * Do a BFS to list all possible jumps.
     */
    std::queue<uint_fast64_t> queue;
    boost::unordered_map<uint64_t, std::vector<uint_fast64_t> > paths;
    std::vector<uint_fast64_t> result;
    uint_fast64_t explored = 0;
    uint_fast64_t v;
    int i, j, i_neig, j_neig, i_root, j_root;

    uint_fast64_t root = currentBitBoard & move;
    queue.push(root);
    explored = root;

    paths[root] = {root};


    std::tie(i_root, j_root) = uint64_to_pair_[root];
    while (!queue.empty()) {
        v = queue.front();
        queue.pop();

        std::tie(i, j) = uint64_to_pair_[v];

        if (!(computed_possible_elementary_move & v)) {
            for (const auto &possibleJumps : k_neighbours_[v]) {
                for (const auto &possibleJump : possibleJumps) {
                    /* Check if there is a pawn to jump over and if the jump is valid */
                    if (((bitBoardWhite | bitBoardBlack) & possibleJump.first.first)
                        && ! ((bitBoardWhite       | bitBoardBlack            )
                              & (possibleJump.second | possibleJump.first.second))) {
                        possible_elementary_move[v].push_back(possibleJump.first.second);
                        break;
                    }
                }
            }
            computed_possible_elementary_move |= v;
        }

        for (uint_fast64_t neig : possible_elementary_move[v]) {
            std::tie(i_neig, j_neig) = uint64_to_pair_[neig];
            if (!(neig & explored)
                /*
                 * Checks that we are not jumping over the root
                 * (it has moved)
                 */
                && !((i + i_neig) / 2 == i_root
                     &&   (j + j_neig)/ 2 == j_root)) {
                queue.push(neig);
                explored |= neig;

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