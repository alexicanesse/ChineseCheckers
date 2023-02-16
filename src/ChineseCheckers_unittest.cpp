/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file ChineseCheckers_unittest.cpp
 * \brief Unit test of the core implementation of the game
 */

/* ChineseCheckers.hpp */
#include "ChineseCheckers.hpp"

/* C libraries */
#include <gtest/gtest.h>

/* C++ libraries */
#include <vector>
#include <algorithm>

/* Other */
#include <Types.hpp>

/*! @brief
 * The fixture for testing class ChineseCheckers
 */
class ChineseCheckersTest : public::testing::Test {
};


/* only public interface should be tested.
 * Each test suit correspond to a method. */

/*
 * Tests for state_of_game
 */

TEST(StateOfGame, WinIsDeclaredForBlackWithoutWinBBaseAndWithoutWInBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{5, 7}, {5, 5}});
    cc.move(0, {{1, 1}, {1, 3}});
    cc.move(1, {{6, 5}, {4, 5}});
    cc.move(0, {{0, 1}, {4, 1}});
    cc.move(1, {{6, 6}, {4, 6}, {4, 4}});
    cc.move(0, {{2, 0}, {4, 0}, {4, 2}});
    cc.move(1, {{7, 6}, {3, 6}, {5, 4}, {3, 4}});
    cc.move(0, {{1, 0}, {5, 0}, {3, 2}, {5, 2}});
    cc.move(1, {{5, 5}, {3, 5}, {3, 3}, {5, 1}, {3, 1}, {1, 1}});
    cc.move(0, {{0, 0}, {6, 0}, {2, 4}});
    cc.move(1, {{7, 5}, {1, 5}, {3, 3}, {5, 1}, {3, 1}});
    cc.move(0, {{3, 0}, {3, 2}, {3, 6}});
    cc.move(1, {{7, 7}, {5, 7}, {3, 7}, {3, 5}, {3, 3}});
    cc.move(0, {{1, 3}, {5, 3}, {3, 5}, {5, 5}, {5, 7}, {7, 7}, {7, 1}});
    cc.move(1, {{6, 7}, {2, 7}});
    cc.move(0, {{5, 2}, {5, 3}});
    cc.move(1, {{5, 6}, {5, 0}, {3, 2}, {3, 0}});
    cc.move(0, {{4, 1}, {4, 3}, {6, 3}});
    cc.move(1, {{4, 7}, {0, 7}});
    cc.move(0, {{1, 2}, {7, 2}});
    cc.move(1, {{4, 5}, {4, 3}, {2, 3}});
    cc.move(0, {{2, 1}, {4, 1}, {4, 3}, {4, 5}});
    cc.move(1, {{0, 7}, {6, 1}});
    cc.move(0, {{2, 4}, {2, 2}, {6, 2}});
    cc.move(1, {{4, 4}, {2, 4}, {2, 2}, {4, 0}, {2, 0}, {0, 2}});
    cc.move(0, {{5, 3}, {4, 4}});
    cc.move(1, {{3, 4}, {3, 2}});
    cc.move(0, {{3, 6}, {3, 7}});
    cc.move(1, {{7, 4}, {1, 4}});
    cc.move(0, {{4, 5}, {5, 4}});
    cc.move(1, {{1, 1}, {1, 0}});
    cc.move(0, {{7, 2}, {7, 3}});
    cc.move(1, {{6, 1}, {0, 1}});
    cc.move(0, {{6, 2}, {7, 2}});
    cc.move(1, {{3, 3}, {5, 1}, {1, 1}});
    cc.move(0, {{6, 3}, {6, 2}});
    cc.move(1, {{2, 7}, {2, 6}});
    cc.move(0, {{3, 7}, {4, 6}});
    cc.move(1, {{2, 6}, {2, 0}, {0, 0}});
    cc.move(0, {{0, 4}, {0, 5}});
    cc.move(1, {{2, 3}, {2, 2}});
    cc.move(0, {{4, 4}, {5, 3}});
    cc.move(1, {{3, 2}, {1, 2}});
    cc.move(0, {{0, 5}, {1, 5}});
    cc.move(1, {{1, 4}, {1, 3}});
    cc.move(0, {{4, 2}, {5, 2}});
    cc.move(1, {{2, 2}, {2, 1}});
    cc.move(0, {{5, 2}, {6, 1}});
    cc.move(1, {{2, 1}, {2, 0}});

    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{3, 1}, {2, 2}});
    cc.move(0, {{4, 6}, {3, 6}});
    cc.move(1, {{1, 3}, {0, 3}});
    cc.move(0, {{6, 1}, {6, 0}});
    cc.move(1, {{2, 2}, {2, 1}});

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), BlackWon);
}

