#include "serializer.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(SerializationTest, InversionTestTheSame) {
  stringstream ss;
  map<array<PlayerKey, 2>, int> m({{{{1, 3}, 7}, {{2, 5}, 8}, {{13, 333}, 712}}});
  Serializer::serialize(m, ss);
  auto output = Serializer::unserialize(ss);
  ASSERT_EQ(m, output);
}

TEST(SerializationTest, InversionTestDifferent) {
  stringstream ss;
  map<array<PlayerKey, 2>, int> m({{{{1, 3}, 7}, {{2, 5}, 8}, {{13, 333}, 713}}});
  map<array<PlayerKey, 2>, int> m2({{{{1, 3}, 7}, {{2, 5}, 8}, {{13, 333}, 712}}});
  Serializer::serialize(m, ss);
  auto output = Serializer::unserialize(ss);
  ASSERT_NE(m2, output);
}

TEST(FileTest, InversionTestTheSame) {
  string filename = "/tmp/test1";
  map<array<PlayerKey, 2>, int> m({{{{1, 3}, 7}, {{2, 5}, 8}, {{13, 333}, 712}}});
  Serializer::write_to_file(filename, m, false);
  auto output = Serializer::read_from_file(filename);
  ASSERT_EQ(m, output);
}

TEST(FileTest, InversionTestDifferent) {
  string filename = "/tmp/test2";
  map<array<PlayerKey, 2>, int> m({{{{1, 3}, 7}, {{2, 5}, 8}, {{13, 333}, 713}}});
  map<array<PlayerKey, 2>, int> m2({{{{1, 3}, 7}, {{2, 5}, 8}, {{13, 333}, 712}}});
  Serializer::write_to_file(filename, m, false);
  auto output = Serializer::read_from_file(filename);
  ASSERT_NE(m2, output);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}