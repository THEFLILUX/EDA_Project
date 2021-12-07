#include <gtest/gtest.h>
#include <stdio.h>

#include <iostream>

#include "BigRTree.hpp"

using namespace utec::spatial;

TEST(FirstTest, BasicTest) {
  // Load data to rtree
  /*int status = remove("../tests/RTree//data.rtree");
  status = remove("../tests/RTree//index.rtree");

  uint Mleaf = 50;     // 50;
  uint Mintern = 110;  // 110;
  std::string path = "../tests/RTree/";

  RTree* rtree = new RTree(path, Mleaf, Mintern);
  // RTree rtree(path);

  BigRTree* bigRTree = new BigRTree(path, rtree);
  // BigRTree bigRTree(rtree);
  //  std::vector<std::string> paths;
  //  paths.push_back("../tests/RTree/test.csv");
  //  paths.push_back("../tests/RTree/test2.csv");
  //  bigRTree.loadFiles(paths, "Pickup_longitude", "Pickup_latitude");
  bigRTree->loadFile("../tests/RTree/test200k.csv", "Pickup_longitude",
                     "Pickup_latitude");
  rtree->printTree();
  rtree->writeToFile();

  delete rtree;
  delete bigRTree;*/
  // 5 min por millon

  std::string path = "../tests/RTree/";
  RTree* rtree = new RTree(path);
  rtree->printTree();
  //-73.960029602050781,40.814136505126953
  std::vector<Trip> trips =
      rtree->rangeSearch(Point(-73.91, 40.80), Point(-73.90, 40.81));
  std::cout << "Query: " << trips.size() << "\n";
  std::cout << "\n";
  std::fstream f;
  std::string path2 = "../tests/RTree/test200k.csv";
  f.open(path2, std::ios::in);
  for (Trip trip : trips) {
    f.seekg(trip.getTripInit(), std::ios::beg);
    uint tam = trip.getTripOffset() - trip.getTripInit() - 1;
    char* buffer = new char[tam + 1];
    f.read(buffer, tam);
    buffer[tam] = '\0';
    std::string row = buffer;
    delete buffer;
    std::cout << row << "\n";
  }
  f.close();
  std::cout << "Fin test\n";
  /*
    Trip trip(1, 1, "path1", 11, 12);
    rtree.insertTrip(trip);
    trip.setParam(2, 2, "path2", 21, 22);
    rtree.insertTrip(trip);
    trip.setParam(10, 10, "path3", 31, 32);
    rtree.insertTrip(trip);
    trip.setParam(11, 12, "path4", 41, 42);
    rtree.insertTrip(trip);
    std::cout << "\n1er print\n";
    rtree.printTree(true);
    trip.setParam(13, 14, "path5", 51, 52);
    rtree.insertTrip(trip);
    std::cout << "\n2nd print\n";
    rtree.printTree(true);
    trip.setParam(20, 20, "path6", 61, 62);
    rtree.insertTrip(trip);
    std::cout << "\n3er print\n";
    rtree.printTree(true);
    trip.setParam(8, 9, "path7", 71, 72);
    rtree.insertTrip(trip);
    std::cout << "\n4to print\n";
    rtree.printTree(true);
    trip.setParam(15, 15, "path8", 81, 82);
    rtree.insertTrip(trip);
    trip.setParam(30, 30, "path9", 91, 92);
    rtree.insertTrip(trip);
    trip.setParam(35, 35, "path10", 101, 102);
    rtree.insertTrip(trip);
    std::cout << "\n5to print\n";
    rtree.printTree(true);
    std::cout << "\nPrint1\n";
    trip.setParam(40, 40, "path11", 111, 112);
    rtree.insertTrip(trip);
    std::cout << "\nPrint2\n";
    trip.setParam(50, 50, "path12", 121, 122);
    rtree.insertTrip(trip);
    std::cout << "\nPrint3\n";
    trip.setParam(41, 41, "path13", 131, 132);
    rtree.insertTrip(trip);
    std::cout << "\n6to print\n";
    rtree.printTree(true);
  */
  EXPECT_TRUE(1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}