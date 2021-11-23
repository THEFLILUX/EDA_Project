#pragma once

#include "NodeIntern.hpp"
#include "NodeLeaf.hpp"

namespace utec {
namespace spatial {

/**
 * RTree implementation
 */
class RTree {
 public:
  RTree(std::string path, uint Mleaf = 0, uint Mintern = 0)
      : rootPath(path), root(nullptr) {
    std::cout << "Ṕath: " << this->rootPath + this->rootName << "\n";
    f.open(this->rootPath + this->rootName,
           std::ios::binary | std::ios::in | std::ios::out);
    if (!f) {
      std::cout << "Not exists, creating file in root constructor\n";
      f.open(this->rootPath + this->rootName,
             std::ios::binary | std::ios::in | std::ios::out);
      // New Rtree
      std::cout << "New RTree\n";
      this->Mleaf = Mleaf;
      this->mleaf = Mleaf / 2;
      this->Mintern = Mintern;
      this->mintern = Mintern / 2;
      this->nodeNumber = 1;
      this->isRootANodeLeaf = true;
      this->download();
    } else {
      // Load existing RTree
      std::cout << "Load RTree\n";
      this->load();
    }
    f.close();
    if (this->isRootANodeLeaf) {
      this->root = new NodeLeaf(
          this->rootPath + "node" + std::to_string(this->nodeNumber) + ".rtree",
          this->Mleaf);
    } else {
      this->root = new NodeIntern(
          this->rootPath + "node" + std::to_string(this->nodeNumber) + ".rtree",
          this->Mintern);
    }
  }
  void writeToFile() {
    std::cout << "Writing rtree to file\n";
    f.open(this->rootPath + this->rootName,
           std::ios::binary | std::ios::out | std::ios::trunc);
    if (f.is_open()) {
      this->download();
      f.close();
    } else
      std::cout << "Open file error in Rtree writeToFIle\n";

    // temporal
    this->root->writeToFile();
  }

  void insertTrip(Trip trip) {
    if (isRootANodeLeaf) {
      root->insertTrip(trip);
      if (root->getSize() == this->Mleaf) {
        std::cout << "Split required\n";
      }
    } else {
      std::cout << "Traverse\n";
    }
  }

  void printTree() {
    std::cout << "\n";
    std::cout << "Mleaf: " << this->Mleaf << "\n";
    std::cout << "mleaf: " << this->mleaf << "\n";
    std::cout << "Mintern: " << this->Mintern << "\n";
    std::cout << "Mintern: " << this->mintern << "\n";
    std::cout << "NodeNumber: " << this->nodeNumber << "\n";
    std::cout << "isRootANodeLeaf: " << this->isRootANodeLeaf << "\n";
    std::cout << "Rootpath: " << this->rootPath << "\n";

    if (isRootANodeLeaf) {
      root->printNode();
    } else {
      // printRTree
    }
  }
  void rangeSearch(Point ini, Point fin) {}

 private:
  NodeBase *root;
  std::string rootPath;  // file with root info
  std::string rootName = "root.rtree";
  bool isRootANodeLeaf;
  std::fstream f;
  uint Mleaf;
  uint mleaf;
  uint Mintern;
  uint mintern;
  uint nodeNumber;

  void load() {
    // Format: bool, uint Mleaf, mleaf, Mint, mint, nodenumber
    read(f, this->isRootANodeLeaf);
    read(f, this->Mleaf);
    read(f, this->mleaf);
    read(f, this->Mintern);
    read(f, this->mintern);
    read(f, this->nodeNumber);
  }
  void download() {
    // Format:bool
    write(f, this->isRootANodeLeaf);
    write(f, this->Mleaf);
    write(f, this->mleaf);
    write(f, this->Mintern);
    write(f, this->mintern);
    write(f, this->nodeNumber);
  }
};

}  // namespace spatial
}  // namespace utec
