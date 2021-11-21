#ifndef AGENT
#define AGENT

#include "constants2.h"
#include "serializer.h"
#include <bits/stdc++.h>
using namespace std;

class Agent {
public:
  Agent() = default;
  ~Agent() = default;
  virtual int tryMakeMove() = 0;
  virtual void fetchStatus(bool success) = 0;
  static string getStrategyName();
};

class RandomAgent : public Agent {
public:
  RandomAgent() : dev(), rng(dev()) {}
  int tryMakeMove() {
    std::uniform_int_distribution<std::mt19937::result_type> dist(
        0, possible_moves.size() - 1);
    current_guess = *std::next(possible_moves.begin(), dist(rng));
    return current_guess;
  }
  virtual void fetchStatus(bool success) {
    (void)success;
    possible_moves.erase(current_guess);
  }

  static string getStrategyName() { return "Random available field agent"; }

private:
  std::set<int> possible_moves = {0, 1, 2, 3};
  int current_guess;
  std::random_device dev;
  std::mt19937 rng;
};

class FirstMoveAgent : public Agent {
public:
  FirstMoveAgent() {}
  int tryMakeMove() {
    current_guess = possible_moves[0];
    return current_guess;
  }
  virtual void fetchStatus(bool success) {
    (void)success;
    possible_moves.erase(possible_moves.begin());
  }
  static string getStrategyName() { return "First available field agent"; }

private:
  std::vector<int> possible_moves = {0, 1, 2, 3};
  int current_guess;
};

PlayerKey generateKey(const vector<pair<int, bool>> &history) {
  PlayerKey ret = 1;
  for (size_t i = 0; i < history.size(); ++i) {
    ret *= 4;
    ret += history[i].first; // field
    ret *= 2;
    ret += history[i].second; // move performed
  }
  return ret;
}

PlayerKey generateActionKey(const vector<pair<int, bool>> &history) {
  return generateKey(history) / 2;
}

PlayerKey
calculateRepresentativeActionKey(const vector<pair<int, bool>> &history) {
  PlayerKey player_max_key = 0;
  for (const auto &permutation : permutations) {
    std::vector<std::pair<int, bool>> transformed_history;
    for (size_t i = 0; i < history.size(); ++i) {
      transformed_history.push_back(
          {permutation[history[i].first], history[i].second});
    }
    PlayerKey player_key = generateActionKey(transformed_history);
    player_max_key = max(player_max_key, player_key);
  }
  return player_max_key;
}
std::vector<int> POSSIBLE_MOVES = {0, 1, 2, 3};

class MatrixAgent : public Agent {
public:
  MatrixAgent(bool lose_move, Player player) : dev(), rng(dev()) {
    s = Serializer::read_from_file<PlayerKey, double>(
        GetOutputFilename("S", player, lose_move));
  }
  int tryMakeMove() {
    // cerr << "key: " << calculateRepresentativeActionKey(history) << endl;
    double base = s[calculateRepresentativeActionKey(history)];
    // cerr << "base: " << base << endl;
    vector<double> move_chances;
    for (auto MOVE : POSSIBLE_MOVES) {
      vector<pair<int, bool>> action_history = history;
      action_history.push_back({MOVE, false});
      PlayerKey action_key = calculateRepresentativeActionKey(action_history);
      double new_chance = s[action_key] / base;
      move_chances.push_back(new_chance);
    }
    // for (auto x : move_chances) {
    //   cout << x <<  " ";
    // }
    // cout << endl;
    std::discrete_distribution<int> distribution(move_chances.begin(),
                                                 move_chances.end());
    current_guess = distribution(rng);
    // cout << current_guess << endl;
    return current_guess;
  }
  virtual void fetchStatus(bool success) {
    history.push_back({current_guess, success});
  }
  static string getStrategyName() { return "Matrix agent"; }

private:
  int current_guess;
  std::vector<pair<int, bool>> history;
  std::map<PlayerKey, double> s;
  std::random_device dev;
  std::mt19937 rng;
};

struct Node {
  int id;
  int field;
  int leftmost_child_id;
  int right_sibling_id;
};

bool operator<(const Node &lhs, const Node &rhs) { return lhs.id < rhs.id; }

struct TreeBasedStrategy {
  map<int, Node> tree;
  int current_node;
};

