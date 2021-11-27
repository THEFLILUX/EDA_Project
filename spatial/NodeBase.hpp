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
  double iniLon;
  double iniLat;
  double finLon;
  double finLat;
  uint vectorsSize;

  NodeBucket() {}
  void print() {
    std::cout << "M: " << this->M << "\n";
    std::cout << "m: " << this->m << "\n";
    std::cout << "MBR: (" << this->iniLon << ", " << this->iniLat << ") ("
              << this->finLon << ", " << this->finLat << ")\n";
    std::cout << "vectorsSize: " << this->vectorsSize << "\n";
  }
};

}  // namespace spatial
}  // namespace utec