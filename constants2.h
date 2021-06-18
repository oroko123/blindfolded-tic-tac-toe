#ifndef CONSTANTS2
#define CONSTANTS2

#include "bits/stdc++.h"
using namespace std;

const string LOG_FILENAME = "linear_program_log";

const string DO_NOT_LOSE_MOVE_P_FILENAME = "do_not_lose_move_P";
const string DO_NOT_LOSE_MOVE_C_FILENAME = "do_not_lose_move_C";
const string DO_NOT_LOSE_MOVE_D_FILENAME = "do_not_lose_move_D";
const string DO_NOT_LOSE_MOVE_R1_FILENAME = "do_not_lose_move_R1";
const string DO_NOT_LOSE_MOVE_R2_FILENAME = "do_not_lose_move_R2";
const string DO_NOT_LOSE_MOVE_S1_FILENAME = "do_not_lose_move_S1";
const string DO_NOT_LOSE_MOVE_S2_FILENAME = "do_not_lose_move_S2";
const string DO_NOT_LOSE_MOVE_LP_PLAYER2_FILENAME =
    "do_not_lose_move_GUROBI_OUT2.lp";

const string LOSE_MOVE_P_FILENAME = "lose_move_P";
const string LOSE_MOVE_C_FILENAME = "lose_move_C";
const string LOSE_MOVE_D_FILENAME = "lose_move_D";
const string LOSE_MOVE_R1_FILENAME = "lose_move_R1";
const string LOSE_MOVE_R2_FILENAME = "lose_move_R2";
const string LOSE_MOVE_S1_FILENAME = "lose_move_S1";
const string LOSE_MOVE_S2_FILENAME = "lose_move_S2";
const string LOSE_MOVE_LP_PLAYER2_FILENAME = "lose_move_GUROBI_OUT2.lp";

enum Result { TIE, PLAYER1WIN, PLAYER2WIN, ONGOING, INVALID };
enum Player { PLAYER1, PLAYER2, NONE }; // WARNING!!! DO NOT CHANGE THE ORDER

struct HistoryKey {
  long long first_ninth;
  long long tenth_eighteen;
  bool operator==(const HistoryKey &other) const {
    return this->first_ninth == other.first_ninth &&
           this->tenth_eighteen == other.tenth_eighteen;
  }
};

typedef long long PlayerKey;

char playerToSign(Player player) {
  switch (player) {
  case PLAYER1:
    return 'X';
  case PLAYER2:
    return 'O';
  default:
    return ' ';
  }
}

const int SIMULATIONS_NUM = 10000;
const int FIELDS_NUM = 4;
const long long PROBABILITY_FACTOR = 64 * 3; // 2^6

const vector<int> id = {0, 1, 2, 3};
const vector<int> ox = {2, 3, 0, 1};
const vector<int> oy = {1, 0, 3, 2};
const vector<int> diag = {0, 2, 1, 3};
const vector<int> sec_diag = {3, 1, 2, 0};
const vector<int> deg_90 = {1, 2, 3, 0};
const vector<int> deg_180 = {2, 3, 0, 1};
const vector<int> deg_270 = {3, 0, 1, 2};
const vector<vector<int>> permutations = {id,       ox,     oy,      diag,
                                          sec_diag, deg_90, deg_180, deg_270};

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
  for (size_t i = 0; i < a.size(); ++i) {
    res.push_back(a[pi[i]]);
  }
  return res;
}

#endif