TEST(StateOfGame, WinIsDeclaredForBlackWithoutWinBBaseAndWithWInBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{5, 7}, {5, 5}});
    cc.move(0, {{1, 1}, {1, 3}});
    cc.move(1, {{6, 5}, {4, 5}});
    cc.move(0, {{0, 1}, {4, 1}});
    cc.move(1, {{6, 6}, {4, 6}, {4, 4}});
    cc.move(0, {{2, 0}, {4, 0}, {4, 2}});
    cc.move(1, {{7, 6}, {3, 6}, {5, 4}, {3, 4}});
    cc.move(0, {{1, 0}, {5, 0}, {3, 2}, {5, 2}});
    cc.move(1, {{5, 5}, {3, 5}, {3, 3}, {5, 1}, {3, 1}, {1, 1}});
    cc.move(0, {{0, 0}, {6, 0}, {2, 4}});
    cc.move(1, {{7, 5}, {1, 5}, {3, 3}, {5, 1}, {3, 1}});
    cc.move(0, {{3, 0}, {3, 2}, {3, 6}});
    cc.move(1, {{7, 7}, {5, 7}, {3, 7}, {3, 5}, {3, 3}});
    cc.move(0, {{1, 3}, {5, 3}, {3, 5}, {5, 5}, {5, 7}, {7, 7}, {7, 1}});
    cc.move(1, {{6, 7}, {2, 7}});
    cc.move(0, {{5, 2}, {5, 3}});
    cc.move(1, {{5, 6}, {5, 0}, {3, 2}, {3, 0}});
    cc.move(0, {{4, 1}, {4, 3}, {6, 3}});
    cc.move(1, {{4, 7}, {0, 7}});
    cc.move(0, {{1, 2}, {7, 2}});
    cc.move(1, {{4, 5}, {4, 3}, {2, 3}});
    cc.move(0, {{2, 1}, {4, 1}, {4, 3}, {4, 5}});
    cc.move(1, {{0, 7}, {6, 1}});
    cc.move(0, {{2, 4}, {2, 2}, {6, 2}});
    cc.move(1, {{4, 4}, {2, 4}, {2, 2}, {4, 0}, {2, 0}, {0, 2}});
    cc.move(0, {{5, 3}, {4, 4}});
    cc.move(1, {{3, 4}, {3, 2}});
    cc.move(0, {{3, 6}, {3, 7}});
    cc.move(1, {{7, 4}, {1, 4}});
    cc.move(0, {{4, 5}, {5, 4}});
    cc.move(1, {{1, 1}, {1, 0}});
    cc.move(0, {{7, 2}, {7, 3}});
    cc.move(1, {{6, 1}, {0, 1}});
    cc.move(0, {{6, 2}, {7, 2}});
    cc.move(1, {{3, 3}, {5, 1}, {1, 1}});
    cc.move(0, {{6, 3}, {6, 2}});
    cc.move(1, {{2, 7}, {2, 6}});
    cc.move(0, {{3, 7}, {4, 6}});
    cc.move(1, {{2, 6}, {2, 0}, {0, 0}});
    cc.move(0, {{0, 4}, {0, 5}});
    cc.move(1, {{2, 3}, {2, 2}});
    cc.move(0, {{4, 4}, {5, 3}});
    cc.move(1, {{3, 2}, {1, 2}});
    cc.move(0, {{0, 5}, {1, 5}});
    cc.move(1, {{1, 4}, {1, 3}});
    cc.move(0, {{4, 2}, {5, 2}});
    cc.move(1, {{2, 2}, {2, 1}});
    cc.move(0, {{5, 2}, {6, 1}});
    cc.move(1, {{2, 1}, {2, 0}});

    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{3, 1}, {2, 2}});
    cc.move(0, {{4, 6}, {3, 6}});
    cc.move(1, {{1, 3}, {0, 3}});
    cc.move(0, {{7, 3}, {7, 4}});
    cc.move(1, {{2, 2}, {2, 1}});

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), BlackWon);
}

TEST(StateOfGame, WinIsDeclaredForBlackWithWinBBaseAndWithoutWInBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{5, 7}, {5, 5}});
    cc.move(0, {{1, 1}, {1, 3}});
    cc.move(1, {{6, 5}, {4, 5}});
    cc.move(0, {{0, 1}, {4, 1}});
    cc.move(1, {{6, 6}, {4, 6}, {4, 4}});
    cc.move(0, {{2, 0}, {4, 0}, {4, 2}});
    cc.move(1, {{7, 6}, {3, 6}, {5, 4}, {3, 4}});
    cc.move(0, {{1, 0}, {5, 0}, {3, 2}, {5, 2}});
    cc.move(1, {{5, 5}, {3, 5}, {3, 3}, {5, 1}, {3, 1}, {1, 1}});
    cc.move(0, {{0, 0}, {6, 0}, {2, 4}});
    cc.move(1, {{7, 5}, {1, 5}, {3, 3}, {5, 1}, {3, 1}});
    cc.move(0, {{3, 0}, {3, 2}, {3, 6}});
    cc.move(1, {{7, 7}, {5, 7}, {3, 7}, {3, 5}, {3, 3}});
    cc.move(0, {{1, 3}, {5, 3}, {3, 5}, {5, 5}, {5, 7}, {7, 7}, {7, 1}});
    cc.move(1, {{6, 7}, {2, 7}});
    cc.move(0, {{5, 2}, {5, 3}});
    cc.move(1, {{5, 6}, {5, 0}, {3, 2}, {3, 0}});
    cc.move(0, {{4, 1}, {4, 3}, {6, 3}});
    cc.move(1, {{4, 7}, {0, 7}});
    cc.move(0, {{1, 2}, {7, 2}});
    cc.move(1, {{4, 5}, {4, 3}, {2, 3}});
    cc.move(0, {{2, 1}, {4, 1}, {4, 3}, {4, 5}});
    cc.move(1, {{0, 7}, {6, 1}});
    cc.move(0, {{2, 4}, {2, 2}, {6, 2}});
    cc.move(1, {{4, 4}, {2, 4}, {2, 2}, {4, 0}, {2, 0}, {0, 2}});
    cc.move(0, {{5, 3}, {4, 4}});
    cc.move(1, {{3, 4}, {3, 2}});
    cc.move(0, {{3, 6}, {3, 7}});
    cc.move(1, {{7, 4}, {1, 4}});
    cc.move(0, {{4, 5}, {5, 4}});
    cc.move(1, {{1, 1}, {1, 0}});
    cc.move(0, {{7, 2}, {7, 3}});
    cc.move(1, {{6, 1}, {0, 1}});
    cc.move(0, {{6, 2}, {7, 2}});
    cc.move(1, {{3, 3}, {5, 1}, {1, 1}});
    cc.move(0, {{6, 3}, {6, 2}});
    cc.move(1, {{2, 7}, {2, 6}});
    cc.move(0, {{3, 7}, {4, 6}});
    cc.move(1, {{2, 6}, {2, 0}, {0, 0}});
    cc.move(0, {{0, 4}, {0, 5}});
    cc.move(1, {{2, 3}, {2, 2}});
    cc.move(0, {{4, 4}, {5, 3}});
    cc.move(1, {{3, 2}, {1, 2}});
    cc.move(0, {{0, 5}, {1, 5}});
    cc.move(1, {{1, 4}, {1, 3}});
    cc.move(0, {{4, 2}, {5, 2}});
    cc.move(1, {{2, 2}, {2, 1}});
    cc.move(0, {{5, 2}, {6, 1}});
    cc.move(1, {{2, 1}, {2, 0}});

    cc.move(0, {{5, 4}, {5, 5}});
    cc.move(1, {{3, 1}, {2, 2}});
    cc.move(0, {{4, 6}, {3, 6}});
    cc.move(1, {{1, 3}, {0, 3}});
    cc.move(0, {{6, 1}, {6, 0}});
    cc.move(1, {{2, 2}, {2, 1}});

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), BlackWon);
}

