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
#include <random>
#include <chrono>
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

std::array<std::pair<int, int>, 64> initUint64ToPair() {
    std::array<std::pair<int, int>, 64> result;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            result[(i * 8) + j] = {i, j};
    }
    return result;
}

std::array<std::array<uint32_t, 8>, 8> initCantorPairing() {
    std::array<std::array<uint32_t, 8>, 8> result ;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            result[i][j] = 1 << ((i + j) * (i + j + 1) / 2 + i);
    }
    return result;
}

std::array<std::array<uint_fast64_t, 8>, 8> initIntToUint64() {
    std::array<std::array<uint_fast64_t, 8>, 8> result;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            result[i][j] = static_cast<uint_fast64_t>(1) << ((i * 8) + j);
    }
    return result;
}

std::array<std::vector<uint_fast64_t>, 64> initDirectNeighbours() {
    std::array<std::vector<uint_fast64_t>, 64> result;

    auto int_to_uint64_ = initIntToUint64();
    std::vector<std::vector<int>> valid_lines = {{-1,  0}, {-1,  1}, {0 , -1},
                                                 {0 ,  1}, {1 , -1}, {1 ,  0}};

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            for (const std::vector<int> &direction: valid_lines) {
                if (i + direction[0] >= 0
                    && j + direction[1] >= 0
                    && i + direction[0] < 8
                    && j + direction[1] < 8) {
                    result[8*i + j].push_back(int_to_uint64_[i + direction[0]][j + direction[1]]);
                }
            }
        }
    }
    return result;
}

std::array<std::vector<
        std::vector<std::pair<std::pair<uint_fast64_t,
        uint_fast64_t>, uint_fast64_t> > >, 64> initKNeighbours() {
    std::array<std::vector<
            std::vector<std::pair<std::pair<uint_fast64_t,
            uint_fast64_t>, uint_fast64_t> > >, 64> result;
    auto int_to_uint64_ = initIntToUint64();
    std::vector<std::vector<int>> valid_lines = {{-1,  0}, {-1,  1}, {0 , -1},
                                                 {0 ,  1}, {1 , -1}, {1 ,  0}};

    /* Loops over all positions of the board. */
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            /* Loop of all directions for a given position. */
            for (const std::vector<int> &direction: valid_lines) {
                std::vector<std::pair<std::pair<uint_fast64_t, uint_fast64_t>, uint_fast64_t > > lines_dir;

                /* Loop over all possible jump from a given position in a given direction. */
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
                result[8*i + j].push_back(lines_dir);
            }
        }
    }
    return result;
};

ChineseCheckers::ChineseCheckers() : uint64_to_pair_(initUint64ToPair()),
                                     cantor_pairing_(initCantorPairing()),
                                     int_to_uint64_(initIntToUint64()),
                                     direct_neighbours_(initDirectNeighbours()),
                                     k_neighbours_(initKNeighbours()),
                                     illegal_positions_(loadIllegalPositions()) {
    /* Compute the k-neighbours for each pawn position (ie, positions accessible by a jump). */
    int i, j, s = 0;

    /* Set up the board. */
    newGame();
    /* Generates Zobrist's keys. */
    generateZobristKeys();
}

void ChineseCheckers::newGame() {
    /* Initialize the grid. */
    bit_boards_.White = winning_positions_black_;
    bit_boards_.Black = winning_positions_white_;

    /* White is playing. */
    who_is_to_play_ = 0;

    /* Initialize the history. */
    number_of_times_seen_.clear();
    positions_seen_.clear();
    /* A good game usually last between 20 and 30 moves. */
    number_of_times_seen_.reserve(64);
    positions_seen_.reserve(64);
    number_of_times_seen_[zobrist_hash_] = 1;
    positions_seen_.push_back(zobrist_hash_);
}

