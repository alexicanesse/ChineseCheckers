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
#define DRAW_VALUE (10);


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
    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    player_to_win_value_ = std::vector<double>({
        0.0/98,  1.0/98,  4.0/98,  9.0/98, 16.0/98, 25.0/98, 36.0/98, 49.0/98,
        1.0/98,  2.0/98,  5.0/98, 10.0/98, 17.0/98, 26.0/98, 37.0/98, 50.0/98,
        4.0/98,  5.0/98,  8.0/98, 13.0/98, 20.0/98, 29.0/98, 40.0/98, 53.0/98,
        9.0/98, 10.0/98, 13.0/98, 18.0/98, 25.0/98, 34.0/98, 45.0/98, 58.0/98,
        16.0/98, 17.0/98, 20.0/98, 25.0/98, 32.0/98, 41.0/98, 52.0/98, 65.0/98,
        25.0/98, 26.0/98, 29.0/98, 34.0/98, 41.0/98, 50.0/98, 62.0/98, 74.0/98,
        36.0/98, 37.0/98, 40.0/98, 45.0/98, 52.0/98, 62.0/98, 72.0/98, 85.0/98,
        49.0/98, 50.0/98, 53.0/98, 58.0/98, 65.0/98, 74.0/98, 85.0/98, 98.0/98});

    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    player_to_lose_value_ = std::vector<double>({
        0.0/588,  1.0/588,  4.0/588,  9.0/588, 16.0/588, 25.0/588, 36.0/588, 49.0/588,
        1.0/588,  2.0/588,  5.0/588, 10.0/588, 17.0/588, 26.0/588, 37.0/588, 50.0/588,
        4.0/588,  5.0/588,  8.0/588, 13.0/588, 20.0/588, 29.0/588, 40.0/588, 53.0/588,
        9.0/588, 10.0/588, 13.0/588, 18.0/588, 25.0/588, 34.0/588, 45.0/588, 58.0/588,
        16.0/588, 17.0/588, 20.0/588, 25.0/588, 32.0/588, 41.0/588, 52.0/588, 65.0/588,
        25.0/588, 26.0/588, 29.0/588, 34.0/588, 41.0/588, 50.0/588, 62.0/588, 74.0/588,
        36.0/588, 37.0/588, 40.0/588, 45.0/588, 52.0/588, 62.0/588, 72.0/588, 85.0/588,
        49.0/588, 50.0/588, 53.0/588, 58.0/588, 65.0/588, 74.0/588, 85.0/588, 98.0/588});

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            player_to_lose_value_map_white_[int_to_uint64_[i][j]] = player_to_lose_value_[(7 - i)*8 + 7 - j];
            player_to_lose_value_map_black_[int_to_uint64_[i][j]] = player_to_lose_value_[i*8 + j];
            player_to_win_value_map_white_[int_to_uint64_[i][j]]  = player_to_win_value_[(7 - i)*8 + 7 - j];
            player_to_win_value_map_black_[int_to_uint64_[i][j]]  = player_to_win_value_[i*8 + j];
        }
    }

    ctz_[0] = 0;
    for (uint_fast64_t i = un_64_; i; i <<= 1) {
        ctz_[i] = __builtin_ctzl(i);
    }

    /* Indicates if there is a jump from (i, j) to (k, l) */
    for (int i = 0; i < 64; ++i)
        possible_elementary_move_[un_64_ << i] = std::vector<uint_fast64_t>(0);

    transposition_table_.reserve(50*50*50);

    loadOpenings();
}

AlphaBeta::AlphaBeta(const std::vector<double> &player_to_win_value_,
                     const std::vector<double> &player_to_lose_value_) {
    this->player_to_win_value_ = player_to_win_value_;
    this->player_to_lose_value_ = player_to_lose_value_;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            player_to_lose_value_map_white_[int_to_uint64_[i][j]] = player_to_lose_value_[(7 - i)*8 + 7 - j];
            player_to_lose_value_map_black_[int_to_uint64_[i][j]] = player_to_lose_value_[i*8 + j];
            player_to_win_value_map_white_[int_to_uint64_[i][j]]  = player_to_win_value_[(7 - i)*8 + 7 - j];
            player_to_win_value_map_black_[int_to_uint64_[i][j]]  = player_to_win_value_[i*8 + j];
        }
    }

    ctz_[0] = 0;
    for (uint_fast64_t i = un_64_; i; i <<= 1) {
        ctz_[i] = __builtin_ctzl(i);
    }

    /* Indicates if there is a jump from (i, j) to (k, l) */
    for (int i = 0; i < 64; ++i)
        possible_elementary_move_[un_64_ << i] = std::vector<uint_fast64_t>(0);

    transposition_table_.reserve(50*50*50);

    loadOpenings();
}

