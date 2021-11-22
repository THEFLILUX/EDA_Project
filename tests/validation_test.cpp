#include <gtest/gtest.h>

#include <iostream>

#include "RTree.hpp"

// Referencia: file = "../tests/images/test8/img8";

using namespace utec::spatial;

TEST(FirstTest, BasicTest) {
  uint Mleaf = 4;
  uint Mintern = 4;
  std::string path = "../tests/RTree/";

  RTree rtree(path, Mleaf, Mintern);
  std::cout << "\n1er print\n";
  rtree.printTree();

  Trip trip(2, 5, "path1", 1);
  rtree.insertTrip(trip);

  std::cout << "\n2do print\n";
  rtree.printTree();

  Trip trip2(1, 8, "path2", 2);
  rtree.insertTrip(trip2);

  std::cout << "\n3er print\n";
  rtree.printTree();
  rtree.writeToFile();

  /*
    std::string path = "../tests/RTree/";

    RTree rtree(path);
    std::cout << "\n1er print\n";
    rtree.printTree();
  */
  EXPECT_TRUE(1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}