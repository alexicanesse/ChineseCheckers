/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file ChineseCheckers.cpp
 * \brief Core implementation of the game
 *
 * Implementation of the ChineseCheckers Class which is an implementation of the core of the game
 *
 */

/* ChineseCheckers.hpp */
#include "ChineseCheckers.hpp"

/* C++ libraries */
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <boost/unordered_map.hpp>
/* The following pragma are used to removed deprecation warning from boost
 * header files. Using them avoid to remove this warning from the entire project.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/python.hpp>
#pragma GCC diagnostic pop


/* Other */
#include <Types.hpp>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

MoveType ChineseCheckers::elementaryMove(const PositionType &original_position,
                                         const PositionType &arrival_position) {
    /* Check if the move is in the grid */
    if (!(   arrival_position[0]  >= 0
          && arrival_position[0]   < 8
          && arrival_position[1]  >= 0
          && arrival_position[1]   < 8
          && original_position[0] >= 0
          && original_position[0]  < 8
          && original_position[1] >= 0
          && original_position[1]  < 8)
        ) {

        return Illegal;
    }

    /*
     * Whatever happens, if the arrival_position is already occupied,
     * then the move is not valid
     */
    if ((this->bit_boards_.White | this->bit_boards_.Black) & int_to_uint64_[arrival_position[0]][arrival_position[1]])
        return Illegal;

    int a = original_position[0];
    int b = original_position[1];
    int c = arrival_position[0];
    int d = arrival_position[1];

    bool rep;

    /* Checks if the direction is legal */
    if ((c - a) && (d - b) && (c + d != a + b))
        return Illegal;

    if ((abs(a-c+b-d) <= 1) && (abs(a-c) + abs(b-d) <= 2))
        return notJump;

    std::vector<int> direction = {sgn(c - a), sgn(d - b)};

    int mid;
    if (direction[0])
        mid = direction[0]*(c - a)/2;
    else
        mid = direction[1]*(d - b)/2;

    /* Check if there is a pawn to jump over */
    if (!((this->bit_boards_.White | this->bit_boards_.Black) &
            int_to_uint64_[a + direction[0]*mid][b + direction[1]*mid])) {
        return Illegal;
    }

    /* Check that there aren't any pawns in the way */
    for (int k = 1; k < mid; ++k) {
        if ((this->bit_boards_.White | this->bit_boards_.Black) &
            int_to_uint64_[a + direction[0]*k][b + direction[1]*k])
            return Illegal;

        if ((this->bit_boards_.White | this->bit_boards_.Black) &
            int_to_uint64_[c - direction[0]*k][d - direction[1]*k])
            return Illegal;
    }
    return Jump;
}

ChineseCheckers::ChineseCheckers() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            uint64_to_pair_[un_64 << ((i * 8) + j)] = {i, j};
            int_to_uint64_[i][j] = un_64 << ((i * 8) + j);
            cantorPairing_[i][j] = 1 << this->cantorPairingFunction(i , j);
        }
    }

    /* Compute neighbouring positions */
    int i, j;
    for (uint_fast64_t pawnPosition = un_64; pawnPosition; pawnPosition <<= 1) {
        // direct_neighbours_[pawnPosition] = std::vector<uint_fast64_t>(0);
        std::tie(i, j) = uint64_to_pair_[pawnPosition];
        for(const std::vector<int> &direction : valid_lines) {
            if (      i + direction[0] >= 0
                   && j + direction[1] >= 0
                   && i + direction[0]  < 8
                   && j + direction[1]  < 8) {
                if (!((bit_boards_.White | bit_boards_.Black) & int_to_uint64_[i + direction[0]][j + direction[1]])) {
                    direct_neighbours_[pawnPosition].push_back(int_to_uint64_[i + direction[0]][j + direction[1]]);
                }
            }
        }
    }

    int s = 0;
    for (uint_fast64_t pawnPosition = un_64; pawnPosition; pawnPosition <<= 1) {
        std::tie(i, j) = uint64_to_pair_[pawnPosition];
        for (const std::vector<int> &direction: valid_lines) {
            std::vector<std::pair<std::pair<uint_fast64_t, uint_fast64_t>, uint_fast64_t > > lines_dir;

            for (int k = 1;    i + direction[0] * (k << 1) < 8
                            && j + direction[1] * (k << 1) < 8
                            && i + direction[0] * (k << 1) >= 0
                            && j + direction[1] * (k << 1) >= 0; ++k) {

                uint_fast64_t line = static_cast<uint_fast64_t>(0);
                for (int l = 1; l < k; ++l) {
                    line |= (int_to_uint64_[i + direction[0] * (k - l)][j + direction[1] * (k - l)]);
                    line |= (int_to_uint64_[i + direction[0] * (k + l)][j + direction[1] * (k + l)]);
                }
                line |= (int_to_uint64_[i + direction[0] * (k << 1)][j + direction[1] * (k << 1)]);

                if (line) {
                    lines_dir.push_back({
                         {int_to_uint64_[i + direction[0]*k][j + direction[1]*k],
                          int_to_uint64_[i + direction[0] * (k << 1)][j + direction[1] * (k << 1)]},
                          line
                    });
                }
            }

            k_neighbours_[pawnPosition].push_back(lines_dir);
        }
    }

    this->new_game();
    this->loadIllegalPositions();
}

