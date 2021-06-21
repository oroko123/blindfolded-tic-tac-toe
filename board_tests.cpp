#include "board2.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(BoardResTest, Player1Wins) {
  Board2 board({PLAYER2, NONE,      // first row
                PLAYER1, PLAYER1}); // second row
  ASSERT_EQ(board.getResult(), PLAYER1WIN);
}

TEST(BoardResTest, Player2Wins) {
  Board2 board({PLAYER2, NONE,      // first row
                PLAYER2, PLAYER1}); // second row
  ASSERT_EQ(board.getResult(), PLAYER2WIN);
}

TEST(BoardResTest, Invalid2) {
  Board2 board({PLAYER1, PLAYER2,   // first row
                PLAYER2, PLAYER1}); // second row
  ASSERT_EQ(board.getResult(), INVALID);
}

TEST(BoardResTest, Player1Wins2) {
  Board2 board({PLAYER1, PLAYER2, // first row
                NONE, PLAYER1});  // second row
  ASSERT_EQ(board.getResult(), PLAYER1WIN);
}

TEST(BoardResTest, Invalid) {
  Board2 board({PLAYER2, PLAYER1,   // first row
                PLAYER2, PLAYER1}); // second row
  ASSERT_EQ(board.getResult(), INVALID);
}

TEST(BoardFreeFieldsTest, FullBoard) {
  Board2 board({PLAYER2, PLAYER2,   // first row
                PLAYER1, PLAYER2}); // second row
  ASSERT_THAT(board.getFreeFields(), ::testing::ElementsAre());
}

TEST(BoardFreeFieldsTest, EmptyBoard) {
  Board2 board({NONE, NONE,   // first row
                NONE, NONE}); // second row
  ASSERT_THAT(board.getFreeFields(), ::testing::ElementsAre(0, 1, 2, 3));
}

TEST(BoardFreeFieldsTest, PartiallyFilled) {
  Board2 board({NONE, PLAYER2,   // first row
                NONE, PLAYER1}); // second row
  ASSERT_THAT(board.getFreeFields(), ::testing::ElementsAre(0, 2));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}