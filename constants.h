#ifndef CONSTANTS
#define CONSTANTS

#include <bits/stdc++.h>
using namespace std;

enum Result { TIE, PLAYER1WIN, PLAYER2WIN, ONGOING, INVALID };
enum Player { PLAYER1, PLAYER2, NONE }; // WARNING!!! DO NOT CHANGE THE ORDER

const int SIMULATIONS_NUM = 10000;
const int FIELDS_NUM = 9;
const long long THREE_TO_POWER_NINE = 19683;

const vector<vector<int>> permutations = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8}, // id
    {6, 7, 8, 3, 4, 5, 0, 1, 2}, // OX
    {2, 1, 0, 5, 4, 3, 8, 7, 6}, // OY
    {0, 3, 6, 1, 4, 7, 2, 5, 8}, // diagonal
    {8, 5, 2, 7, 4, 1, 6, 3, 0}, // second diagonal
    {2, 5, 8, 1, 4, 7, 0, 3, 6}, // 90*
    {8, 7, 6, 5, 4, 3, 2, 1, 0}, // 180*
    {6, 3, 0, 7, 4, 1, 8, 5, 2}  // 270*
};

constexpr Player getOppositePlayer(Player player) {
  if (player == PLAYER1) {
    return PLAYER2;
  } else if (player == PLAYER2) {
    return PLAYER1;
  } else {
    return NONE;
  }
}

vector<Player> permute(const vector<Player> &a, const vector<int> &pi) {
  vector<Player> res;
  for (int i = 0; i < a.size(); i++) {
    res.push_back(a[pi[i]]);
  }
  return res;
}

#endif