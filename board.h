#ifndef BOARD
#define BOARD

#include "constants.h"
#include <bits/stdc++.h>
using namespace std;

class Board {
public:
  Board() : fields(FIELDS_NUM, NONE) {}
  Board(const vector<Player> &fields_) : fields(fields_) {
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
  // vector<long long> getSymetricBoardsKeys() const {
  //   vector<long long> ret;
  //   for (const auto &permutation : permutations) {
  //     ret.push_back(getKey(permute(fields, permutation)));
  //   }
  //   return ret;
  // }
  // long long getKey() { return getKey(fields); }

private:
  // static long long getKey(const vector<Player> &fields) {
  //   long long ret = 0;
  //   for (int i = 0; i < fields.size(); i++) {
  //     ret += fields[i];
  //     ret *= 3;
  //   }
  //   return ret;
  // }

  vector<Player> fields;
  static const vector<vector<int>> winning_sets;
};

const vector<vector<int>> Board::winning_sets = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
    {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

#endif