void ChineseCheckers::generateZobristKeys() {
    /* Get a seed from the system clock to seed the random number generator. */
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 mt(seed);

    /* Generate a Zobrist key for each position on the board */
    for (uint_fast64_t i = un_64_; i; i <<= 1) {
        /* Both player have a different Zobrist's key for a given position. */
        zobrist_keys_[0][i] = mt();
        zobrist_keys_[1][i] = mt();
    }

    /* Pre-compute the Zobrist key for each possible move between two positions on the board. */
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < i; ++j){
            /* Combine the Zobrist keys for the two positions to generate a key for the move. */
            zobrist_keys_moves_[0][(un_64_ << i) | (un_64_ << j)] =
                    zobrist_keys_[0][(un_64_ << i)] ^ zobrist_keys_[0][(un_64_ << j)];
            zobrist_keys_moves_[1][(un_64_ << i) | (un_64_ << j)] =
                    zobrist_keys_[1][(un_64_ << i)] ^ zobrist_keys_[1][(un_64_ << j)];
        }
    }
}

void ChineseCheckers::computeAndSetZobristHash() {
    /* Set the initial Zobrist hash. */
    zobrist_hash_ = 0xFFFFFFFFFFFFFFFF;
    /* Iterate over each possible position of the board. */
    for (uint_fast64_t i = un_64_; i; i <<= 1) {
        /* If there is a white pawn at the given position, XOR the current hash with the corresponding key. */
        if (bit_boards_.White & i)
            zobrist_hash_ ^= zobrist_keys_[0][i];
        /* If there is a Black pawn at the given position, XOR the current hash with the corresponding key. */
        else if (bit_boards_.Black & i)
            zobrist_hash_ ^= zobrist_keys_[1][i];
    }
}

Result ChineseCheckers::stateOfGame() {
    /* Check if player 0 won */
    if ((bit_boards_.White & winning_positions_white_)
        && !(((bit_boards_.White | bit_boards_.Black) & winning_positions_white_)
             ^ winning_positions_white_))
        return WhiteWon;

    /* Check if Player 1 won */
    if ((bit_boards_.Black & winning_positions_black_)
        && !(((bit_boards_.White | bit_boards_.Black) & winning_positions_black_)
             ^ winning_positions_black_))
        return BlackWon;

    /* Check for a draw */
    if (number_of_times_seen_[zobrist_hash_]
        == MAX_NUMBER_OF_CYCLES_FOR_DRAW_)
        return Draw;
    return NotFinished;
}

void ChineseCheckers::moveWithoutVerification(const uint_fast64_t &move) {
    /* Update the right board. */
    if (who_is_to_play_)
        bit_boards_.Black ^= move;
    else
        bit_boards_.White ^= move;

    /* Update the hash. */
    zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][move];
    /* Update the player who is to play next. */
    who_is_to_play_ ^= 1;

    /* Increase the count of the current position. */
    ++number_of_times_seen_[zobrist_hash_];
    if (std::find(positions_seen_.begin(), positions_seen_.end(), zobrist_hash_)
            == positions_seen_.end())
        positions_seen_.push_back(zobrist_hash_);

}

