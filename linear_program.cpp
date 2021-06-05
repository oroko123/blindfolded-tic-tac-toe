#include "bits/stdc++.h"
#include "constants.h"
#include "gurobi_c++.h"
#include "serializer.h"

using namespace std;

class LinearProgram {
public:
  LinearProgram(bool lose_move_)
      : lose_move(lose_move_), env(true), model(env) {
    env.set("LogFile",
            lose_move ? LOSE_MOVE_LOG_FILENAME : DO_NOT_LOSE_MOVE_LOG_FILENAME);
    env.start();
  }
  void Run() {
    ReadMatrices();
    AddZeroElements();
    AddVariables();
    SetObjective();
    AddConstraints();
    Solve();
    PrintSolution();
  }
private:
  void ReadMatrices() {
    try {
      P = Serializer::read_from_file(lose_move ? LOSE_MOVE_P_FILENAME
                                               : DO_NOT_LOSE_MOVE_P_FILENAME);
      C = Serializer::read_from_file(lose_move ? LOSE_MOVE_C_FILENAME
                                               : DO_NOT_LOSE_MOVE_C_FILENAME);
      D = Serializer::read_from_file(lose_move ? LOSE_MOVE_D_FILENAME
                                               : DO_NOT_LOSE_MOVE_D_FILENAME);
    } catch (const exception &e) {
      cout << e.what();
    }
  }
  void AddZeroElements() {
    // add elements to c, d and p
  }
  void AddVariables() {
    // GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
    // GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
    // GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");
  }
  void AddConstraints() {
    // Add constraint: x + 2 y + 3 z <= 4
    // model.addConstr(x + 2 * y + 3 * z <= 4, "c0");

    // // Add constraint: x + y >= 1
    // model.addConstr(x + y >= 1, "c1");
  }
  void Solve() { model.optimize(); }
  void PrintSolution() {

    // cout << x.get(GRB_StringAttr_VarName) << " " << x.get(GRB_DoubleAttr_X)
    //      << endl;
    // cout << y.get(GRB_StringAttr_VarName) << " " << y.get(GRB_DoubleAttr_X)
    //      << endl;
    // cout << z.get(GRB_StringAttr_VarName) << " " << z.get(GRB_DoubleAttr_X)
    //      << endl;

    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
  }
  void SetObjective() {
    // Set objective: maximize x + y + 2 z
    // model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
  }
  bool lose_move;
  GRBEnv env;
  GRBModel model;
  vector<GRBVar> r;
  vector<GRBVar> s_1;
  vector<GRBVar> s_2;
  map<array<PlayerKey, 2>, int> P;
  map<array<PlayerKey, 2>, int> C;
  map<array<PlayerKey, 2>, int> D;

  // map<PlayerKey, float> s;
  // map<PlayerKey, float> r;
};

int main() {
  LinearProgram linear_program(/*lose_move=*/true);
  linear_program.Run();
  return 0;
}