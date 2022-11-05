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


ListOfMoves AlphaBeta::availableMoves(Player player) {
    /* Indicates if there is an jump from (i, j) to (k, l) */
    std::vector<ListOfPositionType> possible_elementary_move(64);

    ListOfMoves result;

    /* Check the case of notJump moves */
    for (auto pownPosition : position_colors_players_.at(player)) {
        /* We check every possible directions */
        switch (pownPosition.at(0)) {
            case 7:
                if (grid_.at(pownPosition.at(0) - 1)
                    .at(pownPosition.at(1)) == Empty) {
                        result.push_back({pownPosition,
                                          {pownPosition.at(0) - 1,
                                           pownPosition.at(1)}});
                    }

                switch (pownPosition.at(1)) {
                    case 7:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) - 1}});
                            }
                        break;

                    case 0:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) + 1}});
                            }

                        if (grid_.at(pownPosition.at(0) - 1)
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0) - 1,
                                                   pownPosition.at(1) + 1}});
                            }
                        break;

                    default:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) + 1}});
                            }

                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) - 1}});
                            }

                        if (grid_.at(pownPosition.at(0) - 1)
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0) - 1,
                                                   pownPosition.at(1) + 1}});
                            }
                        break;
                }
                break;

            case 0:
                if (grid_.at(pownPosition.at(0) + 1)
                    .at(pownPosition.at(1)) == Empty) {
                        result.push_back({pownPosition,
                                          {pownPosition.at(0) + 1,
                                           pownPosition.at(1)}});
                    }

                switch (pownPosition.at(1)) {
                    case 7:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) - 1}});
                            }

                        if (grid_.at(pownPosition.at(0) + 1)
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0) + 1,
                                                   pownPosition.at(1) - 1}});
                            }
                        break;

                    case 0:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) + 1}});
                            }
                        break;

                    default:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) + 1}});
                            }

                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) - 1}});
                            }

                        if (grid_.at(pownPosition.at(0) + 1)
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0) + 1,
                                                   pownPosition.at(1) - 1}});
                            }
                        break;
                }
                break;

            default:
                if (grid_.at(pownPosition.at(0) + 1)
                    .at(pownPosition.at(1)) == Empty) {
                        result.push_back({pownPosition,
                                          {pownPosition.at(0) + 1,
                                           pownPosition.at(1)}});
                    }

                if (grid_.at(pownPosition.at(0) - 1)
                    .at(pownPosition.at(1)) == Empty) {
                        result.push_back({pownPosition,
                                          {pownPosition.at(0) - 1,
                                           pownPosition.at(1)}});
                    }

                switch (pownPosition.at(1)) {
                    case 7:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) - 1}});
                            }

                        if (grid_.at(pownPosition.at(0) + 1)
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0) + 1,
                                                   pownPosition.at(1) - 1}});
                            }
                        break;

                    case 0:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) + 1}});
                            }

                        if (grid_.at(pownPosition.at(0) - 1)
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0) - 1,
                                                   pownPosition.at(1) + 1}});
                            }
                        break;

                    default:
                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) + 1}});
                            }

                        if (grid_.at(pownPosition.at(0))
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0),
                                                   pownPosition.at(1) - 1}});
                            }

                        if (grid_.at(pownPosition.at(0) + 1)
                            .at(pownPosition.at(1) - 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0) + 1,
                                                   pownPosition.at(1) - 1}});
                            }

                        if (grid_.at(pownPosition.at(0) - 1)
                            .at(pownPosition.at(1) + 1) == Empty) {
                                result.push_back({pownPosition,
                                                  {pownPosition.at(0) - 1,
                                                   pownPosition.at(1) + 1}});
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
                if (grid_.at(i + k).at(j) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_.at(i + k + l).at(j) != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move.at((i << 3) | j)
                                .push_back({i + (k << 1), j});
                    break;
                }
            }

            /* Jumping on line- */
            for (int k = 1; i >= (k << 1); ++k) {
                /* Check if there is a pown to jump over */
                if (grid_.at(i - k).at(j)) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_.at(i - k - l).at(j) != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move.at((i << 3) | j)
                                .push_back({i - (k << 1), j});
                    break;
                }
            }

            /* Jumping on row+ */
            for (int k = 1; j + (k << 1) < 8; ++k) {
                /* Check if there is a pown to jump over */
                if (grid_.at(i).at(j + k) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_.at(i).at(j + k + l) != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move.at((i << 3) | j)
                                .push_back({i, j + (k << 1)});
                    break;
                }
            }

            /* Jumping on row- */
            for (int k = 1; j >= (k << 1); ++k) {
                /* Check if there is a pown to jump over */
                if (grid_.at(i).at(j - k) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_.at(i).at(j - k - l) != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move.at((i << 3) | j)
                                .push_back({i, j - (k << 1)});
                    break;
                }
            }

            /* Jumping on diag right */
            for (int k = 1; i + (k << 1) < 8 && j >= (k << 1); ++k) {
                /* Check if there is a pown to jump over */
                if (grid_.at(i + k).at(j - k) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_.at(i + k + l).at(j - k - l) != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move.at((i << 3) | j)
                                .push_back({i + (k << 1), j - (k << 1)});
                    break;
                }
            }

            /* Jumping on diag left */
            for (int k = 1; j + (k << 1) < 8 && i >= (k << 1); ++k) {
                /* Check if there is a pown to jump over */
                if (grid_.at(i - k).at(j + k) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l) {
                        if (grid_.at(i - k - l).at(j + k + l) != Empty) {
                            isPossible = false;
                            break;
                        }
                    }
                    if (isPossible)
                        possible_elementary_move.at((i << 3) | j)
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
    for (auto root : position_colors_players_.at(player)) {
        queue.push(root);

        std::map<PositionType, bool> explored;
        std::map<PositionType, ListOfPositionType> paths;
        explored[root] = true;
        paths[root] = {root};
        while (!queue.empty()) {
            PositionType v = queue.front();
            queue.pop();

            for (PositionType neig :
                        possible_elementary_move.at(8*v.at(0) + v.at(1))) {
                if (!explored.contains(neig)
                    /*
                     * Checks that we are not jumping over the root
                     * (it has moved)
                     */
                    && !((v.at(0) + neig.at(0))/2 == root.at(0)
                         && (v.at(1) + neig.at(1))/2 == root.at(1))) {
                    queue.push(neig);
                    explored[neig] = true;
                    paths[neig] = paths.at(v);
                    paths.at(neig).push_back(neig);
                }
            }
        }

        paths.extract(root);
        /* we add to result all the paths we found from this root */
        for (auto move : paths)
            result.push_back(move.second);
    }

    return result;
}

