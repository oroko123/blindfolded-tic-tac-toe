#ifndef STATE
#define STATE

#include "board.h"
#include "constants.h"
#include <bits/stdc++.h>
using namespace std;

struct HistoryKey {
  long long first_ninth;
  long long tenth_eighteen;
  bool operator==(const HistoryKey &other) const {
    return this->first_ninth == other.first_ninth &&
           this->tenth_eighteen == other.tenth_eighteen;
  }
};

typedef long long PlayerKey;

class GameState {
public:
  GameState(bool lose_move_)
      : judge_board(Board()), players_boards({Board(), Board()}),
        player_turn(PLAYER1), lose_move(lose_move_) {}
  vector<int> getPossibleMoves() const {
    return players_boards[player_turn].getFreeFields();
  }
  Result getResult() const { return judge_board.getResult(); }
  GameState performMove(int field) const {
    GameState newState = *this;
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

  array<PlayerKey, 2> calculateRepresentativeHistoryPair() const {

    array<PlayerKey, 2> player_max_keys({0, 0});

    for (const auto &permutation : permutations) {
      vector<tuple<int, Player, bool>> transformed_history;
      for (int i = 0; i < history.size(); i++) {
        transformed_history.push_back({permutation[get<0>(history[i])],
                                       get<1>(history[i]), get<2>(history[i])});
      }
      for (auto player : {PLAYER1, PLAYER2}) {
        PlayerKey player_key = generatePlayerKey(transformed_history, player);
        player_max_keys[player] = max(player_max_keys[player], player_key);
      }
    }
    return player_max_keys;
  }

  static PlayerKey
  generatePlayerKey(const vector<tuple<int, Player, bool>> &history,
                    Player player) {
    PlayerKey ret = 1;
    for (int i = 0; i < history.size(); i++) {
      if (get<1>(history[i]) != player) {
        continue;
      }
      ret *= 9;
      ret += get<0>(history[i]); // field
      ret *= 2;
      ret += get<2>(history[i]); // move performed
    }
    return ret;
  }

private:
  PlayerKey generatePlayerKey(Player player) {
    return generatePlayerKey(history, player);
  }
  Board judge_board;
  vector<Board> players_boards;
  Player player_turn;
  bool lose_move;
  vector<tuple<int, Player, bool>> history;
};

class LoseMoveWhenConflictGameState : public GameState {
public:
  LoseMoveWhenConflictGameState() : GameState(/*lose_move_=*/true) {}
};

class DoNotLoseMoveWhenConflictGameState : public GameState {
public:
  DoNotLoseMoveWhenConflictGameState() : GameState(/*lose_move_=*/false) {}
};

#endif