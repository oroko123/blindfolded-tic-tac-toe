#include <bits/stdc++.h>
using namespace std;

enum Result { TIE, PLAYER1WIN, PLAYER2WIN, ONGOING, INVALID };
enum Player { PLAYER1, PLAYER2, NONE };

const int FIELDS_NUM = 9;

constexpr Player getOppositePlayer(Player player) {
  if (player == PLAYER1) {
    return PLAYER2;
  } else if (player == PLAYER2) {
    return PLAYER1;
  } else {
    return NONE;
  }
}

class Board {
public:
  Board() : fields(FIELDS_NUM, NONE) {}
  Board(const vector<Player> fields_) : fields(fields_) {
    assert(fields.size() == FIELDS_NUM);
  }
  Result getResult() const {
    vector<int> playersWinningSets = {0, 0, 0};
    for (auto player : {PLAYER1, PLAYER2}) {
      for (const auto &winning_set : winning_sets) {
        bool not_winning = false;
        for (int field : winning_set) {
          if (fields[field] != player) {
            not_winning = true;
            break;
          }
        }
        if (!not_winning) {
          playersWinningSets[player]++;
        }
      }
    }
    if (playersWinningSets[PLAYER1] > 0 && playersWinningSets[PLAYER2] > 0) {
      return INVALID;
    } else if (playersWinningSets[PLAYER1] == 1) {
      return PLAYER1WIN;
    } else if (playersWinningSets[PLAYER2] == 1) {
      return PLAYER2WIN;
    } else {
      int fields_taken = 0;
      for (int field : fields) {
        if (field != NONE) {
          fields_taken++;
        }
      }
      return (fields_taken == FIELDS_NUM) ? TIE : ONGOING;
    }
  }
  Player getField(int field) const { return fields[field]; }
  void setField(int field, Player player) { fields[field] = player; }
  vector<int> getFreeFields() const {
    vector<int> ret;
    for (int i = 0; i < fields.size(); i++) {
      if (fields[i] == NONE) {
        ret.push_back(i);
      }
    }
    return ret;
  }
  void print() const {
    cout << fields[0] << " " << fields[1] << " " << fields[2] << endl;
    cout << fields[3] << " " << fields[4] << " " << fields[5] << endl;
    cout << fields[6] << " " << fields[7] << " " << fields[8] << endl;
    cout << endl;
  }

private:
  vector<Player> fields;
  static const vector<vector<int>> winning_sets;
};

const vector<vector<int>> Board::winning_sets = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
    {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

class GameState {
public:
  GameState(bool lose_move_)
      : judge_board(Board()), players_boards({Board(), Board()}),
        player_turn(PLAYER1), lose_move(lose_move_) {}
  GameState(const Board &judge_board_, const Board &player1_board,
            const Board &player2_board, Player player_turn_, bool lose_move_)
      : judge_board(judge_board_),
        players_boards({player1_board, player2_board}),
        player_turn(player_turn_), lose_move(lose_move_) {}
  vector<int> getPossibleMoves() const {
    return players_boards[player_turn].getFreeFields();
  }
  Result getResult() const { return judge_board.getResult(); }
  GameState performMove(int field) const {
    GameState newState = *this;
    Player field_owner = judge_board.getField(field);
    assert(field_owner != player_turn);
    if (field_owner == NONE) {
      newState.judge_board.setField(field, player_turn);
      newState.players_boards[player_turn].setField(field, player_turn);
      newState.player_turn = getOppositePlayer(player_turn);
    } else {
      newState.players_boards[player_turn].setField(field, field_owner);
      if (lose_move) {
        newState.player_turn = getOppositePlayer(player_turn);
      }
    }
    return newState;
  }
  void printBoard() const { judge_board.print(); }

private:
  Board judge_board;
  vector<Board> players_boards;
  Player player_turn;
  bool lose_move;
};

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
  assert(countStatesInSubtree(state) == 1);
}

void test_2() {
  Board board({PLAYER2, PLAYER2, PLAYER1, PLAYER1, PLAYER2, NONE, PLAYER2,
               PLAYER1, PLAYER1});
  Board player1board({PLAYER2, NONE, PLAYER1, PLAYER1, PLAYER2, NONE, PLAYER2,
                      PLAYER1, PLAYER1});
  LoseMoveWhenConflictGameState state(board, player1board, board, PLAYER1);
  int count = countStatesInSubtree(state);
  if (count != 4) {
    cout << "Test 2 failed." << endl;
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
  cout << "Test 3: " << countStatesInSubtree(state) << endl;
}

void test_4() {
  Board board({PLAYER1, PLAYER2, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  Board player1board({PLAYER1, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  Board player2board({NONE, PLAYER2, NONE, NONE, NONE, NONE, NONE, NONE, NONE});
  DoNotLoseMoveWhenConflictGameState state(board, player1board, player2board,
                                           PLAYER1);
  cout << "Test 4: " << countStatesInSubtree(state) << endl;
}

void test_lose_move() {
  LoseMoveWhenConflictGameState initialGameState;
  cout << "Test lose move: " << countStatesInSubtree(initialGameState) << endl;
}

void test_do_not_lose_move() {
  DoNotLoseMoveWhenConflictGameState initialGameState;
  cout << "Test do not lose move: " << countStatesInSubtree(initialGameState)
       << endl;
}

int main() {
  // test_1();
  // test_2();
  // test_3();
  // test_4();
  // test_lose_move();
  // test_do_not_lose_move();
  return 0;
}