void AlphaBeta::availableMoves(std::vector<uint_fast64_t> &result) {
    result.reserve(100);
    uint_fast64_t computed_possible_elementary_move_ = 0;
    uint_fast64_t bit_boards_all  = (bit_boards_.White | bit_boards_.Black);
    uint_fast64_t currentBitBoard = who_is_to_play_ ? bit_boards_.Black : bit_boards_.White;

    /* Check the case of Jump moves */
    /*
     * Do a BFS to list all possible jumps.
     * Each pown is a root
     */
    uint_fast64_t queue;
    uint_fast64_t explored = 0;
    uint_fast64_t v;
    int i, j, i_neig, j_neig, i_root_times_2, j_root_times_2;

    /* When another root is chosen, the queue is already empty */
    uint_fast64_t root;
    uint_fast64_t pawnPositionMask = currentBitBoard;

    std::vector<uint_fast64_t> temp_elementary_move;
    temp_elementary_move.reserve(12);
    
    for (root = pawnPositionMask & -pawnPositionMask;
         pawnPositionMask & -pawnPositionMask;
         root = pawnPositionMask & -pawnPositionMask) {
        pawnPositionMask ^= root;

        queue    = root;
        explored = root;

        std::tie(i_root_times_2, j_root_times_2) = uint64_to_pair_[root];
        i_root_times_2 <<= 1;
        j_root_times_2 <<= 1;

        while (queue) {
            v = queue & -queue;
            queue ^= v;

            std::tie(i, j) = uint64_to_pair_[v];

            if (!(computed_possible_elementary_move_ & v)) {
                temp_elementary_move.clear();
                for (const auto &possibleJumps : k_neighbours_[v]) {
                    for (const auto &possibleJump : possibleJumps) {
                        /* Check if there is a pawn to jump over and if the jump is valid */
                        if ((bit_boards_all & possibleJump.first.first)
                            && ! (bit_boards_all
                                  & (possibleJump.second | possibleJump.first.second))) {
                            temp_elementary_move.push_back(possibleJump.first.second);
                            break;
                        }
                    }
                }
                possible_elementary_move_[v] = temp_elementary_move;
                computed_possible_elementary_move_ |= v;
            }

            for (uint_fast64_t neig : possible_elementary_move_[v]) {
                std::tie(i_neig, j_neig) = uint64_to_pair_[neig];
                if (!(neig & explored)
                    /*
                     * Checks that we are not jumping over the root
                     * (it has moved)
                     */
                    && !((i + i_neig) == i_root_times_2
                         &&   (j + j_neig) == j_root_times_2)) {
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
            if (!((bit_boards_.White | bit_boards_.Black) & neig))
                result.push_back(pawnPosition | neig);
        }
    }
}

ListOfPositionType AlphaBeta::getMove(const int &depth, const double &alpha, const double &beta) {
    maximizing_player_ = who_is_to_play_;
    heuristic_value_   = heuristicValue();
    fullDepth_         = depth;

    if (opening_.find(bit_boards_) != opening_.end())
        return retrieveMoves(opening_[bit_boards_]);

    transposition_table_.clear();

    best_move_ = 0;

    int d = 1;
    do {
        if (AlphaBetaEval(d,
                      MINUS_INFTY,
                      MINUS_INFTY,
                      false,
                      true) == MINUS_INFTY)
            return retrieveMoves(best_move_);
        d += 2;
    }  while (won_[maximizing_player_] && (d <= depth + 2));

    double val = AlphaBetaEval(depth,
                               MINUS_INFTY,
                               PLUS_INFTY,
                               false,
                               true);

    won_[maximizing_player_] = (!won_[maximizing_player_]
                                    && (val == MINUS_INFTY));
    //std::cout << std::setw(2) << rank << " / " << number_of_moves << "\n";
    return retrieveMoves(best_move_);
}

uint_fast64_t AlphaBeta::getMove64(const int &depth) {
    maximizing_player_ = who_is_to_play_;
    heuristic_value_   = heuristicValue();
    fullDepth_         = depth;

    transposition_table_.clear();
    transposition_table_.reserve(50*50*50*50);

    best_move_ = 0;

    int d = 1;
    do {
        if (AlphaBetaEval(d,
                          MINUS_INFTY,
                          MINUS_INFTY,
                          false,
                          true) == MINUS_INFTY)
            return best_move_;
        d += 2;
    }  while (won_[maximizing_player_] && (d <= depth + 2));

    double val = AlphaBetaEval(depth,
                               MINUS_INFTY,
                               PLUS_INFTY,
                               false,
                               true);

    won_[maximizing_player_] = (!won_[maximizing_player_]
                                && (val == MINUS_INFTY));
    return best_move_;
}

const double AlphaBeta::AlphaBetaEval(const int &depth,
                             double alpha,
                             double beta,
                             const bool &maximizingPlayer,
                             const bool &keepMove) {
    /* Check if the current node is a terminating node */
    if (who_is_to_play_) {
        if ((bit_boards_.White & winning_positions_white_) /* Did white win ? */
            && ((bit_boards_.White | bit_boards_.Black) & winning_positions_white_)
                    == winning_positions_white_) {
            return maximizing_player_ ? PLUS_INFTY : MINUS_INFTY;
        }
    } else if (!who_is_to_play_
                && (bit_boards_.Black & winning_positions_black_) /* Did black win ? */
                && ((bit_boards_.White | bit_boards_.Black) & winning_positions_black_)
                        == winning_positions_black_) {
        return maximizing_player_ ? MINUS_INFTY : PLUS_INFTY;
    }

    uint_fast64_t hash = hashGrid();
    if (number_of_times_seen_[hash] == MAX_NUMBER_OF_CYCLES_FOR_DRAW_) { /* Is there a draw ? */
        return DRAW_VALUE;
    } else { /* the game is not over */
        if (depth == 0)
            return heuristic_value_;

        /* Use a transposition table to boost performances */
        it_transposition_table_ = transposition_table_.find(bit_boards_);
        if (it_transposition_table_ != transposition_table_.end()
            && it_transposition_table_->second.second == depth) {
            /* retrieve the value from the transposition table */
            return it_transposition_table_->second.first;
        }
    }

    /* Retrieve the possibles moves */
    std::vector<uint_fast64_t> possible_moves;
    availableMoves(possible_moves);

    /* Sort according to the value of the move in order to increase the number of cut-offs */
    if (0 && keepMove) tensorflowSortMoves(possible_moves);
    else std::sort(possible_moves.begin(), possible_moves.end(), comp_move_);

    /* We do not consider all moves in order to have a speed up */
    //if (MINUS_INFTY != beta) possible_moves.resize(std::min(15UL, possible_moves.size()));


    double value = maximizingPlayer ? MINUS_INFTY - 1 : PLUS_INFTY + 1;
    double buff;

//temp
    if (keepMove) {
        number_of_moves = possible_moves.size();
        rank  = number_of_moves;
    }
    int r = 0;
    for (const auto &move:possible_moves) {
//temp
        ++r;
        updateHeuristicValue(move);

        /* Applying the move */
        who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;
        who_is_to_play_ ^= 1;

        /* indicates that this position has been seen another time */
        uint64_t hash = hashGrid();
        ++number_of_times_seen_[hash];


        /* Checks for an illegal position */
        if (isPositionIllegal()) {
            reverseMove(move);
            updateHeuristicValueBack(move);
            continue;
        }

        buff = AlphaBetaEval(depth - 1,
                             alpha,
                             beta,
                             !maximizingPlayer,
                             false);

        --number_of_times_seen_[hash];
        who_is_to_play_ ^= 1;
        who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;

        updateHeuristicValueBack(move);

        if (maximizingPlayer && buff > value) {
            alpha = std::max(buff, alpha);
            value = buff;
            if (value >= beta)
                break; /* beta cutoff */
        } else if (buff < value) {
            beta  = std::min(buff, beta);
            value = buff;
            if (keepMove) {
                best_move_ = move;
                rank = r;
            }
            if (value <= alpha)
                break; /* alpha cutoff */
        }
    }

    /* store the value in the transposition table */
    if (depth < fullDepth_ - 1) transposition_table_.emplace(bit_boards_, std::make_pair(value, depth));

    /* return value */
    return value;
}

double AlphaBeta::heuristicValue() {
    double result = 0;
    for (uint_fast64_t pawnPosition = un_64_; pawnPosition; pawnPosition <<= 1) {
        if (pawnPosition & bit_boards_.White) {
            if (maximizing_player_)
                result -= player_to_lose_value_map_white_[pawnPosition];
            else
                result +=player_to_win_value_map_white_[pawnPosition];
        } else if (pawnPosition & bit_boards_.Black) {
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
            heuristic_value_ += player_to_win_value_map_black_[move & ~bit_boards_.Black]
                                - player_to_win_value_map_black_[move & bit_boards_.Black];
        } else {
            heuristic_value_ += player_to_lose_value_map_black_[move & bit_boards_.Black]
                               - player_to_lose_value_map_black_[move & ~bit_boards_.Black];
        }
    } else {
        if (maximizing_player_) {
            heuristic_value_ += player_to_lose_value_map_white_[move & bit_boards_.White]
                                - player_to_lose_value_map_white_[move & ~bit_boards_.White];
        } else {
            heuristic_value_ += player_to_win_value_map_white_[move & ~bit_boards_.White]
                                - player_to_win_value_map_white_[move & bit_boards_.White];
        }
    }
}

inline void AlphaBeta::updateHeuristicValueBack(const uint_fast64_t &move) {
    if (who_is_to_play_) {
        if (maximizing_player_) {
            heuristic_value_ += player_to_win_value_map_black_[move & bit_boards_.Black]
                                - player_to_win_value_map_black_[move & ~bit_boards_.Black];
        } else {
            heuristic_value_ += player_to_lose_value_map_black_[move & ~bit_boards_.Black]
                                - player_to_lose_value_map_black_[move & bit_boards_.Black];
        }
    } else {
        if (maximizing_player_) {
            heuristic_value_ += player_to_lose_value_map_white_[move & ~bit_boards_.White]
                                - player_to_lose_value_map_white_[move & bit_boards_.White];
        } else {
            heuristic_value_ += player_to_win_value_map_white_[move & bit_boards_.White]
                                - player_to_win_value_map_white_[move & ~bit_boards_.White];
        }
    }
}

void AlphaBeta::reverseMove(const uint_fast64_t &move) {
    --number_of_times_seen_[hashGrid()];
    who_is_to_play_ ^= 1;
    who_is_to_play_ ? bit_boards_.Black ^= move : bit_boards_.White ^= move;
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

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            player_to_lose_value_map_white_[int_to_uint64_[i][j]] = player_to_lose_value_[(7 - i)*8 + 7 - j];
            player_to_lose_value_map_black_[int_to_uint64_[i][j]] = player_to_lose_value_[i*8 + j];
        }
    }

    transposition_table_.clear();
}

