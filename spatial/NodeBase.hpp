#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "MBR.hpp"
#include "Trip.hpp"

namespace utec {
namespace spatial {

class NodeBase {
 public:
  virtual void printNode() = 0;
  virtual void insertTrip(Trip trip) = 0;

  virtual void readToFile() = 0;
  virtual void writeToFile() = 0;

  virtual uint getSize() = 0;
  // virtual MBR getMBR() = 0;
  virtual void insertMBR(MBR mbr, uint child) = 0;
  virtual void resetVectors() = 0;
  virtual uint getNodeID() = 0;

  virtual std::vector<Trip> getTrips() = 0;
  virtual std::vector<MBR> getMBRs() = 0;
  virtual std::vector<uint> getChildren() = 0;
  virtual void updateMBRbyIndex(uint index, MBR newMBR) = 0;

 private:
  virtual void load() = 0;
  virtual void download(bool isNew) = 0;
};

template <typename T>
void write(std::fstream& f, T& t) {
  f.write((char*)&t, sizeof(t));
}

template <typename T>
void read(std::fstream& f, T& t) {
  f.read((char*)&t, sizeof(t));
}

bool is_empty(std::fstream& f) {
  return f.peek() == std::fstream::traits_type::eof();
}

class IndexBucket {
 public:
  long posIni;
  bool isNodeLeaf;

  IndexBucket() {}
  IndexBucket(long posIni, bool isNodeLeaf)
      : posIni(posIni), isNodeLeaf(isNodeLeaf) {}
  void print() {
    std::cout << "posIni: " << posIni << "\n";
    std::cout << "isNodeLeaf: " << isNodeLeaf << "\n";
  }
};

class NodeBucket {
 public:
  uint M;
  uint m;
  // MBR mbr;
  uint vectorsSize;

  NodeBucket() {}
  void print() {
    std::cout << "\tPrint bucket:\n";
    std::cout << "\tM: " << this->M << "\n";
    std::cout << "\tm: " << this->m << "\n";
    // std::cout << "\tMBR: " << this->mbr << "\n";
    std::cout << "\tvectorsSize: " << this->vectorsSize << "\n";
  }
};

}  // namespace spatial
}  // namespace utec