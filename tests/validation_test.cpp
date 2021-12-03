#include <gtest/gtest.h>
#include <stdio.h>

#include <iostream>

#include "RTree.hpp"

// Referencia: file = "../tests/images/test8/img8";

using namespace utec::spatial;

TEST(FirstTest, BasicTest) {
  int status = remove("../tests/RTree//data.rtree");
  if (status == 0)
    std::cout << "Borrado 1\n";
  else
    exit(0);
  status = remove("../tests/RTree//index.rtree");
  if (status == 0)
    std::cout << "Borrado 2\n";
  else
    exit(0);

  uint Mleaf = 4;
  uint Mintern = 4;
  std::string path = "../tests/RTree/";

  RTree rtree(path, Mleaf, Mintern);

  Trip trip(1, 1, "path1", 11, 12);
  rtree.insertTrip(trip);

  trip.setParam(2, 2, "path2", 21, 22);
  rtree.insertTrip(trip);

  trip.setParam(10, 10, "path3", 31, 32);
  rtree.insertTrip(trip);

  trip.setParam(11, 12, "path4", 41, 42);
  rtree.insertTrip(trip);

  std::cout << "\n1er print\n";
  rtree.printTree();

  trip.setParam(13, 14, "path5", 51, 52);
  rtree.insertTrip(trip);

  std::cout << "\n2nd print\n";
  rtree.printTree();

  EXPECT_TRUE(1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}