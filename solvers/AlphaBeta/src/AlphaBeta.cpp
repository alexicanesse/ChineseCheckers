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

#define PLUS_INFTY (100000)
#define MINUS_INFTY (-100000)
#define DRAW_VALUE (-50000);


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

/* Other */
#include "Types.hpp"
#include "ChineseCheckers.hpp"

AlphaBeta::AlphaBeta() {
    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    this->player_to_win_value_ = std::vector< std::vector<double> >({
        { 0,  1,  4,  9, 16, 25, 36, 49},
        { 1,  2,  5, 10, 17, 26, 37, 50},
        { 4,  5,  8, 13, 20, 29, 40, 53},
        { 9, 10, 13, 18, 25, 34, 45, 58},
        {16, 17, 20, 25, 32, 41, 52, 65},
        {25, 26, 29, 34, 41, 50, 62, 74},
        {36, 37, 40, 45, 52, 62, 72, 85},
        {49, 50, 53, 58, 65, 74, 85, 98}
    });

    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    this->player_to_loose_value_ = std::vector< std::vector<double> >({
        { 0,  1,  4,  9, 16, 25, 36, 49},
        { 1,  2,  5, 10, 17, 26, 37, 50},
        { 4,  5,  8, 13, 20, 29, 40, 53},
        { 9, 10, 13, 18, 25, 34, 45, 58},
        {16, 17, 20, 25, 32, 41, 52, 65},
        {25, 26, 29, 34, 41, 50, 62, 74},
        {36, 37, 40, 45, 52, 62, 72, 85},
        {49, 50, 53, 58, 65, 74, 85, 98}
    });
}

AlphaBeta::AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
                     const std::vector< std::vector<double> > &player_to_loose_value_) {
    this->player_to_win_value_ = player_to_win_value_;
    this->player_to_loose_value_ = player_to_loose_value_;
}

ListOfMoves AlphaBeta::availableMoves(const Player &player, const bool &full) {
    /* Indicates if there is a jump from (i, j) to (k, l) */
    std::vector<ListOfPositionType> possible_elementary_move(64);
    std::vector<std::vector<int>> valid_lines = {{-1,  0},
                                                 {-1,  1},
                                                 {0 , -1},
                                                 {0 ,  1},
                                                 {1 , -1},
                                                 {1 ,  0}};

    ListOfMoves result;
    /* Check the case of notJump moves */
    for (const auto &pownPosition : position_colors_players_[player]) {
        for(const std::vector<int> &direction : valid_lines) {
            if (   pownPosition[0] + direction[0] >= 0
                && pownPosition[1] + direction[1] >= 0
                && pownPosition[0] + direction[0]  < 8
                && pownPosition[1] + direction[1]  < 8) {
                if (grid_[pownPosition[0] + direction[0]]
                         [pownPosition[1] + direction[1]] == Empty) {
                    result.push_back({pownPosition,
                                      {pownPosition[0] + direction[0],
                                       pownPosition[1] + direction[1]}});
                }
            }
        }
    }

    /* Check the case of Jump moves */
    /*
     * Do a BFS to list all possible jumps.
     * Each pown is a root
     */
    std::queue<PositionType> queue;
    std::unordered_map<uint64_t, bool> explored;
    std::unordered_map<uint64_t, ListOfPositionType> paths;
    int i = -1;
    int j = -1;
    bool isPossible = true;
    /* When another root is chosen, the queue is already empty */
    for (auto root : position_colors_players_[player]) {
        queue.push(root);

        explored.clear();
        paths.clear();
        explored[hashPosition(root)] = true;
        paths[hashPosition(root)] = {root};
        while (!queue.empty()) {
            PositionType v = queue.front();
            queue.pop();


            if (possible_elementary_move[(v[0] << 3) | v[1]].empty()) {
                i = v[0];
                j = v[1];
                for(const std::vector<int> &direction : valid_lines) {
                    for (int k = 1;    i + direction[0]*(k << 1) < 8
                                    && j + direction[1]*(k << 1) < 8
                                    && i + direction[0]*(k << 1) >= 0
                                    && j + direction[1]*(k << 1) >= 0; ++k){
                        /* Check if there is a pown to jump over */
                        if (grid_[i + direction[0]*k]
                                 [j + direction[1]*k] != Empty) {
                            /* Check if the jump is valid */
                            isPossible = true;
                            for (int l = 1; l <= k; ++l) {
                                if (grid_[i + direction[0]*(k + l)]
                                         [j + direction[1]*(k + l)] != Empty) {
                                    isPossible = false;
                                    break;
                                }
                            }
                            if (isPossible)
                                possible_elementary_move[(i << 3) | j]
                                        .push_back({i + direction[0]*(k << 1),
                                                    j + direction[1]*(k << 1)});
                            break;
                        }
                    }
                }
            }


            for (PositionType neig :
                    possible_elementary_move[(v[0] << 3) | v[1]]) {
                if (!explored.contains(hashPosition(neig))
                    /*
                     * Checks that we are not jumping over the root
                     * (it has moved)
                     */
                    && !((v[0] + neig[0])/2 == root[0]
                         && (v[1] + neig[1])/2 == root[1])) {
                    queue.push(neig);
                    explored[hashPosition(neig)] = true;

                    if (full) {
                        paths[hashPosition(neig)] = paths[hashPosition(v)];
                        paths[hashPosition(neig)].push_back(neig);
                    } else {
                        result.push_back({root, neig});
                    }
                }
            }
        }

        if (full) {
            paths.extract(hashPosition(root));
            /* we add to result all the paths we found from this root */
            for (const auto &move : paths)
                result.push_back(move.second);
        }
    }

    return result;
}

