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
  CountingResultT countSymetricStatesInSubtree(const GameState &state) {
    CountingResultT counting_res(1, 1);
    for (const auto symetric_state : state.getSymetricHistoriesKeys()) {
      if (states_in_subtree.find(symetric_state) != states_in_subtree.end()) {
        cout << "Found symetry between states: " << endl;
        state.printBoard();
        return {states_in_subtree.at(symetric_state), 0};
      }
    }
    Result res = state.getResult();
    if (res == INVALID) {
      state.printBoard();
      cerr << "BOARD IS INVALID" << endl;
      exit(0);
    }
    if (res != ONGOING) {
      return counting_res;
    } else {
      for (int move : state.getPossibleMoves()) {
        CountingResultT subres =
            countSymetricStatesInSubtree(state.performMove(move));
        counting_res.full_count += subres.full_count;
        counting_res.non_izometric_count += subres.non_izometric_count;
        progress_bar.full_count += subres.full_count;
        progress_bar.non_izometric_count += subres.non_izometric_count;
      }
      states_in_subtree[state.getHistoryKey()] = counting_res.full_count;

      if (progress_bar.full_count > 2 * last_printed.full_count) {
        last_printed = progress_bar;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        cout << "Elapsed time: " << elapsed.count() << " s\n";
        cout << "Full count: " << progress_bar.full_count
             << " Izometric: " << progress_bar.non_izometric_count << endl;
      }

      progress_bar.full_count -= counting_res.full_count;
      progress_bar.non_izometric_count -= counting_res.non_izometric_count;
    }
    return counting_res;
  }

  long long countStatesInSubtree(const GameState &state) {
    Result res = state.getResult();
    if (res == INVALID) {
      state.printBoard();
      cerr << "BOARD IS INVALID" << endl;
      exit(0);
    }
    long long count = 1;
    if (res != ONGOING) {
      return count;
    } else {
      vector<int> possibleMoves = state.getPossibleMoves();
      for (int move : possibleMoves) {
        count += countStatesInSubtree(state.performMove(move));
      }
      return count;
    }
  }

private:
  map<HistoryKey, long long> states_in_subtree;
  CountingResultT progress_bar;
  CountingResultT last_printed;
  std::chrono::_V2::system_clock::time_point start;
};

class LoseMoveWhenConflictGameState : public GameState {
public:
  LoseMoveWhenConflictGameState() : GameState(/*lose_move_=*/true) {}
  LoseMoveWhenConflictGameState(const Board &judge_board_,
                                const Board &player1_board,
                                const Board &player2_board, Player player_turn_)
      : GameState(judge_board_, player1_board, player2_board, player_turn_,
                  /*lose_move_=*/true) {}
};

class DoNotLoseMoveWhenConflictGameState : public GameState {
public:
  DoNotLoseMoveWhenConflictGameState() : GameState(/*lose_move_=*/false) {}
  DoNotLoseMoveWhenConflictGameState(const Board &judge_board_,
                                     const Board &player1_board,
                                     const Board &player2_board,
                                     Player player_turn_)
      : GameState(judge_board_, player1_board, player2_board, player_turn_,
                  /*lose_move_=*/false) {}
};

void test_1() {
  Board board(
      {PLAYER1, PLAYER2, PLAYER2, NONE, PLAYER1, NONE, NONE, NONE, PLAYER1});
  LoseMoveWhenConflictGameState state(board, board, board, PLAYER1);
  Counter counter;
  assert(counter.countStatesInSubtree(state) == 1);
  assert(counter.countSymetricStatesInSubtree(state).full_count == 1);
}

