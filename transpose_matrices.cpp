#include "bits/stdc++.h"
#include "constants.h"
#include "serializer.h"

#include <experimental/filesystem>

using namespace std;

namespace fs = std::experimental::filesystem;

map<array<PlayerKey, 2>, int> TransposeMatrix(map<array<PlayerKey, 2>, int> m) {
  map<array<PlayerKey, 2>, int> mt;
  for (const auto &[k, v] : m) {
    PlayerKey fi = get<0>(k);
    PlayerKey se = get<1>(k);
    mt.insert({{se, fi}, v});
  }
  return mt;
}

void Run(bool lose_move) {
  cout << "Reading matrices" << endl;
  //TODO include "P" matrix as well
  for (std::string matrix_name : {"C", "D"}) {
    std::string matrix_filename = GetMatrixFilename(matrix_name, lose_move);
    if (fs::exists(matrix_filename)) {
      try {
        auto m = Serializer::read_from_file<array<PlayerKey, 2>, int>(
            matrix_filename);
        auto mt = TransposeMatrix(m);
        std::string matrix_transpose_filename =
            GetMatrixFilename(matrix_name + "T", lose_move);
        Serializer::write_to_file(matrix_transpose_filename, mt, false);
        cout << "Reverted " << matrix_name << " matrix to "
             << matrix_transpose_filename << endl;
      } catch (const exception &e) {
        cout << e.what();
      }
    }
  }
}

int main() {
  Run(/*lose_move=*/true);
  Run(/*lose_move=*/false);
}