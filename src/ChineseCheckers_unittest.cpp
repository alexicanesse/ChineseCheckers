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
 * Tests for stateOfGame
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
    EXPECT_EQ(cc.stateOfGame(), BlackWon);
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
    EXPECT_EQ(cc.stateOfGame(), BlackWon);
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
    EXPECT_EQ(cc.stateOfGame(), BlackWon);
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
    EXPECT_EQ(cc.stateOfGame(), BlackWon);
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
    EXPECT_EQ(cc.stateOfGame(), WhiteWon);
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
    EXPECT_EQ(cc.stateOfGame(), WhiteWon);
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
    EXPECT_EQ(cc.stateOfGame(), WhiteWon);
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
    EXPECT_EQ(cc.stateOfGame(), WhiteWon);
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
    EXPECT_EQ(cc.stateOfGame(), NotFinished);
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
    EXPECT_EQ(cc.stateOfGame(), NotFinished);
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
    EXPECT_EQ(cc.stateOfGame(), false);
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
    EXPECT_EQ(cc.stateOfGame(), false);
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
    EXPECT_EQ(cc.stateOfGame(), true);
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
    EXPECT_EQ(cc.stateOfGame(), true);
}


/*
 * Tests for neGgame
 */
TEST(newGame, bitBoardWhiteIsCorrecltyInitialized) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t expectedValue = 0b0000000000000000000000000000000000000001000000110000011100001111;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.newGame();
    uint_fast64_t obtainedValue = cc.getBitBoardWhite();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(newGame, bitBoardBlackIsCorrecltyInitialized) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t expectedValue = 0b1111000011100000110000001000000000000000000000000000000000000000;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.newGame();
    uint_fast64_t obtainedValue = cc.getBitBoardBlack();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}



/*
 * Tests for getBitBoardWhite
 */

TEST(GetBitBoardWhite, IsCorrecltyInitialized) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t obtainedValue = cc.getBitBoardWhite();
    uint_fast64_t expectedValue = 0b0000000000000000000000000000000000000001000000110000011100001111;

    /* Act */

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetBitBoardWhite, IsCorrecltyChangedAfterAMove) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t expectedValue = 0b0000000000000000000000000000000000000001000000110000011100011011;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    uint_fast64_t obtainedValue = cc.getBitBoardWhite();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetBitBoardWhite, IsCorrecltyChangedAfterTwoMoves) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t expectedValue = 0b0000000000000000000000000000000000000001000000110000011100011011;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{4, 7}, {3, 7}});
    uint_fast64_t obtainedValue = cc.getBitBoardWhite();


    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetBitBoardWhite, IsCorrecltyChangedAfterIllegalMove) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t expectedValue = 0b0000000000000000000000000000000000000001000000110000011100001111;

    /* Act */
    cc.move(1, {{0, 2}, {0, 4}});
    uint_fast64_t obtainedValue = cc.getBitBoardWhite();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

/*
 * Tests for getBitBoardBlack
 */

TEST(GetBitBoardBlack, IsCorrecltyInitialized) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t obtainedValue = cc.getBitBoardBlack();
    uint_fast64_t expectedValue = 0b1111000011100000110000001000000000000000000000000000000000000000;

    /* Act */

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetBitBoardBlack, IsCorrecltyChangedAfterAMove) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t expectedValue = 0b1111000011100000110000001000000000000000000000000000000000000000;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    uint_fast64_t obtainedValue = cc.getBitBoardBlack();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetBitBoardBlack, IsCorrecltyChangedAfterTwoMoves) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t expectedValue = 0b1111000011100000110000000000000010000000000000000000000000000000;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{4, 7}, {3, 7}});
    uint_fast64_t obtainedValue = cc.getBitBoardBlack();


    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

TEST(GetBitBoardBlack, IsCorrecltyChangedAfterTwoIllegalMoves) {
    /* Arrange */
    ChineseCheckers cc;
    uint_fast64_t expectedValue = 0b1111000011100000110000001000000000000000000000000000000000000000;

    /* Act */
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(0, {{4, 7}, {3, 7}});
    uint_fast64_t obtainedValue = cc.getBitBoardBlack();

    /* Assert */
    EXPECT_EQ(expectedValue, obtainedValue);
}

/*
 * Tests for getWhoIsToPlay()
 */

TEST(GetWhoIsToPlay, IsSetToZero) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */

    /* Assert */
    EXPECT_EQ(cc.getWhoIsToPlay(), 0);
}

TEST(GetWhoIsToPlay, IsSetToOneAfterAMove) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{3, 0}, {4, 0}});

    /* Assert */
    EXPECT_EQ(cc.getWhoIsToPlay(), 1);
}

TEST(GetWhoIsToPlay, IsSetToTwoAfterTwoMoves) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{3, 0}, {4, 0}});
    cc.move(1, {{5, 7}, {4, 7}});

    /* Assert */
    EXPECT_EQ(cc.getWhoIsToPlay(), 1);
}

TEST(GetWhoIsToPlay, DoesntChangeWhenMoveIsIllegal) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 0}, {1, 0}});

    /* Assert */
    EXPECT_EQ(cc.getWhoIsToPlay(), 0);
}

TEST(IllegalPositions, WhiteSideNotIllegal) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{7, 4}, {7, 3}});
    cc.move(0, {{0, 2}, {0, 3}});

    /* Assert */
    EXPECT_EQ(cc.move(1, {{7, 5}, {7, 4}}), true);
}

