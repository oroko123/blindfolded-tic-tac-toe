#include "board.h"
#include "constants.h"
#include "state.h"
#include <bits/stdc++.h>

using namespace std;

typedef struct CountingResult {
  CountingResult(long long full_count_, long long non_izometric_count_)
      : full_count(full_count_), non_izometric_count(non_izometric_count_) {}
  long long full_count;
  long long non_izometric_count;
} CountingResultT;

class Counter {
public:
  Counter() : progress_bar(0, 0), last_printed(0, 0) {
    start = std::chrono::high_resolution_clock::now();
  }
  long long countMatrixSize(const GameState &state,
                            const vector<long long> &players_izometry_coefs) {
    Result res = state.getResult();
    if (res == INVALID) {
      state.printBoard();
      cerr << "BOARD IS INVALID" << endl;
      exit(0);
    }
    long long count = 1;
    if (res != ONGOING) {
      long long win_coef = res == PLAYER1WIN
                               ? PROBABILITY_FACTOR
                               : (res == PLAYER2WIN ? -PROBABILITY_FACTOR : 0);

      array<PlayerKey, 2> history_id =
          state.calculateRepresentativeHistoryPair();
      assert(win_coef % (players_izometry_coefs[0] * players_izometry_coefs[1]) == 0);
      matrix[history_id] +=
          win_coef / (players_izometry_coefs[0] * players_izometry_coefs[1]);
      if (matrix[history_id] == 0) {
        matrix.erase(history_id);
      }

      return count;
    } else {
      vector<int> moves = state.getPossibleMoves();
      Player player_turn = state.getPlayerTurn();
      map<PlayerKey, vector<int>> izometries;
      map<int, long long> izometry_coefs;
      vector<pair<GameState, int>> new_states_with_moves;
      for (int move : moves) {
        new_states_with_moves.push_back({state.performMove(move), move});
      }
      for (auto &new_state_and_move : new_states_with_moves) {
        array<PlayerKey, 2> history_id =
            new_state_and_move.first.calculateRepresentativeHistoryPair();
        izometries[history_id[player_turn]].push_back(
            new_state_and_move.second);
      }
      for (auto e : izometries) {
        for (auto move : e.second) {
          izometry_coefs[move] = e.second.size();
        }
      }
      for (auto &new_state_and_move : new_states_with_moves) {
        vector<long long> new_players_izometry_coefs(players_izometry_coefs);
        new_players_izometry_coefs[player_turn] *=
            izometry_coefs[new_state_and_move.second];
        count += countMatrixSize(new_state_and_move.first,
                                 new_players_izometry_coefs);
      }
      progress_bar.full_count = max(progress_bar.full_count, count);
      progress_bar.non_izometric_count = matrix.size();
      if (progress_bar.full_count > 2 * last_printed.full_count) {
        last_printed = progress_bar;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        cout << "Elapsed time: " << elapsed.count() << " s\n";
        cout << "Full count: " << progress_bar.full_count << endl;
        printSolution();
      }
      return count;
    }
  }
  void printSolution() {
    cout << "Matrix size: " << matrix.size() << endl;
    unordered_map<long long, long long> unique_results;
    for (auto p : matrix) {
      unique_results[p.second]++;
    }
    for (auto e : unique_results) {
      cout << "Element: " << e.first << ", count: " << e.second << endl;
    }
  }

private:
  map<array<PlayerKey, 2>, long long> matrix;
  CountingResultT progress_bar;
  CountingResultT last_printed;
  std::chrono::_V2::system_clock::time_point start;
};

void test_do_not_lose_move() {
  DoNotLoseMoveWhenConflictGameState initialGameState;
  Counter counter;
  counter.countMatrixSize(initialGameState, {1LL, 1LL});
  counter.printSolution();
}

void test_lose_move() {
  LoseMoveWhenConflictGameState initialGameState;
  Counter counter;
  counter.countMatrixSize(initialGameState, {1LL, 1LL});
  counter.printSolution();
}

int main() {
  test_do_not_lose_move();
  test_lose_move();
  return 0;
}