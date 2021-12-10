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
  /*
    int status = remove(pathData.c_str());
    status = remove(pathIndx.c_str());

    uint Mleaf = 100;    // 50;
    uint Mintern = 220;  // 110;

    RTree* rtree = new RTree(path, true, Mleaf, Mintern);

    BigRTree* bigRTree = new BigRTree(path, rtree);
    std::vector<std::string> names;
    names.push_back("green_tripdata_2014-07.csv");
    names.push_back("green_tripdata_2014-08.csv");
    names.push_back("green_tripdata_2014-09.csv");
    names.push_back("green_tripdata_2014-10.csv");
    names.push_back("green_tripdata_2014-11.csv");
    names.push_back("green_tripdata_2014-12.csv");
    names.push_back("green_tripdata_2015-01.csv");
    names.push_back("green_tripdata_2015-02.csv");
    names.push_back("green_tripdata_2015-03.csv");
    names.push_back("green_tripdata_2015-04.csv");
    names.push_back("green_tripdata_2015-05.csv");
    names.push_back("green_tripdata_2015-06.csv");
    names.push_back("green_tripdata_2015-07.csv");
    names.push_back("green_tripdata_2015-08.csv");
    names.push_back("green_tripdata_2015-09.csv");
    names.push_back("green_tripdata_2015-10.csv");
    names.push_back("green_tripdata_2015-11.csv");
    names.push_back("green_tripdata_2015-12.csv");
    names.push_back("green_tripdata_2016-01.csv");
    names.push_back("green_tripdata_2016-02.csv");
    names.push_back("green_tripdata_2016-03.csv");
    names.push_back("green_tripdata_2016-04.csv");
    names.push_back("green_tripdata_2016-05.csv");
    names.push_back("green_tripdata_2016-06.csv");

    bigRTree->loadFiles(names, "Pickup_longitude", "Pickup_latitude");
    names.clear();
    names.push_back("yellow_tripdata_2015-01.csv");
    names.push_back("yellow_tripdata_2015-02.csv");
    names.push_back("yellow_tripdata_2015-03.csv");
    names.push_back("yellow_tripdata_2015-04.csv");
    names.push_back("yellow_tripdata_2015-05.csv");
    names.push_back("yellow_tripdata_2015-06.csv");
    names.push_back("yellow_tripdata_2015-07.csv");
    names.push_back("yellow_tripdata_2015-08.csv");
    names.push_back("yellow_tripdata_2015-09.csv");
    names.push_back("yellow_tripdata_2015-10.csv");
    names.push_back("yellow_tripdata_2015-11.csv");
    names.push_back("yellow_tripdata_2015-12.csv");
    bigRTree->loadFiles(names, "pickup_longitude", "pickup_latitude");
    names.clear();
    rtree->printTree();
    rtree->writeToFile();

    delete rtree;
    delete bigRTree; */

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