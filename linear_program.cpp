#include "bits/stdc++.h"
#include "constants2.h"
#include "gurobi_c++.h"
#include "serializer.h"

#include <experimental/filesystem>

using namespace std;

namespace fs = std::experimental::filesystem;

class LinearProgram {
public:
  LinearProgram(bool lose_move_, Player player_, const GRBEnv &env)
      : lose_move(lose_move_), player(player_), model(env) {
    model.set(GRB_IntParam_Method, GRB_METHOD_DUAL);
  }
  void Run() {
    AddStartingElements();
    AssertMatrices();
    AddVariables();
    AddConstraints();
    SetObjective();
    FreeMemory();
    Solve();
  }
  void ReadMatrices() {
    cout << "Reading matrices" << endl;
    if (player == PLAYER1) {
      std::string pt_filename = GetMatrixFilename("PT", lose_move);
      std::string ct_filename = GetMatrixFilename("CT", lose_move);
      std::string d_filename = GetMatrixFilename("D", lose_move);
      if (!fs::exists(pt_filename)) {
        cout << "There is no P matrix in current dir" << endl;
        exit(1);
      }
      if (!fs::exists(ct_filename)) {
        cout << "There is no CT matrix in current dir" << endl;
        exit(1);
      }
      if (!fs::exists(d_filename)) {
        cout << "There is no DT matrix in current dir" << endl;
        exit(1);
      }
      try {
        PT = Serializer::read_from_file<array<PlayerKey, 2>, int>(pt_filename);
        CT = Serializer::read_from_file<array<PlayerKey, 2>, int>(ct_filename);
        D = Serializer::read_from_file<array<PlayerKey, 2>, int>(d_filename);
      } catch (const exception &e) {
        cout << e.what();
      }
    } else { // player == PLAYER2
      std::string p_filename = GetMatrixFilename("P", lose_move);
      std::string c_filename = GetMatrixFilename("C", lose_move);
      std::string dt_filename = GetMatrixFilename("DT", lose_move);
      if (!fs::exists(p_filename)) {
        cout << "There is no P matrix in current dir" << endl;
        exit(1);
      }
      if (!fs::exists(c_filename)) {
        cout << "There is no C matrix in current dir" << endl;
        exit(1);
      }
      if (!fs::exists(dt_filename)) {
        cout << "There is no DT matrix in current dir" << endl;
        exit(1);
      }
      try {
        P = Serializer::read_from_file<array<PlayerKey, 2>, int>(p_filename);
        C = Serializer::read_from_file<array<PlayerKey, 2>, int>(c_filename);
        DT = Serializer::read_from_file<array<PlayerKey, 2>, int>(dt_filename);
      } catch (const exception &e) {
        cout << e.what();
      }
    }
  }
  void AssertMatrices() {
    if (player == PLAYER1) {
      assert(!PT.empty());
      assert(!CT.empty());
      assert(!D.empty());
      assert(P.empty());
      assert(C.empty());
      assert(DT.empty());
    }
    if (player == PLAYER2) {
      assert(PT.empty());
      assert(CT.empty());
      assert(D.empty());
      assert(!P.empty());
      assert(!C.empty());
      assert(!DT.empty());
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
    Serializer::write_to_file(GetOutputFilename("R", player, lose_move), r_res,
                              false);
    Serializer::write_to_file(GetOutputFilename("S", player, lose_move), s_res,
                              false);
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
  void AddStartingElements() {
    switch (player) {
    case PLAYER1:
      CT.insert({{0, 0}, 1});
      D.insert({{0, 0}, 1});
      break;
    case PLAYER2:
      C.insert({{0, 0}, 1});
      DT.insert({{0, 0}, 1});
      break;
    default:
      assert(false);
      break;
    }
  }
  void AddVariables() {
    cout << "Adding variables s and r" << endl;
    switch (player) {
    case PLAYER1:
      for (const auto &[k, v] : CT) {
        (void)v;
        player_states[PLAYER2].insert(get<1>(k));
        player_actionstates[PLAYER2].insert(get<0>(k));
      }
      for (const auto &[k, v] : D) {
        (void)v;
        player_states[PLAYER1].insert(get<0>(k));
        player_actionstates[PLAYER1].insert(get<1>(k));
      }
      for (auto b : player_actionstates[PLAYER1]) {
        s[b] = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS);
      }
      for (auto i : player_states[PLAYER2]) {
        r[i] = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
      }
      break;
    case PLAYER2:
      for (const auto &[k, v] : C) {
        (void)v;
        player_states[PLAYER2].insert(get<0>(k));
        player_actionstates[PLAYER2].insert(get<1>(k));
      }
      for (const auto &[k, v] : DT) {
        (void)v;
        player_states[PLAYER1].insert(get<1>(k));
        player_actionstates[PLAYER1].insert(get<0>(k));
      }
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

  // naming as for PLAYER2.
  void AddConstraints() {
    std::chrono::_V2::system_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    cout << "Adding constraints" << endl;
    int total_constraints = D.size() + P.size() + C.size();
    int added_constraints = 0;
    int last_added_constraints = 0;

    if (player == PLAYER2) {
      GRBLinExpr lhs, rhs;
      auto p_it = P.begin();
      auto dt_it = DT.begin();
      for (auto b : player_actionstates[getOppositePlayer(player)]) {
        while (p_it != P.end()) {
          PlayerKey b_p = get<0>(p_it->first);
          PlayerKey a = get<1>(p_it->first);
          int v_p = p_it->second;
          if (b_p == b) {
            lhs += v_p * s[a];
            p_it = P.erase(p_it);
          } else {
            break;
          }
        }
        while (dt_it != DT.end()) {
          PlayerKey b_dt = get<0>(dt_it->first);
          PlayerKey j = get<1>(dt_it->first);
          int v_dt = dt_it->second;
          if (b_dt == b) {
            rhs += v_dt * r[j];
            dt_it = DT.erase(dt_it);
          } else {
            break;
          }
        }
        model.addConstr(lhs, GRB_LESS_EQUAL, rhs);
        added_constraints += lhs.size() + rhs.size();
        lhs.clear();
        rhs.clear();
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
      }
      assert(P.empty());
      assert(DT.empty());
      {
        GRBLinExpr lhs;
        int rhs_const = 1;
        auto c_it = C.begin();
        int curr_i = get<0>(c_it->first);
        while (c_it != C.end()) {
          PlayerKey i = get<0>(c_it->first);
          PlayerKey a = get<1>(c_it->first);
          int v_c = c_it->second;
          if (curr_i == i) {
            lhs += v_c * s[a];
            c_it = C.erase(c_it);
          } else {
            model.addConstr(lhs, GRB_EQUAL, rhs_const);
            lhs.clear();
            rhs_const = 0;
            curr_i = i;
          }
        }
        if (lhs.size() > 0) {
          model.addConstr(lhs, GRB_EQUAL, rhs_const);
          lhs.clear();
        }
      }
    }

    if (player == PLAYER1) {
      GRBLinExpr lhs, rhs;
      auto pt_it = PT.begin();
      auto ct_it = CT.begin();
      for (auto a : player_actionstates[getOppositePlayer(player)]) {
        while (pt_it != PT.end()) {
          PlayerKey a_p = get<0>(pt_it->first);
          PlayerKey b = get<1>(pt_it->first);
          int v_p = pt_it->second;
          if (a_p == a) {
            lhs += -v_p * s[b];
            pt_it = PT.erase(pt_it);
          } else {
            break;
          }
        }
        while (ct_it != CT.end()) {
          PlayerKey a_ct = get<0>(ct_it->first);
          PlayerKey i = get<1>(ct_it->first);
          int v_dt = ct_it->second;
          if (a_ct == a) {
            rhs += v_dt * r[i];
            ct_it = CT.erase(ct_it);
          } else {
            break;
          }
        }
        model.addConstr(lhs, GRB_LESS_EQUAL, rhs);
        added_constraints += lhs.size() + rhs.size();
        lhs.clear();
        rhs.clear();
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
      }
      assert(PT.empty());
      assert(CT.empty());
      {
        GRBLinExpr lhs;
        int rhs_const = 1;
        auto d_it = D.begin();
        int curr_j = get<0>(d_it->first);
        while (d_it != D.end()) {
          PlayerKey j = get<0>(d_it->first);
          PlayerKey b = get<1>(d_it->first);
          int v_d = d_it->second;
          if (curr_j == j) {
            lhs += v_d * s[b];
            d_it = D.erase(d_it);
          } else {
            model.addConstr(lhs, GRB_EQUAL, rhs_const);
            lhs.clear();
            rhs_const = 0;
            curr_j = j;
          }
        }
        if (lhs.size() > 0) {
          model.addConstr(lhs, GRB_EQUAL, rhs_const);
          lhs.clear();
        }
      }
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
    model.write(GetOutputFilename("LINEAR_PROGRAM", player, lose_move));
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
  map<array<PlayerKey, 2>, int> PT;
  map<array<PlayerKey, 2>, int> CT;
  map<array<PlayerKey, 2>, int> DT;
};

void run(bool lose_move, Player player) {
  std::ofstream ofs;

  std::string log_filename = GetLogFilename(lose_move, player);
  ofs.open(log_filename, std::ofstream::out | std::ofstream::trunc);
  ofs.close();

  GRBEnv env(true);
  env.set("LogFile", log_filename);
  env.start();
  LinearProgram linear_program(lose_move, player, env);
  linear_program.ReadMatrices();
  linear_program.Run();
  linear_program.PrintSolutionToFile();
  linear_program.PrintSolutionToScreen();
}

int main() {
  try {
    run(/*lose_move=*/false, /*player=*/PLAYER1);
    run(/*lose_move=*/false, /*player=*/PLAYER2);
    run(/*lose_move=*/true, /*player=*/PLAYER1);
    run(/*lose_move=*/true, /*player=*/PLAYER2);
  } catch (GRBException e) {
    cout << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  }
}