TEST(StateOfGame, WinIsDeclaredForBlackWithWinBBaseAndWithWInBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{5, 7}, {5, 5}});
    cc.move(0, {{1, 1}, {1, 3}});
    cc.move(1, {{6, 5}, {4, 5}});
    cc.move(0, {{0, 1}, {4, 1}});
    cc.move(1, {{6, 6}, {4, 6}, {4, 4}});
    cc.move(0, {{2, 0}, {4, 0}, {4, 2}});
    cc.move(1, {{7, 6}, {3, 6}, {5, 4}, {3, 4}});
    cc.move(0, {{1, 0}, {5, 0}, {3, 2}, {5, 2}});
    cc.move(1, {{5, 5}, {3, 5}, {3, 3}, {5, 1}, {3, 1}, {1, 1}});
    cc.move(0, {{0, 0}, {6, 0}, {2, 4}});
    cc.move(1, {{7, 5}, {1, 5}, {3, 3}, {5, 1}, {3, 1}});
    cc.move(0, {{3, 0}, {3, 2}, {3, 6}});
    cc.move(1, {{7, 7}, {5, 7}, {3, 7}, {3, 5}, {3, 3}});
    cc.move(0, {{1, 3}, {5, 3}, {3, 5}, {5, 5}, {5, 7}, {7, 7}, {7, 1}});
    cc.move(1, {{6, 7}, {2, 7}});
    cc.move(0, {{5, 2}, {5, 3}});
    cc.move(1, {{5, 6}, {5, 0}, {3, 2}, {3, 0}});
    cc.move(0, {{4, 1}, {4, 3}, {6, 3}});
    cc.move(1, {{4, 7}, {0, 7}});
    cc.move(0, {{1, 2}, {7, 2}});
    cc.move(1, {{4, 5}, {4, 3}, {2, 3}});
    cc.move(0, {{2, 1}, {4, 1}, {4, 3}, {4, 5}});
    cc.move(1, {{0, 7}, {6, 1}});
    cc.move(0, {{2, 4}, {2, 2}, {6, 2}});
    cc.move(1, {{4, 4}, {2, 4}, {2, 2}, {4, 0}, {2, 0}, {0, 2}});
    cc.move(0, {{5, 3}, {4, 4}});
    cc.move(1, {{3, 4}, {3, 2}});
    cc.move(0, {{3, 6}, {3, 7}});
    cc.move(1, {{7, 4}, {1, 4}});
    cc.move(0, {{4, 5}, {5, 4}});
    cc.move(1, {{1, 1}, {1, 0}});
    cc.move(0, {{7, 2}, {7, 3}});
    cc.move(1, {{6, 1}, {0, 1}});
    cc.move(0, {{6, 2}, {7, 2}});
    cc.move(1, {{3, 3}, {5, 1}, {1, 1}});
    cc.move(0, {{6, 3}, {6, 2}});
    cc.move(1, {{2, 7}, {2, 6}});
    cc.move(0, {{3, 7}, {4, 6}});
    cc.move(1, {{2, 6}, {2, 0}, {0, 0}});
    cc.move(0, {{0, 4}, {0, 5}});
    cc.move(1, {{2, 3}, {2, 2}});
    cc.move(0, {{4, 4}, {5, 3}});
    cc.move(1, {{3, 2}, {1, 2}});
    cc.move(0, {{0, 5}, {1, 5}});
    cc.move(1, {{1, 4}, {1, 3}});
    cc.move(0, {{4, 2}, {5, 2}});
    cc.move(1, {{2, 2}, {2, 1}});
    cc.move(0, {{5, 2}, {6, 1}});
    cc.move(1, {{2, 1}, {2, 0}});

    cc.move(0, {{4, 6}, {5, 6}});
    cc.move(1, {{3, 1}, {2, 1}});

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), BlackWon);
}

TEST(StateOfGame, WinIsDeclaredForWhiteWithoutBinWBaseAndWithoutBInBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{2, 0}, {2, 2}});
    cc.move(1, {{5, 7}, {3, 7}});
    cc.move(0, {{1, 2}, {3, 2}});
    cc.move(1, {{6, 6}, {4, 6}});
    cc.move(0, {{1, 1}, {3, 1}, {3, 3}});
    cc.move(1, {{5, 6}, {3, 6}});
    cc.move(0, {{0, 1}, {4, 1}, {2, 3}, {4, 3}});
    cc.move(1, {{7, 5}, {7, 3}});
    cc.move(0, {{4, 3}, {3, 4}});
    cc.move(1, {{6, 7}, {6, 3}});
    cc.move(0, {{1, 0}, {5, 0}, {1, 4}, {5, 4}});
    cc.move(1, {{3, 6}, {2, 7}});
    cc.move(0, {{0, 0}, {6, 0}, {0, 6}});
    cc.move(1, {{7, 6}, {1, 6}, {5, 2}});
    cc.move(0, {{3, 3}, {3, 5}});
    cc.move(1, {{7, 4}, {7, 2}});
    cc.move(0, {{3, 4}, {3, 6}, {5, 6}});
    cc.move(1, {{3, 7}, {1, 7}});
    cc.move(0, {{2, 1}, {2, 3}});
    cc.move(1, {{4, 7}, {3, 7}});
    cc.move(0, {{3, 0}, {3, 4}, {7, 4}});
    cc.move(1, {{6, 5}, {0, 5}});
    cc.move(0, {{0, 3}, {4, 3}, {6, 1}, {6, 5}});
    cc.move(1, {{7, 3}, {5, 3}, {5, 1}});
    cc.move(0, {{3, 2}, {3, 3}});
    cc.move(1, {{7, 2}, {6, 2}});
    cc.move(0, {{0, 2}, {4, 2}});
    cc.move(1, {{4, 6}, {3, 6}});
    cc.move(0, {{2, 2}, {2, 4}});
    cc.move(1, {{3, 6}, {2, 6}});
    cc.move(0, {{0, 6}, {4, 6}, {6, 6}});
    cc.move(1, {{7, 7}, {7, 1}});
    cc.move(0, {{2, 3}, {4, 3}});
    cc.move(1, {{3, 7}, {4, 7}});
    cc.move(0, {{3, 5}, {4, 5}});
    cc.move(1, {{4, 7}, {5, 7}});
    cc.move(0, {{4, 3}, {4, 7}, {6, 7}});
    cc.move(1, {{5, 7}, {4, 7}});
    cc.move(0, {{4, 5}, {5, 5}});
    cc.move(1, {{6, 3}, {6, 1}});
    cc.move(0, {{5, 5}, {5, 7}});
    cc.move(1, {{1, 7}, {0, 7}});
    cc.move(0, {{3, 3}, {3, 4}});
    cc.move(1, {{2, 7}, {2, 5}});
    cc.move(0, {{2, 4}, {4, 4}, {6, 4}});
    cc.move(1, {{0, 7}, {0, 3}});
    cc.move(0, {{5, 4}, {5, 5}});
    cc.move(1, {{0, 5}, {1, 4}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{6, 1}, {6, 0}});
    cc.move(0, {{6, 6}, {7, 6}});
    cc.move(1, {{6, 0}, {5, 0}});
    cc.move(0, {{4, 2}, {4, 6}, {6, 6}});
    cc.move(1, {{2, 5}, {1, 6}});
    cc.move(0, {{5, 5}, {7, 5}, {7, 7}});
    cc.move(1, {{1, 6}, {1, 5}});
    cc.move(0, {{4, 4}, {4, 5}});
    cc.move(1, {{2, 6}, {2, 5}});

    cc.move(0, {{6, 5}, {7, 5}});
    cc.move(1, {{4, 7}, {4, 6}});
    cc.move(0, {{4, 5}, {4, 7}});
    cc.move(1, {{0, 3}, {1, 3}});
    cc.move(0, {{6, 4}, {6, 5}});
    /* Assert */
    EXPECT_EQ(cc.state_of_game(), WhiteWon);
}

