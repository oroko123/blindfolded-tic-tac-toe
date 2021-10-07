#ifndef STATE2
#define STATE2

#include "board2.h"
#include "constants2.h"
#include <bits/stdc++.h>
using namespace std;

class GameState2 {
public:
  GameState2(bool lose_move_)
      : judge_board(Board2()), players_boards({Board2(), Board2()}),
        player_turn(PLAYER1), lose_move(lose_move_) {}
  vector<int> getPossibleMoves() const {
    return players_boards[player_turn].getFreeFields();
  }
  Result getResult() const { return judge_board.getResult(); }
  GameState2 performMove(int field) const {
    GameState2 newState = *this;
    Player field_owner = judge_board.getField(field);
    assert(field_owner != player_turn);
    bool move_performed = false;
    if (field_owner == NONE) {
      move_performed = true;
      newState.judge_board.setField(field, player_turn);
      newState.players_boards[player_turn].setField(field, player_turn);
      newState.player_turn = getOppositePlayer(player_turn);
    } else {
      newState.players_boards[player_turn].setField(field, field_owner);
      if (lose_move) {
        newState.player_turn = getOppositePlayer(player_turn);
      }
    }
    newState.history.push_back({field, player_turn, move_performed});
    return newState;
  }
  void printBoard() const { judge_board.print(); }
  Player getPlayerTurn() const { return player_turn; }
  bool wasLastAccepted() const {
    if (history.empty()) {
      return false;
    }
    return get<2>(history.back());
  }

  array<PlayerKey, 2> calculateRepresentativeStateKeyPair() const {

    array<PlayerKey, 2> player_max_keys({0, 0});

    for (const auto &permutation : permutations) {
      vector<tuple<int, Player, bool>> transformed_history;
      for (size_t i = 0; i < history.size(); ++i) {
        transformed_history.push_back({permutation[get<0>(history[i])],
                                       get<1>(history[i]), get<2>(history[i])});
      }
      for (auto player : {PLAYER1, PLAYER2}) {
        PlayerKey player_key =
            generatePlayerStateKey(transformed_history, player);
        player_max_keys[player] = max(player_max_keys[player], player_key);
      }
    }
    return player_max_keys;
  }

  array<PlayerKey, 2> calculateRepresentativeActionStateKeyPair() const {
    array<PlayerKey, 2> res = calculateRepresentativeStateKeyPair();
    return {res[0] / 2, res[1] / 2};
  }

  static PlayerKey
  generatePlayerStateKey(const vector<tuple<int, Player, bool>> &history,
                         Player player) {
    PlayerKey ret = 1;
    for (size_t i = 0; i < history.size(); ++i) {
      if (get<1>(history[i]) != player) {
        continue;
      }
      ret *= 4;
      ret += get<0>(history[i]); // field
      ret *= 2;
      ret += get<2>(history[i]); // move performed
    }
    return ret;
  }

private:
  PlayerKey generatePlayerStateKey(Player player) const {
    return generatePlayerStateKey(history, player);
  }
  Board2 judge_board;
  vector<Board2> players_boards;
  Player player_turn;
  bool lose_move;
  vector<tuple<int, Player, bool>> history;
};

class LoseMoveWhenConflictGameState2 : public GameState2 {
public:
  LoseMoveWhenConflictGameState2() : GameState2(/*lose_move_=*/true) {}
};

class DoNotLoseMoveWhenConflictGameState2 : public GameState2 {
public:
  DoNotLoseMoveWhenConflictGameState2() : GameState2(/*lose_move_=*/false) {}
};

#endif