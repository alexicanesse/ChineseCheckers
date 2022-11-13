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

/* The fixture for testing class ChineseCheckers */
class ChineseCheckersTest : public::testing::Test {
};


/* only public interface should be tested.
 * Each test suit correspond to a method. */

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