class Player1NotLosingAgent1 : public Agent {
public:
  Player1NotLosingAgent1() {
    map<int, Node> tree;
    tree[0] = {0, 4, 1, -1};
    tree[1] = {1, 1, 3, 2};
    tree[2] = {2, 0, 6, -1};
    tree[3] = {3, 7, -1, 4};
    tree[4] = {4, 6, 8, 5};
    tree[5] = {5, 8, 11, -1};
    tree[6] = {6, 8, -1, 7};
    tree[7] = {7, 6, 13, -1};
    tree[8] = {8, 2, -1, 9};
    tree[9] = {9, 5, 15, 10};
    tree[10] = {10, 8, 18, -1};
    tree[11] = {11, 0, -1, 12};
    tree[12] = {12, 3, 20, -1};
    tree[13] = {13, 3, -1, 14};
    tree[14] = {14, 2, -1, -1};
    tree[15] = {15, 3, -1, 16};
    tree[16] = {16, 0, -1, 17};
    tree[17] = {17, 8, -1, -1};
    tree[18] = {18, 0, -1, 19};
    tree[19] = {19, 3, -1, -1};
    tree[20] = {20, 5, -1, 21};
    tree[21] = {21, 2, -1, -1};
    strategy = TreeBasedStrategy{tree, 0};
  }
  int tryMakeMove() {
    assert(strategy.current_node != -1);
    return strategy.tree[strategy.current_node].field;
  }
  virtual void fetchStatus(bool success) {
    if (success) {
      strategy.current_node =
          strategy.tree[strategy.current_node].leftmost_child_id;
    } else {
      strategy.current_node =
          strategy.tree[strategy.current_node].right_sibling_id;
    }
  }
  static string getStrategyName() {
    return "Not losing agent for player1 of do_not_lose_move version. Source: "
           "https://twitter.com/peakNiche/status/1074752119373225984, user "
           "Justin";
  }

private:
  TreeBasedStrategy strategy;
};

class Player1NotLosingAgent2 : public Agent {
public:
  Player1NotLosingAgent2() {
    map<int, Node> tree;
    tree[0] = {0, 4, 1, -1};
    tree[1] = {1, 1, 3, 2};
    tree[2] = {2, 0, 6, -1};
    tree[3] = {3, 7, -1, 4};
    tree[4] = {4, 6, 8, 5};
    tree[5] = {5, 8, 11, -1};
    tree[6] = {6, 8, -1, 7};
    tree[7] = {7, 6, 13, -1};
    tree[8] = {8, 2, -1, 9};
    tree[9] = {9, 5, 15, 10};
    tree[10] = {10, 8, 18, -1};
    tree[11] = {11, 0, -1, 12};
    tree[12] = {12, 3, 20, -1};
    tree[13] = {13, 3, -1, 14};
    tree[14] = {14, 2, -1, -1};
    tree[15] = {15, 3, -1, 16};
    tree[16] = {16, 0, -1, 17};
    tree[17] = {17, 8, -1, -1};
    tree[18] = {18, 0, -1, 19};
    tree[19] = {19, 3, -1, -1};
    tree[20] = {20, 5, -1, 21};
    tree[21] = {21, 2, -1, -1};
    strategy = TreeBasedStrategy{tree, 0};
  }
  int tryMakeMove() {
    assert(strategy.current_node != -1);
    return strategy.tree[strategy.current_node].field;
  }
  virtual void fetchStatus(bool success) {
    if (success) {
      strategy.current_node =
          strategy.tree[strategy.current_node].leftmost_child_id;
    } else {
      strategy.current_node =
          strategy.tree[strategy.current_node].right_sibling_id;
    }
  }
  static string getStrategyName() {
    return "Not losing agent for player1 of do_not_lose_move version. Source: "
           "https://fivethirtyeight.com/features/santa-needs-some-help-with-math/, user "
           "Barry King";
  }

private:
  TreeBasedStrategy strategy;
};


class Player1NotLosingAgent3 : public Agent {
public:
  Player1NotLosingAgent3() {
    map<int, Node> tree;
    tree[0] = {0, 1, 1, -1};
    tree[1] = {1, 4, 2, 10};
    tree[2] = {2, 7, -1, 3};
    tree[3] = {3, 8, 4, 7};
    tree[4] = {4, 0, -1, 5};
    tree[5] = {5, 3, -1, 6};
    tree[6] = {6, 6, -1, -1};
    tree[7] = {7, 6, 8, -1};
    tree[8] = {8, 2, -1, 9};
    tree[9] = {9, 5, -1, -1};
    tree[10] = {10, 0, 11, -1};
    tree[11] = {11, 2, -1, 12};
    tree[12] = {12, 6, 13, -1};
    tree[13] = {13, 2, -1, 14};
    tree[14] = {14, 5, -1, -1};
    strategy = TreeBasedStrategy{tree, 0};
  }
  int tryMakeMove() {
    assert(strategy.current_node != -1);
    return strategy.tree[strategy.current_node].field;
  }
  virtual void fetchStatus(bool success) {
    if (success) {
      strategy.current_node =
          strategy.tree[strategy.current_node].leftmost_child_id;
    } else {
      strategy.current_node =
          strategy.tree[strategy.current_node].right_sibling_id;
    }
  }
  static string getStrategyName() {
    return "Not losing agent for player1 of do_not_lose_move version. Source: "
           "https://fivethirtyeight.com/features/santa-needs-some-help-with-math/, user "
           "Kenny Long";
  }

private:
  TreeBasedStrategy strategy;
};
#endif