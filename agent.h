#ifndef AGENT
#define AGENT

#include <bits/stdc++.h>
using namespace std;

class Agent {
public:
  Agent() = default;
  ~Agent() = default;
  virtual int makeMove(const vector<int> &possible_moves) = 0;
  virtual string getStrategyName() const = 0;
};

class RandomAgent : public Agent {
public:
  RandomAgent() : dev(), rng(dev()) {}
  int makeMove(const vector<int> &possible_moves) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(
        0, possible_moves.size() - 1);
    return possible_moves[dist(rng)];
  }
  string getStrategyName() const { return "Random available field agent"; }

private:
  std::random_device dev;
  std::mt19937 rng;
};

class FirstAgent : public Agent {
public:
  FirstAgent() {}
  int makeMove(const vector<int> &possible_moves) { return possible_moves[0]; }
  string getStrategyName() const { return "First available field agent"; }
};

#endif