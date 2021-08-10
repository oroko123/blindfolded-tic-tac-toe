#include "constants2.h"
#include <gtest/gtest.h>

TEST(PermutationsTest, Id) {
  vector<Player> board({PLAYER1, PLAYER2, NONE,      // first row
                        PLAYER2, PLAYER1, NONE,      // second row
                        PLAYER2, PLAYER1, PLAYER1}); // third row
  ASSERT_EQ(permute(board, id), board);
}

TEST(PermutationsTest, Ox) {
  vector<Player> board({PLAYER1, PLAYER2, NONE,               // first row
                        PLAYER2, PLAYER1, NONE,               // second row
                        PLAYER2, PLAYER1, PLAYER1});          // third row
  vector<Player> transposed_board({PLAYER2, PLAYER1, PLAYER1, // first row
                                   PLAYER2, PLAYER1, NONE,    // second row
                                   PLAYER1, PLAYER2, NONE});  // third row
  ASSERT_EQ(permute(board, ox), transposed_board);
}
TEST(PermutationsTest, Oy) {
  vector<Player> board({PLAYER1, PLAYER2, NONE,                 // first row
                        PLAYER2, PLAYER1, NONE,                 // second row
                        PLAYER2, PLAYER1, PLAYER1});            // third row
  vector<Player> transposed_board({NONE, PLAYER2, PLAYER1,      // first row
                                   NONE, PLAYER1, PLAYER2,      // second row
                                   PLAYER1, PLAYER1, PLAYER2}); // third row
  ASSERT_EQ(permute(board, oy), transposed_board);
}
TEST(PermutationsTest, Diag) {
  vector<Player> board({PLAYER1, PLAYER2, NONE,               // first row
                        PLAYER2, PLAYER1, NONE,               // second row
                        PLAYER2, PLAYER1, PLAYER1});          // third row
  vector<Player> transposed_board({PLAYER1, PLAYER2, PLAYER2, // first row
                                   PLAYER2, PLAYER1, PLAYER1, // second row
                                   NONE, NONE, PLAYER1});     // third row
  ASSERT_EQ(permute(board, diag), transposed_board);
}
TEST(PermutationsTest, SecDiag) {
  vector<Player> board({PLAYER1, PLAYER2, NONE,                 // first row
                        PLAYER2, PLAYER1, NONE,                 // second row
                        PLAYER2, PLAYER1, PLAYER1});            // third row
  vector<Player> transposed_board({PLAYER1, NONE, NONE,         // first row
                                   PLAYER1, PLAYER1, PLAYER2,   // second row
                                   PLAYER2, PLAYER2, PLAYER1}); // third row
  ASSERT_EQ(permute(board, sec_diag), transposed_board);
}
TEST(PermutationsTest, Deg90) {
  vector<Player> board({PLAYER1, PLAYER2, NONE,                 // first row
                        PLAYER2, PLAYER1, NONE,                 // second row
                        PLAYER2, PLAYER1, PLAYER1});            // third row
  vector<Player> transposed_board({NONE, NONE, PLAYER1,         // first row
                                   PLAYER2, PLAYER1, PLAYER1,   // second row
                                   PLAYER1, PLAYER2, PLAYER2}); // third row
  ASSERT_EQ(permute(board, deg_90), transposed_board);
}
TEST(PermutationsTest, Deg180) {
  vector<Player> board({PLAYER1, PLAYER2, NONE,               // first row
                        PLAYER2, PLAYER1, NONE,               // second row
                        PLAYER2, PLAYER1, PLAYER1});          // third row
  vector<Player> transposed_board({PLAYER1, PLAYER1, PLAYER2, // first row
                                   NONE, PLAYER1, PLAYER2,    // second row
                                   NONE, PLAYER2, PLAYER1});  // third row
  ASSERT_EQ(permute(board, deg_180), transposed_board);
}
TEST(PermutationsTest, Deg270) {
  vector<Player> board({PLAYER1, PLAYER2, NONE,               // first row
                        PLAYER2, PLAYER1, NONE,               // second row
                        PLAYER2, PLAYER1, PLAYER1});          // third row
  vector<Player> transposed_board({PLAYER2, PLAYER2, PLAYER1, // first row
                                   PLAYER1, PLAYER1, PLAYER2, // second row
                                   PLAYER1, NONE, NONE});     // third row
  ASSERT_EQ(permute(board, deg_270), transposed_board);
}
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}