double AlphaBeta::evaluate(const Player &player) {
    double result = 0;
    switch (player) {
        case 0:  /* White */
            if (this->maximizing_player_) {
                for (const PositionType &pown: position_colors_players_[0])
                    result += this->player_to_loose_value_[7 - pown[0]][7 - pown[1]];
            } else {
                for (const PositionType &pown: position_colors_players_[0])
                    result += this->player_to_win_value_[7 - pown[0]][7 - pown[1]];
            }
            break;

        case 1:  /* black */
            if (this->maximizing_player_) {
                for (const PositionType &pown: position_colors_players_[1])
                    result += this->player_to_loose_value_[pown[0]][pown[1]];
            } else {
                for (const PositionType &pown: position_colors_players_[1])
                    result += this->player_to_win_value_[pown[0]][pown[1]];
            }
            break;

        default:
            break;
    }
    return result;
}

ListOfPositionType AlphaBeta::getMove(const int &depth, const double &alpha, const double &beta) {
    this->maximizing_player_ = this->who_is_to_play_;

    AlphaBetaEval(depth, alpha, beta, false, true);

    //std::cout << "Rank : " << std::setw(2) << rank << " / " << std::setw(2) << number_of_moves << " " << std::setprecision(2) << (100 - 100*static_cast<double>(rank)/number_of_moves) << "%\n";

    return this->best_move_;
}