void test_2() {
  Board board({PLAYER2, PLAYER2, PLAYER1, PLAYER1, PLAYER2, NONE, PLAYER2,
               PLAYER1, PLAYER1});
  Board player1board({PLAYER2, NONE, PLAYER1, PLAYER1, PLAYER2, NONE, PLAYER2,
                      PLAYER1, PLAYER1});
  LoseMoveWhenConflictGameState state(board, player1board, board, PLAYER1);
  Counter counter;
  int count = counter.countStatesInSubtree(state);
  if (count != 4) {
    cout << "Test 2 failed." << endl;
    cout << "Is: " << count << endl;
    cout << "Should be : 4" << endl;
    state.printBoard();
    exit(0);
  }
  count = counter.countSymetricStatesInSubtree(state).full_count;
  if (count != 4) {
    cout << "Test 2  with symetry failed." << endl;
    cout << "Is: " << count << endl;
    cout << "Should be : 4" << endl;
    state.printBoard();
    exit(0);
  }
}

void test_3() {
  Board board({PLAYER1, PLAYER2, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  Board player1board({PLAYER1, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  Board player2board({NONE, PLAYER2, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  LoseMoveWhenConflictGameState state(board, player1board, player2board,
                                      PLAYER1);
  Counter counter;
  auto start = std::chrono::high_resolution_clock::now();
  cout << "Test 3: " << counter.countStatesInSubtree(state) << endl;
  auto mid = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = mid - start;
  cout << "Elapsed time: " << elapsed.count() << " s\n";
  cout << "Test 3 with symetry: "
       << counter.countSymetricStatesInSubtree(state).full_count << endl;
  auto end = std::chrono::high_resolution_clock::now();
  elapsed = end - mid;
  cout << "Elapsed time: " << elapsed.count() << " s\n";
}

void test_4() {
  Board board({PLAYER1, PLAYER2, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  Board player1board({PLAYER1, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  Board player2board({NONE, PLAYER2, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  DoNotLoseMoveWhenConflictGameState state(board, player1board, player2board,
                                           PLAYER1);
  Counter counter;
  auto start = std::chrono::high_resolution_clock::now();
  cout << "Test 4: " << counter.countStatesInSubtree(state) << endl;
  auto mid = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = mid - start;
  cout << "Elapsed time: " << elapsed.count() << " s\n";
  cout << "Test 4 with symetry: "
       << counter.countSymetricStatesInSubtree(state).full_count << endl;
  auto end = std::chrono::high_resolution_clock::now();
  elapsed = end - mid;
  cout << "Elapsed time: " << elapsed.count() << " s\n";
}

void test_5() {
  Board board({PLAYER1, NONE, NONE, NONE, PLAYER2, NONE, NONE, NONE, PLAYER1});
  Board player1board(
      {PLAYER1, NONE, NONE, NONE, PLAYER2, NONE, NONE, NONE, PLAYER1});
  Board player2board(
      {PLAYER1, NONE, NONE, NONE, PLAYER2, NONE, NONE, NONE, PLAYER1});
  DoNotLoseMoveWhenConflictGameState state(board, player1board, player2board,
                                           PLAYER2);
  Counter counter;
  auto start = std::chrono::high_resolution_clock::now();
  cout << "Test 5 with symetry: "
       << counter.countSymetricStatesInSubtree(state).full_count << endl;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cout << "Elapsed time: " << elapsed.count() << " s\n";
}

void test_do_not_lose_move() {
  DoNotLoseMoveWhenConflictGameState initialGameState;
  Counter counter;
  auto res = counter.countSymetricStatesInSubtree(initialGameState);
  cout << "Test do not lose move: " << endl;
  cout << "Full count: " << res.full_count << endl;
  cout << "Non izometric states: " << res.non_izometric_count << endl;
}

void test_lose_move() {
  LoseMoveWhenConflictGameState initialGameState;
  Counter counter;
  auto res = counter.countSymetricStatesInSubtree(initialGameState);
  cout << "Test lose move: " << endl;
  cout << "Full count: " << res.full_count << endl;
  cout << "Non izometric states: " << res.non_izometric_count << endl;
}

int main() {
  // test_1();
  // test_2();
  // test_3();
  // test_4();
  // test_5();
  test_do_not_lose_move();
  test_lose_move();
  return 0;
}