bool ChineseCheckers::move(const Player &player,
                           const ListOfPositionType &list_moves) {
    /* Check if the game is over */
    if (stateOfGame() != NotFinished) {
        std::cout << "Game is over! " << stateOfGame() << "\n";
        return false;
    }

    /* Check that there actually is a move to play */
    if (list_moves.empty())
        return false;

    /* Check that the right player is playing */
    if (((bit_boards_.White & (un_64_ << ((list_moves[0][0] * 8) + list_moves[0][1]))) && player == 1)
        || ((bit_boards_.Black & (un_64_ << ((list_moves[0][0] * 8) + list_moves[0][1]))) && player == 0))
        return false;

    /* Check that it's the current player's turn to play. */
    if (player != who_is_to_play_)
        return false;

    /* Check that every move is legal */
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
    if (who_is_to_play_) {
        bit_boards_.Black |= int_to_uint64_[list_moves[n-1][0]][list_moves[n-1][1]];
        bit_boards_.Black &= ~int_to_uint64_[list_moves[0][0]][list_moves[0][1]];
    } else {
        bit_boards_.White |=  int_to_uint64_[list_moves[n-1][0]][list_moves[n-1][1]];
        bit_boards_.White &= ~int_to_uint64_[list_moves[0][0]][list_moves[0][1]];
    }

    /* Checks for an illegal position */
    if (isPositionIllegal()) {
        /* If illegal, undo the move and return false */
        if (who_is_to_play_) {
            bit_boards_.Black |=  int_to_uint64_[list_moves[0][0]][list_moves[0][1]];
            bit_boards_.Black &= ~int_to_uint64_[list_moves[n-1][0]][list_moves[n-1][1]];
        } else {
            bit_boards_.White |=  int_to_uint64_[list_moves[0][0]][list_moves[0][1]];
            bit_boards_.White &= ~int_to_uint64_[list_moves[n-1][0]][list_moves[n-1][1]];
        }
        return false;
    }

    /* Update the Zobrist hash with the current move. */
    zobrist_hash_ ^= zobrist_keys_moves_[who_is_to_play_][
            (un_64_ << (list_moves[  0][0]*8 + list_moves[  0][1]))
            | (un_64_ << (list_moves[n-1][0]*8 + list_moves[n-1][1]))];

    /* Switch to the other player's turn. */
    who_is_to_play_ ^= 1;

    /* Increase the count of the current position. */
    ++number_of_times_seen_[zobrist_hash_];
    if (std::find(positions_seen_.begin(), positions_seen_.end(), zobrist_hash_)
        == positions_seen_.end())
        positions_seen_.push_back(zobrist_hash_);

    return true;
}

MoveType ChineseCheckers::elementaryMove(const PositionType &original_position,
                                         const PositionType &arrival_position) const {
    /* Check if the move is in the grid. */
    if (!(   arrival_position[0]  >= 0
          && arrival_position[0]   < 8
          && arrival_position[1]  >= 0
          && arrival_position[1]   < 8
          && original_position[0] >= 0
          && original_position[0]  < 8
          && original_position[1] >= 0
          && original_position[1]  < 8)
        ) {
        /* If the move is not in the grid, return Illegal. */
        return Illegal;
    }

    /* Whatever happens, if the arrival_position is already occupied,
     * then the move is not valid */
    if ((bit_boards_.White | bit_boards_.Black) & int_to_uint64_[arrival_position[0]][arrival_position[1]])
        return Illegal;

    int a = original_position[0];
    int b = original_position[1];
    int c = arrival_position[0];
    int d = arrival_position[1];

    bool rep;

    /* Check if the move is in a valid direction. */
    if ((c - a) && (d - b) && (c + d != a + b))
        return Illegal;

    /* Check if the move is not a jump. */
    if ((abs(a-c+b-d) <= 1) && (abs(a-c) + abs(b-d) <= 2))
        return notJump;

    /* Compute the direction of the move. */
    std::vector<int> direction = {sgn(c - a), sgn(d - b)};

    /* Compute the position of the pawn jump over. */
    int mid;
    if (direction[0])
        mid = direction[0]*(c - a)/2;
    else
        mid = direction[1]*(d - b)/2;

    /* Check if there is a pawn to jump over */
    if (!((bit_boards_.White | bit_boards_.Black) &
            int_to_uint64_[a + direction[0]*mid][b + direction[1]*mid])) {
        return Illegal;
    }

    /* Check that there aren't any pawns in the way */
    for (int k = 1; k < mid; ++k) {
        if ((bit_boards_.White | bit_boards_.Black) &
            int_to_uint64_[a + direction[0]*k][b + direction[1]*k])
            return Illegal;

        if ((bit_boards_.White | bit_boards_.Black) &
            int_to_uint64_[c - direction[0]*k][d - direction[1]*k])
            return Illegal;
    }
    /* If everything is valid, return Jump. */
    return Jump;
}

boost::unordered_map<uint32_t, bool> ChineseCheckers::loadIllegalPositions() {
    boost::unordered_map<uint32_t, bool> result;
    std::ifstream inFile("./raw_data/illegal_moves.dat");
    /* Iterate through the file and load each element through the file. */
    std::string line;
    uint32_t hash;

    while (std::getline(inFile, line)) {
        std::istringstream ss(line);
        ss >> std::hex >> hash;

        result[hash] = true;
    }
    /* Close the file. */
    inFile.close();

    return result;
}

