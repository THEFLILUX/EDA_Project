#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

class NodeIntern : public NodeBase {
 public:
  NodeIntern(std::string index, std::string data, uint nodeID, uint M = 0,
             bool isNew = false)
      : index(index), data(data), nodeID(nodeID) {
    if (isNew) {
      // New nodeLeaf
      this->mbr = MBR(Point(-1, -1));

      this->nodeInternBucket.M = M;
      this->nodeInternBucket.m = M / 2;
      this->nodeInternBucket.vectorsSize = 0;
      this->download(isNew);
    } else {
      // Load existing nodeLeaf
      std::cout << "Load nodeIntern\n";
      this->load();
    }
  }

  void readToFile() override {}
  void writeToFile() override {}

  uint getSize() override { return 0; }

  void insertMBR(MBR mbr) {
    if (!MBRs.empty()) {
      this->MBRs.push_back(mbr);
      this->children.push_back(this->MBRs.size());
    } else {
      std::cout << "Split nodeIntern";
    }
  }

  void printNode() override {
    std::cout << "\nNode Intern\n";
    std::cout << "nodeID: " << this->nodeID << "\n";
    this->nodeInternBucket.print();
    for (int i = 0; i < MBRs.size(); i++) {
      std::cout << "\tMBR " << i << ": (" << MBRs[i].getIniLon() << ", "
                << MBRs[i].getIniLat() << ") (" << MBRs[i].getFinLon() << ", "
                << MBRs[i].getFinLat() << ") Child: " << children[i] << "\n";
    }
  }
  void insertTrip(Trip trip) override { /* Nothing */
  }

 private:
  uint nodeID;
  NodeBucket nodeInternBucket;
  std::vector<MBR> MBRs;
  std::vector<uint> children;
  MBR mbr;
  std::string index;
  std::string data;

  void load() override {
    std::fstream findex;
    std::fstream fdata;
    findex.open(this->index, std::ios::binary | std::ios::in | std::ios::out);
    fdata.open(this->data, std::ios::binary | std::ios::in | std::ios::out);

    this->MBRs.clear();
    this->children.clear();

    IndexBucket indexBucket;
    findex.seekg((this->nodeID - 1) * sizeof(IndexBucket), std::ios::beg);
    read(findex, indexBucket);

    fdata.seekg(indexBucket.posIni, std::ios::beg);
    read(fdata, this->nodeInternBucket);
    MBR mbr;
    uint child;
    for (int i = 0; i < this->nodeInternBucket.vectorsSize; i++) {
      read(fdata, mbr);
      read(fdata, child);
      this->MBRs.push_back(mbr);
      this->children.push_back(child);
    }

    findex.close();
    fdata.close();

    this->mbr =
        MBR(this->nodeInternBucket.iniLon, this->nodeInternBucket.iniLat,
            this->nodeInternBucket.finLon, this->nodeInternBucket.finLat);
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
      IndexBucket indexBucket(posIni, false);
      write(findex, indexBucket);
    } else {
      IndexBucket indexBucket;
      findex.seekg((this->nodeID - 1) * sizeof(IndexBucket), std::ios::beg);
      read(findex, indexBucket);
      fdata.seekp(indexBucket.posIni, std::ios::beg);
    }

    this->nodeInternBucket.vectorsSize = this->MBRs.size();
    this->nodeInternBucket.iniLon = this->mbr.getIniLon();
    this->nodeInternBucket.iniLat = this->mbr.getIniLat();
    this->nodeInternBucket.finLon = this->mbr.getFinLon();
    this->nodeInternBucket.finLat = this->mbr.getFinLat();

    write(fdata, this->nodeInternBucket);

    MBR mbrDump;
    uint childDump;
    for (int i = 0; i < this->nodeInternBucket.M; i++) {
      if (i < this->MBRs.size()) {
        write(fdata, this->MBRs[i]);
        write(fdata, this->children[i]);
      } else {
        write(fdata, mbrDump);
        write(fdata, childDump);
      }
    }

    findex.close();
    fdata.close();
  }
};

}  // namespace spatial
}  // namespace utec