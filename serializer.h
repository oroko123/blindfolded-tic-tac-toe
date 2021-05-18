#ifndef SERIALIZER
#define SERIALIZER

#include "constants.h"
#include <bits/stdc++.h>

using namespace std;

class Serializer {
public:
  static void serialize(const map<array<PlayerKey, 2>, int> &matrix,
                        ostream &stream) {
    for (auto &[key, value] : matrix) {
      stream.write((char *)&key, sizeof(key));
      stream.write((char *)&value, sizeof(value));
    }
  }
  static map<array<PlayerKey, 2>, int> unserialize(istream &stream) {
    map<array<PlayerKey, 2>, int> ret;
    while (stream.good()) {
      array<PlayerKey, 2> key;
      int value;
      stream.read((char *)&key, sizeof(key));
      stream.read((char *)&value, sizeof(value));
      ret.insert({key, value});
    }
    return ret;
  }
  static map<array<PlayerKey, 2>, int>
  read_from_file(const string &filename) {
    ifstream file;
    ios_base::openmode flags = ofstream::in | ofstream::binary;
    file.open(filename, flags);
    auto ret = unserialize(file);
    file.close();
    return ret;
  }
  static void write_to_file(const string &filename,
                            const map<array<PlayerKey, 2>, int> &matrix,
                            bool append) {
    ofstream file;
    ios_base::openmode flags = ofstream::out | ofstream::binary;
    if (append) {
      flags |= ofstream::app;
    }
    file.open(filename, flags);
    serialize(matrix, file);
    file.close();
  }
};

#endif
