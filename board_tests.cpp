#include "board.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(BoardResTest, Player1Wins) {
  Board board({PLAYER1, PLAYER2, NONE,      // first row
               PLAYER2, PLAYER1, NONE,      // second row
               PLAYER2, PLAYER1, PLAYER1}); // third row
  ASSERT_EQ(board.getResult(), PLAYER1WIN);
}

TEST(BoardResTest, Player2Wins) {
  Board board({PLAYER1, PLAYER2, PLAYER2,   // first row
               PLAYER2, PLAYER2, NONE,      // second row
               PLAYER2, PLAYER1, PLAYER1}); // third row
  ASSERT_EQ(board.getResult(), PLAYER2WIN);
}

TEST(BoardResTest, Tie) {
  Board board({PLAYER1, PLAYER2, PLAYER1,   // first row
               PLAYER1, PLAYER2, PLAYER2,   // second row
               PLAYER2, PLAYER1, PLAYER1}); // third row
  ASSERT_EQ(board.getResult(), TIE);
}

TEST(BoardResTest, Ongoing) {
  Board board({PLAYER1, PLAYER2, PLAYER2,   // first row
               PLAYER2, PLAYER2, NONE,      // second row
               PLAYER1, PLAYER1, PLAYER2}); // third row
  ASSERT_EQ(board.getResult(), ONGOING);
}

TEST(BoardResTest, Invalid) {
  Board board({PLAYER2, PLAYER2, PLAYER2, // first row
               PLAYER1, PLAYER1, PLAYER1, // second row
               NONE, NONE, NONE});        // third row
  ASSERT_EQ(board.getResult(), INVALID);
}

TEST(BoardFreeFieldsTest, FullBoard) {
  Board board({PLAYER2, PLAYER2, PLAYER2,   // first row
               PLAYER1, PLAYER1, PLAYER1,   // second row
               PLAYER2, PLAYER2, PLAYER2}); // third row
  ASSERT_THAT(board.getFreeFields(), ::testing::ElementsAre());
}

TEST(BoardFreeFieldsTest, EmptyBoard) {
  Board board({NONE, NONE, NONE,   // first row
               NONE, NONE, NONE,   // second row
               NONE, NONE, NONE}); // third row
  ASSERT_THAT(board.getFreeFields(),
              ::testing::ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8));
}

TEST(BoardFreeFieldsTest, PartiallyFilled) {
  Board board({NONE, NONE, PLAYER2,   // first row
               NONE, PLAYER1, NONE,   // second row
               PLAYER2, NONE, NONE}); // third row
  ASSERT_THAT(board.getFreeFields(), ::testing::ElementsAre(0, 1, 3, 5, 7, 8));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}