TEST(StateOfGame, WinIsDeclaredForWhiteWithoutBinWBaseAndWithBInBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{2, 0}, {2, 2}});
    cc.move(1, {{5, 7}, {3, 7}});
    cc.move(0, {{1, 2}, {3, 2}});
    cc.move(1, {{6, 6}, {4, 6}});
    cc.move(0, {{1, 1}, {3, 1}, {3, 3}});
    cc.move(1, {{5, 6}, {3, 6}});
    cc.move(0, {{0, 1}, {4, 1}, {2, 3}, {4, 3}});
    cc.move(1, {{7, 5}, {7, 3}});
    cc.move(0, {{4, 3}, {3, 4}});
    cc.move(1, {{6, 7}, {6, 3}});
    cc.move(0, {{1, 0}, {5, 0}, {1, 4}, {5, 4}});
    cc.move(1, {{3, 6}, {2, 7}});
    cc.move(0, {{0, 0}, {6, 0}, {0, 6}});
    cc.move(1, {{7, 6}, {1, 6}, {5, 2}});
    cc.move(0, {{3, 3}, {3, 5}});
    cc.move(1, {{7, 4}, {7, 2}});
    cc.move(0, {{3, 4}, {3, 6}, {5, 6}});
    cc.move(1, {{3, 7}, {1, 7}});
    cc.move(0, {{2, 1}, {2, 3}});
    cc.move(1, {{4, 7}, {3, 7}});
    cc.move(0, {{3, 0}, {3, 4}, {7, 4}});
    cc.move(1, {{6, 5}, {0, 5}});
    cc.move(0, {{0, 3}, {4, 3}, {6, 1}, {6, 5}});
    cc.move(1, {{7, 3}, {5, 3}, {5, 1}});
    cc.move(0, {{3, 2}, {3, 3}});
    cc.move(1, {{7, 2}, {6, 2}});
    cc.move(0, {{0, 2}, {4, 2}});
    cc.move(1, {{4, 6}, {3, 6}});
    cc.move(0, {{2, 2}, {2, 4}});
    cc.move(1, {{3, 6}, {2, 6}});
    cc.move(0, {{0, 6}, {4, 6}, {6, 6}});
    cc.move(1, {{7, 7}, {7, 1}});
    cc.move(0, {{2, 3}, {4, 3}});
    cc.move(1, {{3, 7}, {4, 7}});
    cc.move(0, {{3, 5}, {4, 5}});
    cc.move(1, {{4, 7}, {5, 7}});
    cc.move(0, {{4, 3}, {4, 7}, {6, 7}});
    cc.move(1, {{5, 7}, {4, 7}});
    cc.move(0, {{4, 5}, {5, 5}});
    cc.move(1, {{6, 3}, {6, 1}});
    cc.move(0, {{5, 5}, {5, 7}});
    cc.move(1, {{1, 7}, {0, 7}});
    cc.move(0, {{3, 3}, {3, 4}});
    cc.move(1, {{2, 7}, {2, 5}});
    cc.move(0, {{2, 4}, {4, 4}, {6, 4}});
    cc.move(1, {{0, 7}, {0, 3}});
    cc.move(0, {{5, 4}, {5, 5}});
    cc.move(1, {{0, 5}, {1, 4}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{6, 1}, {6, 0}});
    cc.move(0, {{6, 6}, {7, 6}});
    cc.move(1, {{6, 0}, {5, 0}});
    cc.move(0, {{4, 2}, {4, 6}, {6, 6}});
    cc.move(1, {{2, 5}, {1, 6}});
    cc.move(0, {{5, 5}, {7, 5}, {7, 7}});
    cc.move(1, {{1, 6}, {1, 5}});
    cc.move(0, {{4, 4}, {4, 5}});
    cc.move(1, {{2, 6}, {2, 5}});

    cc.move(0, {{6, 5}, {7, 5}});
    cc.move(1, {{0, 3}, {1, 3}});
    cc.move(0, {{6, 4}, {6, 5}});
    /* Assert */
    EXPECT_EQ(cc.state_of_game(), WhiteWon);
}

