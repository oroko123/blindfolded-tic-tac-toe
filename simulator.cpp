#include "agent.h"
#include "board2.h"
#include "constants2.h"
#include "state2.h"
#include <bits/stdc++.h>
using namespace std;

class Simulator {
public:
  Simulator(bool lose_move)
      : agents({new MatrixAgent(lose_move, PLAYER1),
                new MatrixAgent(lose_move, PLAYER2)}),
        game_state(lose_move) {}

  Result run() {
    while (game_state.getResult() == ONGOING) {
      Player player_turn = game_state.getPlayerTurn();
      int move = agents[player_turn]->tryMakeMove();
      game_state = game_state.performMove(move);
      agents[player_turn]->fetchStatus(game_state.wasLastAccepted());
    }
    return game_state.getResult();
  }
  vector<Agent *> getAgents() { return agents; }

private:
  vector<Agent *> agents;
  GameState2 game_state;
};

int main() {
  int agent1_res = 0;
  int agent2_res = 0;
  for (int i = 0; i < SIMULATIONS_NUM; i++) {
    Simulator simulator(/*lose_move=*/false);
    if (i % 100 == 0) {
      cerr << "SIMULATION NO. " << i << endl;
    }
    Result res = simulator.run();
    if (res == TIE) {
      agent1_res++;
      agent2_res++;
    } else if (res == PLAYER1WIN) {
      agent1_res += 2;
    } else { // res == PLAYER2WIN
      agent2_res += 2;
    }
  }
  cout << "Score 1: " << agent1_res << endl;
  cout << "Score 2: " << agent2_res << endl;
  return 0;
}