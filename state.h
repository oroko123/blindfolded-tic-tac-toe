#ifndef STATE
#define STATE

#include "board.h"
#include "constants.h"
#include <bits/stdc++.h>
using namespace std;

template <class T> inline void hash_combine(std::size_t &s, const T &v) {
  std::hash<T> h;
  s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

struct HistoryKey {
  long long first_ninth;
  long long tenth_eighteen;
  bool operator==(const HistoryKey &other) const {
    return this->first_ninth == other.first_ninth &&
           this->tenth_eighteen == other.tenth_eighteen;
  }
};

template <class T> class GenericStructHash;

template <> struct GenericStructHash<HistoryKey> {
  std::size_t operator()(HistoryKey const &key) const {
    std::size_t res = 0;
    hash_combine(res, key.first_ninth);
    hash_combine(res, key.tenth_eighteen);
    return res;
  }
};

typedef long long PlayerKey;

bool operator<(const HistoryKey &lhs, const HistoryKey &rhs) {
  if (lhs.first_ninth < rhs.first_ninth) {
    return true;
  } else if (lhs.first_ninth > rhs.first_ninth) {
    return false;
  }
  return lhs.tenth_eighteen < rhs.tenth_eighteen;
}

class GameState {
public:
  GameState(bool lose_move_)
      : judge_board(Board()), players_boards({Board(), Board()}),
        player_turn(PLAYER1),
        lose_move(lose_move_) /*,  player_keys({0, 0}) */ {}
  /*,  key(0) , history_key({0, 0}) */
  GameState(const Board &judge_board_, const Board &player1_board,
            const Board &player2_board, Player player_turn_, bool lose_move_)
      : judge_board(judge_board_),
        players_boards({player1_board, player2_board}),
        player_turn(player_turn_), lose_move(lose_move_) {
    // key = generateKey();
    // history_key = generateHistoryKey();
    // for (auto player : {PLAYER1, PLAYER2}) {
    //   player_keys[player] = generatePlayerKey(player);
    // }
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
    // newState.updateHistoryKey();
    // for (auto player : {PLAYER1, PLAYER2}) {
    //   newState.updatePlayerKey(player);
    // }
    return newState;
  }
  // void updateKey() { key = generateKey(); }
  // void updateHistoryKey() { history_key = generateHistoryKey(); }
  // void updatePlayerKey(Player player) {
  //   player_keys[player] = generatePlayerKey(player);
  // }
  void printBoard() const { judge_board.print(); }
  Player getPlayerTurn() const { return player_turn; }
  // long long getKey() const { return key; }
  // vector<pair<int, Player>> getHistory() const { return history; }
  // HistoryKey getHistoryKey() const { return history_key; }

  // vector<HistoryKey> getSymetricHistoriesKeys() const {
  //   vector<HistoryKey> res;

  //   for (const auto &permutation : permutations) {
  //     vector<pair<int, Player>> transformed_history;
  //     for (int i = 0; i < history.size(); i++) {
  //       transformed_history.push_back(
  //           make_pair(permutation[history[i].first], history[i].second));
  //     }
  //     res.push_back(GameState::generateHistoryKey(transformed_history));
  //   }
  //   return res;
  // }

  vector<PlayerKey> getSymetricPlayerHistoriesKeys(Player player) const {
    vector<PlayerKey> res;

    for (const auto &permutation : permutations) {
      vector<pair<int, Player>> transformed_history;
      for (int i = 0; i < history.size(); i++) {
        transformed_history.push_back(
            make_pair(permutation[history[i].first], history[i].second));
      }
      res.push_back(GameState::generatePlayerKey(transformed_history, player));
    }
    return res;
  }

  vector<PlayerKey> calculateRepresentativeHistoryPair() const {

    vector<PlayerKey> player_max_keys({0, 0});

    for (const auto &permutation : permutations) {
      vector<pair<int, Player>> transformed_history;
      for (int i = 0; i < history.size(); i++) {
        transformed_history.push_back(
            make_pair(permutation[history[i].first], history[i].second));
      }
      for (auto player : {PLAYER1, PLAYER2}) {
        PlayerKey player_key = generatePlayerKey(transformed_history, player);
        player_max_keys[player] = max(player_max_keys[player], player_key);
      }
    }
    return player_max_keys;
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

  static PlayerKey generatePlayerKey(const vector<pair<int, Player>> &history,
                                     Player player) {
    PlayerKey ret = 0;
    for (int i = 0; i < history.size(); i++) {
      if (history[i].second != player) {
        continue;
      }
      ret += history[i].first; // field
      ret *= 9;
      ret += (i == history.size() - 1 ||
              history[i + 1].second != player); // move performed
      ret *= 2;
    }
    return ret;
  }

  // static HistoryKey
  // generateHistoryKey(const vector<pair<int, Player>> &history) {
  //   HistoryKey ret({0, 0});
  //   for (int i = 0; i < 9 && i < history.size(); i++) {
  //     ret.first_ninth += history[i].first;
  //     ret.first_ninth *= 9;
  //     ret.first_ninth += history[i].second;
  //     ret.first_ninth *= 2;
  //   }
  //   for (int i = 9; i < history.size(); i++) {
  //     ret.tenth_eighteen += history[i].first;
  //     ret.tenth_eighteen *= 9;
  //     ret.tenth_eighteen += history[i].second;
  //     ret.tenth_eighteen *= 2;
  //   }
  //   return ret;
  // }
  // HistoryKey generateHistoryKey() { return generateHistoryKey(history); }
  PlayerKey generatePlayerKey(Player player) {
    return generatePlayerKey(history, player);
  }
  Board judge_board;
  vector<Board> players_boards;
  Player player_turn;
  bool lose_move;
  // HistoryKey history_key;
  // vector<PlayerKey> player_keys;
  // long long key;
  vector<pair<int, Player>> history;
};

#endif