TEST(StateOfGame, WinIsDeclaredForWhiteWithBinWBaseAndWithoutBInBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{2, 0}, {2, 2}});
    cc.move(1, {{5, 7}, {3, 7}});
    cc.move(0, {{1, 2}, {3, 2}});
    cc.move(1, {{6, 6}, {4, 6}});
    cc.move(0, {{1, 1}, {3, 1}, {3, 3}});
    cc.move(1, {{5, 6}, {3, 6}});
    cc.move(0, {{0, 1}, {4, 1}, {2, 3}, {4, 3}});
    cc.move(1, {{7, 5}, {7, 3}});
    cc.move(0, {{4, 3}, {3, 4}});
    cc.move(1, {{6, 7}, {6, 3}});
    cc.move(0, {{1, 0}, {5, 0}, {1, 4}, {5, 4}});
    cc.move(1, {{3, 6}, {2, 7}});
    cc.move(0, {{0, 0}, {6, 0}, {0, 6}});
    cc.move(1, {{7, 6}, {1, 6}, {5, 2}});
    cc.move(0, {{3, 3}, {3, 5}});
    cc.move(1, {{7, 4}, {7, 2}});
    cc.move(0, {{3, 4}, {3, 6}, {5, 6}});
    cc.move(1, {{3, 7}, {1, 7}});
    cc.move(0, {{2, 1}, {2, 3}});
    cc.move(1, {{4, 7}, {3, 7}});
    cc.move(0, {{3, 0}, {3, 4}, {7, 4}});
    cc.move(1, {{6, 5}, {0, 5}});
    cc.move(0, {{0, 3}, {4, 3}, {6, 1}, {6, 5}});
    cc.move(1, {{7, 3}, {5, 3}, {5, 1}});
    cc.move(0, {{3, 2}, {3, 3}});
    cc.move(1, {{7, 2}, {6, 2}});
    cc.move(0, {{0, 2}, {4, 2}});
    cc.move(1, {{4, 6}, {3, 6}});
    cc.move(0, {{2, 2}, {2, 4}});
    cc.move(1, {{3, 6}, {2, 6}});
    cc.move(0, {{0, 6}, {4, 6}, {6, 6}});
    cc.move(1, {{7, 7}, {7, 1}});
    cc.move(0, {{2, 3}, {4, 3}});
    cc.move(1, {{3, 7}, {4, 7}});
    cc.move(0, {{3, 5}, {4, 5}});
    cc.move(1, {{4, 7}, {5, 7}});
    cc.move(0, {{4, 3}, {4, 7}, {6, 7}});
    cc.move(1, {{5, 7}, {4, 7}});
    cc.move(0, {{4, 5}, {5, 5}});
    cc.move(1, {{6, 3}, {6, 1}});
    cc.move(0, {{5, 5}, {5, 7}});
    cc.move(1, {{1, 7}, {0, 7}});
    cc.move(0, {{3, 3}, {3, 4}});
    cc.move(1, {{2, 7}, {2, 5}});
    cc.move(0, {{2, 4}, {4, 4}, {6, 4}});
    cc.move(1, {{0, 7}, {0, 3}});
    cc.move(0, {{5, 4}, {5, 5}});
    cc.move(1, {{0, 5}, {1, 4}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{6, 1}, {6, 0}});
    cc.move(0, {{6, 6}, {7, 6}});
    cc.move(1, {{6, 0}, {5, 0}});
    cc.move(0, {{4, 2}, {4, 6}, {6, 6}});
    cc.move(1, {{2, 5}, {1, 6}});
    cc.move(0, {{5, 5}, {7, 5}, {7, 7}});
    cc.move(1, {{1, 6}, {1, 5}});
    cc.move(0, {{4, 4}, {4, 5}});
    cc.move(1, {{2, 6}, {2, 5}});

    cc.move(0, {{6, 5}, {7, 5}});
    cc.move(1, {{4, 7}, {4, 6}});
    cc.move(0, {{6, 4}, {6, 5}});
    cc.move(1, {{1, 4}, {2, 4}});
    cc.move(0, {{4, 5}, {4, 7}});

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), WhiteWon);
}

TEST(StateOfGame, WinIsDeclaredForWhiteWithBinWBaseAndBBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{2, 0}, {2, 2}});
    cc.move(1, {{5, 7}, {3, 7}});
    cc.move(0, {{1, 2}, {3, 2}});
    cc.move(1, {{6, 6}, {4, 6}});
    cc.move(0, {{1, 1}, {3, 1}, {3, 3}});
    cc.move(1, {{5, 6}, {3, 6}});
    cc.move(0, {{0, 1}, {4, 1}, {2, 3}, {4, 3}});
    cc.move(1, {{7, 5}, {7, 3}});
    cc.move(0, {{4, 3}, {3, 4}});
    cc.move(1, {{6, 7}, {6, 3}});
    cc.move(0, {{1, 0}, {5, 0}, {1, 4}, {5, 4}});
    cc.move(1, {{3, 6}, {2, 7}});
    cc.move(0, {{0, 0}, {6, 0}, {0, 6}});
    cc.move(1, {{7, 6}, {1, 6}, {5, 2}});
    cc.move(0, {{3, 3}, {3, 5}});
    cc.move(1, {{7, 4}, {7, 2}});
    cc.move(0, {{3, 4}, {3, 6}, {5, 6}});
    cc.move(1, {{3, 7}, {1, 7}});
    cc.move(0, {{2, 1}, {2, 3}});
    cc.move(1, {{4, 7}, {3, 7}});
    cc.move(0, {{3, 0}, {3, 4}, {7, 4}});
    cc.move(1, {{6, 5}, {0, 5}});
    cc.move(0, {{0, 3}, {4, 3}, {6, 1}, {6, 5}});
    cc.move(1, {{7, 3}, {5, 3}, {5, 1}});
    cc.move(0, {{3, 2}, {3, 3}});
    cc.move(1, {{7, 2}, {6, 2}});
    cc.move(0, {{0, 2}, {4, 2}});
    cc.move(1, {{4, 6}, {3, 6}});
    cc.move(0, {{2, 2}, {2, 4}});
    cc.move(1, {{3, 6}, {2, 6}});
    cc.move(0, {{0, 6}, {4, 6}, {6, 6}});
    cc.move(1, {{7, 7}, {7, 1}});
    cc.move(0, {{2, 3}, {4, 3}});
    cc.move(1, {{3, 7}, {4, 7}});
    cc.move(0, {{3, 5}, {4, 5}});
    cc.move(1, {{4, 7}, {5, 7}});
    cc.move(0, {{4, 3}, {4, 7}, {6, 7}});
    cc.move(1, {{5, 7}, {4, 7}});
    cc.move(0, {{4, 5}, {5, 5}});
    cc.move(1, {{6, 3}, {6, 1}});
    cc.move(0, {{5, 5}, {5, 7}});
    cc.move(1, {{1, 7}, {0, 7}});
    cc.move(0, {{3, 3}, {3, 4}});
    cc.move(1, {{2, 7}, {2, 5}});
    cc.move(0, {{2, 4}, {4, 4}, {6, 4}});
    cc.move(1, {{0, 7}, {0, 3}});
    cc.move(0, {{5, 4}, {5, 5}});
    cc.move(1, {{0, 5}, {1, 4}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{6, 1}, {6, 0}});
    cc.move(0, {{6, 6}, {7, 6}});
    cc.move(1, {{6, 0}, {5, 0}});
    cc.move(0, {{4, 2}, {4, 6}, {6, 6}});
    cc.move(1, {{2, 5}, {1, 6}});
    cc.move(0, {{5, 5}, {7, 5}, {7, 7}});
    cc.move(1, {{1, 6}, {1, 5}});
    cc.move(0, {{4, 4}, {4, 5}});
    cc.move(1, {{2, 6}, {2, 5}});

    cc.move(0, {{6, 5}, {7, 5}});
    cc.move(1, {{1, 4}, {2, 4}});
    cc.move(0, {{6, 4}, {6, 5}});

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), WhiteWon);
}

