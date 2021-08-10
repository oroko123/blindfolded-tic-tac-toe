#include "state2.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(GeneratePlayerKeyTest, TestCalculation) {
  PlayerKey player1_key =
      GameState2::generatePlayerStateKey({{3, PLAYER1, true},
                                         {2, PLAYER2, true},
                                         {1, PLAYER1, true}},
                                        PLAYER1);
  PlayerKey player2_key =
      GameState2::generatePlayerStateKey({{0, PLAYER2, true},
                                         {0, PLAYER1, false},
                                         {1, PLAYER1, true},
                                         {2, PLAYER2, true}},
                                        PLAYER2);
  ASSERT_EQ(player1_key, 8 * 8 + 8 * (3 * 2 + 1) + (1 * 2 + 1));
  ASSERT_EQ(player2_key, 8 * 8 + 8 * (0 * 2 + 1) + (2 * 2 + 1));
}

TEST(GeneratePlayerKeyTest, TestLeadingZero) {
  PlayerKey player2_key_1 =
      GameState2::generatePlayerStateKey({{0, PLAYER1, true},
                                         {0, PLAYER2, false},
                                         {1, PLAYER1, true},
                                         {1, PLAYER2, false},
                                         {3, PLAYER1, true}},
                                        PLAYER2);
  PlayerKey player2_key_2 = GameState2::generatePlayerStateKey(
      {{1, PLAYER1, true}, {1, PLAYER2, false}, {3, PLAYER1, true}}, PLAYER2);
  ASSERT_EQ(player2_key_1, 8 * 8 + (1 * 2 + 0));
  ASSERT_EQ(player2_key_2, 8 + (1 * 2 + 0));
}

TEST(HistoryGeneration, TestTheSame) {
  LoseMoveWhenConflictGameState2 state;
  auto state1 =
      state.performMove(0).performMove(3).performMove(3).performMove(1);
  auto state2 =
      state.performMove(0).performMove(3).performMove(3).performMove(2);
  ASSERT_EQ(state1.calculateRepresentativeActionStateKeyPair(),
            state2.calculateRepresentativeActionStateKeyPair());
}

TEST(HistoryGeneration, TestDifferent) {
  LoseMoveWhenConflictGameState2 state;
  auto state1 =
      state.performMove(0).performMove(1).performMove(1).performMove(2);
  auto state2 =
      state.performMove(0).performMove(1).performMove(1).performMove(3);
  ASSERT_NE(state1.calculateRepresentativeActionStateKeyPair(),
            state2.calculateRepresentativeActionStateKeyPair());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}