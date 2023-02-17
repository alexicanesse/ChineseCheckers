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

/* Other */
#include "Types.hpp"
#include "ChineseCheckers.hpp"

AlphaBeta::AlphaBeta() {
    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    this->player_to_win_value_ = std::vector< std::vector<double> >({
        {-0.3938238518301734326, -0.5451218675269360325, -0.2935530671008477754, -0.6398157314495950265,  0.2784347610439370513, -0.1282987227675611019, -0.03258174717713029078, -0.120710121190223188},
        {-0.7375643525522759258, -0.4039952045156076643, -1,                      0.3904086832407110408,  0.4601396357937372894,  0.04656015792129830633, 0.6680736568650978979,   0.1150415374141592062},
        {-0.9364539654925738699, -0.6921924899595155445, -0.02774197319415187477, 0.08243665746845017794, 0.09619669833876696996, 0.3122506730637361061,  0.1595027137582161469,   0.5073433323253344618},
        {-0.5610695178396322147, -0.01498783551808732968, 0.6798990620298687171,  0.5701116357866707895,  0.2351936074509310415,  0.1404915093975186158,  0.2424080791753430164,   0.05194004231859112447},
        { 0.09693799493631916064, 0.2056825962109903594,  0.1802180488039032691, -0.03663997163899753579,-0.2163150050717693729, -0.5587610233631423728, -0.5528970997533583231,   0.5997231200353169145},
        {-0.01801199603258134968,-0.1168183770803564447, -0.12718467504105746,   -0.2100363886277294145,  0.3575083921335427961,  0.2393040111903078637,  0.3367997234226347003,   0.3120377599411665592},
        { 0.07377311332871627769, 0.7895393100783636431, -0.05750468202472907436, 0.5801751701001993666,  0.9126858432540749799, -0.01410435975665137731, 0.03579198295219955528,  0.1074913569714456146},
        { 0.6534482199177553685, -0.02676775416495711171, 0.3477505442611183439, -0.1526205197974657979,  0.5795929977996185922, -0.2201390495536915404, -0.09235462427808295571,  0.7152513487799585512}});

    /* this is meant to be seen from black perspective: white should
     * use symmetries to use this matrix. */
    this->player_to_loose_value_ = std::vector< std::vector<double> >({
        {-0.6106546279377850217, -0.8199757509151277546, 0.1640245356280993771, 0.5302405095846406402, 0.1431545000928561573,  0.4873912932388315133,   0.005699567492755297504, 0.1559569869872791115},
        {-0.4530245334764181897, -0.3565691891788236867, 0.1312744526160834202,-0.2515333957010222332, 0.01200145537821968589,-0.008992836222938121668,-0.9555872767930339018,  -0.1528050921833504772},
        { 0.5968604515677119471,  0.09910491369856155863,0.6065494893302656454, 0.1626503760990643532,-0.461706796166293898,   0.01695308180221040059, -0.7853661593595141488,   0.4577671585503212959},
        { 0.08928349287541292867, 0.09125070228034412589,-0.2746490931404308999,-0.120630016196057871, 0.3826419013608214903,  0.2001524758032698081,  -0.7208585827830876758,  -0.5410395263166249968},
        { 0.2204740833613630935, -0.2040798063430427078, -0.1758263753378166705, 0.443283338246931502, -0.3648843081300794378, 0.3084153829903355515,   0.586237198506328383,   -0.5261022253812194238},
        {-0.1605121532511817883,  0.1938726958797774846, -0.2443240510056174375, 0.1202861970824684046, 0.6468625239252131509, -0.05548460730664772633, 0.1005029277810626648, -0.2733098730564146317},
        {0.676857254037366074, -0.2864446634523950541, -0.7074368423396788508, -0.4362880910768491272, 0.6686428532238517519, 0.1633777774661320792, 0.3913251458888382661, 0.4149641807650202852},
        {-0.1419548025857928497, -0.4761503678250039728, 0.6143547088015027802, -0.377542154579426914, -0.2740492418784775674, 0.1763990824264763113, 0.4944970475758798667, -0.02683867796016010943}
    });

    this->loadOpenings();
}

AlphaBeta::AlphaBeta(const std::vector< std::vector<double> > &player_to_win_value_,
                     const std::vector< std::vector<double> > &player_to_loose_value_) {
    this->player_to_win_value_ = player_to_win_value_;
    this->player_to_loose_value_ = player_to_loose_value_;

    this->loadOpenings();
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

    if (0 && this->opening.contains(this->hashGrid())) {
        std::cout << "Found opening\n";
        return this->opening[this->hashGrid()];
    }

    AlphaBetaEval(depth, alpha, beta, false, true);

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
            return 0;
            break;

        default: /* the game is not over */
            break;
    }

    /* check if the current position has been searched before */
    /* compute the hash value of the current board position */
    uint64_t hash = hashMatrix(this->grid_, this->who_is_to_play_);
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
    if (0 && keepMove)
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

        /* Checks for an illegal position */
        if (this->isPositionIllegal()) {
            this->reverseMove(move);
            continue;
        }

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
    return evaluate(this->maximizing_player_)
                    - evaluate(1 - this->maximizing_player_);
}

void AlphaBeta::reverseMove(const ListOfPositionType &move){
    --this->number_of_times_seen.at(hashMatrix(this->grid_, 0));

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

        this->opening[hash] = move;
    }

    /* Close the file */
    inFile.close();
}