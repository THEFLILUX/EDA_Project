#pragma once

#include "NodeIntern.hpp"
#include "NodeLeaf.hpp"

/*
IndexFile:
0: long posIni, bool isNodeLeaf
1:
*/
namespace utec {
namespace spatial {

class RootBucket {
 public:
  uint Mleaf;
  uint mleaf;
  uint Mintern;
  uint mintern;
  uint rootNumber;
  bool isRootANodeLeaf;
  uint nextNodeNumber;

  RootBucket() {}
  void print() {
    std::cout << "Mleaf: " << Mleaf << "\n";
    std::cout << "mleaf: " << mleaf << "\n";
    std::cout << "Minter: " << Mintern << "\n";
    std::cout << "mintern: " << mintern << "\n";
    std::cout << "rootNumber: " << rootNumber << "\n";
    std::cout << "isRootANodeLeaf: " << isRootANodeLeaf << "\n";
    std::cout << "nextNodeNumber: " << nextNodeNumber << "\n";
  }
};

/**
 * RTree implementation
 */
class RTree {
 public:
  RTree(std::string path, uint Mleaf = 0, uint Mintern = 0)
      : rootPath(path), root(nullptr) {
    std::cout << "Index Path: " << this->rootPath + this->indexName << "\n";
    std::cout << "Data Path: " << this->rootPath + this->dataName << "\n";

    findex.open(this->rootPath + this->indexName,
                std::ios::binary | std::ios::app);
    findex.close();
    fdata.open(this->rootPath + this->dataName,
               std::ios::binary | std::ios::app);
    fdata.close();

    findex.open(this->rootPath + this->indexName,
                std::ios::binary | std::ios::in | std::ios::out);
    fdata.open(this->rootPath + this->dataName,
               std::ios::binary | std::ios::in | std::ios::out);
    if (is_empty(findex)) {
      std::cout
          << "Not exists, creating index and data files in root constructor\n";
      // New Rtree
      std::cout << "New RTree\n";
      this->rootBucket.Mleaf = Mleaf;
      this->rootBucket.mleaf = Mleaf / 2;
      this->rootBucket.Mintern = Mintern;
      this->rootBucket.mintern = Mintern / 2;
      this->rootBucket.rootNumber = 1;
      this->rootBucket.isRootANodeLeaf = true;
      this->rootBucket.nextNodeNumber = 2;

      this->download();

      this->close();
      root = new NodeLeaf(this->rootPath + this->indexName,
                          this->rootPath + this->dataName, 1, Mleaf, true);
      this->open();

      delete root;
      root = nullptr;
    } else {
      // Load existing RTree
      std::cout << "Load RTree\n";
      this->load();
    }
    if (this->rootBucket.isRootANodeLeaf) {
      this->close();
      this->root = new NodeLeaf(
          this->rootPath + this->indexName, this->rootPath + this->dataName,
          this->rootBucket.rootNumber, this->rootBucket.Mleaf);
      this->open();
    } else {
      this->close();
      this->root = new NodeIntern(
          this->rootPath + this->indexName, this->rootPath + this->dataName,
          this->rootBucket.rootNumber, this->rootBucket.Mintern);
      this->open();
    }
  }
  ~RTree() {
    findex.close();
    fdata.close();
  }
  void writeToFile() {
    std::cout << "Writing rtree to file\n";
    this->download();

    // temporal
    this->close();
    this->root->writeToFile();
    this->open();
  }

  void insertTrip(Trip trip) {
    if (this->rootBucket.isRootANodeLeaf) {
      root->insertTrip(trip);
      if (root->getSize() == this->rootBucket.Mleaf) {
        std::cout << "Split required\n";
      }
    } else {
      std::cout << "Traverse\n";
    }
  }

  void printTree() {
    std::cout << "\n";
    std::cout << "rootPath: " << rootPath << "\n";
    std::cout << "indexName: " << indexName << "\n";
    std::cout << "dataName: " << dataName << "\n";
    this->rootBucket.print();

    if (this->rootBucket.isRootANodeLeaf) {
      root->printNode();
    } else {
      // printRTree
    }
  }
  void rangeSearch(Point ini, Point fin) {}

 private:
  NodeBase *root;
  std::string rootPath;  // file with root info
  std::string indexName = "index.rtree";
  std::string dataName = "data.rtree";
  std::fstream fdata;
  std::fstream findex;
  RootBucket rootBucket;

  void open() {
    findex.open(this->rootPath + this->indexName,
                std::ios::binary | std::ios::in | std::ios::out);
    fdata.open(this->rootPath + this->dataName,
               std::ios::binary | std::ios::in | std::ios::out);
  }

  void close() {
    findex.close();
    fdata.close();
  }

  void load() {
    fdata.seekp(0, std::ios::beg);
    read(fdata, this->rootBucket);
  }
  void download() {
    long posIni = 0;
    fdata.seekp(0, std::ios::beg);

    write(fdata, this->rootBucket);
  }
};

}  // namespace spatial
}  // namespace utec