bool ChineseCheckers::move(const Player &player,
                           const ListOfPositionType &list_moves) {
    /* Check if the game is over */
    if (this->state_of_game() != NotFinished) {
        std::cout << "Game is over! " << this->state_of_game() << "\n";
        return false;
    }

    /* Check that there actually is a move to play */
    if (list_moves.empty())
        return false;

    /* Check that the right player is playing */
    if (((this->bit_boards_.White & (un_64 << ((list_moves[0][0] * 8) + list_moves[0][1]))) && player == 1)
        || ((this->bit_boards_.Black & (un_64 << ((list_moves[0][0] * 8) + list_moves[0][1]))) && player == 0))
        return false;

    if (player ^ this->who_is_to_play_)
        return false;

    /* Check that every moves are legals */
    int n = static_cast<int>(list_moves.size());
    MoveType fst_move = elementaryMove(list_moves[0], list_moves[1]);

    if (fst_move == Illegal) {
        return false;
    } else if (fst_move == notJump) {
        if (n != 2)
            return false;
    } else {
        for (int i=1; i < n - 1; ++i) {
            if (elementaryMove(list_moves[i], list_moves[i + 1]) != Jump) {
                return false;
            }
        }
    }

    /* Applying the move */
    if (this->who_is_to_play_) {
        this->bit_boards_.Black |= int_to_uint64_[list_moves[n-1][0]][list_moves[n-1][1]];
        this->bit_boards_.Black &= ~int_to_uint64_[list_moves[0][0]][list_moves[0][1]];
    } else {
        this->bit_boards_.White |=  int_to_uint64_[list_moves[n-1][0]][list_moves[n-1][1]];
        this->bit_boards_.White &= ~int_to_uint64_[list_moves[0][0]][list_moves[0][1]];
    }

    /* Checks for an illegal position */
    if (this->isPositionIllegal()) {
        if (this->who_is_to_play_) {
            this->bit_boards_.Black |=  int_to_uint64_[list_moves[0][0]][list_moves[0][1]];
            this->bit_boards_.Black &= ~int_to_uint64_[list_moves[n-1][0]][list_moves[n-1][1]];
        } else {
            this->bit_boards_.White |=  int_to_uint64_[list_moves[0][0]][list_moves[0][1]];
            this->bit_boards_.White &= ~int_to_uint64_[list_moves[n-1][0]][list_moves[n-1][1]];
        }
        return false;
    }

    this->who_is_to_play_ ^= 1;

    /* indicates that this position has been seen another time */
    uint64_t hash = hashGrid();
    if (this->number_of_times_seen.contains(hash))
        this->number_of_times_seen[hash]++;
    else
        this->number_of_times_seen.insert({hash, 1});

    return true;
}


#warning remove this->
void ChineseCheckers::moveWithoutVerification(const uint_fast64_t &move) {
    if (this->who_is_to_play_)
        this->bit_boards_.Black ^= move;
    else
        this->bit_boards_.White ^= move;

    this->who_is_to_play_ ^= 1;

    /* indicates that this position has been seen another time */
    uint64_t hash = hashGrid();
    if (this->number_of_times_seen.contains(hash))
        ++this->number_of_times_seen[hash];
    else
        this->number_of_times_seen[hash] = 1;
}

/*
 * returns true or false to indicate if
 * the current position is a winning position
 */
Result ChineseCheckers::state_of_game() {
    /* Check if player 0 won */
    if ((this->bit_boards_.White & winning_positions_white_)
           && !(((this->bit_boards_.White | this->bit_boards_.Black) & winning_positions_white_)
                ^ winning_positions_white_))
        return WhiteWon;

    /* Check if Player 1 won */
    if ((this->bit_boards_.Black & winning_positions_black_)
        && !(((this->bit_boards_.White | this->bit_boards_.Black) & winning_positions_black_)
             ^ winning_positions_black_))
        return BlackWon;

    /* Check for a draw */
    if (this->number_of_times_seen[hashGrid()]
                                == MAX_NUMBER_OF_CYCLES_FOR_DRAW_)
        return Draw;
    return NotFinished;
}

void ChineseCheckers::new_game() {
    /* Initialize the grid */
    this->bit_boards_.White = this->winning_positions_black_;
    this->bit_boards_.Black = this->winning_positions_white_;

    this->who_is_to_play_ = 0;

    /* init the history */
    this->number_of_times_seen.clear();
    this->number_of_times_seen[hashGrid()] = 1;
}

