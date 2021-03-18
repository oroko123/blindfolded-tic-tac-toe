#ifndef STATE
#define STATE

#include "board.h"
#include "constants.h"
#include <bits/stdc++.h>
using namespace std;

struct HistoryKey {
  long long first_sixth;
  long long seventh_twelfth;
  long long thirteen;
};

bool operator<(const HistoryKey &lhs, const HistoryKey &rhs) {
  if (lhs.first_sixth < rhs.first_sixth) {
    return true;
  }
  if (lhs.seventh_twelfth < rhs.seventh_twelfth) {
    return true;
  }
  return lhs.thirteen < rhs.thirteen;
}

class GameState {
public:
  GameState(bool lose_move_)
      : judge_board(Board()), players_boards({Board(), Board()}),
        player_turn(PLAYER1), lose_move(lose_move_) /*,  key(0) */,
        history_key({0, 0, 0}) {}
  GameState(const Board &judge_board_, const Board &player1_board,
            const Board &player2_board, Player player_turn_, bool lose_move_)
      : judge_board(judge_board_),
        players_boards({player1_board, player2_board}),
        player_turn(player_turn_), lose_move(lose_move_) {
    // key = generateKey();
    history_key = generateHistoryKey();
  }
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
    newState.history.push_back(make_pair(field, player_turn));
    // newState.updateKey();
    newState.updateHistoryKey();
    return newState;
  }
  // void updateKey() { key = generateKey(); }
  void updateHistoryKey() { history_key = generateHistoryKey(); }
  void printBoard() const { judge_board.print(); }
  Player getPlayerTurn() const { return player_turn; }
  // long long getKey() const { return key; }
  // vector<pair<int, Player>> getHistory() const { return history; }
  HistoryKey getHistoryKey() const { return history_key; }

  vector<HistoryKey> getSymetricHistoriesKeys() const {
    vector<HistoryKey> res;

    for (const auto &permutation : permutations) {
      vector<pair<int, Player>> transformed_history;
      for (int i = 0; i < history.size(); i++) {
        transformed_history.push_back(
            make_pair(permutation[history[i].first], history[i].second));
      }
      res.push_back(GameState::generateHistoryKey(transformed_history));
    }
    return res;
  }
  // vector<long long> getSymetricJudgeStateKeys() const {
  //   vector<long long> judge_keys = judge_board.getSymetricBoardsKeys();
  //   vector<long long> player1_keys =
  //       players_boards[PLAYER1].getSymetricBoardsKeys();
  //   vector<long long> player2_keys =
  //       players_boards[PLAYER2].getSymetricBoardsKeys();
  //   vector<long long> res;
  //   for (int i = 0; i < 9; i++) {
  //     res.push_back(generateKey(judge_keys[i], player1_keys[i],
  //     player2_keys[i],
  //                               player_turn));
  //   }
  //   return res;
  // }

private:
  // static long long generateKey(long long judge_key, long long player1_key,
  //                              long long player2_key, Player player_turn) {
  //   long long k = 0;
  //   k += judge_key;
  //   k *= THREE_TO_POWER_NINE;
  //   k += player1_key;
  //   k *= THREE_TO_POWER_NINE;
  //   k += player2_key;
  //   k *= 2;
  //   k += player_turn;
  //   return k;
  // }
  // long long generateKey() {
  //   return generateKey(judge_board.getKey(),
  //   players_boards[PLAYER1].getKey(),
  //                      players_boards[PLAYER2].getKey(), player_turn);
  // }
  static HistoryKey
  generateHistoryKey(const vector<pair<int, Player>> &history) {
    HistoryKey ret({0, 0, 0});
    for (int i = 0; i < 6 && i < history.size(); i++) {
      ret.first_sixth += history[i].first;
      ret.first_sixth *= 9;
      ret.first_sixth += history[i].second;
      ret.first_sixth *= 2;
    }
    for (int i = 6; i < 12 && i < history.size(); i++) {
      ret.seventh_twelfth += history[i].first;
      ret.seventh_twelfth *= 9;
      ret.seventh_twelfth += history[i].second;
      ret.seventh_twelfth *= 2;
    }
    for (int i = 12; i < 18 && i < history.size(); i++) {
      ret.thirteen += history[i].first;
      ret.thirteen *= 9;
      ret.thirteen += history[i].second;
      ret.thirteen *= 2;
    }
    return ret;
  }
  HistoryKey generateHistoryKey() { return generateHistoryKey(history); }
  Board judge_board;
  vector<Board> players_boards;
  Player player_turn;
  bool lose_move;
  HistoryKey history_key;
  // long long key;
  vector<pair<int, Player>> history;
};

#endif