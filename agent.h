#ifndef AGENT
#define AGENT

#include "constants.h"
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
  std::set<int> possible_moves = {0, 1, 2, 3, 4, 5, 6, 7, 8};
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
  std::vector<int> possible_moves = {0, 1, 2, 3, 4, 5, 6, 7, 8};
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
std::vector<int> POSSIBLE_MOVES = {0, 1, 2, 3, 4, 5, 6, 7, 8};

class MatrixAgent : public Agent {
public:
  MatrixAgent(bool lose_move, Player player) : dev(), rng(dev()) {
    s = Serializer::read_from_file<PlayerKey, double>(GetOutputFilename("S", player, lose_move));
  }
  int tryMakeMove() {
    double base = s[calculateRepresentativeActionKey(history)];
    vector<double> move_chances;
    for (auto MOVE : POSSIBLE_MOVES) {
      vector<pair<int, bool>> action_history = history;
      action_history.push_back({MOVE, false});
      PlayerKey action_key = calculateRepresentativeActionKey(action_history);
      double new_chance = s[action_key] / base;
      move_chances.push_back(new_chance);
    }
    std::discrete_distribution<int> distribution(move_chances.begin(),
                                                 move_chances.end());
    current_guess = distribution(rng);
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
#endif