TEST(IllegalPositions, WhiteSideIllegal) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{7, 4}, {7, 3}});
    cc.move(0, {{0, 2}, {0, 3}});
    cc.move(1, {{7, 5}, {7, 4}});

    /* Assert */
    EXPECT_EQ(cc.move(0, {{0, 1}, {0, 2}}), false);
}

TEST(IllegalPositions, BlackSideNotIllegal) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{7, 4}, {7, 3}});
    cc.move(0, {{0, 4}, {0, 3}});
    cc.move(1, {{7, 5}, {7, 4}});

    /* Assert */
    EXPECT_EQ(cc.move(0, {{0, 3}, {0, 4}}), true);
}

TEST(IllegalPositions, BlackSideIllegal) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{7, 4}, {7, 3}});
    cc.move(0, {{0, 4}, {0, 3}});
    cc.move(1, {{7, 5}, {7, 4}});
    cc.move(0, {{0, 3}, {0, 4}});

    /* Assert */
    EXPECT_EQ(cc.move(0, {{7, 6}, {7, 5}}), false);
}

TEST(IllegalPositions, IllegalWithNaiveBlack) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{7, 4}, {7, 3}});
    cc.move(0, {{0, 4}, {1, 4}});
    cc.move(1, {{7, 3}, {6, 3}});
    cc.move(0, {{1, 4}, {2, 4}});
    cc.move(1, {{6, 3}, {5, 3}});
    cc.move(0, {{2, 4}, {3, 4}});
    cc.move(1, {{5, 3}, {4, 3}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{4, 3}, {3, 3}});
    cc.move(0, {{4, 4}, {5, 4}});
    cc.move(1, {{3, 3}, {2, 3}});
    cc.move(0, {{5, 4}, {6, 4}});
    cc.move(1, {{2, 3}, {1, 3}});
    cc.move(0, {{0, 2}, {0, 3}});
    cc.move(1, {{7, 5}, {7, 4}});
    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{7, 4}, {7, 3}});
    cc.move(0, {{0, 4}, {1, 4}});
    cc.move(1, {{7, 3}, {6, 3}});
    cc.move(0, {{1, 4}, {2, 4}});
    cc.move(1, {{6, 3}, {5, 3}});
    cc.move(0, {{2, 4}, {3, 4}});
    cc.move(1, {{5, 3}, {4, 3}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{4, 3}, {3, 3}});
    cc.move(0, {{4, 4}, {5, 4}});
    cc.move(1, {{3, 3}, {2, 3}});
    cc.move(0, {{6, 4}, {7, 4}});
    cc.move(1, {{1, 3}, {0, 3}});
    cc.move(0, {{5, 4}, {6, 4}});
    cc.move(1, {{2, 3}, {1, 3}});
    cc.move(0, {{0, 1}, {0, 2}});
    cc.move(1, {{7, 6}, {7, 5}});
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{7, 5}, {7, 3}});
    cc.move(0, {{7, 4}, {7, 5}});
    cc.move(1, {{0, 3}, {0, 2}});

    /* Assert */
    EXPECT_EQ(cc.move(0, {{6, 4}, {7, 4}}), true);
}

TEST(IllegalPositions, IllegalWithNaiveWhite) {
    /* Arrange */
    ChineseCheckers cc;

    /* Act */
    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{7, 4}, {7, 3}});
    cc.move(0, {{0, 4}, {1, 4}});
    cc.move(1, {{7, 3}, {6, 3}});
    cc.move(0, {{1, 4}, {2, 4}});
    cc.move(1, {{6, 3}, {5, 3}});
    cc.move(0, {{2, 4}, {3, 4}});
    cc.move(1, {{5, 3}, {4, 3}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{4, 3}, {3, 3}});
    cc.move(0, {{4, 4}, {5, 4}});
    cc.move(1, {{3, 3}, {2, 3}});
    cc.move(0, {{5, 4}, {6, 4}});
    cc.move(1, {{2, 3}, {1, 3}});
    cc.move(0, {{0, 2}, {0, 3}});
    cc.move(1, {{7, 5}, {7, 4}});
    cc.move(0, {{0, 3}, {0, 4}});
    cc.move(1, {{7, 4}, {7, 3}});
    cc.move(0, {{0, 4}, {1, 4}});
    cc.move(1, {{7, 3}, {6, 3}});
    cc.move(0, {{1, 4}, {2, 4}});
    cc.move(1, {{6, 3}, {5, 3}});
    cc.move(0, {{2, 4}, {3, 4}});
    cc.move(1, {{5, 3}, {4, 3}});
    cc.move(0, {{3, 4}, {4, 4}});
    cc.move(1, {{4, 3}, {3, 3}});
    cc.move(0, {{4, 4}, {5, 4}});
    cc.move(1, {{3, 3}, {2, 3}});
    cc.move(0, {{6, 4}, {7, 4}});
    cc.move(1, {{1, 3}, {0, 3}});
    cc.move(0, {{5, 4}, {6, 4}});
    cc.move(1, {{2, 3}, {1, 3}});
    cc.move(0, {{0, 1}, {0, 2}});
    cc.move(1, {{7, 6}, {7, 5}});
    cc.move(0, {{0, 2}, {0, 4}});
    cc.move(1, {{0, 3}, {0, 2}});
    cc.move(0, {{7, 4}, {7, 3}});

    /* Assert */
    EXPECT_EQ(cc.move(1, {{1, 3}, {0, 3}}), true);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