TEST(StateOfGame, WinIsNotDeclaredEarlyForWhiteWithBinWBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{2, 0}, {2, 2}});
    cc.move(1, {{5, 7}, {3, 7}});
    cc.move(0, {{1, 2}, {3, 2}});
    cc.move(1, {{6, 6}, {4, 6}});
    cc.move(0, {{1, 1}, {3, 1}, {3, 3}});
    cc.move(1, {{5, 6}, {3, 6}});
    cc.move(0, {{0, 1}, {4, 1}, {2, 3}, {4, 3}});
    cc.move(1, {{7, 5}, {7, 3}});
    cc.move(0, {{4, 3}, {3, 4}});
    cc.move(1, {{6, 7}, {6, 3}});
    cc.move(0, {{1, 0}, {5, 0}, {1, 4}, {5, 4}});
    cc.move(1, {{3, 6}, {2, 7}});
    cc.move(0, {{0, 0}, {6, 0}, {0, 6}});
    cc.move(1, {{7, 6}, {1, 6}, {5, 2}});
    cc.move(0, {{3, 3}, {3, 5}});
    cc.move(1, {{7, 4}, {7, 2}});
    cc.move(0, {{3, 4}, {3, 6}, {5, 6}});
    cc.move(1, {{3, 7}, {1, 7}});
    cc.move(0, {{2, 1}, {2, 3}});
    cc.move(1, {{4, 7}, {3, 7}});
    cc.move(0, {{3, 0}, {3, 4}, {7, 4}});
    cc.move(1, {{6, 5}, {0, 5}});
    cc.move(0, {{0, 3}, {4, 3}, {6, 1}, {6, 5}});
    cc.move(1, {{7, 3}, {5, 3}, {5, 1}});
    cc.move(0, {{3, 2}, {3, 3}});
    cc.move(1, {{7, 2}, {6, 2}});
    cc.move(0, {{0, 2}, {4, 2}});
    cc.move(1, {{4, 6}, {3, 6}});
    cc.move(0, {{2, 2}, {2, 4}});
    cc.move(1, {{3, 6}, {2, 6}});
    cc.move(0, {{0, 6}, {4, 6}, {6, 6}});
    cc.move(1, {{7, 7}, {7, 1}});
    cc.move(0, {{2, 3}, {4, 3}});
    cc.move(1, {{3, 7}, {4, 7}});
    cc.move(0, {{3, 5}, {4, 5}});
    cc.move(1, {{4, 7}, {5, 7}});
    cc.move(0, {{4, 3}, {4, 7}, {6, 7}});
    cc.move(1, {{5, 7}, {4, 7}});
    cc.move(0, {{4, 5}, {5, 5}});
    cc.move(1, {{6, 3}, {6, 1}});
    cc.move(0, {{5, 5}, {5, 7}});
    cc.move(1, {{1, 7}, {0, 7}});
    cc.move(0, {{3, 3}, {3, 4}});
    cc.move(1, {{2, 7}, {2, 5}});
    cc.move(0, {{2, 4}, {4, 4}, {6, 4}});
    cc.move(1, {{0, 7}, {0, 3}});
    cc.move(0, {{5, 4}, {5, 5}});
    cc.move(1, {{0, 5}, {1, 4}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{6, 1}, {6, 0}});
    cc.move(0, {{6, 6}, {7, 6}});
    cc.move(1, {{6, 0}, {5, 0}});
    cc.move(0, {{4, 2}, {4, 6}, {6, 6}});
    cc.move(1, {{2, 5}, {1, 6}});
    cc.move(0, {{5, 5}, {7, 5}, {7, 7}});
    cc.move(1, {{1, 6}, {1, 5}});
    cc.move(0, {{4, 4}, {4, 5}});
    cc.move(1, {{2, 6}, {2, 5}});

    cc.move(0, {{6, 5}, {7, 5}});
    cc.move(1, {{1, 4}, {2, 4}});

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), NotFinished);
}

TEST(StateOfGame, WinIsNotDeclaredEarlyForWithoutWithBinWBase) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{2, 0}, {2, 2}});
    cc.move(1, {{5, 7}, {3, 7}});
    cc.move(0, {{1, 2}, {3, 2}});
    cc.move(1, {{6, 6}, {4, 6}});
    cc.move(0, {{1, 1}, {3, 1}, {3, 3}});
    cc.move(1, {{3, 6}, {3, 6}});
    cc.move(0, {{0, 1}, {4, 1}, {2, 3}, {4, 3}});
    cc.move(1, {{7, 5}, {7, 3}});
    cc.move(0, {{4, 3}, {3, 4}});
    cc.move(1, {{6, 7}, {6, 3}});
    cc.move(0, {{1, 0}, {5, 0}, {1, 4}, {5, 4}});
    cc.move(1, {{3, 6}, {2, 7}});
    cc.move(0, {{0, 0}, {6, 0}, {0, 6}});
    cc.move(1, {{7, 6}, {1, 6}, {5, 2}});
    cc.move(0, {{3, 3}, {3, 5}});
    cc.move(1, {{7, 4}, {7, 2}});
    cc.move(0, {{3, 4}, {3, 6}, {5, 6}});
    cc.move(1, {{3, 7}, {1, 7}});
    cc.move(0, {{2, 1}, {2, 3}});
    cc.move(1, {{4, 7}, {3, 7}});
    cc.move(0, {{3, 0}, {3, 4}, {7, 4}});
    cc.move(1, {{6, 5}, {0, 5}});
    cc.move(0, {{0, 3}, {4, 3}, {6, 1}, {6, 5}});
    cc.move(1, {{7, 3}, {5, 3}, {5, 1}});
    cc.move(0, {{3, 2}, {3, 3}});
    cc.move(1, {{7, 2}, {6, 2}});
    cc.move(0, {{0, 2}, {4, 2}});
    cc.move(1, {{4, 6}, {3, 6}});
    cc.move(0, {{2, 2}, {2, 4}});
    cc.move(1, {{3, 6}, {2, 6}});
    cc.move(0, {{0, 6}, {4, 6}, {6, 6}});
    cc.move(1, {{7, 7}, {7, 1}});
    cc.move(0, {{2, 3}, {4, 3}});
    cc.move(1, {{3, 7}, {4, 7}});
    cc.move(0, {{3, 5}, {4, 5}});
    cc.move(1, {{4, 7}, {5, 7}});
    cc.move(0, {{4, 3}, {4, 7}, {6, 7}});
    cc.move(1, {{5, 7}, {4, 7}});
    cc.move(0, {{4, 5}, {5, 5}});
    cc.move(1, {{6, 3}, {6, 1}});
    cc.move(0, {{5, 5}, {5, 7}});
    cc.move(1, {{1, 7}, {0, 7}});
    cc.move(0, {{3, 3}, {3, 4}});
    cc.move(1, {{2, 7}, {2, 5}});
    cc.move(0, {{2, 4}, {4, 4}, {6, 4}});
    cc.move(1, {{0, 7}, {0, 3}});
    cc.move(0, {{5, 4}, {5, 5}});
    cc.move(1, {{0, 5}, {1, 4}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{6, 1}, {6, 0}});
    cc.move(0, {{6, 6}, {7, 6}});
    cc.move(1, {{6, 0}, {5, 0}});
    cc.move(0, {{4, 2}, {4, 6}, {6, 6}});
    cc.move(1, {{2, 5}, {1, 6}});
    cc.move(0, {{5, 5}, {7, 5}, {7, 7}});
    cc.move(1, {{1, 6}, {1, 5}});
    cc.move(0, {{4, 4}, {4, 5}});
    cc.move(1, {{2, 6}, {2, 5}});

    cc.move(0, {{6, 5}, {7, 5}});
    cc.move(1, {{0, 3}, {1, 3}});

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), NotFinished);
}

