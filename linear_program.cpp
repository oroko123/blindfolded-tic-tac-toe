#include "bits/stdc++.h"
#include "constants.h"
#include "gurobi_c++.h"
#include "serializer.h"

#include <experimental/filesystem>

using namespace std;

namespace fs = std::experimental::filesystem;

class LinearProgram {
public:
  LinearProgram(bool lose_move_, Player player_, const GRBEnv &env)
      : lose_move(lose_move_), player(player_), model(env) {}
  LinearProgram(bool lose_move_, Player player_, const GRBEnv &env,
                const map<array<PlayerKey, 2>, int> &P_,
                const map<array<PlayerKey, 2>, int> &C_,
                const map<array<PlayerKey, 2>, int> &D_)
      : lose_move(lose_move_), player(player_), model(env), P(P_), C(C_),
        D(D_) {}
  void Run() {
    AddVariables();
    AddConstraints();
    SetObjective();
    FreeMemory();
    Solve();
  }
  void ReadMatrices() {
    cout << "Reading matrices" << endl;
    if (!fs::exists(lose_move ? LOSE_MOVE_P_FILENAME : DO_NOT_LOSE_MOVE_P_FILENAME)) {
      cout << "There is no P matrix in current dir" << endl;
      exit(1);
    }
    if (!fs::exists(lose_move ? LOSE_MOVE_C_FILENAME : DO_NOT_LOSE_MOVE_C_FILENAME)) {
      cout << "There is no C matrix in current dir" << endl;
      exit(1);
    }
    if (!fs::exists(lose_move ? LOSE_MOVE_D_FILENAME : DO_NOT_LOSE_MOVE_D_FILENAME)) {
      cout << "There is no D matrix in current dir" << endl;
      exit(1);
    }
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
      r_res[k] = v.get(GRB_DoubleAttr_X);
    }
    map<PlayerKey, double> s_res;
    for (auto &[k, v] : s) {
      s_res[k] = v.get(GRB_DoubleAttr_X);
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
      cout << k << ": " << v.get(GRB_DoubleAttr_X) << endl;
    }
    cout << "s:" << endl;
    for (auto &[k, v] : s) {
      cout << k << ": " << v.get(GRB_DoubleAttr_X) << endl;
    }
  }