int AlphaBeta::evaluate(Player player) {
    int result = 0;
    switch (player) {
        case 0: /* White */
            for (PositionType pown : position_colors_players_.at(0))
                result += 14 - pown.at(0) - pown.at(1);
            break;

        case 1: /* black */
            for (PositionType pown : position_colors_players_.at(1))
                result += pown.at(0) + pown.at(1);
            break;

        default:
            break;
    }
    return result;
}

ListOfPositionType AlphaBeta::getMove(int depth, double alpha, double beta) {
    this->maximizing_player_ = this->who_is_to_play_;
    AlphaBetaEval(depth, alpha, beta, false, true);

    this->value_of_states_.clear();

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
            return 0;
            break;

        default: /* the game is not over */
            break;
    }

    if (depth == 0)
        return heuristicValue();

    /* The state is not a termination node */
    int value = 0;
    ListOfMoves possible_moves = this->availableMoves(this->who_is_to_play_);

    /* keeping state to restore to after applying temp moves */
    std::map<GridType, int> temp_number_of_times_seen =
            this->number_of_times_seen;
    Player temp_who_is_to_play_ = this->who_is_to_play_;
    GridType temp_grid_ = this->grid_;
    std::vector<std::vector<PositionType>> temp_position_colors_players_
            = this->position_colors_players_;

    ListOfPositionType best_move;
    if (maximizingPlayer) {
        value = -100000; /* -\infty */
        for (ListOfPositionType move : possible_moves) {
            /* restore to current game state */
            this->number_of_times_seen = temp_number_of_times_seen;
            this->who_is_to_play_ = temp_who_is_to_play_;
            this->grid_ = temp_grid_;
            this->position_colors_players_ = temp_position_colors_players_;

            this->moveWithoutVerification(this->who_is_to_play_, move);
            int buff = AlphaBetaEval(depth - 1,
                                     alpha,
                                     beta,
                                     !maximizingPlayer,
                                     false);
            if (buff >= value) {
                value = buff;
                best_move = move;
                if (value >= beta)
                    break; /* beta cutoff */
            }
            alpha = std::max(alpha, static_cast<double>(value));
        }
    } else {
        value = 100000; /* +\infty */
        /* For each possible move */
        for (ListOfPositionType move : possible_moves) {
            /* restore to current game state */
            this->number_of_times_seen = temp_number_of_times_seen;
            this->who_is_to_play_ = temp_who_is_to_play_;
            this->grid_ = temp_grid_;
            this->position_colors_players_ = temp_position_colors_players_;

            this->moveWithoutVerification(this->who_is_to_play_, move);
            int buff = AlphaBetaEval(depth - 1,
                                     alpha,
                                     beta,
                                     !maximizingPlayer,
                                     false);
            if (buff <= value) {
                value = buff;
                best_move = move;
                if (value <= alpha)
                    break; /* alpha cutoff */
            }
            beta = std::min(beta, static_cast<double>(value));
        }
    }

    /* restore to current game state */
    this->number_of_times_seen = temp_number_of_times_seen;
    this->who_is_to_play_ = temp_who_is_to_play_;
    this->grid_ = temp_grid_;
    this->position_colors_players_ = temp_position_colors_players_;

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
