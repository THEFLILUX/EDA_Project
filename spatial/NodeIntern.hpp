#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

class NodeIntern : public NodeBase {
 public:
  NodeIntern(std::string index, std::string data, uint nodeID, uint M,
             bool isNew = false)
      : index(index), data(data), nodeID(nodeID), M(M) {
    this->nodeInternBucket = new NodeBucket();
    if (isNew) {
      // New nodeIntern
      // std::cout << "New nodeIntern\n";
      this->nodeInternBucket->vectorsSize = 0;
      this->download(isNew);
    } else {
      // Load existing nodeIntern
      // std::cout << "Load nodeIntern\n";
      this->load();
    }
  }
  virtual ~NodeIntern() {
    this->MBRs.clear();
    this->children.clear();
    delete this->nodeInternBucket;
  }

  uint getSize() override { return this->MBRs.size(); }
  uint getNodeID() override { return this->nodeID; }
  std::vector<MBR>& getMBRs() override { return this->MBRs; }
  std::vector<uint>& getChildren() override { return this->children; }

  void writeToFile() override {
    // std::cout << "Writing NodeIntern to file\n";
    this->download(false);
  }

  void printNode() override {
    std::cout << "\nPrint Node Intern\n";
    std::cout << "nodeID: " << this->nodeID << "\n";
    this->nodeInternBucket->print();
    std::cout << "Size: " << this->MBRs.size() << "\n";
    for (int i = 0; i < this->MBRs.size(); i++) {
      std::cout << "\tMBR " << i << ": (" << MBRs[i].getIniLon() << ", "
                << MBRs[i].getIniLat() << ") (" << MBRs[i].getFinLon() << ", "
                << MBRs[i].getFinLat() << ") Child: " << children[i] << "\n";
    }
  }

 private:
  uint nodeID;
  uint M;
  NodeBucket* nodeInternBucket;
  std::vector<MBR> MBRs;
  std::vector<uint> children;
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
    read(fdata, *this->nodeInternBucket);
    MBR mbr;
    uint child;
    for (int i = 0; i < this->nodeInternBucket->vectorsSize; i++) {
      read(fdata, mbr);
      read(fdata, child);
      this->MBRs.push_back(mbr);
      this->children.push_back(child);
    }

    findex.flush();
    fdata.flush();
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
      IndexBucket indexBucket(posIni, false);
      write(findex, indexBucket);
    } else {
      IndexBucket indexBucket;
      findex.seekg((this->nodeID - 1) * sizeof(IndexBucket), std::ios::beg);
      read(findex, indexBucket);
      fdata.seekp(indexBucket.posIni, std::ios::beg);
    }

    this->nodeInternBucket->vectorsSize = this->MBRs.size();

    write(fdata, *this->nodeInternBucket);

    MBR mbrDump;
    uint childDump;
    for (int i = 0; i < this->M; i++) {
      if (i < this->MBRs.size()) {
        write(fdata, this->MBRs[i]);
        write(fdata, this->children[i]);
      } else {
        write(fdata, mbrDump);
        write(fdata, childDump);
      }
    }

    findex.flush();
    fdata.flush();
    findex.close();
    fdata.close();
  }
};

}  // namespace spatial
}  // namespace utec