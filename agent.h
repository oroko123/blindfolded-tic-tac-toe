#ifndef AGENT
#define AGENT

#include <bits/stdc++.h>
using namespace std;

class Agent {
public:
  Agent() = default;
  ~Agent() = default;
  virtual int tryMakeMove() = 0;
  virtual void fetchStatus(bool success) = 0;
  virtual string getStrategyName() const = 0;
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
    if (success) {
      possible_moves.erase(current_guess);
    }
  }
  
  string getStrategyName() const { return "Random available field agent"; }

private:
  std::set<int> possible_moves = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  int current_guess;
  std::random_device dev;
  std::mt19937 rng;
};

class FirstAgent : public Agent {
public:
  FirstAgent() {}
  int tryMakeMove() { 
    current_guess = possible_moves[0];
    return current_guess;
  }
  virtual void fetchStatus(bool success) {
    if (success) {
      possible_moves.erase(possible_moves.begin());
    }
  }
  string getStrategyName() const { return "First available field agent"; }
private:
  std::vector<int> possible_moves = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  int current_guess;
};

#endif