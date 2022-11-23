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

/* AlphaBeta.hpp */
#include "AlphaBeta.hpp"

/* C++ Libraries */
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

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


ListOfMoves AlphaBeta::availableMoves(Player player) {
    /* Indicates if there is a jump from (i, j) to (k, l) */
    std::vector<ListOfPositionType> possible_elementary_move(64);

    ListOfMoves result;

    /* Check the case of notJump moves */
    for (const auto &pownPosition : position_colors_players_[player]) {
        /* We check every possible directions */
        switch (pownPosition[0]) {
            case 7:
                if (grid_[pownPosition[0] - 1]
                    [pownPosition[1]] == Empty) {
                        result.push_back({pownPosition,
                                          {pownPosition[0] - 1,
                                           pownPosition[1]}});
                    }

                switch (pownPosition[1]) {
                    case 7:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] - 1}});
                            }
                        break;

                    case 0:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] + 1}});
                            }

                        if (grid_[pownPosition[0] - 1]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0] - 1,
                                                   pownPosition[1] + 1}});
                            }
                        break;

                    default:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] + 1}});
                            }

                        if (grid_[pownPosition[0]]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] - 1}});
                            }

                        if (grid_[pownPosition[0] - 1]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0] - 1,
                                                   pownPosition[1] + 1}});
                            }
                        break;
                }
                break;

            case 0:
                if (grid_[pownPosition[0] + 1]
                    [pownPosition[1]] == Empty) {
                        result.push_back({pownPosition,
                                          {pownPosition[0] + 1,
                                           pownPosition[1]}});
                    }

                switch (pownPosition[1]) {
                    case 7:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] - 1}});
                            }

                        if (grid_[pownPosition[0] + 1]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0] + 1,
                                                   pownPosition[1] - 1}});
                            }
                        break;

                    case 0:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] + 1}});
                            }
                        break;

                    default:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] + 1}});
                            }

                        if (grid_[pownPosition[0]]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] - 1}});
                            }

                        if (grid_[pownPosition[0] + 1]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0] + 1,
                                                   pownPosition[1] - 1}});
                            }
                        break;
                }
                break;

            default:
                if (grid_[pownPosition[0] + 1]
                    [pownPosition[1]] == Empty) {
                        result.push_back({pownPosition,
                                          {pownPosition[0] + 1,
                                           pownPosition[1]}});
                    }

                if (grid_[pownPosition[0] - 1]
                    [pownPosition[1]] == Empty) {
                        result.push_back({pownPosition,
                                          {pownPosition[0] - 1,
                                           pownPosition[1]}});
                    }

                switch (pownPosition[1]) {
                    case 7:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] - 1}});
                            }

                        if (grid_[pownPosition[0] + 1]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0] + 1,
                                                   pownPosition[1] - 1}});
                            }
                        break;

                    case 0:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] + 1}});
                            }

                        if (grid_[pownPosition[0] - 1]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0] - 1,
                                                   pownPosition[1] + 1}});
                            }
                        break;

                    default:
                        if (grid_[pownPosition[0]]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] + 1}});
                            }

                        if (grid_[pownPosition[0]]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0],
                                                   pownPosition[1] - 1}});
                            }

                        if (grid_[pownPosition[0] + 1]
                            [pownPosition[1] - 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0] + 1,
                                                   pownPosition[1] - 1}});
                            }

                        if (grid_[pownPosition[0] - 1]
                            [pownPosition[1] + 1] == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition[0] - 1,
                                                   pownPosition[1] + 1}});
                            }
                        break;
                }
                break;
        }
    }

    /* Check the case of Jump moves */
    bool isPossible = true;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            /* Jumping on line+ */
            for (int k = 1; i + (k << 1) < 8; ++k) {
                /* Check if there is a pown to jump over */
                if (grid_[i + k][j] != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_[i + k + l][j] != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move[(i << 3) | j]
                                .push_back({i + (k << 1), j});
                    break;
                }
            }

            /* Jumping on line- */
            for (int k = 1; i >= (k << 1); ++k) {
                /* Check if there is a pown to jump over */
                if (grid_[i - k][j]) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_[i - k - l][j] != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move[(i << 3) | j]
                                .push_back({i - (k << 1), j});
                    break;
                }
            }

            /* Jumping on row+ */
            for (int k = 1; j + (k << 1) < 8; ++k) {
                /* Check if there is a pown to jump over */
                if (grid_[i][j + k] != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_[i][j + k + l] != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move[(i << 3) | j]
                                .push_back({i, j + (k << 1)});
                    break;
                }
            }

            /* Jumping on row- */
            for (int k = 1; j >= (k << 1); ++k) {
                /* Check if there is a pown to jump over */
                if (grid_[i][j - k] != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_[i][j - k - l] != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move[(i << 3) | j]
                                .push_back({i, j - (k << 1)});
                    break;
                }
            }

            /* Jumping on diag right */
            for (int k = 1; i + (k << 1) < 8 && j >= (k << 1); ++k) {
                /* Check if there is a pown to jump over */
                if (grid_[i + k][j - k] != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_[i + k + l][j - k - l] != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move[(i << 3) | j]
                                .push_back({i + (k << 1), j - (k << 1)});
                    break;
                }
            }

            /* Jumping on diag left */
            for (int k = 1; j + (k << 1) < 8 && i >= (k << 1); ++k) {
                /* Check if there is a pown to jump over */
                if (grid_[i - k][j + k] != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_[i - k - l][j + k + l] != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move[(i << 3) | j]
                                .push_back({i - (k << 1), j + (k << 1)});
                    break;
                }
            }
        }
    }

    /*
     * Do a BFS to list all possible jumps.
     * Each pown is a root
     */
    std::queue<PositionType> queue;
    /* When another root is chosen, the queue is already empty */
    for (auto root : position_colors_players_[player]) {
        queue.push(root);

        std::map<PositionType, bool> explored;
        std::map<PositionType, ListOfPositionType> paths;
        explored[root] = true;
        paths[root] = {root};
        while (!queue.empty()) {
            PositionType v = queue.front();
            queue.pop();

            for (PositionType neig :
                        possible_elementary_move[(v[0] << 3) | v[1]]) {
                if (!explored.contains(neig)
                    /*
                     * Checks that we are not jumping over the root
                     * (it has moved)
                     */
                    && !((v[0] + neig[0])/2 == root[0]
                         && (v[1] + neig[1])/2 == root[1])) {
                    queue.push(neig);
                    explored[neig] = true;
                    paths[neig] = paths[v];
                    paths[neig].push_back(neig);
                }
            }
        }

        paths.extract(root);
        /* we add to result all the paths we found from this root */
        for (const auto &move : paths)
            result.push_back(move.second);
    }

    return result;
}