void ChineseCheckers::print_grid_() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (this->bit_boards_.White & int_to_uint64_[i][j])
                std::cout << std::setw(2) << 1 << " ";
            else if (this->bit_boards_.Black & int_to_uint64_[i][j])
                std::cout << std::setw(2) << -1 << " ";
            else
                std::cout << std::setw(2) << 0 << " ";
        }
        std::cout << "\n";
    }
}

void ChineseCheckers::print_who_is_to_play_() {
    std::cout << this->who_is_to_play_ << "\n";
}

Player ChineseCheckers::get_who_is_to_play_() const {
    return this->who_is_to_play_;
}

boost::unordered_map<uint64_t, int>
        ChineseCheckers::get_number_of_times_seen() const {
    return this->number_of_times_seen;
}


/* FNV-1a hash function */
inline uint64_t ChineseCheckers::hashGrid() {
    return (0x100000001b3 * (0xcbf29ce484222325 ^ bit_boards_.White) ^ bit_boards_.Black);
}

int ChineseCheckers::cantorPairingFunction(const int &x, const int &y) {
    return (x + y) * (x + y + 1) / 2 + x;
}

void ChineseCheckers::loadIllegalPositions() {
    std::ifstream inFile("./raw_data/illegal_moves.dat");
    /* Iterate through the file and load each element through the file */
    std::string line;
    uint32_t hash;

    while (std::getline(inFile, line)) {
        std::istringstream ss(line);
        ss >> std::hex >> hash;

        this->illegalPositions[hash] = true;
    }
    /* Close the file */
    inFile.close();
}

bool ChineseCheckers::isPositionIllegal() {
    int i = 0, j = 0;

    /* white side */
    uint_fast32_t code = 0;

    /* First version of the code. It checks if the position *can* be illegal */
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 6 - i; ++j) {
            if (this->bit_boards_.Black & int_to_uint64_[7 - i][7 - j])
                code |= cantorPairing_[i][j];
        }
    }

    if (this->illegalPositions.contains(code)) {
       /* full test */
        for (i = 7; i > 1; --i) {
            for (j = 7; j > 8 - i; --j) {
                if (this->bit_boards_.Black & int_to_uint64_[7 - i][7 - j]) {
                    /*
                     * if the location is occupied with an enemy piece,
                     * the bit is set to 1
                     */
                    code |= cantorPairing_[i][j];
                } else if (this->bit_boards_.White & int_to_uint64_[7 - i][7 - j]) {
                    /*
                     *  if the location is occupied with an ally piece, there are two cases :
                     *  either the piece can move, therefore the bit is set to 0
                     *  or it cannot move, therefore the bit is set to 1
                     */
                    code |= cantorPairing_[i][j];
                    for (const std::vector<int> &direction : valid_lines_illegal) {
                        if (this->elementaryMove(
                                {7 - i, 7 - j},
                                {7 - i - direction[0], 7 - j - direction[1]})
                            != Illegal) {
                            code &= (0b111111111111111111111
                                     ^ cantorPairing_[i][j]);
                        }
                    }
                }
                /* if the location is unoccupied, the bit is set to 0 */
            }
        }
        if (this->illegalPositions.contains(code))
            return true;
    }




    /* black side */
    code = 0;

    /* First version of the code. It checks if the position *can* be illegal */
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 6 - i; ++j) {
            if (this->bit_boards_.White & int_to_uint64_[i][j])
                code |= cantorPairing_[i][j];
        }
    }

    if (this->illegalPositions.contains(code)) {
        /* full test */
        for (i = 0; i < 6; ++i) {
            for (j = 0; j < 6 - i; ++j) {
                if (this->bit_boards_.White & int_to_uint64_[i][j]) {
                    /*
                     * if the location is occupied with an enemy piece,
                     * the bit is set to 1
                     */
                    code |= cantorPairing_[i][j];
                } else if (this->bit_boards_.Black & int_to_uint64_[i][j]) {
                    /*
                     *  if the location is occupied with an ally piece, there are two cases :
                     *  either the piece can move, therefore the bit is set to 0
                     *  or it cannot move, therefore the bit is set to 1
                     */
                    code |= cantorPairing_[i][j];
                    for (const std::vector<int> &direction : valid_lines_illegal) {
                        if (this->elementaryMove({i, j}, {i + direction[0],
                                                          j + direction[1]})
                            != Illegal) {
                            code &= (0b111111111111111111111
                                     ^ cantorPairing_[i][j]);
                        }
                    }
                }
                /* if the location is unoccupied, the bit is set to 0 */
            }
        }
    }
    return this->illegalPositions.contains(code);
}

uint_fast64_t ChineseCheckers::get_bitBoardWhite() {
    return this->bit_boards_.White;
}

uint_fast64_t ChineseCheckers::get_bitBoardBlack() {
    return this->bit_boards_.Black;
}
