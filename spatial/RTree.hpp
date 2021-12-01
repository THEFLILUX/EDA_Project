#pragma once

#include <unordered_set>

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
      : rootPath(path), nodePtr(nullptr) {
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
      nodePtr = new NodeLeaf(this->rootPath + this->indexName,
                             this->rootPath + this->dataName, 1, Mleaf, true);
      this->open();

      delete nodePtr;
      nodePtr = nullptr;
    } else {
      // Load existing RTree
      std::cout << "Load RTree\n";
      this->load();
    }
    this->close();
    if (this->rootBucket.isRootANodeLeaf) {
      this->nodePtr = new NodeLeaf(
          this->rootPath + this->indexName, this->rootPath + this->dataName,
          this->rootBucket.rootNumber, this->rootBucket.Mleaf);
    } else {
      this->nodePtr = new NodeIntern(
          this->rootPath + this->indexName, this->rootPath + this->dataName,
          this->rootBucket.rootNumber, this->rootBucket.Mintern);
    }
    this->open();
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
    this->nodePtr->writeToFile();
    this->open();
  }

  void insertTrip(Trip trip) {
    if (this->rootBucket.isRootANodeLeaf) {
      this->nodePtr->insertTrip(trip);
      if (this->nodePtr->getSize() == this->rootBucket.Mleaf + 1) {
        // Split root
        std::cout << "NodeLeaf root split required\n";
        this->close();
        NodeBase* newNodeLeaf = new NodeLeaf(
            this->rootPath + this->indexName, this->rootPath + this->dataName,
            this->rootBucket.nextNodeNumber++, this->rootBucket.Mleaf, true);

        this->rootBucket.isRootANodeLeaf = false;
        this->rootBucket.rootNumber = this->rootBucket.nextNodeNumber;

        NodeBase* newNodeIntern = new NodeIntern(
            this->rootPath + this->indexName, this->rootPath + this->dataName,
            this->rootBucket.nextNodeNumber++, this->rootBucket.Mintern, true);
        this->open();

        this->splitNodeLeaf(this->nodePtr, newNodeLeaf);

        newNodeIntern->insertMBR(this->nodePtr->getMBR(),
                                 this->nodePtr->getNodeID());
        newNodeIntern->insertMBR(newNodeLeaf->getMBR(),
                                 newNodeLeaf->getNodeID());

        // Regresar a memoria secundaria
        this->close();
        this->nodePtr->writeToFile();
        newNodeLeaf->writeToFile();
        newNodeIntern->writeToFile();
        delete this->nodePtr;
        delete newNodeLeaf;
        delete newNodeIntern;
        // Cargo el root
        this->nodePtr = new NodeIntern(
            this->rootPath + this->indexName, this->rootPath + this->dataName,
            this->rootBucket.rootNumber, this->rootBucket.Mintern);
        this->open();
        std::cout << "NodeLeaf root finish split\n";
      }
    } else {
      this->insertTripRec(this->rootBucket.rootNumber, trip);
    }
  }

  void printTree() {
    std::cout << "\n";
    std::cout << "rootPath: " << rootPath << "\n";
    std::cout << "indexName: " << indexName << "\n";
    std::cout << "dataName: " << dataName << "\n";
    this->rootBucket.print();

    if (this->rootBucket.isRootANodeLeaf) {
      nodePtr->printNode();
    } else {
      // recursive print
      this->printRec(this->rootBucket.rootNumber);
    }
  }
  void rangeSearch(Point ini, Point fin) {}

 private:
  NodeBase* nodePtr;
  std::string rootPath;  // file with root info
  std::string indexName = "index.rtree";
  std::string dataName = "data.rtree";
  std::fstream fdata;
  std::fstream findex;
  RootBucket rootBucket;
  IndexBucket indexBucket;

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
  void loadNodeInNodePtr(uint nodeNumber) {
    findex.seekg((nodeNumber - 1) * sizeof(IndexBucket), std::ios::beg);
    read(findex, this->indexBucket);
    this->close();
    this->nodePtr->writeToFile();
    delete this->nodePtr;
    if (this->indexBucket.isNodeLeaf) {
      this->nodePtr = new NodeLeaf(this->rootPath + this->indexName,
                                   this->rootPath + this->dataName, nodeNumber,
                                   this->rootBucket.Mleaf);
    } else {
      this->nodePtr = new NodeIntern(this->rootPath + this->indexName,
                                     this->rootPath + this->dataName,
                                     nodeNumber, this->rootBucket.Mintern);
    }
    this->open();
  }

  void insertTripRec(uint nodeNumber, Trip trip);
  void splitNodeLeaf(NodeBase*& firstNode, NodeBase*& secondNode);
  void splitNodeIntern(NodeBase*& firstNode, NodeBase*& secondNode);
  void printRec(uint nodeNumber);
};

