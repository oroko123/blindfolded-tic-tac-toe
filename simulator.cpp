#include "agent.h"
#include "board.h"
#include "constants.h"
#include "state.h"
#include <bits/stdc++.h>
using namespace std;

const bool DEBUG = false;

class Simulator {
public:
  Simulator(bool lose_move)
      : agents({new Player1NotLosingAgent3(),
                new RandomAgent()}),
        game_state(lose_move) {}

  Result run() {
    while (game_state.getResult() == ONGOING) {
      Player player_turn = game_state.getPlayerTurn();
      int move = agents[player_turn]->tryMakeMove();
      game_state = game_state.performMove(move);
      agents[player_turn]->fetchStatus(game_state.wasLastAccepted());
      if (DEBUG) {
        game_state.printBoard();
      }
    }
    return game_state.getResult();
  }

private:
  vector<Agent *> agents;
  GameState game_state;
};

int main() {
  int tie_res = 0;
  int agent1_res = 0;
  int agent2_res = 0;
  for (int i = 0; i < SIMULATIONS_NUM; i++) {
    Simulator simulator(/*lose_move=*/false);
    if (i % 100 == 0) {
      cerr << "SIMULATION NO. " << i << endl;
    }
    Result res = simulator.run();
    if (res == TIE) {
      tie_res++;
    } else if (res == PLAYER1WIN) {
      agent1_res ++;
    } else { // res == PLAYER2WIN
      agent2_res ++;
    }
  }
  cout << "Player 1 wins: " << agent1_res << endl;
  cout << "Ties: " << tie_res << endl;
  cout << "Player 2 wins: " << agent2_res << endl;
  cout << "Estimated game value: " << double(agent1_res - agent2_res) / SIMULATIONS_NUM << endl;
  return 0;
}