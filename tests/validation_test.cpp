#include <gtest/gtest.h>
#include <stdio.h>

#include <iostream>

#include "BigRTree.hpp"
#include "Tester.hpp"

using namespace utec::spatial;

TEST(FirstTest, BasicTest) {
  // Load data to rtree
  std::string path = "../tests/RTree/RawData/";
  std::string pathData = path + "data.rtree";
  std::string pathIndx = path + "index.rtree";

  /*int status = remove(pathData.c_str());
  status = remove(pathIndx.c_str());

  uint Mleaf = 50;     // 50;
  uint Mintern = 110;  // 110;

  RTree* rtree = new RTree(path, Mleaf, Mintern);

  BigRTree* bigRTree = new BigRTree(path, rtree);
  //  std::vector<std::string> names;
  //  names.push_back("test.csv");
  //  names.push_back("test2.csv");
  //  bigRTree.loadFiles(paths, "Pickup_longitude", "Pickup_latitude");
  bigRTree->loadFile("green_tripdata_2015-01.csv", "Pickup_longitude",
                     "Pickup_latitude");
  rtree->printTree();
  rtree->writeToFile();

  delete rtree;
  delete bigRTree;*/

  RTree* rtree = new RTree(path);
  Tester* tester = new Tester(path, rtree);
  rtree->printTree();

  tester->test(Point(-73.91, 40.80), Point(-73.90, 40.81));

  std::cout << "Fin test\n";

  EXPECT_TRUE(1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}