void RTree::insertTripRec(uint nodeNumber, Trip trip) {
  // Cargar el node
  this->loadNodeInNodePtr(nodeNumber);

  if (this->indexBucket.isNodeLeaf) {
    // Nodo Hoja
    this->nodePtr->insertTrip(trip);
    if (this->nodePtr->getSize() == this->rootBucket.Mleaf + 1) {
      std::cout << "NodeLeaf split required\n";
      this->close();
      NodeBase* newNodeLeaf = new NodeLeaf(
          this->rootPath + this->indexName, this->rootPath + this->dataName,
          this->rootBucket.nextNodeNumber++, this->rootBucket.Mleaf, true);
      this->open();
      this->splitNodeLeaf(this->nodePtr, newNodeLeaf);
      // Cambiar root al node Intern

      std::cout << "Fin leaf node split\n";
    }
  }
}
void RTree::splitNodeLeaf(NodeBase*& firstNode, NodeBase*& secondNode) {
  std::vector<Trip> trips = firstNode->getTrips();
  firstNode->resetVectors();

  Trip trip1, trip2;
  uint ind1, ind2;
  dist_t maxTripDistance = 0;
  for (uint i = 0; i < trips.size(); i++) {
    for (uint j = i + 1; j < trips.size(); j++) {
      dist_t localDistance = trips[i] - trips[j];
      if (localDistance > maxTripDistance) {
        trip1 = trips[i];
        trip2 = trips[j];
        ind1 = i;
        ind2 = j;
        maxTripDistance = localDistance;
      }
    }
  }
  firstNode->insertTrip(trip1);
  secondNode->insertTrip(trip2);

  std::unordered_set<uint> inserted;
  inserted.insert(ind1);
  inserted.insert(ind2);
  bool toFirst;
  while (inserted.size() < trips.size()) {
    uint ind = 0;
    dist_t dist_min = -1;
    for (uint i = 0; i < trips.size(); i++) {
      if (inserted.count(i) > 0) continue;
      dist_t dist1 = firstNode->getMBR() - trips[i];
      dist_t dist2 = secondNode->getMBR() - trips[i];
      if (dist_min == -1) {
        ind = i;
        if (dist1 < dist2) {
          dist_min = dist1;
          toFirst = true;
        } else {
          dist_min = dist2;
          toFirst = false;
        }
      } else {
        if (dist1 < dist_min) {
          dist_min = dist1;
          toFirst = true;
          ind = i;
        }
        if (dist2 < dist_min) {
          dist_min = dist2;
          toFirst = false;
          ind = i;
        }
      }
    }
    if (toFirst)
      firstNode->insertTrip(trips[ind]);
    else
      secondNode->insertTrip(trips[ind]);
    inserted.insert(ind);
    if (trips.size() - std::max(firstNode->getSize(), secondNode->getSize()) ==
        this->rootBucket.mleaf)
      break;
  }
  toFirst = firstNode->getSize() < secondNode->getSize();
  if (inserted.size() < trips.size()) {
    for (uint i = 0; i < trips.size(); i++) {
      if (inserted.count(i) > 0) continue;
      if (toFirst)
        firstNode->insertTrip(trips[i]);
      else
        secondNode->insertTrip(trips[i]);
    }
  }
}
void RTree::splitNodeIntern(NodeBase*& firstNode, NodeBase*& secondNode) {}
void RTree::printRec(uint nodeNumber) {
  // Cargar el node
  this->loadNodeInNodePtr(nodeNumber);

  this->nodePtr->printNode();

  if (!this->indexBucket.isNodeLeaf) {
    // Nodo Interno
    std::vector<uint> children = this->nodePtr->getChildren();
    for (uint number : children) {
      this->printRec(number);
    }
  }
}

}  // namespace spatial
}  // namespace utec