TEST(StateOfGame, DrawIsNotDeclaredEarlyFromWhite) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    for (int i = 0; i <= MAX_NUMBER_OF_CYCLES_FOR_DRAW_; ++i) {
        if (i & 1) {
            cc.move(0, {{0, 2},
                        {0, 4}});
            cc.move(1, {{4, 7},
                        {3, 7}});
        } else {
            cc.move(0, {{0, 4},
                        {0, 2}});
            cc.move(1, {{3, 7},
                        {4, 7}});
        }
    }

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), false);
}

TEST(StateOfGame, DrawIsNotDeclaredEarlyFromBlack) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{3, 0}, {4, 0}});
    for (int i = 0; i <= MAX_NUMBER_OF_CYCLES_FOR_DRAW_; ++i) {
        if (i & 1) {
            cc.move(1, {{4, 7},
                        {3, 7}});
            cc.move(0, {{0, 2},
                        {0, 4}});
        } else {
            cc.move(1, {{3, 7},
                        {4, 7}});
            cc.move(0, {{0, 4},
                        {0, 2}});
        }
    }

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), false);
}

TEST(StateOfGame, DrawIsDeclaredFromWhite) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    for (int i = 0; i <= MAX_NUMBER_OF_CYCLES_FOR_DRAW_ + 1; ++i) {
        if (i & 1) {
            cc.move(0, {{0, 2},
                        {0, 4}});
            cc.move(1, {{4, 7},
                        {3, 7}});
        } else {
            cc.move(0, {{0, 4},
                        {0, 2}});
            cc.move(1, {{3, 7},
                        {4, 7}});
        }
    }

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), true);
}

TEST(StateOfGame, DrawIsDeclaredFromBlack) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{3, 0}, {4, 0}});
    for (int i = 0; i <= MAX_NUMBER_OF_CYCLES_FOR_DRAW_ + 1; ++i) {
        if (i & 1) {
            cc.move(1, {{4, 7},
                        {3, 7}});
            cc.move(0, {{0, 2},
                        {0, 4}});
        } else {
            cc.move(1, {{3, 7},
                        {4, 7}});
            cc.move(0, {{0, 4},
                        {0, 2}});
        }
    }

    /* Assert */
    EXPECT_EQ(cc.state_of_game(), true);
}


/*
 * Tests for new_game
 */
TEST(newGame, GridIsCorrecltyInitialized) {
    /* Arrange */
    ChineseCheckers cc;
    GridType expectedValue
            {{White , White, White, White, Empty, Empty, Empty, Empty},
             {White , White, White, Empty, Empty, Empty, Empty, Empty},
             {White , White, Empty, Empty, Empty, Empty, Empty, Empty},
             {White , Empty, Empty, Empty, Empty, Empty, Empty, Empty},
             {Empty , Empty, Empty, Empty, Empty, Empty, Empty, Black},
             {Empty , Empty, Empty, Empty, Empty, Empty, Black, Black},
             {Empty , Empty, Empty, Empty, Empty, Black, Black, Black},
             {Empty , Empty, Empty, Empty, Black, Black, Black, Black}};

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.new_game();
    GridType obtainedValue = cc.get_grid_();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(newGame, PositionColorsPlayersIsCorrecltyInitialized) {
    /* Arrange */
    ChineseCheckers cc;
    std::vector<std::vector<PositionType>>
            expectedValues {
            {{7, 7}, {7, 6}, {7, 5}, {7, 4}, {6, 7},
                    {6, 6}, {6, 5}, {5, 7}, {5, 6}, {4, 7}},
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0},
                    {1, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 0}}
    };
    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.new_game();
    std::vector<std::vector<PositionType>> obtainedValues
            = cc.get_position_colors_players_();
    /* we sort the vectors to be able to compare them */
    sort(obtainedValues.begin(), obtainedValues.end());
    sort(expectedValues.begin(), expectedValues.end());

    /* Assert */
    EXPECT_EQ(expectedValues, obtainedValues);
}



/*
 * Tests for get_grid_
 */

