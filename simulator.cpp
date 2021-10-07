#include "agent.h"
#include "board.h"
#include "constants.h"
#include "state.h"
#include <bits/stdc++.h>
using namespace std;

class Simulator {
public:
  Simulator(Agent *agent1_, Agent *agent2_)
      : agents({agent1_, agent2_}), game_state(/*lose_move_=*/false) {}

  Result run() {
    while (game_state.getResult() == ONGOING) {
      Player player_turn = game_state.getPlayerTurn();
      int move = agents[player_turn]->tryMakeMove();
      game_state = game_state.performMove(move);
      agents[player_turn]->fetchStatus(game_state.wasLastAccepted());
    }
    return game_state.getResult();
  }
  void reset() { game_state = GameState(/*lose_move_=*/false); }

private:
  vector<Agent *> agents;
  GameState game_state;
};

int main() {
  Agent *agent1 = new RandomAgent();
  Agent *agent2 = new FirstAgent();
  Simulator simulator(agent1, agent2);
  int agent1_res = 0;
  int agent2_res = 0;
  for (int i = 0; i < SIMULATIONS_NUM; i++) {
    Result res = simulator.run();
    if (res == TIE) {
      agent1_res++;
      agent2_res++;
    } else if (res == PLAYER1WIN) {
      agent1_res += 2;
    } else { // res == PLAYER2WIN
      agent2_res += 2;
    }
    simulator.reset();
  }
  cout << "Moves first: " << agent1->getStrategyName() << endl;
  cout << "Score: " << agent1_res << endl;
  cout << "Moves second: " << agent2->getStrategyName() << endl;
  cout << "Score: " << agent2_res << endl;
  return 0;
}