void AlphaBeta::setPlayerToWinValue(const std::vector<double> &player_to_win_value_) {
    this->player_to_win_value_ = player_to_win_value_;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            player_to_win_value_map_white_[int_to_uint64_[i][j]]  = player_to_win_value_[(7 - i)*8 + 7 - j];
            player_to_win_value_map_black_[int_to_uint64_[i][j]]  = player_to_win_value_[i*8 + j];
        }
    }

    transposition_table_.clear();
}

void AlphaBeta::loadOpenings() {
    std::ifstream inFile("raw_data/openings.dat");

    /* Iterate through the file and load each element through the file */
    std::string line;
    uint_fast64_t move;
    bitBoards_t bb;
    while(std::getline(inFile, line)) {
        std::istringstream ss(line);
        ss >> std::hex >> bb.White >> std::hex >> bb.Black >> std::hex >> move;
        opening_[bb] = move;
    }

    /* Close the file */
    inFile.close();
}

/* FNV-1a hash function */
inline uint64_t AlphaBeta::hashGrid() {
    return (0x100000001b3 * (0xcbf29ce484222325 ^ bit_boards_.White) ^ bit_boards_.Black);
}

ListOfPositionType AlphaBeta::retrieveMoves(const uint_fast64_t &move) {
    uint_fast64_t computed_possible_elementary_move_ = 0;

    for (auto &x : possible_elementary_move_)
        x.second.clear();

    uint_fast64_t currentBitBoard = who_is_to_play_ ? bit_boards_.Black : bit_boards_.White;

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

    std::vector<uint_fast64_t> temp_elementary_move;
    temp_elementary_move.reserve(20);
    std::tie(i_root, j_root) = uint64_to_pair_[root];
    while (!queue.empty()) {
        v = queue.front();
        queue.pop();

        std::tie(i, j) = uint64_to_pair_[v];

        if (!(computed_possible_elementary_move_ & v)) {
            temp_elementary_move.clear();
            for (const auto &possibleJumps : k_neighbours_[v]) {
                for (const auto &possibleJump : possibleJumps) {
                    /* Check if there is a pawn to jump over and if the jump is valid */
                    if (((bit_boards_.White | bit_boards_.Black) & possibleJump.first.first)
                        && ! ((bit_boards_.White   | bit_boards_.Black        )
                            & (possibleJump.second | possibleJump.first.second))) {
                        temp_elementary_move.push_back(possibleJump.first.second);
                        break;
                    }
                }
            }
            possible_elementary_move_[v] = temp_elementary_move;
            computed_possible_elementary_move_ |= v;
        }

        for (uint_fast64_t neig : possible_elementary_move_[v]) {
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

void AlphaBeta::tensorflowSortMoves(std::vector<uint_fast64_t> &possible_moves) {
    /* We create a tensor for tensorFlow to evaluate all moves at once */
    std::vector<uint8_t> bb_temp;
    std::vector<float> data_;
    data_.reserve(128 * possible_moves.size());

    /* We get the right representation of each move */
    bitBoards_t bb;
    for (const uint_fast64_t &move : possible_moves) {
        bb = bit_boards_;
        if (who_is_to_play_) bb.Black ^= move;
        else                 bb.White ^= move;

        bb_temp = bitBoardsAsVector(bb);
        data_.insert(data_.end(), bb_temp.begin(), bb_temp.end());
    }

    cppflow::tensor tensor_data_ = cppflow::tensor(data_, {static_cast<int>(possible_moves.size()), 128});
    std::vector<cppflow::tensor> output = (*model)({{"serving_default_dense_input:0", tensor_data_}},
                                                    {"StatefulPartitionedCall:0"});

    std::unordered_map<uint_fast64_t, double> res;
    auto output_data_ = output[0].get_data<double>();
    for (int d = 0; d < possible_moves.size(); ++d)
        res[possible_moves[d]] = output_data_[d];

    auto compMoveW = [&](const uint_fast64_t &a, const uint_fast64_t &b) {
        return res[a] < res[b];
    };

    auto compMoveB = [&](const uint_fast64_t &a, const uint_fast64_t &b) {
        return res[a] > res[b];
    };

    if (who_is_to_play_) std::sort(possible_moves.begin(), possible_moves.end(), compMoveB);
    else                 std::sort(possible_moves.begin(), possible_moves.end(), compMoveW);
}

std::vector<uint8_t> AlphaBeta::bitBoardsAsVector(const bitBoards_t &bb) {
    /*
     * Copy the grid to a vector and reverse it if black is playing.
     * We need to reverse it if black is playing because the intuition
     * has been trained for white only.
     */
    std::vector<uint8_t> grid_temp(128, 0);
    int i;
    for (i = 0; i < 64; ++i) {
        if (bb.White & (un_64_ << i)) grid_temp[63  - i] = 1;
        if (bb.Black & (un_64_ << i)) grid_temp[127 - i] = 1;
    }
    return grid_temp;
}