TEST(GetGrid, IsCorrecltyInitialized) {
    /* Arrange */
    ChineseCheckers cc;
    GridType obtainedValue = cc.get_grid_();
    GridType expectedValue
            {{White , White, White, White, Empty, Empty, Empty, Empty},
             {White , White, White, Empty, Empty, Empty, Empty, Empty},
             {White , White, Empty, Empty, Empty, Empty, Empty, Empty},
             {White , Empty, Empty, Empty, Empty, Empty, Empty, Empty},
             {Empty , Empty, Empty, Empty, Empty, Empty, Empty, Black},
             {Empty , Empty, Empty, Empty, Empty, Empty, Black, Black},
             {Empty , Empty, Empty, Empty, Empty, Black, Black, Black},
             {Empty , Empty, Empty, Empty, Black, Black, Black, Black}};

    /* Act */

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetGrid, IsCorrecltyChangedAfterAMove) {
    /* Arrange */
    ChineseCheckers cc;
    GridType expectedValue
            {{White , White, Empty, White, White, Empty, Empty, Empty},
             {White , White, White, Empty, Empty, Empty, Empty, Empty},
             {White , White, Empty, Empty, Empty, Empty, Empty, Empty},
             {White , Empty, Empty, Empty, Empty, Empty, Empty, Empty},
             {Empty , Empty, Empty, Empty, Empty, Empty, Empty, Black},
             {Empty , Empty, Empty, Empty, Empty, Empty, Black, Black},
             {Empty , Empty, Empty, Empty, Empty, Black, Black, Black},
             {Empty , Empty, Empty, Empty, Black, Black, Black, Black}};

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    GridType obtainedValue = cc.get_grid_();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetGrid, IsCorrecltyChangedAfterTwoMoves) {
    /* Arrange */
    ChineseCheckers cc;
    GridType expectedValue
            {{White , White, Empty, White, White, Empty, Empty, Empty},
             {White , White, White, Empty, Empty, Empty, Empty, Empty},
             {White , White, Empty, Empty, Empty, Empty, Empty, Empty},
             {White , Empty, Empty, Empty, Empty, Empty, Empty, Black},
             {Empty , Empty, Empty, Empty, Empty, Empty, Empty, Empty},
             {Empty , Empty, Empty, Empty, Empty, Empty, Black, Black},
             {Empty , Empty, Empty, Empty, Empty, Black, Black, Black},
             {Empty , Empty, Empty, Empty, Black, Black, Black, Black}};

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{4, 7}, {3, 7}});
    GridType obtainedValue = cc.get_grid_();


    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetGrid, IsCorrecltyChangedAfterIllegalMove) {
    /* Arrange */
    ChineseCheckers cc;
    GridType expectedValue
            {{White , White, White, White, Empty, Empty, Empty, Empty},
             {White , White, White, Empty, Empty, Empty, Empty, Empty},
             {White , White, Empty, Empty, Empty, Empty, Empty, Empty},
             {White , Empty, Empty, Empty, Empty, Empty, Empty, Empty},
             {Empty , Empty, Empty, Empty, Empty, Empty, Empty, Black},
             {Empty , Empty, Empty, Empty, Empty, Empty, Black, Black},
             {Empty , Empty, Empty, Empty, Empty, Black, Black, Black},
             {Empty , Empty, Empty, Empty, Black, Black, Black, Black}};

    /* Act */
    cc.move(1, {{0, 2}, {0, 4}});
    GridType obtainedValue = cc.get_grid_();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}


/*
 * Tests for get_position_colors_players_
 */

TEST(GetPositionColorPlayers, IsCorrecltyInitialized) {
    /* Arrange */
    ChineseCheckers cc;
    std::vector<std::vector<PositionType>> obtainedValues
            = cc.get_position_colors_players_();
    std::vector<std::vector<PositionType>>
        expectedValues {
            {{7, 7}, {7, 6}, {7, 5}, {7, 4}, {6, 7},
             {6, 6}, {6, 5}, {5, 7}, {5, 6}, {4, 7}},
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0},
             {1, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 0}}
    };
    /* Act */
    /* we sort the vectors to be able to compare them */
    sort(obtainedValues.begin(), obtainedValues.end());
    sort(expectedValues.begin(), expectedValues.end());

    /* Assert */
    EXPECT_EQ(expectedValues, obtainedValues);
}

TEST(GetPositionColorPlayers, IsCorrecltyChangedAfterAMove) {
    /* Arrange */
    ChineseCheckers cc;
    std::vector<std::vector<PositionType>>
            expectedValues {
            {{7, 7}, {7, 6}, {7, 5}, {7, 4}, {6, 7},
                    {6, 6}, {6, 5}, {5, 7}, {5, 6}, {4, 7}},
            {{0, 0}, {0, 1}, {0, 4}, {0, 3}, {1, 0},
                    {1, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 0}}
    };
    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    std::vector<std::vector<PositionType>> obtainedValues
            = cc.get_position_colors_players_();
    /* we sort the vectors to be able to compare them */
    sort(obtainedValues.begin(), obtainedValues.end());
    sort(expectedValues.begin(), expectedValues.end());

    /* Assert */
    EXPECT_EQ(expectedValues, obtainedValues);
}

TEST(GetPositionColorPlayers, IsCorrecltyChangedAfterTwoMoves) {
    /* Arrange */
    ChineseCheckers cc;
    std::vector<std::vector<PositionType>>
            expectedValues {
            {{7, 7}, {7, 6}, {7, 5}, {7, 4}, {6, 7},
                    {6, 6}, {6, 5}, {5, 7}, {5, 6}, {3, 7}},
            {{0, 0}, {0, 1}, {0, 4}, {0, 3}, {1, 0},
                    {1, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 0}}
    };
    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{4, 7}, {3, 7}});
    std::vector<std::vector<PositionType>> obtainedValues
            = cc.get_position_colors_players_();
    /* we sort the vectors to be able to compare them */
    sort(obtainedValues.begin(), obtainedValues.end());
    sort(expectedValues.begin(), expectedValues.end());

    /* Assert */
    EXPECT_EQ(expectedValues, obtainedValues);
}

TEST(GetPositionColorPlayers, IsCorrecltyChangedAfterIllegalMove) {
    /* Arrange */
    ChineseCheckers cc;
    std::vector<std::vector<PositionType>>
            expectedValues {
            {{7, 7}, {7, 6}, {7, 5}, {7, 4}, {6, 7},
                    {6, 6}, {6, 5}, {5, 7}, {5, 6}, {4, 7}},
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0},
                    {1, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 0}}
    };
    /* Act */
    cc.move(1, {{0, 2}, {0, 4}});
    std::vector<std::vector<PositionType>> obtainedValues
            = cc.get_position_colors_players_();
    /* we sort the vectors to be able to compare them */
    sort(obtainedValues.begin(), obtainedValues.end());
    sort(expectedValues.begin(), expectedValues.end());

    /* Assert */
    EXPECT_EQ(expectedValues, obtainedValues);
}



/*
 * Tests for get_who_is_to_play_()
 */

TEST(GetWhoIsToPlay, IsSetToZero) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */

    /* Assert */
    EXPECT_EQ(cc.get_who_is_to_play_(), 0);
}

TEST(GetWhoIsToPlay, IsSetToOneAfterAMove) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{3, 0}, {4, 0}});

    /* Assert */
    EXPECT_EQ(cc.get_who_is_to_play_(), 1);
}

TEST(GetWhoIsToPlay, IsSetToTwoAfterTwoMoves) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{3, 0}, {4, 0}});
    cc.move(1, {{5, 7}, {4, 7}});

    /* Assert */
    EXPECT_EQ(cc.get_who_is_to_play_(), 1);
}

TEST(GetWhoIsToPlay, DoesntChangeWhenMoveIsIllegal) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 0}, {1, 0}});

    /* Assert */
    EXPECT_EQ(cc.get_who_is_to_play_(), 0);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