int AlphaBeta::evaluate(Player player) {
    int result = 0;
    switch (player) {
        case 0:  /* White */
            if (player == this->maximizing_player_) {
                for (const PositionType &pown: position_colors_players_[0])
                    result += this->player_to_win_value_[7 - pown[0]][7 - pown[1]];
            } else {
                for (const PositionType &pown: position_colors_players_[0])
                    result += this->player_to_loose_value_[7 - pown[0]][7 - pown[1]];
            }
/*            for (const PositionType &pown : position_colors_players_[0])
                result += (7 - pown[0])*(7 - pown[0])
                          + (7 - pown[1])*(7 - pown[1]);*/
            break;

        case 1:  /* black */
            if (player == this->maximizing_player_) {
                for (const PositionType &pown: position_colors_players_[1])
                    result += this->player_to_win_value_[pown[0]][pown[1]];
            } else {
                for (const PositionType &pown: position_colors_players_[1])
                    result += this->player_to_loose_value_[pown[0]][pown[1]];
            }
/*            for (const PositionType &pown : position_colors_players_[1])
                result += pown[0]*pown[0] + pown[1]*pown[1];
            break;*/

        default:
            break;
    }
    return result;
}
#warning use inf
ListOfPositionType AlphaBeta::getMove(int depth, double alpha, double beta) {
    this->maximizing_player_ = this->who_is_to_play_;
    AlphaBetaEval(depth, alpha, beta, false, true);
    return this->best_move_;
}

