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
  uint numberTrips;

  RootBucket() {}
  void print() {
    std::cout << "Mleaf: " << Mleaf << "\n";
    std::cout << "mleaf: " << mleaf << "\n";
    std::cout << "Minter: " << Mintern << "\n";
    std::cout << "mintern: " << mintern << "\n";
    std::cout << "rootNumber: " << rootNumber << "\n";
    std::cout << "isRootANodeLeaf: " << isRootANodeLeaf << "\n";
    std::cout << "nextNodeNumber: " << nextNodeNumber << "\n";
    std::cout << "numberTrips: " << numberTrips << "\n";
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
      this->rootBucket.Mleaf = Mleaf;
      this->rootBucket.mleaf = Mleaf / 2;
      this->rootBucket.Mintern = Mintern;
      this->rootBucket.mintern = Mintern / 2;
      this->rootBucket.rootNumber = 1;
      this->rootBucket.isRootANodeLeaf = true;
      this->rootBucket.nextNodeNumber = 2;
      this->rootBucket.numberTrips = 0;

      this->download();

      this->close();
      nodePtr = new NodeLeaf(this->rootPath + this->indexName,
                             this->rootPath + this->dataName, 1, Mleaf, true);
      this->open();

      delete nodePtr;
      nodePtr = nullptr;
    } else {
      // Load existing RTree
      // std::cout << "Load RTree\n";
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

    this->loadNodeInNodePtr(this->rootBucket.rootNumber, false);
  }
  ~RTree() {
    findex.close();
    fdata.close();
  }
  void writeToFile() {
    // std::cout << "Writing rtree to file\n";
    this->download();
  }

  void insertTrip(Trip trip) {
    if (this->rootBucket.isRootANodeLeaf) {
      this->nodePtr->insertTrip(trip);
      if (this->nodePtr->getSize() == this->rootBucket.Mleaf + 1) {
        // Split root
        // std::cout << "NodeLeaf root split required\n";
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

        MBR mbr1(MBRNULL), mbr2(MBRNULL);

        this->splitNodeLeaf(this->nodePtr, newNodeLeaf, mbr1, mbr2);

        newNodeIntern->insertMBR(mbr1, this->nodePtr->getNodeID());
        newNodeIntern->insertMBR(mbr2, newNodeLeaf->getNodeID());

        // Regresar a memoria secundaria
        this->close();
        newNodeLeaf->writeToFile();
        newNodeIntern->writeToFile();
        delete newNodeLeaf;
        delete newNodeIntern;
        this->open();

        // Cargo el root
        this->loadNodeInNodePtr(this->rootBucket.rootNumber);
        // std::cout << "NodeLeaf root finish split\n";
      }
    } else {
      MBR mbr1(MBRNULL), mbr2(MBRNULL);
      this->insertTripRec(this->rootBucket.rootNumber, trip, mbr1, mbr2);
      if (mbr1 != mbr2) {
        // Hubo un split, nuevo nodo raíz
        this->close();
        this->loadNodeInNodePtr(this->rootBucket.rootNumber);
        this->rootBucket.rootNumber = this->rootBucket.nextNodeNumber;

        NodeBase* newNodeInternRoot = new NodeIntern(
            this->rootPath + this->indexName, this->rootPath + this->dataName,
            this->rootBucket.nextNodeNumber++, this->rootBucket.Mintern, true);
        this->open();

        newNodeInternRoot->insertMBR(mbr1, this->nodePtr->getNodeID());
        newNodeInternRoot->insertMBR(mbr2, this->rootBucket.rootNumber - 1);

        // Regresar a memoria secundaria
        this->close();
        newNodeInternRoot->writeToFile();
        this->nodePtr->writeToFile();
        this->open();
      }
    }
    this->rootBucket.numberTrips++;
  }

  void printTree(bool printAll = false) {
    std::cout << "\n";
    std::cout << "rootPath: " << rootPath << "\n";
    std::cout << "indexName: " << indexName << "\n";
    std::cout << "dataName: " << dataName << "\n";
    this->rootBucket.print();

    if (this->rootBucket.isRootANodeLeaf) {
      nodePtr->printNode();
    } else {
      // recursive print
      if (printAll) this->printRec(this->rootBucket.rootNumber);
    }
  }
  std::vector<Trip> rangeSearch(Point ini, Point fin) {
    std::vector<Trip> result;
    MBR mbr(ini, fin);
    rangeSearchRec(result, mbr, this->rootBucket.rootNumber);
    return result;
  }

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
  void loadNodeInNodePtr(uint nodeNumber, bool first = true) {
    if (first && this->nodePtr->getNodeID() == nodeNumber) return;

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

  void insertTripRec(uint nodeNumber, Trip trip, MBR& mbr1, MBR& mbr2);
  void splitNodeLeaf(NodeBase*& firstNode, NodeBase*& secondNode, MBR& mbr1,
                     MBR& mbr2);
  void splitNodeIntern(NodeBase*& firstNode, NodeBase*& secondNode, MBR& mbr1,
                       MBR& mbr2);
  void printRec(uint nodeNumber);
  void rangeSearchRec(std::vector<Trip>& result, MBR& mbr, uint nodeNumber);
};