int ChineseCheckers::cantorPairingFunction(const int &x, const int &y) {
    return (x + y) * (x + y + 1) / 2 + x;
}

bool ChineseCheckers::isPositionIllegal() const {
    int i = 0, j = 0;

    /* White side. */
    uint32_t code = 0;

    /* First version of the code. It checks if the position *can* be illegal. */
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 6 - i; ++j) {
            if (bit_boards_.Black & int_to_uint64_[7 - i][7 - j])
                code |= cantor_pairing_[i][j];
        }
    }

    if (illegal_positions_.find(code) != illegal_positions_.end()) {
       /* Full test. */
        for (i = 7; i > 1; --i) {
            for (j = 7; j > 8 - i; --j) {
                if (bit_boards_.Black & int_to_uint64_[7 - i][7 - j]) {
                    /* If the location is occupied with an enemy piece,
                     * the bit is set to 1. */
                    code |= cantor_pairing_[i][j];
                } else if (bit_boards_.White & int_to_uint64_[7 - i][7 - j]) {
                    /* If the location is occupied with an allay piece, there are two cases :
                     *  either the piece can move, therefore the bit is set to 0,
                     *  or it cannot move, therefore the bit is set to 1. */
                    code |= cantor_pairing_[i][j];
                    for (const std::vector<int> &direction : valid_lines_illegal) {
                        if (elementaryMove(
                                {7 - i, 7 - j},
                                {7 - i - direction[0], 7 - j - direction[1]})
                            != Illegal) {
                            code &= (0b111111111111111111111
                                     ^ cantor_pairing_[i][j]);
                        }
                    }
                }
                /* If the location is unoccupied, the bit is set to 0. */
            }
        }
        if (illegal_positions_.find(code) != illegal_positions_.end())
            return true;
    }


    /* Black side. */
    code = 0;

    /* First version of the code. It checks if the position *can* be illegal. */
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 6 - i; ++j) {
            if (bit_boards_.White & int_to_uint64_[i][j])
                code |= cantor_pairing_[i][j];
        }
    }

    if (illegal_positions_.find(code) != illegal_positions_.end()) {
        /* Full test. */
        for (i = 0; i < 6; ++i) {
            for (j = 0; j < 6 - i; ++j) {
                if (bit_boards_.White & int_to_uint64_[i][j]) {
                    /* If the location is occupied with an enemy piece,
                     * the bit is set to 1. */
                    code |= cantor_pairing_[i][j];
                } else if (bit_boards_.Black & int_to_uint64_[i][j]) {
                    /* If the location is occupied with an allay piece, there are two cases :
                     *  either the piece can move, therefore the bit is set to 0,
                     *  or it cannot move, therefore the bit is set to 1. */
                    code |= cantor_pairing_[i][j];
                    for (const std::vector<int> &direction : valid_lines_illegal) {
                        if (elementaryMove({i, j}, {i + direction[0],
                                                          j + direction[1]})
                            != Illegal) {
                            code &= (0b111111111111111111111
                                     ^ cantor_pairing_[i][j]);
                        }
                    }
                }
                /* If the location is unoccupied, the bit is set to 0. */
            }
        }
    }
    return illegal_positions_.find(code) != illegal_positions_.end();
}

uint_fast64_t ChineseCheckers::getBitBoardWhite() const {
    return bit_boards_.White;
}

uint_fast64_t ChineseCheckers::getBitBoardBlack() const {
    return bit_boards_.Black;
}

Player ChineseCheckers::getWhoIsToPlay() const {
    return who_is_to_play_;
}

void ChineseCheckers::printGrid() const {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (bit_boards_.White & int_to_uint64_[i][j])
                std::cout << std::setw(2) << 1 << " ";
            else if (bit_boards_.Black & int_to_uint64_[i][j])
                std::cout << std::setw(2) << -1 << " ";
            else
                std::cout << std::setw(2) << 0 << " ";
        }
        std::cout << "\n";
    }
}

void ChineseCheckers::printWhoIsToPlay() const {
    std::cout << who_is_to_play_ << "\n";
}