double AlphaBeta::AlphaBetaEval(const int &depth,
                             double alpha,
                             double beta,
                             const bool &maximizingPlayer,
                             const bool &keepMove) {
    /* Check if the current node is a terminating node */
    switch (this->state_of_game()) {
        case WhiteWon:
            if (this->maximizing_player_ == 0) return MINUS_INFTY;
            else return PLUS_INFTY;
            break;

        case BlackWon:
            if (this->maximizing_player_ == 1) return MINUS_INFTY;
            else return PLUS_INFTY;
            break;

        case Draw:
            if (this->maximizing_player_ ==
                1 - this->who_is_to_play_) return 50000;
            else return -50000;
            break;

        default: /* the game is not over */
            break;
    }

    /* check if the current position has been searched before */
    /* compute the hash value of the current board position */
    unsigned long long hash = hashMatrix(this->grid_, this->who_is_to_play_);
    auto it = transTable.find(hash);
    if (it != transTable.end() && it->second.second >= depth) {
        /* retrieve the value from the transposition table */
        double value = it->second.first;
        if (!keepMove) return value;

        /* update beta if value is lower */
        beta = std::min(value, beta);
        /* update alpha if value is higher */
        alpha = std::max(value, alpha);
    }

    if (depth == 0) return heuristicValue();

    /* Sort according to the value of the move in order to increase the number of cut-offs */
    ListOfMoves possible_moves = this->availableMoves(this->who_is_to_play_, keepMove);
    if (keepMove)
        this->tensorflowOrderMoves(possible_moves);
    else
        this->sortDepth1(possible_moves);

    //possible_moves.resize(possible_moves.size()/2);
    double value = 0;
    if (maximizingPlayer)
        value = MINUS_INFTY;
    else
        value = PLUS_INFTY;

//temp
    int r = 0;
    if (keepMove) number_of_moves = possible_moves.size();
    for (const ListOfPositionType &move : possible_moves) {
//temp
        ++r;
        this->moveWithoutVerification(this->who_is_to_play_, move);
        double buff = AlphaBetaEval(depth - 1,
                                 alpha,
                                 beta,
                                 !maximizingPlayer,
                                 false);

        this->reverseMove(move);

        if (maximizingPlayer) {
            alpha = std::max(alpha, static_cast<double>(buff));
            if (buff >= value) {
                value = buff;
                if (value >= beta)
                    break; /* beta cutoff */
            }
        } else {
            beta = std::min(beta, static_cast<double>(buff));
            if (buff <= value) {
                value = buff;
                if (keepMove) this->best_move_ = move;
                if (keepMove) rank = r;
                if (value <= alpha)
                    break; /* alpha cutoff */
            }
        }
    }

    /* store the value in the transposition table */
    transTable[hash] = std::make_pair(value, depth);

    /* return value */
    return value;
}

double AlphaBeta::heuristicValue() {
    return 6*evaluate(this->maximizing_player_)
                    - evaluate(1 - this->maximizing_player_);
}

void AlphaBeta::reverseMove(const ListOfPositionType &move){
    --this->number_of_times_seen.at(this->grid_);

    this->who_is_to_play_ = 1 - this->who_is_to_play_;
    this->grid_[move.back()[0]][move.back()[1]] = Empty;
    if (this->who_is_to_play_)
        this->grid_[move.front()[0]][move.front()[1]] = Black;
    else
        this->grid_[move.front()[0]][move.front()[1]] = White;

    for (int i = 0; i < 10; ++i) {
        if (this->position_colors_players_[this->who_is_to_play_][i][0]
            == move.back()[0]
            && this->position_colors_players_[this->who_is_to_play_][i][1]
               == move.back()[1]) {
            this->position_colors_players_[this->who_is_to_play_][i][0]
                    = move.front()[0];
            this->position_colors_players_[this->who_is_to_play_][i][1]
                    = move.front()[1];
            break;
        }
    }
}

/* FNV-1a hash function */
inline uint64_t AlphaBeta::fnv1a(uint64_t h, const int &x) {
    h ^= (uint64_t) x;
    h *= 0x100000001b3;
    return h;
}

inline uint64_t AlphaBeta::fnv1aColor(uint64_t h, const Color &x) {
    h ^= (uint64_t) x;
    h *= 0x100000001b3;
    return h;
}

uint64_t AlphaBeta::hashMatrix(const GridType &matrix, const int &player) {
    uint64_t hash = 0xcbf29ce484222325; /* FNV-1a seed value */
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            /* combine the hash values of the individual elements */
            hash = fnv1aColor(hash, matrix[i][j]);
        }
    }
    return fnv1a(hash, player);;
}

inline uint64_t AlphaBeta::hashMove(const ListOfPositionType &move) {
    uint64_t hash = 0xcbf29ce484222325; /* FNV-1a seed value */
    hash = fnv1a(hash, move[0][0]);
    hash = fnv1a(hash, move[0][1]);
    hash = fnv1a(hash, move.back()[0]);
    hash = fnv1a(hash, move.back()[1]);
    return hash;
}

inline uint64_t AlphaBeta::hashPosition(const PositionType &move) {
    uint64_t hash = 0xcbf29ce484222325; /* FNV-1a seed value */
    hash = fnv1a(hash, move[0]);
    hash = fnv1a(hash, move[1]);
    return hash;
}

Player AlphaBeta::get_maximizing_player_() const {
    return this->maximizing_player_;
}