void RTree::insertTripRec(uint nodeNumber, Trip trip, MBR& mbr1, MBR& mbr2) {
  // Cargar el node
  this->loadNodeInNodePtr(nodeNumber);

  if (this->indexBucket.isNodeLeaf) {
    // Nodo Hoja
    this->nodePtr->insertTrip(trip);
    if (this->nodePtr->getSize() == this->rootBucket.Mleaf + 1) {
      // std::cout << "NodeLeaf split required\n";

      this->close();
      NodeBase* newNodeLeaf = new NodeLeaf(
          this->rootPath + this->indexName, this->rootPath + this->dataName,
          this->rootBucket.nextNodeNumber++, this->rootBucket.Mleaf, true);
      this->open();

      MBR mbr1Tmp(MBRNULL), mbr2Tmp(MBRNULL);
      this->splitNodeLeaf(this->nodePtr, newNodeLeaf, mbr1Tmp, mbr2Tmp);
      // std::cout << "Fin leaf node split\n";

      // Regresar a memoria secundaria nuevo nodo
      this->close();
      newNodeLeaf->writeToFile();
      this->nodePtr->writeToFile();
      this->open();

      mbr1 = mbr1Tmp;
      mbr2 = mbr2Tmp;
    }
  } else {
    // Nodo intern
    std::vector<MBR> MBRs = this->nodePtr->getMBRs();
    std::vector<uint> children = this->nodePtr->getChildren();
    for (uint i = 0; i < MBRs.size(); i++) {
      if (MBRs[i] - trip == 0) {
        MBR mbr1Tmp(MBRs[i]), mbr2Tmp(MBRs[i]);
        this->insertTripRec(children[i], trip, mbr1Tmp, mbr2Tmp);
        if (mbr1Tmp != mbr2Tmp) {
          // Hubo un split
          this->loadNodeInNodePtr(nodeNumber);

          this->nodePtr->updateMBRbyIndex(i, mbr1Tmp);
          this->nodePtr->insertMBR(mbr2Tmp,
                                   this->rootBucket.nextNodeNumber - 1);
          if (this->nodePtr->getSize() == this->rootBucket.Mintern + 1) {
            // Split intern node
            // std::cout << "NodeIntern split required\n";
            this->close();
            NodeBase* newNodeIntern =
                new NodeIntern(this->rootPath + this->indexName,
                               this->rootPath + this->dataName,
                               this->rootBucket.nextNodeNumber++,
                               this->rootBucket.Mintern, true);
            this->open();
            MBR mbr1Tmp2(MBRNULL), mbr2Tmp2(MBRNULL);
            this->splitNodeIntern(this->nodePtr, newNodeIntern, mbr1Tmp2,
                                  mbr2Tmp2);
            mbr1 = mbr1Tmp2;
            mbr2 = mbr2Tmp2;

            // Regresar a memoria secundaria nuevo nodo
            this->close();
            newNodeIntern->writeToFile();
            this->nodePtr->writeToFile();
            this->open();
          }
        }
        return;
      }
    }
    // No entró en ningún MBR
    // escoger el MBR que se expanda menos
    double minExpansion = MBRs[0] / trip;
    uint ind = 0;
    for (uint i = 1; i < MBRs.size(); i++) {
      double locExpansion = MBRs[i] / trip;
      if (locExpansion < minExpansion) {
        minExpansion = locExpansion;
        ind = i;
      }
    }
    MBR newMBR = MBRs[ind] * trip;
    this->nodePtr->updateMBRbyIndex(ind, newMBR);
    MBR mbr1Tmp(newMBR), mbr2Tmp(newMBR);
    this->insertTripRec(children[ind], trip, mbr1Tmp, mbr2Tmp);
    if (mbr1Tmp != mbr2Tmp) {
      // Hubo un split
      this->loadNodeInNodePtr(nodeNumber);

      this->nodePtr->updateMBRbyIndex(ind, mbr1Tmp);
      this->nodePtr->insertMBR(mbr2Tmp, this->rootBucket.nextNodeNumber - 1);
      if (this->nodePtr->getSize() == this->rootBucket.Mintern + 1) {
        // Split intern node
        // std::cout << "NodeIntern split required\n";
        this->close();
        NodeBase* newNodeIntern = new NodeIntern(
            this->rootPath + this->indexName, this->rootPath + this->dataName,
            this->rootBucket.nextNodeNumber++, this->rootBucket.Mintern, true);
        this->open();
        MBR mbr1Tmp2(MBRNULL), mbr2Tmp2(MBRNULL);
        this->splitNodeIntern(this->nodePtr, newNodeIntern, mbr1Tmp2, mbr2Tmp2);
        mbr1 = mbr1Tmp2;
        mbr2 = mbr2Tmp2;

        // Regresar a memoria secundaria nuevo nodo
        this->close();
        newNodeIntern->writeToFile();
        this->nodePtr->writeToFile();
        this->open();
      }
    }
  }
}
void RTree::splitNodeLeaf(NodeBase*& firstNode, NodeBase*& secondNode,
                          MBR& mbr1, MBR& mbr2) {
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
  if (trip1.getLon() < trip2.getLon()) {
    firstNode->insertTrip(trip1);
    secondNode->insertTrip(trip2);
    mbr1 *= trip1;
    mbr2 *= trip2;
  } else {
    firstNode->insertTrip(trip2);
    secondNode->insertTrip(trip1);
    mbr1 *= trip2;
    mbr2 *= trip1;
  }

  std::unordered_set<uint> inserted;
  inserted.insert(ind1);
  inserted.insert(ind2);
  bool toFirst;
  while (inserted.size() < trips.size()) {
    uint ind = 0;
    dist_t dist_min = -1;
    for (uint i = 0; i < trips.size(); i++) {
      if (inserted.count(i) > 0) continue;
      dist_t dist1 = mbr1 - trips[i];
      dist_t dist2 = mbr2 - trips[i];
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
    if (toFirst) {
      firstNode->insertTrip(trips[ind]);
      mbr1 *= trips[ind];
    } else {
      secondNode->insertTrip(trips[ind]);
      mbr2 *= trips[ind];
    }
    inserted.insert(ind);
    if (trips.size() - std::max(firstNode->getSize(), secondNode->getSize()) ==
        this->rootBucket.mleaf)
      break;
  }
  toFirst = firstNode->getSize() < secondNode->getSize();
  if (inserted.size() < trips.size()) {
    for (uint i = 0; i < trips.size(); i++) {
      if (inserted.count(i) > 0) continue;
      if (toFirst) {
        firstNode->insertTrip(trips[i]);
        mbr1 *= trips[i];
      } else {
        secondNode->insertTrip(trips[i]);
        mbr2 *= trips[i];
      }
    }
  }
}
void RTree::splitNodeIntern(NodeBase*& firstNode, NodeBase*& secondNode,
                            MBR& mbr1, MBR& mbr2) {
  std::vector<MBR> MBRs = firstNode->getMBRs();
  std::vector<uint> children = firstNode->getChildren();
  firstNode->resetVectors();

  MBR mbrTmp1(MBRNULL), mbrTmp2(MBRNULL);
  uint ind1, ind2;
  dist_t maxMBRDistance = 0;
  for (uint i = 0; i < MBRs.size(); i++) {
    for (uint j = i + 1; j < MBRs.size(); j++) {
      dist_t localDistance = MBRs[i] - MBRs[j];
      if (localDistance > maxMBRDistance) {
        mbrTmp1 = MBRs[i];
        mbrTmp2 = MBRs[j];
        ind1 = i;
        ind2 = j;
        maxMBRDistance = localDistance;
      }
    }
  }
  ///*///*////
  if (mbrTmp1.getIniLon() < mbrTmp2.getIniLon() ||
      (mbrTmp1.getIniLon() == mbrTmp2.getIniLon() &&
       mbrTmp1.getIniLat() < mbrTmp2.getIniLat())) {
    firstNode->insertMBR(mbrTmp1, children[ind1]);
    secondNode->insertMBR(mbrTmp2, children[ind2]);
    mbr1 *= mbrTmp1;
    mbr2 *= mbrTmp2;
  } else {
    firstNode->insertMBR(mbrTmp2, children[ind2]);
    secondNode->insertMBR(mbrTmp1, children[ind1]);
    mbr1 *= mbrTmp1;
    mbr2 *= mbrTmp2;
  }

  std::unordered_set<uint> inserted;
  inserted.insert(ind1);
  inserted.insert(ind2);
  bool toFirst;
  while (inserted.size() < MBRs.size()) {
    uint ind = 0;
    dist_t dist_min = -1;
    for (uint i = 0; i < MBRs.size(); i++) {
      if (inserted.count(i) > 0) continue;
      dist_t dist1 = mbr1 - MBRs[i];
      dist_t dist2 = mbr2 - MBRs[i];
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
    if (toFirst) {
      firstNode->insertMBR(MBRs[ind], children[ind]);
      mbr1 *= MBRs[ind];
    } else {
      secondNode->insertMBR(MBRs[ind], children[ind]);
      mbr2 *= MBRs[ind];
    }
    inserted.insert(ind);
    if (MBRs.size() - std::max(firstNode->getSize(), secondNode->getSize()) ==
        this->rootBucket.mintern)
      break;
  }
  toFirst = firstNode->getSize() < secondNode->getSize();
  if (inserted.size() < MBRs.size()) {
    for (uint i = 0; i < MBRs.size(); i++) {
      if (inserted.count(i) > 0) continue;
      if (toFirst) {
        firstNode->insertMBR(MBRs[i], children[i]);
        mbr1 *= MBRs[i];
      } else {
        secondNode->insertMBR(MBRs[i], children[i]);
        mbr2 *= MBRs[i];
      }
    }
  }
}
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

void RTree::rangeSearchRec(std::vector<Trip>& result, MBR& mbr,
                           uint nodeNumber) {
  // Cargar el node
  // std::cout << "nodeNumber: " << nodeNumber << "\n";
  this->loadNodeInNodePtr(nodeNumber);

  if (this->indexBucket.isNodeLeaf) {
    // Nodo hijo
    std::vector<Trip> trips = this->nodePtr->getTrips();
    for (Trip trip : trips) {
      if (mbr - trip == 0) {
        result.push_back(trip);
      }
    }
  } else {
    std::vector<uint> children = this->nodePtr->getChildren();
    std::vector<MBR> mbrs = this->nodePtr->getMBRs();
    for (uint i = 0; i < children.size(); i++) {
      if (mbr - mbrs[i] == 0) {
        rangeSearchRec(result, mbr, children[i]);
      }
    }
  }
}

}  // namespace spatial
}  // namespace utec