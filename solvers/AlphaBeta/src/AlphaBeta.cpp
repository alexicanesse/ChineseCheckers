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

/* Other */
#include "Types.hpp"
#include "ChineseCheckers.hpp"


ListOfMoves AlphaBeta::availableMoves(Player player){
    GridType grid = this->get_grid_();
    
    /* Indicates if there is an jump from (i, j) to (k, l) */
    std::vector<ListOfPositionType> possible_elementary_move(64);

    
    ListOfMoves result;

    /* Check the case of notJump moves */
    std::vector<std::vector<PositionType>> position_colors_players_
                                        = get_position_colors_players_();
    for (auto pownPosition : position_colors_players_.at(player)) {
        /* We check every possible directions */
        if (    pownPosition.at(0) + 1 < 8
            && (grid.at(pownPosition.at(0) + 1).at(pownPosition.at(1)) == Empty))
            result.push_back({pownPosition, {pownPosition.at(0) + 1, pownPosition.at(1)}});

        if (    pownPosition.at(0) - 1 >= 0
            && (grid.at(pownPosition.at(0) - 1).at(pownPosition.at(1)) == Empty))
            result.push_back({pownPosition, {pownPosition.at(0) - 1, pownPosition.at(1)}});

        if (    pownPosition.at(1) + 1 < 8
            && (grid.at(pownPosition.at(0)).at(pownPosition.at(1) + 1) == Empty))
            result.push_back({pownPosition, {pownPosition.at(0), pownPosition.at(1) + 1}});

        if (    pownPosition.at(1) - 1 >= 0
            && (grid.at(pownPosition.at(0)).at(pownPosition.at(1) - 1) == Empty))
            result.push_back({pownPosition, {pownPosition.at(0), pownPosition.at(1) - 1}});
        
        if (    pownPosition.at(0) + 1 < 8
            &&  pownPosition.at(1) - 1 >= 0
            && (grid.at(pownPosition.at(0) + 1).at(pownPosition.at(1) - 1) == Empty))
            result.push_back({pownPosition, {pownPosition.at(0) + 1, pownPosition.at(1) - 1}});
        
        if (    pownPosition.at(0) - 1 >= 0
            &&  pownPosition.at(1) + 1 < 8
            && (grid.at(pownPosition.at(0) - 1).at(pownPosition.at(1) + 1) == Empty))
            result.push_back({pownPosition, {pownPosition.at(0) - 1, pownPosition.at(1) + 1}});
    }
    
    /* Check the case of Jump moves */
    bool isPossible = true;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            /* Jumping on line+ */
            for (int k = 1; i + 2*k < 8; ++k) {
                /* Check if there is a pown to jump over */
                if (grid.at(i + k).at(j) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l){
                        if (grid.at(i + k + l).at(j) != Empty)
                            isPossible = false;
                    }
                    if (isPossible)
                        possible_elementary_move.at(8*i + j).push_back({i + 2*k, j});
                    break;
                }
            }

            /* Jumping on line- */
            for (int k = 1; i - 2*k >= 0; ++k) {
                /* Check if there is a pown to jump over */
                if (grid.at(i - k).at(j)) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l){
                        if (grid.at(i - k - l).at(j) != Empty)
                            isPossible = false;
                    }
                    if (isPossible)
                        possible_elementary_move.at(8*i + j).push_back({i - 2*k, j});
                    break;
                }
            }

            /* Jumping on row+ */
            for (int k = 1; j + 2*k < 8; ++k) {
                /* Check if there is a pown to jump over */
                if (grid.at(i).at(j + k) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l){
                        if (grid.at(i).at(j + k + l) != Empty)
                            isPossible = false;
                    }
                    if (isPossible)
                        possible_elementary_move.at(8*i + j).push_back({i, j + 2*k});
                    break;
                }
            }

            /* Jumping on row- */
            for (int k = 1; j - 2*k >= 0; ++k) {
                /* Check if there is a pown to jump over */
                if (grid.at(i).at(j - k) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l){
                        if (grid.at(i).at(j - k - l) != Empty)
                            isPossible = false;
                    }
                    if (isPossible)
                        possible_elementary_move.at(8*i + j).push_back({i, j - 2*k});
                    break;
                }
            }
            
            /* Jumping on diag right */
            for (int k = 1; i + 2*k < 8 && j - 2*k >= 0; ++k) {
                /* Check if there is a pown to jump over */
                if (grid.at(i + k).at(j - k) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l){
                        if (grid.at(i + k + l).at(j - k - l) != Empty)
                            isPossible = false;
                    }
                    if (isPossible)
                        possible_elementary_move.at(8*i + j).push_back({i + 2*k, j - 2*k});
                    break;
                }
            }
            
            /* Jumping on diag left */
            for (int k = 1; j + 2*k < 8 && i - 2*k >= 0; ++k) {
                /* Check if there is a pown to jump over */
                if (grid.at(i - k).at(j + k) != Empty) {
                    /* Check if the jump is valid */
                    isPossible = true;
                    for (int l = 1; l <= k; ++l){
                        if (grid.at(i - k - l).at(j + k + l) != Empty)
                            isPossible = false;
                    }
                    if (isPossible)
                        possible_elementary_move.at(8*i + j).push_back({i - 2*k, j + 2*k});
                    break;
                }
            }
        }
    }
    
    /*
     * Do a BFS to list all possible jumps.
     * Each pown is a root
     */
    for (auto root : position_colors_players_.at(player)) {
        std::queue<PositionType> queue;
        queue.push(root);
        
        std::map<PositionType, bool> explored;
        std::map<PositionType, ListOfPositionType> paths;
        explored[root] = true;
        paths[root] = {root};
        while (!queue.empty()) {
            PositionType v = queue.front();
            queue.pop();
            
            for (PositionType neig : possible_elementary_move.at(8*v.at(0) + v.at(1))) {
                if (!explored.contains(neig)
                    /* Checks that we are not jumping over the root (it has moved) */
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
