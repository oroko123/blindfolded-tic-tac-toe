#include "state.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(GeneratePlayerKeyTest, TestCalculation) {
  PlayerKey player1_key =
      GameState::generatePlayerStateKey({{3, PLAYER1, true},
                                         {4, PLAYER2, true},
                                         {1, PLAYER1, true},
                                         {1, PLAYER2, false},
                                         {6, PLAYER1, true}},
                                        PLAYER1);
  PlayerKey player2_key =
      GameState::generatePlayerStateKey({{3, PLAYER1, true},
                                         {4, PLAYER2, true},
                                         {1, PLAYER1, true},
                                         {1, PLAYER2, false},
                                         {6, PLAYER1, true}},
                                        PLAYER2);
  ASSERT_EQ(player1_key, 18 * 18 * 18 + 18 * 18 * (3 * 2 + 1) +
                             18 * (1 * 2 + 1) + 6 * 2 + 1);
  ASSERT_EQ(player2_key, 18 * 18 + 18 * (4 * 2 + 1) + (1 * 2 + 0));
}

TEST(GeneratePlayerKeyTest, TestLeadingZero) {
  PlayerKey player2_key_1 =
      GameState::generatePlayerStateKey({{0, PLAYER1, true},
                                         {0, PLAYER2, false},
                                         {1, PLAYER1, true},
                                         {1, PLAYER2, false},
                                         {6, PLAYER1, true}},
                                        PLAYER2);
  PlayerKey player2_key_2 = GameState::generatePlayerStateKey(
      {{1, PLAYER1, true}, {1, PLAYER2, false}, {6, PLAYER1, true}}, PLAYER2);
  ASSERT_EQ(player2_key_1, 18 * 18 + (1 * 2 + 0));
  ASSERT_EQ(player2_key_2, 18 + (1 * 2 + 0));
}

TEST(HistoryGeneration, TestTheSame) {
  DoNotLoseMoveWhenConflictGameState state;
  auto state1 =
      state.performMove(0).performMove(1).performMove(4).performMove(7);
  auto state2 =
      state.performMove(6).performMove(3).performMove(4).performMove(5);
  ASSERT_EQ(state1.calculateRepresentativeActionStateKeyPair(),
            state2.calculateRepresentativeActionStateKeyPair());
}

TEST(HistoryGeneration, TestDifferent) {
  DoNotLoseMoveWhenConflictGameState state;
  auto state1 =
      state.performMove(0).performMove(1).performMove(4).performMove(7);
  auto state2 =
      state.performMove(0).performMove(1).performMove(4).performMove(5);
  ASSERT_NE(state1.calculateRepresentativeActionStateKeyPair(),
            state2.calculateRepresentativeActionStateKeyPair());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}