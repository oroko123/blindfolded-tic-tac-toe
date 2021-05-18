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
    } else if (playersWinningSets[PLAYER1] > 0) {
      return PLAYER1WIN;
    } else if (playersWinningSets[PLAYER2] > 0) {
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
    for (size_t i = 0; i < fields.size(); ++i) {
      if (fields[i] == NONE) {
        ret.push_back(i);
      }
    }
    return ret;
  }
  void print() const {
    for (int i = 0; i < 3; i++) {
      cout << playerToSign(fields[3 * i]) << " | "
           << playerToSign(fields[3 * i + 1]) << " | "
           << playerToSign(fields[3 * i + 2]) << endl;
    }
    cout << endl;
  }

private:
  vector<Player> fields;
  static const vector<vector<int>> winning_sets;
};

const vector<vector<int>> Board::winning_sets = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
    {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

#endif