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

    compMoveVect = [this](const std::vector<uint_fast64_t> &a,
                          const std::vector<uint_fast64_t> &b){
        double valueA = 0;
        double valueB = 0;
        if (maximizing_player_) {
            return player_to_win_value_map_black_[a.back()]
                   + player_to_win_value_map_black_[b[0]]
                   < player_to_win_value_map_black_[b.back()]
                     + player_to_win_value_map_black_[a[0]];
        } else {
            return player_to_win_value_map_white_[a.back()]
                   + player_to_win_value_map_white_[b[0]]
                   < player_to_win_value_map_white_[b.back()]
                     + player_to_win_value_map_white_[a[0]];
        }
    };

    /* Indicates if there is a jump from (i, j) to (k, l) */
    for (int i = 0; i < 64; ++i)
        possible_elementary_move[un_64 << i] = std::vector<uint_fast64_t>(0);

    loadOpenings();
}

void AlphaBeta::availableMoves(std::vector< std::vector<uint_fast64_t> > &result, const bool &full) {
    result.reserve(50);
    uint_fast64_t computed_possible_elementary_move = 0;

    for (auto &x : possible_elementary_move)
        x.second.clear();

    uint_fast64_t currentBitBoard = who_is_to_play_ ? bitBoardBlack : bitBoardWhite;

    int i, j;
    /* Check the case of notJump moves */
    for (uint_fast64_t pawnPosition = un_64; pawnPosition; pawnPosition <<= 1) {
        /* Check if there is a pawn */
        if (!(pawnPosition & currentBitBoard))
            continue;

        for (const auto &neig : direct_neighbours_[pawnPosition]) {
            if (!((bitBoardWhite | bitBoardBlack) & neig))
                result.push_back({pawnPosition, neig});
        }
    }

    /* Check the case of Jump moves */
    /*
     * Do a BFS to list all possible jumps.
     * Each pown is a root
     */
    std::queue<uint_fast64_t> queue;
    boost::unordered_map<uint64_t, std::vector<uint_fast64_t> > paths;
    uint_fast64_t explored = 0;
    uint_fast64_t v;
    int i_neig, j_neig, i_root, j_root;
    /* When another root is chosen, the queue is already empty */
    for (uint_fast64_t root = un_64; root; root <<= 1) {
        /* Check if there is a pawn */
        if (!(root & currentBitBoard))
            continue;

        queue.push(root);

        paths.clear();

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

                    if (full) {
                        paths[neig].reserve(paths[v].size() + 1);
                        paths[neig] = paths[v];
                        paths[neig].push_back(neig);
                    } else {
                        result.push_back({root, neig});
                    }
                }
            }
        }

        if (full) {
            paths.extract(root);
            /* we add to result all the paths we found from this root */
            for (const auto &move : paths) {
                result.push_back(move.second);
            }
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
    /*best_move_.clear();
    AlphaBetaEval(depth,
                  -20,
                  heuristic_value_,
                  false,
                  true);

    if(best_move_.size() == 0)*/
    AlphaBetaEval(depth, -20, 20, false, true);
    //std::cout << AlphaBetaEval(depth, -20, 20, false, true) << "\n";

    //print_grid_();

    ListOfPositionType move;
    int pos;
    move.reserve(best_move_.size());
    for (const auto &pose : best_move_) {
        pos = ctz[pose];
        move.push_back({pos /8, pos % 8});
    }

    return move;
}

const double AlphaBeta::AlphaBetaEval(const int &depth,
                             double alpha,
                             double beta,
                             const bool &maximizingPlayer,
                             const bool &keepMove) {
    if (depth == 0)
        return heuristic_value_;

    /* Sort according to the value of the move in order to increase the number of cut-offs */
    std::vector< std::vector<uint_fast64_t> > possible_moves;

    availableMoves(possible_moves, keepMove);
    std::sort(possible_moves.begin(), possible_moves.end(), compMoveVect);

    // possible_moves.resize(std::min(10, std::max(1, possible_moves.size()/(1 + (fullDepth_ - depth)))));

    //if (0 && keepMove)
    //    tensorflowOrderMoves(possible_moves);
    //else
    //    sortDepth1Light(moves);

    // possible_moves.resize(20);
    double value;
    double buff;
    if (maximizingPlayer)
        value = MINUS_INFTY - 1;
    else
        value = PLUS_INFTY + 1;

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
            reverseMoveLight(move);
            updateHeuristicValueBack(move);
            continue;
        }

        /* Check if the current node is a terminating node */
        switch (state_of_game()) {
            case WhiteWon:
                if (maximizing_player_ == 0) {
                    if (keepMove) {
                        best_move_ = move;
                        reverseMoveLight(move);
                        return MINUS_INFTY;
                    }
                    buff = MINUS_INFTY;
                } else {
                    reverseMoveLight(move);
                    return PLUS_INFTY;
                }
                break;

            case BlackWon:
                if (maximizing_player_ == 1) {
                    reverseMoveLight(move);
                    if (keepMove) {
                        best_move_ = move;
                        reverseMoveLight(move);
                        return MINUS_INFTY;
                    }
                    buff = MINUS_INFTY;
                } else {
                    reverseMoveLight(move);
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

        reverseMoveLight(move);
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

inline void AlphaBeta::updateHeuristicValue(const std::vector<uint_fast64_t> &move) {
    if (who_is_to_play_) {
        if (maximizing_player_) {
            heuristic_value_ += player_to_win_value_map_black_[move.back()]
                                - player_to_win_value_map_black_[move[0]];
        } else {
            heuristic_value_ += player_to_lose_value_map_black_[move[0]]
                                - player_to_lose_value_map_black_[move.back()];
        }
    } else {
        if (maximizing_player_) {
            heuristic_value_ += player_to_lose_value_map_white_[move[0]]
                                - player_to_lose_value_map_white_[move.back()];
        } else {
            heuristic_value_ += player_to_win_value_map_white_[move.back()]
                                - player_to_win_value_map_white_[move[0]];
        }
    }
}

inline void AlphaBeta::updateHeuristicValueBack(const std::vector<uint_fast64_t> &move) {
    if (who_is_to_play_) {
        if (maximizing_player_) {
            heuristic_value_ += player_to_win_value_map_black_[move[0]]
                                - player_to_win_value_map_black_[move.back()];
        } else {
            heuristic_value_ += player_to_lose_value_map_black_[move.back()]
                                - player_to_lose_value_map_black_[move[0]];
        }
    } else {
        if (maximizing_player_) {
            heuristic_value_ += player_to_lose_value_map_white_[move.back()]
                                - player_to_lose_value_map_white_[move[0]];
        } else {
            heuristic_value_ += player_to_win_value_map_white_[move[0]]
                                - player_to_win_value_map_white_[move.back()];
        }
    }
}

void AlphaBeta::reverseMoveLight(const std::vector<uint_fast64_t> &move) {
    uint64_t hash = hashGrid();
    if(number_of_times_seen.contains(hash))
        --number_of_times_seen[hash];

    who_is_to_play_ ^= 1;

    if (who_is_to_play_)
        bitBoardBlack = (bitBoardBlack | move[0]) & ~move.back();
    else
        bitBoardWhite = (bitBoardWhite | move[0]) & ~move.back();
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
}

void AlphaBeta::set_player_to_win_value_(
        std::vector< std::vector<double> > &player_to_win_value_  ) {
    player_to_win_value_ = player_to_win_value_;
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
    return 0x100000001b3 * (0x100000001b3 * (0xcbf29ce484222325 ^ bitBoardWhite) ^ bitBoardBlack);
}