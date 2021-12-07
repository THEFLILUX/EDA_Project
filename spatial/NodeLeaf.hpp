#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

class NodeLeaf : public NodeBase {
 public:
  NodeLeaf(std::string index, std::string data, uint nodeID, uint M = 0,
           bool isNew = false)
      : index(index), data(data), nodeID(nodeID) {
    if (isNew) {
      // New nodeLeaf
      // std::cout << "New nodeLeaf\n";
      this->nodeLeafBucket.M = M;
      this->nodeLeafBucket.m = M / 2;
      this->nodeLeafBucket.vectorsSize = 0;
      this->download(isNew);
    } else {
      // Load existing nodeLeaf
      // std::cout << "Load nodeLeaf\n";
      this->load();
    }
  }

  uint getSize() override { return this->trips.size(); }
  std::vector<Trip>& getTrips() override { return this->trips; }
  uint getNodeID() override { return this->nodeID; }

  void writeToFile() override {
    // std::cout << "Writing NodeLeaf to file\n";
    this->download(false);
  }

  void printNode() override {
    std::cout << "\nPrint Node Leaf\n";
    std::cout << "nodeID: " << this->nodeID << "\n";
    this->nodeLeafBucket.print();
    for (int i = 0; i < trips.size(); i++) {
      std::cout << "\tTrip " << i << ": (" << trips[i].getLon() << ", "
                << trips[i].getLat() << ") tripInit: " << trips[i].getTripInit()
                << " tripOffset: " << trips[i].getTripOffset();
      trips[i].print();
    }
  }

  std::vector<MBR>& getMBRs() override {
    std::vector<MBR> mbrs;
    return mbrs;
  }
  std::vector<uint>& getChildren() override {
    std::vector<uint> children;
    return children;
  }

 private:
  uint nodeID;
  NodeBucket nodeLeafBucket;
  std::vector<Trip> trips;
  std::string index;
  std::string data;

  void load() override {
    std::fstream findex;
    std::fstream fdata;
    findex.open(this->index, std::ios::binary | std::ios::in | std::ios::out);
    fdata.open(this->data, std::ios::binary | std::ios::in | std::ios::out);

    this->trips.clear();

    IndexBucket indexBucket;
    findex.seekg((this->nodeID - 1) * sizeof(IndexBucket), std::ios::beg);
    read(findex, indexBucket);

    fdata.seekg(indexBucket.posIni, std::ios::beg);
    read(fdata, this->nodeLeafBucket);
    Trip trip;
    for (int i = 0; i < this->nodeLeafBucket.vectorsSize; i++) {
      read(fdata, trip);
      this->trips.push_back(trip);
    }

    findex.close();
    fdata.close();
  }
  void download(bool isNew) override {
    std::fstream findex;
    std::fstream fdata;
    findex.open(this->index, std::ios::binary | std::ios::in | std::ios::out);
    fdata.open(this->data, std::ios::binary | std::ios::in | std::ios::out);

    if (isNew) {
      findex.seekp(0, std::ios::end);
      fdata.seekp(0, std::ios::end);
      long posIni = fdata.tellp();
      IndexBucket indexBucket(posIni, true);
      write(findex, indexBucket);
    } else {
      IndexBucket indexBucket;
      findex.seekg((this->nodeID - 1) * sizeof(IndexBucket), std::ios::beg);
      read(findex, indexBucket);
      fdata.seekp(indexBucket.posIni, std::ios::beg);
    }
    this->nodeLeafBucket.vectorsSize = this->trips.size();

    write(fdata, this->nodeLeafBucket);

    Trip dump;
    for (int i = 0; i < this->nodeLeafBucket.M; i++) {
      if (i < this->trips.size())
        write(fdata, this->trips[i]);
      else
        write(fdata, dump);
    }

    findex.close();
    fdata.close();
  }
};

}  // namespace spatial
}  // namespace utec