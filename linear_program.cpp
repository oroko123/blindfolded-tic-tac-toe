#include "bits/stdc++.h"
#include "constants.h"
#include "gurobi_c++.h"
#include "serializer.h"

using namespace std;

class LinearProgram {
public:
  LinearProgram(bool lose_move_, Player player_)
      : lose_move(lose_move_), player(player_), env(true), model(env) {
    env.set("LogFile",
            lose_move ? LOSE_MOVE_LOG_FILENAME : DO_NOT_LOSE_MOVE_LOG_FILENAME);
    env.start();
  }
  LinearProgram(bool lose_move_, Player player_,
                const map<array<PlayerKey, 2>, int> &P_,
                const map<array<PlayerKey, 2>, int> &C_,
                const map<array<PlayerKey, 2>, int> &D_)
      : lose_move(lose_move_), player(player_), env(true), model(env), P(P_),
        C(C_), D(D_) {
    env.set("LogFile",
            lose_move ? LOSE_MOVE_LOG_FILENAME : DO_NOT_LOSE_MOVE_LOG_FILENAME);
    env.start();
  }
  void Run() {
    ValidateInput();
    AddZeroElements();
    AddVariables();
    SetObjective();
    AddConstraints();
    Solve();
  }
  void ReadMatrices() {
    cout << "Reading matrices" << endl;
    try {
      P = Serializer::read_from_file<array<PlayerKey, 2>, int>(
          lose_move ? LOSE_MOVE_P_FILENAME : DO_NOT_LOSE_MOVE_P_FILENAME);
      C = Serializer::read_from_file<array<PlayerKey, 2>, int>(
          lose_move ? LOSE_MOVE_C_FILENAME : DO_NOT_LOSE_MOVE_C_FILENAME);
      D = Serializer::read_from_file<array<PlayerKey, 2>, int>(
          lose_move ? LOSE_MOVE_D_FILENAME : DO_NOT_LOSE_MOVE_D_FILENAME);
    } catch (const exception &e) {
      cout << e.what();
    }
  }

  void PrintSolutionToFile() {
    cout << "Writing solution to files..." << endl;
    map<PlayerKey, double> r_res;
    for (auto &[k, v] : r) {
      r_res[k] = v.get(GRB_DoubleAttr_ObjVal);
    }
    map<PlayerKey, double> s_res;
    for (auto &[k, v] : s) {
      s_res[k] = v.get(GRB_DoubleAttr_ObjVal);
    }
    Serializer::write_to_file(lose_move ? LOSE_MOVE_R2_FILENAME
                                        : DO_NOT_LOSE_MOVE_R2_FILENAME,
                              r_res, false);
    Serializer::write_to_file(lose_move ? LOSE_MOVE_S2_FILENAME
                                        : DO_NOT_LOSE_MOVE_S2_FILENAME,
                              s_res, false);
    cout << "Calculated result: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
  }