private:
  void AddVariables() {
    cout << "Adding variables s and r" << endl;
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
    std::chrono::_V2::system_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    cout << "Adding constraints" << endl;
    int total_constraints = D.size() + P.size() + C.size();
    int added_constraints = 0;
    int last_added_constraints = 0;

    map<array<PlayerKey, 2>, int> DT;
    for (const auto &[k, v] : D) {
      PlayerKey j = get<0>(k);
      PlayerKey b = get<1>(k);
      DT.insert({{b, j}, v});
    }

    GRBLinExpr lhs, rhs;
    PlayerKey curr_b = get<0>(P.begin()->first);
    auto p_it = P.begin();
    auto dt_it = DT.begin();
    auto c_it = C.begin();
    while (p_it != P.end()) {
      PlayerKey b_p = get<0>(p_it->first);
      PlayerKey a = get<1>(p_it->first);
      int v_p = p_it->second;
      p_it = P.erase(p_it);
      if (b_p == curr_b) {
        lhs += v_p * s[a];
      } else {
        while (dt_it != DT.end()) {
          PlayerKey b_dt = get<0>(dt_it->first);
          PlayerKey j = get<1>(dt_it->first);
          int v_dt = dt_it->second;
          dt_it = DT.erase(dt_it);
          if (b_dt == curr_b) {
            rhs += v_dt * r[j];
          } else {
            model.addConstr(lhs, GRB_GREATER_EQUAL, rhs);
            added_constraints += lhs.size() + rhs.size();
            if (added_constraints - last_added_constraints > 10'000'000) {
              cout << "Added " << added_constraints << " out of "
                   << total_constraints << " ("
                   << (float)added_constraints / total_constraints * 100 << ")%"
                   << endl;
              last_added_constraints = added_constraints;
              auto curr = std::chrono::high_resolution_clock::now();
              std::chrono::duration<double> elapsed = curr - start;
              cout << "Elapsed time: " << elapsed.count() << " s" << endl;
            }
            lhs.clear();
            rhs.clear();
            assert(b_p == b_dt);
            curr_b = b_p;
            lhs += v_p * s[a];
            rhs += v_dt * r[j];
            break;
          }
        }
      }
    }
    while (dt_it != DT.end()) {
      PlayerKey b_dt = get<0>(dt_it->first);
      PlayerKey j = get<1>(dt_it->first);
      int v_dt = dt_it->second;
      dt_it = DT.erase(dt_it);
      assert(b_dt == curr_b);
      rhs += v_dt * r[j];
    }
    assert((lhs.size() == 0 && rhs.size() == 0) ||
           (lhs.size() > 0 && rhs.size() > 0));
    if (lhs.size() > 0 && rhs.size() > 0) {
      model.addConstr(lhs, GRB_GREATER_EQUAL, rhs);
      lhs.clear();
      rhs.clear();
    }
    PlayerKey curr_i = get<0>(C.begin()->first);
    int rhs_const = 1;
    while (c_it != C.end()) {
      PlayerKey i = get<0>(c_it->first);
      PlayerKey a = get<1>(c_it->first);
      int v_c = c_it->second;
      c_it = C.erase(c_it);
      if (curr_i == i) {
        lhs += v_c * s[a];
      } else {
        model.addConstr(lhs, GRB_EQUAL, rhs);
        lhs.clear();
        rhs = 0;
      }
    }
    if (lhs.size() > 0) {
      model.addConstr(lhs, GRB_EQUAL, rhs_const);
      lhs.clear();
    }
  }
  void SetObjective() {
    cout << "Setting objective" << endl;
    GRBLinExpr obj = r[0];
    model.setObjective(obj, GRB_MINIMIZE);
  }
  void FreeMemory() {
    P.clear();
    C.clear();
    D.clear();
    for (auto player : {PLAYER1, PLAYER2}) {
      player_states[player].clear();
      player_actionstates[player].clear();
    }
  }
  void Solve() {
    model.write(lose_move ? LOSE_MOVE_LP_PLAYER2_FILENAME
                          : DO_NOT_LOSE_MOVE_LP_PLAYER2_FILENAME);
    cout << "Solving" << endl;
    model.optimize();
  }
  bool lose_move;
  Player player;
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

  map<array<PlayerKey, 2>, int> P = {{{0, 0}, 0},  {{1, 2}, 1}, {{1, 3}, -1},
                                     {{2, 1}, -1}, {{2, 3}, 1}, {{3, 1}, 1},
                                     {{3, 2}, -1}};
  map<array<PlayerKey, 2>, int> C = {
      {{0, 0}, -1}, {{0, 1}, 1}, {{0, 2}, 1}, {{0, 3}, 1}};
  map<array<PlayerKey, 2>, int> D = {
      {{0, 0}, -1}, {{0, 1}, 1}, {{0, 2}, 1}, {{0, 3}, 1}};

  std::ofstream ofs;
  ofs.open(LOG_FILENAME, std::ofstream::out | std::ofstream::trunc);
  ofs.close();

  GRBEnv env(true);
  env.set("LogFile", LOG_FILENAME);
  env.start();
  LinearProgram linear_program(/*lose_move=*/true, /*player=*/PLAYER2, env, P,
                               C, D);
  linear_program.Run();
  linear_program.PrintSolutionToScreen();
}

void run_lose_move_player2() {
  std::ofstream ofs;
  ofs.open(LOG_FILENAME, std::ofstream::out | std::ofstream::trunc);
  ofs.close();

  GRBEnv env(true);
  env.set("LogFile", LOG_FILENAME);
  env.start();
  LinearProgram linear_program(/*lose_move=*/false, /*player=*/PLAYER2, env);
  linear_program.ReadMatrices();
  linear_program.Run();
  linear_program.PrintSolutionToFile();
}

int main() {
  try {
    // test_poker();
    run_lose_move_player2();
  } catch (GRBException e) {
    cout << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  }
}