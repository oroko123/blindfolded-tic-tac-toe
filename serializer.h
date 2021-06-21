#ifndef SERIALIZER
#define SERIALIZER

#include "constants2.h"
#include "gurobi_c++.h"
#include <bits/stdc++.h>

using namespace std;

class Serializer {
public:
  template <typename K, typename V>
  static void serialize(const map<K, V> &matrix, ostream &stream) {
    for (auto &[key, value] : matrix) {
      stream.write((char *)&key, sizeof(key));
      stream.write((char *)&value, sizeof(value));
    }
  }
  template <typename K, typename V>
  static map<K, V> unserialize(istream &stream) {
    map<K, V> ret;
    while (stream.good()) {
      K key;
      int value;
      stream.read((char *)&key, sizeof(key));
      stream.read((char *)&value, sizeof(value));
      ret.insert({key, value});
    }
    return ret;
  }
  template <typename K, typename V>
  static map<K, V> read_from_file(const string &filename) {
    ifstream file;
    ios_base::openmode flags = ofstream::in | ofstream::binary;
    file.open(filename, flags);
    auto ret = unserialize<K, V>(file);
    file.close();
    return ret;
  }
  template <typename K, typename V>
  static void write_to_file(const string &filename, const map<K, V> &matrix,
                            bool append) {
    ofstream file;
    ios_base::openmode flags = ofstream::out | ofstream::binary;
    if (append) {
      flags |= ofstream::app;
    }
    file.open(filename, flags);
    serialize<K, V>(matrix, file);
    file.close();
  }
};

#endif