int AlphaBeta::AlphaBetaEval(const int depth,
                             double alpha,
                             double beta,
                             const bool maximizingPlayer,
                             const bool keepMove) {
    /* Check if the current node is a terminating node */
    switch (this->state_of_game()) {
        case WhiteWon:
            if (this->maximizing_player_ == 0)
                return -100000;
            else
                return 100000;
            break;

        case BlackWon:
            if (this->maximizing_player_ == 1)
                return -100000;
            else
                return 100000;
            break;

        case Draw:
            if (this->maximizing_player_ == 1 - this->who_is_to_play_)
                return 50000;
            else
                return -50000;
            break;

        default: /* the game is not over */
            break;
    }

    if (depth == 0)
        return heuristicValue();

    /* The state is not a termination node */
    int value = 0;
    ListOfMoves possible_moves = this->availableMoves(this->who_is_to_play_);
#warning rename moveType
    if (keepMove) {
        auto compFunc = [this](ListOfPositionType a,
                ListOfPositionType b) {
            double valueA = 0;
            double valueB = 0;
            switch (maximizing_player_) {
                case 0:
                    valueA = (7 - a.back()[0])*(7 - a.back()[0])
                              + (7 - a.back()[1])*(7 - a.back()[1]);
                    valueA -= (7 - a.front()[0])*(7 - a.front()[0])
                              + (7 - a.front()[1])*(7 - a.front()[1]);

                    valueB = (7 - b.back()[0])*(7 - b.back()[0])
                             + (7 - b.back()[1])*(7 - b.back()[1]);
                    valueB -= (7 - b.front()[0])*(7 - b.front()[0])
                              + (7 - b.front()[1])*(7 - b.front()[1]);
                    break;
            }

            return valueA < valueB;
        };

        std::sort(possible_moves.begin(), possible_moves.end(), compFunc);
    }


    ListOfPositionType best_move;
    if (maximizingPlayer) {
        value = -100000; /* -\infty */
        for (const ListOfPositionType &move : possible_moves) {
            this->moveWithoutVerification(this->who_is_to_play_, move);
            int buff = AlphaBetaEval(depth - 1,
                                     alpha,
                                     beta,
                                     !maximizingPlayer,
                                     false);

            --this->number_of_times_seen.at(this->grid_);
            this->who_is_to_play_ = 1 - this->who_is_to_play_;
            this->grid_[move.back()[0]][move.back()[1]] = Empty;
            this->grid_[move.front()[0]][move.front()[1]]
                    = (Color) (this->who_is_to_play_ + 1);
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


            alpha = std::max(alpha, static_cast<double>(buff));
            if (buff >= value) {
                value = buff;
                best_move = move;
                if (value >= beta)
                    break; /* beta cutoff */
            }
        }
    } else {
        value = 100000; /* +\infty */
        /* For each possible move */
        for (const ListOfPositionType &move : possible_moves) {
            this->moveWithoutVerification(this->who_is_to_play_, move);
            int buff = AlphaBetaEval(depth - 1,
                                     alpha,
                                     beta,
                                     !maximizingPlayer,
                                     false);

            --this->number_of_times_seen.at(this->grid_);
            this->who_is_to_play_ = 1 - this->who_is_to_play_;
            this->grid_[move.back()[0]][move.back()[1]] = Empty;
            this->grid_[move.front()[0]][move.front()[1]]
                    = (Color) (this->who_is_to_play_ + 1);
            for (int i = 0; i < 10; ++i) {
                if (this->position_colors_players_[this->who_is_to_play_][i][0]
                == move.back()[0]
                && this->position_colors_players_[this->who_is_to_play_][i][1]
                        == move.back()[1]) {
                    this->position_colors_players_[this->who_is_to_play_][i][0]
                    = move.front()[0];
                    this->position_colors_players_[this->who_is_to_play_][i][1]
                    = move.front()[1];
                }
            }

            beta = std::min(beta, static_cast<double>(buff));
            if (buff <= value) {
                value = buff;
                best_move = move;
                if (value <= alpha)
                    break; /* alpha cutoff */
            }
        }
    }


    /* set the best move if asked to */
    if (keepMove)
        this->best_move_ = best_move;

    /* return value */
    return value;
}

int AlphaBeta::heuristicValue() {
    return 6*evaluate(this->maximizing_player_)
                    - evaluate(1 - this->maximizing_player_);
}