std::vector<std::vector<double> > AlphaBeta::get_player_to_loose_value_() {
    return this->player_to_loose_value_;
}

std::vector<std::vector<double> > AlphaBeta::get_player_to_win_value_() {
    return this->player_to_win_value_;
}

void AlphaBeta::set_player_to_loose_value_(
        std::vector< std::vector<double> > &player_to_loose_value_) {
    this->player_to_loose_value_ = player_to_loose_value_;
}

void AlphaBeta::set_player_to_win_value_(
        std::vector< std::vector<double> > &player_to_win_value_  ) {
    this->player_to_win_value_ = player_to_win_value_;
}

void AlphaBeta::tensorflowOrderMoves(ListOfMoves &possible_moves) {
    /* copy the grid and reverse it if black is playing */
    std::vector<float> grid_temp(64);
    if (this->who_is_to_play_ == 0) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j)
                grid_temp[8 * i + j] = this->grid_[i][j];
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j)
                grid_temp[8 * (7 - i) + (7 - j)] = this->grid_[i][j];
        }
    }

    /* modify it in the right format */
    //for (int i = 0; i < 64; ++i)
        //if (grid_temp[i] == 2) grid_temp[i] = -1;

    /* switch colors if black */
    if (this->who_is_to_play_ == 1) {
        for (int i = 0; i < 64; ++i)
            grid_temp[i] *= -1;
    }

    std::vector<float> data_;
    /* duplicate it enough times */
    for (int d = 0; d < possible_moves.size(); ++d) {
        data_.insert(data_.begin(), grid_temp.begin(), grid_temp.end());
    }

    int i;
    int j;
    /* make the moves */
    for (int d = 0; d < possible_moves.size(); ++d) {
        i = possible_moves[d].front()[0];
        j = possible_moves[d].front()[1];
        if (this->who_is_to_play_ == 1) {
            i = 7 - i;
            j = 7 - j;
        }

        data_[64 * d + i * 8 + j] = 0;

        i = possible_moves[d].back()[0];
        j = possible_moves[d].back()[1];
        if (this->who_is_to_play_ == 1) {
            i = 7 - i;
            j = 7 - j;
        }

        data_[64 * d + i * 8 + j] = 1;
    }

    cppflow::tensor tensor_data_ = cppflow::tensor(data_, {static_cast<int>(possible_moves.size()), 64});

    std::vector<cppflow::tensor> output = (*model)({{"serving_default_dense_input:0", tensor_data_}},
                                                    {"StatefulPartitionedCall:0"});

    std::map<ListOfPositionType, double> res;

    auto output_data_ = output[0].get_data<float>();
    for (int d = 0; d < possible_moves.size(); ++d)
        res[possible_moves[d]] = output_data_[d];

    auto compMove = [&](const ListOfPositionType &a, const ListOfPositionType &b) {
        return res[a] < res[b];
    };

    std::sort(possible_moves.begin(), possible_moves.end(), compMove);
}

void AlphaBeta::sortDepth1(ListOfMoves &possible_moves) {
    auto compMove = [this](const ListOfPositionType &a, const ListOfPositionType &b){
        double valueA = 0;
        double valueB = 0;
        switch (maximizing_player_) {
            case 0:
                valueA += this->player_to_win_value_
                          [7 - a.back()[0]][7 - a.back()[1]]
                          - this->player_to_win_value_
                          [7 - a.front()[0]][7 - a.front()[1]];
                valueB += this->player_to_win_value_
                          [7 - b.back()[0]][7 - b.back()[1]]
                          - this->player_to_win_value_
                          [7 - b.front()[0]][7 - b.front()[1]];

                break;

            case 1:
                valueA += this->player_to_loose_value_
                          [a.back()[0]][a.back()[1]]
                          - this->player_to_loose_value_
                          [a.front()[0]][a.front()[1]];
                valueB += this->player_to_loose_value_
                          [b.back()[0]][b.back()[1]]
                          - this->player_to_loose_value_
                          [b.front()[0]][b.front()[1]];
                break;
        }
        return valueA < valueB;
    };
    std::sort(possible_moves.begin(), possible_moves.end(), compMove);
}