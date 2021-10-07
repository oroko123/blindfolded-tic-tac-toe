#include "board2.h"
#include "constants2.h"
#include "state2.h"
#include "serializer.h"
#include <bits/stdc++.h>

using namespace std;

typedef struct CountingResult
{
  CountingResult(long long full_count_, long long non_izometric_count_)
      : full_count(full_count_), non_izometric_count(non_izometric_count_) {}
  long long full_count;
  long long non_izometric_count;
} CountingResultT;

class Counter
{
public:
  Counter() : progress_bar(0, 0), last_printed(0, 0)
  {
    start = std::chrono::high_resolution_clock::now();
  }
  long long countMatrixSize(const GameState2 &state,
                            const vector<int> &players_izometry_coefs)
  {
    Result res = state.getResult();
    if (res == INVALID)
    {
      state.printBoard();
      cerr << "BOARD IS INVALID" << endl;
      exit(0);
    }
    long long count = 1;
    if (res != ONGOING)
    {
      int win_coef = res == PLAYER1WIN
                         ? PROBABILITY_FACTOR
                         : (res == PLAYER2WIN ? -PROBABILITY_FACTOR : 0);
      if (win_coef == 0)
      {
        return count;
      }
      array<PlayerKey, 2> history_id =
          state.calculateRepresentativeActionStateKeyPair();
      assert(win_coef %
                 (players_izometry_coefs[0] * players_izometry_coefs[1]) ==
             0);
      // we construct action_state pair
      matrix_p[history_id] +=
          win_coef / (players_izometry_coefs[0] * players_izometry_coefs[1]);
      if (matrix_p[history_id] == 0)
      {
        matrix_p.erase(history_id);
      }

      return count;
    }
    else
    {
      vector<int> moves = state.getPossibleMoves();
      Player player_turn = state.getPlayerTurn();
      PlayerKey player_state_key =
          state.calculateRepresentativeStateKeyPair()[player_turn];
      map<PlayerKey, vector<int>> izometries;
      map<int, int> izometry_coefs;
      vector<tuple<GameState2, int, PlayerKey>> new_states_with_moves_and_keys;
      for (int move : moves)
      {
        auto newState = state.performMove(move);
        new_states_with_moves_and_keys.push_back(
            {newState, move,
             newState
                 .calculateRepresentativeActionStateKeyPair()[player_turn]});
      }
      for (auto &[state, move, key] : new_states_with_moves_and_keys)
      {
        (void)state;
        izometries[key].push_back(move);
      }
      int sum = 0;
      for (auto &[key, value] : izometries)
      {
        for (auto move : value)
        {
          izometry_coefs[move] = value.size();
          sum += value.size();
        }
        // we put positive value for the next action-move;
        auto current_value =
            player_matrix[player_turn][{player_state_key, key}];
        // state.printBoard();
        // cerr << playerToSign(player_turn) << " " << current_value << " " << key
        //      << endl;
        if (current_value != 0 && current_value != (int)value.size()) {
          for (auto& [k ,v] :izometry_coefs) {
            cerr << k << " " << v << endl;
          }
          cerr << key << endl;
          for (auto& x : value) {
            cerr << x << " ";
          }
        }
        assert(current_value == 0 || current_value == (int)value.size());
        player_matrix[player_turn][{player_state_key, key}] = value.size();
      }
      // we put negative value for the previous action-move;
      auto current_value =
          player_matrix[player_turn][{player_state_key, player_state_key / 2}];
      assert(current_value == 0 || current_value == -1);
      player_matrix[player_turn][{player_state_key, player_state_key / 2}] =
          -1;
      for (auto &[state, move, key] : new_states_with_moves_and_keys)
      {
        (void)key;
        vector<int> new_players_izometry_coefs(players_izometry_coefs);
        new_players_izometry_coefs[player_turn] *= izometry_coefs[move];
        count += countMatrixSize(state, new_players_izometry_coefs);
      }
      progress_bar.full_count = max(progress_bar.full_count, count);
      progress_bar.non_izometric_count = matrix_p.size();
      if (progress_bar.full_count > 2 * last_printed.full_count)
      {
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
  void printSolution()
  {
    cout << "Matrix size: " << matrix_p.size() << endl;
    unordered_map<int, int> unique_results;
    for (auto p : matrix_p)
    {
      unique_results[p.second]++;
    }
    for (auto e : unique_results)
    {
      cout << "Element: " << e.first << ", count: " << e.second << endl;
    }
  }
  const map<array<PlayerKey, 2>, int> &getP() { return matrix_p; }
  const map<array<PlayerKey, 2>, int> &getC() { return player_matrix[PLAYER2]; }
  const map<array<PlayerKey, 2>, int> &getD() { return player_matrix[PLAYER1]; }

private:
  map<array<PlayerKey, 2>, int>
      matrix_p; // <player1_action_key, player2_action_key>
  map<array<PlayerKey, 2>, int>
      player_matrix[2]; // <state_key, action_key
  CountingResultT progress_bar;
  CountingResultT last_printed;
  std::chrono::_V2::system_clock::time_point start;
};

void test_do_not_lose_move()
{
  DoNotLoseMoveWhenConflictGameState2 initialGameState;
  Counter counter;
  counter.countMatrixSize(initialGameState, {1LL, 1LL});
  counter.printSolution();
  Serializer::write_to_file(GetMatrixFilename("P", /*lose_move=*/false), counter.getP(), false);
  Serializer::write_to_file(GetMatrixFilename("C", /*lose_move=*/false), counter.getC(), false);
  Serializer::write_to_file(GetMatrixFilename("D", /*lose_move=*/false), counter.getD(), false);
}

void test_lose_move()
{
  LoseMoveWhenConflictGameState2 initialGameState;
  Counter counter;
  counter.countMatrixSize(initialGameState, {1LL, 1LL});
  counter.printSolution();
  Serializer::write_to_file(GetMatrixFilename("P", /*lose_move=*/true), counter.getP(), false);
  Serializer::write_to_file(GetMatrixFilename("C", /*lose_move=*/true), counter.getC(), false);
  Serializer::write_to_file(GetMatrixFilename("D", /*lose_move=*/true), counter.getD(), false);
}

int main()
{
  test_do_not_lose_move();
  test_lose_move();
  return 0;
}