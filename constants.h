#ifndef CONSTANTS
#define CONSTANTS

#include "bits/stdc++.h"

enum Result { TIE, PLAYER1WIN, PLAYER2WIN, ONGOING, INVALID };
enum Player { PLAYER1, PLAYER2, NONE }; // WARNING!!! DO NOT CHANGE THE ORDER

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

const int SIMULATIONS_NUM = 10000;
const int FIELDS_NUM = 9;

const std::vector<int> id = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const std::vector<int> ox = {6, 7, 8, 3, 4, 5, 0, 1, 2};
const std::vector<int> oy = {2, 1, 0, 5, 4, 3, 8, 7, 6};
const std::vector<int> diag = {0, 3, 6, 1, 4, 7, 2, 5, 8};
const std::vector<int> sec_diag = {8, 5, 2, 7, 4, 1, 6, 3, 0};
const std::vector<int> deg_90 = {2, 5, 8, 1, 4, 7, 0, 3, 6};
const std::vector<int> deg_180 = {8, 7, 6, 5, 4, 3, 2, 1, 0};
const std::vector<int> deg_270 = {6, 3, 0, 7, 4, 1, 8, 5, 2};
const std::vector<std::vector<int>> permutations = {
    id, ox, oy, diag, sec_diag, deg_90, deg_180, deg_270};

constexpr Player getOppositePlayer(Player player) {
  if (player == PLAYER1) {
    return PLAYER2;
  } else if (player == PLAYER2) {
    return PLAYER1;
  } else {
    return NONE;
  }
}

std::vector<Player> permute(const std::vector<Player> &a,
                            const std::vector<int> &pi) {
  std::vector<Player> res;
  for (size_t i = 0; i < a.size(); ++i) {
    res.push_back(a[pi[i]]);
  }
  return res;
}

#endif