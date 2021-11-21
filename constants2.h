#ifndef CONSTANTS2
#define CONSTANTS2

#include "bits/stdc++.h"
using namespace std;

enum Result { TIE, PLAYER1WIN, PLAYER2WIN, ONGOING, INVALID };
enum Player { PLAYER1, PLAYER2, NONE }; // WARNING!!! DO NOT CHANGE THE ORDER

int SECONDS_IN_24_HOURS = 24 * 60 * 60;

std::string GetLogFilename(bool lose_move, Player player) {
  std::string full_log_filename = "linear_program_log";
  if (lose_move) {
    full_log_filename = "lose_move_" + full_log_filename;
  } else {
    full_log_filename = "do_not_lose_move_" + full_log_filename;
  }
  if (player == PLAYER2) {
    full_log_filename = full_log_filename + "_P2";
  } else { // player == PLAYER1
    full_log_filename = full_log_filename + "_P1";
  }
  return full_log_filename;
}

std::string GetMatrixFilename(std::string name, bool lose_move) {
  const std::string DO_NOT_LOSE_MOVE_P_FILENAME = "do_not_lose_move_P";
  const std::string DO_NOT_LOSE_MOVE_C_FILENAME = "do_not_lose_move_C";
  const std::string DO_NOT_LOSE_MOVE_D_FILENAME = "do_not_lose_move_D";
  const std::string DO_NOT_LOSE_MOVE_PT_FILENAME = "do_not_lose_move_PT";
  const std::string DO_NOT_LOSE_MOVE_CT_FILENAME = "do_not_lose_move_CT";
  const std::string DO_NOT_LOSE_MOVE_DT_FILENAME = "do_not_lose_move_DT";

  const std::string LOSE_MOVE_P_FILENAME = "lose_move_P";
  const std::string LOSE_MOVE_C_FILENAME = "lose_move_C";
  const std::string LOSE_MOVE_D_FILENAME = "lose_move_D";
  const std::string LOSE_MOVE_PT_FILENAME = "lose_move_PT";
  const std::string LOSE_MOVE_CT_FILENAME = "lose_move_CT";
  const std::string LOSE_MOVE_DT_FILENAME = "lose_move_DT";
  const std::set<std::string> names({"P", "C", "D", "PT", "CT", "DT"});
  assert(names.find(name) != names.end());
  if (name == "P") {
    return lose_move ? LOSE_MOVE_P_FILENAME : DO_NOT_LOSE_MOVE_P_FILENAME;
  } else if (name == "C") {
    return lose_move ? LOSE_MOVE_C_FILENAME : DO_NOT_LOSE_MOVE_C_FILENAME;
  } else if (name == "D") {
    return lose_move ? LOSE_MOVE_D_FILENAME : DO_NOT_LOSE_MOVE_D_FILENAME;
  } else if (name == "PT") {
    return lose_move ? LOSE_MOVE_PT_FILENAME : DO_NOT_LOSE_MOVE_PT_FILENAME;
  } else if (name == "CT") {
    return lose_move ? LOSE_MOVE_CT_FILENAME : DO_NOT_LOSE_MOVE_CT_FILENAME;
  } else { // name == "DT"
    return lose_move ? LOSE_MOVE_DT_FILENAME : DO_NOT_LOSE_MOVE_DT_FILENAME;
  }
}

std::string GetOutputFilename(std::string type, Player player, bool lose_move) {
  const std::string DO_NOT_LOSE_MOVE_R1_FILENAME = "do_not_lose_move_R1";
  const std::string DO_NOT_LOSE_MOVE_R2_FILENAME = "do_not_lose_move_R2";
  const std::string DO_NOT_LOSE_MOVE_S1_FILENAME = "do_not_lose_move_S1";
  const std::string DO_NOT_LOSE_MOVE_S2_FILENAME = "do_not_lose_move_S2";
  const std::string DO_NOT_LOSE_MOVE_LP_PLAYER1_FILENAME =
      "do_not_lose_move_GUROBI_OUT1.lp";
  const std::string DO_NOT_LOSE_MOVE_LP_PLAYER2_FILENAME =
      "do_not_lose_move_GUROBI_OUT2.lp";

  const std::string LOSE_MOVE_R1_FILENAME = "lose_move_R1";
  const std::string LOSE_MOVE_R2_FILENAME = "lose_move_R2";
  const std::string LOSE_MOVE_S1_FILENAME = "lose_move_S1";
  const std::string LOSE_MOVE_S2_FILENAME = "lose_move_S2";
  const std::string LOSE_MOVE_LP_PLAYER1_FILENAME = "lose_move_GUROBI_OUT1.lp";
  const std::string LOSE_MOVE_LP_PLAYER2_FILENAME = "lose_move_GUROBI_OUT2.lp";
  const std::set<std::string> types({"S", "R", "LINEAR_PROGRAM"});
  assert(types.find(type) != types.end());
  if (type == "S") {
    if (player == PLAYER1)
      return lose_move ? LOSE_MOVE_S1_FILENAME : DO_NOT_LOSE_MOVE_S1_FILENAME;
    if (player == PLAYER2)
      return lose_move ? LOSE_MOVE_S2_FILENAME : DO_NOT_LOSE_MOVE_S2_FILENAME;
    assert(false);
  } else if (type == "R") {
    if (player == PLAYER1)
      return lose_move ? LOSE_MOVE_R1_FILENAME : DO_NOT_LOSE_MOVE_R1_FILENAME;
    if (player == PLAYER2)
      return lose_move ? LOSE_MOVE_R2_FILENAME : DO_NOT_LOSE_MOVE_R2_FILENAME;
    assert(false);
  } else { // type == "LINEAR_PROGRAM"
    if (player == PLAYER1)
      return lose_move ? LOSE_MOVE_LP_PLAYER1_FILENAME
                       : DO_NOT_LOSE_MOVE_LP_PLAYER1_FILENAME;
    if (player == PLAYER2)
      return lose_move ? LOSE_MOVE_LP_PLAYER2_FILENAME
                       : DO_NOT_LOSE_MOVE_LP_PLAYER2_FILENAME;
    assert(false);
  }
}

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

const int SIMULATIONS_NUM = 100000;
const int FIELDS_NUM = 4;
const std::vector<int> all_fields_vec = {0, 1, 2, 3};
const std::vector<int> corner_fields_vec = {0, 1, 2, 3};
const std::vector<int> side_fields_vec = {};
const std::vector<int> middle_fields_vec = {};

const vector<vector<int>> winning_sets = {
    {0, 1}, {0, 2}, {0, 3}, {1, 2},
    {1, 3}, {2, 3}};

const vector<int> id = {0, 1, 2, 3};
const vector<int> ox = {2, 3, 0, 1};
const vector<int> oy = {1, 0, 3, 2};
const vector<int> diag = {0, 2, 1, 3};
const vector<int> sec_diag = {3, 1, 2, 0};
const vector<int> deg_90 = {1, 3, 0, 2};
const vector<int> deg_180 = {3, 2, 1, 0};
const vector<int> deg_270 = {2, 0, 3, 1};
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