  void PrintSolutionToScreen() {
    cout << "r:" << endl;
    for (auto &[k, v] : r) {
      cout << k << ": " << v.get(GRB_DoubleAttr_ObjVal) << endl;
    }
    cout << "s:" << endl;
    for (auto &[k, v] : s) {
      cout << k << ": " << v.get(GRB_DoubleAttr_ObjVal) << endl;
    }
  }

private:
  void ValidateInput() {
    cout << "Validating input" << endl;
    for (const auto &matrix : {C, D}) {
      map<PlayerKey, int> sums;
      for (const auto &[k, v] : matrix) {
        sums[k[0]] += v;
      }
      for (const auto &[k, v] : sums) {
        (void)k;
        assert(v == 0);
      }
    }
  }
  void AddZeroElements() {
    cout << "Adding starting elements to C and D" << endl;
    C.insert({{0, 1}, 1});
    D.insert({{0, 1}, 1});
  }
  void AddVariables() {
    cout << "Adding variables s and r" << endl;
    set<PlayerKey> player_states[2];
    set<PlayerKey> player_actionstates[2];
    for (const auto &[k, v] : C) {
      (void)v;
      player_states[PLAYER2].insert(get<0>(k));
      player_actionstates[PLAYER2].insert(get<1>(k));
    }
    for (const auto &[k, v] : D) {
      (void)v;
      player_states[PLAYER1].insert(get<0>(k));
      player_actionstates[PLAYER1].insert(get<1>(k));
    }
    switch (player) {
    case PLAYER1:
      for (auto b : player_actionstates[PLAYER1]) {
        s[b] = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS);
      }
      for (auto i : player_states[PLAYER2]) {
        r[i] = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
      }
      break;
    case PLAYER2:
      for (auto a : player_actionstates[PLAYER2]) {
        s[a] = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS);
      }
      for (auto j : player_states[PLAYER1]) {
        r[j] = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
      }
      break;
    default:
      assert(false);
      break;
    }
  }

  // currently for PLAYER2
  void AddConstraints() {
    cout << "Adding constraints" << endl;
    map<PlayerKey, GRBLinExpr> lhs1, rhs1, lhs2;
    for (auto [k, v] : D) {
      PlayerKey j = get<0>(k);
      PlayerKey b = get<1>(k);
      lhs1[b] += v * r[j];
    }
    for (auto [k, v] : P) {
      PlayerKey a = get<0>(k);
      PlayerKey b = get<1>(k);
      rhs1[b] += v * s[a];
    }
    for (auto e : player_actionstates[PLAYER1]) {
      model.addConstr(lhs1[e], GRB_GREATER_EQUAL, rhs1[e]);
    }
    for (auto [k, v] : C) {
      PlayerKey i = get<0>(k);
      PlayerKey a = get<1>(k);
      lhs2[a] += v * s[i];
    }
    for (auto e : player_actionstates[PLAYER2]) {
      model.addConstr(lhs2[e], GRB_EQUAL, C[{0, e}]);
    }
  }
  void SetObjective() {
    cout << "Setting objective" << endl;
    GRBLinExpr obj = r[0];
    model.setObjective(obj, GRB_MINIMIZE);
  }
  void Solve() {
    cout << "Solving" << endl;
    model.optimize();
  }
  bool lose_move;
  Player player;
  GRBEnv env;
  GRBModel model;
  map<PlayerKey, GRBVar> r;
  map<PlayerKey, GRBVar> s;
  set<PlayerKey> player_states[2];       // [J, I]
  set<PlayerKey> player_actionstates[2]; // [B, A]
  map<array<PlayerKey, 2>, int> P;
  map<array<PlayerKey, 2>, int> C;
  map<array<PlayerKey, 2>, int> D;
};

void test_poker() {

  map<array<PlayerKey, 2>, int> P = {{{1, 2}, 1}, {{1, 3}, -1}, {{2, 1}, -1},
                                     {{2, 3}, 1}, {{3, 1}, 1},  {{3, 2}, -1}};
  map<array<PlayerKey, 2>, int> C = {
      {{0, 0}, -1}, {{0, 1}, 1}, {{0, 2}, 1}, {{0, 3}, 1}};
  map<array<PlayerKey, 2>, int> D = {
      {{0, 0}, -1}, {{0, 1}, 1}, {{0, 2}, 1}, {{0, 3}, 1}};
  LinearProgram linear_program(/*lose_move=*/true, /*player=*/PLAYER2, P, C, D);
  linear_program.Run();
  linear_program.PrintSolutionToScreen();
}

void run_lose_move_player2() {
  LinearProgram linear_program(/*lose_move=*/true, /*player=*/PLAYER2);
  linear_program.ReadMatrices();
  linear_program.Run();
  linear_program.PrintSolutionToFile();
}
int main() {
  try {
    test_poker();
    // run_lose_move_player2();
  } catch (GRBException e) {
    cout << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  }
}