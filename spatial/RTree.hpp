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

  void insertTrip(Trip& trip) {
    if (this->rootBucket.isRootANodeLeaf) {
      this->nodePtr->getTrips().push_back(trip);
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

        MBR* mbr1 = new MBR(MBRNULL);
        MBR* mbr2 = new MBR(MBRNULL);

        this->splitNodeLeaf(this->nodePtr, newNodeLeaf, mbr1, mbr2);

        newNodeIntern->getMBRs().push_back(*mbr1);
        newNodeIntern->getChildren().push_back(this->nodePtr->getNodeID());
        newNodeIntern->getMBRs().push_back(*mbr2);
        newNodeIntern->getChildren().push_back(newNodeLeaf->getNodeID());

        delete mbr1;
        delete mbr2;
        mbr1 = nullptr;
        mbr2 = nullptr;

        // Regresar a memoria secundaria
        this->close();
        newNodeLeaf->writeToFile();
        newNodeIntern->writeToFile();
        delete newNodeLeaf;
        delete newNodeIntern;
        newNodeLeaf = nullptr;
        newNodeIntern = nullptr;
        this->open();

        // Cargo el root
        this->loadNodeInNodePtr(this->rootBucket.rootNumber);
        // std::cout << "NodeLeaf root finish split\n";
      }
    } else {
      MBR *mbr1 = nullptr, *mbr2 = nullptr;
      this->insertTripRec(this->rootBucket.rootNumber, trip, mbr1, mbr2);
      if (mbr1) {
        // Hubo un split, nuevo nodo raíz
        this->close();
        this->loadNodeInNodePtr(this->rootBucket.rootNumber);
        this->rootBucket.rootNumber = this->rootBucket.nextNodeNumber;

        NodeBase* newNodeInternRoot = new NodeIntern(
            this->rootPath + this->indexName, this->rootPath + this->dataName,
            this->rootBucket.nextNodeNumber++, this->rootBucket.Mintern, true);
        this->open();

        newNodeInternRoot->getMBRs().push_back(*mbr1);
        newNodeInternRoot->getChildren().push_back(this->nodePtr->getNodeID());
        newNodeInternRoot->getMBRs().push_back(*mbr2);
        newNodeInternRoot->getChildren().push_back(this->rootBucket.rootNumber -
                                                   1);
        delete mbr1;
        delete mbr2;
        mbr1 = nullptr;
        mbr2 = nullptr;

        // Regresar a memoria secundaria
        this->close();
        newNodeInternRoot->writeToFile();
        delete newNodeInternRoot;
        newNodeInternRoot = nullptr;
        this->nodePtr->writeToFile();
        this->open();

        this->loadNodeInNodePtr(this->rootBucket.rootNumber);
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
    this->nodePtr = nullptr;
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

  void insertTripRec(uint nodeNumber, Trip& trip, MBR*& mbr1, MBR*& mbr2);
  void splitNodeLeaf(NodeBase*& firstNode, NodeBase*& secondNode, MBR*& mbr1,
                     MBR*& mbr2);
  void splitNodeIntern(NodeBase*& firstNode, NodeBase*& secondNode, MBR*& mbr1,
                       MBR*& mbr2);
  void printRec(uint nodeNumber);
  void rangeSearchRec(std::vector<Trip>& result, MBR& mbr, uint nodeNumber);

  void afterNodeInternSplit(uint& nodeNumber, uint& i, MBR*& mbr1Tmp,
                            MBR*& mbr2Tmp, MBR*& mbr1, MBR*& mbr2) {
    this->loadNodeInNodePtr(nodeNumber);

    this->nodePtr->getMBRs()[i] = *mbr1Tmp;
    this->nodePtr->getMBRs().push_back(*mbr2Tmp);
    this->nodePtr->getChildren().push_back(this->rootBucket.nextNodeNumber - 1);

    delete mbr1Tmp;
    delete mbr2Tmp;
    mbr1Tmp = nullptr;
    mbr2Tmp = nullptr;

    if (this->nodePtr->getSize() == this->rootBucket.Mintern + 1) {
      // Split intern node
      // std::cout << "NodeIntern split required\n";
      this->close();
      NodeBase* newNodeIntern = new NodeIntern(
          this->rootPath + this->indexName, this->rootPath + this->dataName,
          this->rootBucket.nextNodeNumber++, this->rootBucket.Mintern, true);
      this->open();
      MBR* mbr1Tmp2 = new MBR(MBRNULL);
      MBR* mbr2Tmp2 = new MBR(MBRNULL);

      this->splitNodeIntern(this->nodePtr, newNodeIntern, mbr1Tmp2, mbr2Tmp2);

      mbr1 = mbr1Tmp2;
      mbr2 = mbr2Tmp2;

      // Regresar a memoria secundaria nuevo nodo
      this->close();
      newNodeIntern->writeToFile();
      this->nodePtr->writeToFile();
      delete newNodeIntern;
      newNodeIntern = nullptr;
      this->open();
    }
  }
};

void RTree::insertTripRec(uint nodeNumber, Trip& trip, MBR*& mbr1, MBR*& mbr2) {
  // Cargar el node
  this->loadNodeInNodePtr(nodeNumber);

  if (this->indexBucket.isNodeLeaf) {
    // Nodo Hoja
    this->nodePtr->getTrips().push_back(trip);
    if (this->nodePtr->getSize() == this->rootBucket.Mleaf + 1) {
      // std::cout << "NodeLeaf split required\n";

      this->close();
      NodeBase* newNodeLeaf = new NodeLeaf(
          this->rootPath + this->indexName, this->rootPath + this->dataName,
          this->rootBucket.nextNodeNumber++, this->rootBucket.Mleaf, true);
      this->open();

      MBR* mbr1Tmp = new MBR(MBRNULL);
      MBR* mbr2Tmp = new MBR(MBRNULL);
      this->splitNodeLeaf(this->nodePtr, newNodeLeaf, mbr1Tmp, mbr2Tmp);
      // std::cout << "Fin leaf node split\n";

      // Regresar a memoria secundaria nuevo nodo
      this->close();
      newNodeLeaf->writeToFile();
      this->nodePtr->writeToFile();
      delete newNodeLeaf;
      newNodeLeaf = nullptr;
      this->open();

      mbr1 = new MBR(MBRNULL);
      mbr2 = new MBR(MBRNULL);
      mbr1 = mbr1Tmp;
      mbr2 = mbr2Tmp;
    }
  } else {
    // Nodo intern
    for (uint i = 0; i < this->nodePtr->getSize(); i++) {
      if (this->nodePtr->getMBRs()[i] - trip == 0) {
        MBR* mbr1Tmp = nullptr;
        MBR* mbr2Tmp = nullptr;

        this->insertTripRec(this->nodePtr->getChildren()[i], trip, mbr1Tmp,
                            mbr2Tmp);

        if (mbr1Tmp) {
          // Hubo un split
          this->afterNodeInternSplit(nodeNumber, i, mbr1Tmp, mbr2Tmp, mbr1,
                                     mbr2);
        }
        return;
      }
    }
    // No entró en ningún MBR, escoger el MBR que se expanda menos
    double minExpansion = this->nodePtr->getMBRs()[0] / trip;
    uint ind = 0;
    for (uint i = 1; i < this->nodePtr->getSize(); i++) {
      double locExpansion = this->nodePtr->getMBRs()[i] / trip;
      if (locExpansion < minExpansion) {
        minExpansion = locExpansion;
        ind = i;
      }
    }
    this->nodePtr->getMBRs()[ind] = this->nodePtr->getMBRs()[ind] * trip;
    MBR *mbr1Tmp = nullptr, *mbr2Tmp = nullptr;

    this->insertTripRec(this->nodePtr->getChildren()[ind], trip, mbr1Tmp,
                        mbr2Tmp);

    if (mbr1Tmp) {
      // Hubo un split
      this->afterNodeInternSplit(nodeNumber, ind, mbr1Tmp, mbr2Tmp, mbr1, mbr2);
    }
  }
}
void RTree::splitNodeLeaf(NodeBase*& firstNode, NodeBase*& secondNode,
                          MBR*& mbr1, MBR*& mbr2) {
  // FirsNode tiene los Trips a dividir
  std::vector<Trip> trips(firstNode->getTrips().size());
  std::copy(firstNode->getTrips().begin(), firstNode->getTrips().end(),
            trips.begin());
  firstNode->getTrips().clear();
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
    firstNode->getTrips().push_back(trip1);
    secondNode->getTrips().push_back(trip2);
    *mbr1 *= trip1;
    *mbr2 *= trip2;
  } else {
    firstNode->getTrips().push_back(trip2);
    secondNode->getTrips().push_back(trip1);
    *mbr1 *= trip2;
    *mbr2 *= trip1;
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
      dist_t dist1 = *mbr1 - trips[i];
      dist_t dist2 = *mbr2 - trips[i];
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
      firstNode->getTrips().push_back(trips[ind]);
      *mbr1 *= trips[ind];
    } else {
      secondNode->getTrips().push_back(trips[ind]);
      *mbr2 *= trips[ind];
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
        firstNode->getTrips().push_back(trips[i]);
        *mbr1 *= trips[i];
      } else {
        secondNode->getTrips().push_back(trips[i]);
        *mbr2 *= trips[i];
      }
    }
  }
  trips.clear();
  inserted.clear();
}

void RTree::splitNodeIntern(NodeBase*& firstNode, NodeBase*& secondNode,
                            MBR*& mbr1, MBR*& mbr2) {
  std::vector<MBR> MBRs(firstNode->getChildren().size());
  std::vector<uint> children(firstNode->getChildren().size());
  std::copy(firstNode->getMBRs().begin(), firstNode->getMBRs().end(),
            MBRs.begin());
  std::copy(firstNode->getChildren().begin(), firstNode->getChildren().end(),
            children.begin());
  firstNode->getMBRs().clear();
  firstNode->getChildren().clear();

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
  if (mbrTmp1.getIniLon() < mbrTmp2.getIniLon() ||
      (mbrTmp1.getIniLon() == mbrTmp2.getIniLon() &&
       mbrTmp1.getIniLat() < mbrTmp2.getIniLat())) {
    firstNode->getMBRs().push_back(mbrTmp1);
    firstNode->getChildren().push_back(children[ind1]);
    secondNode->getMBRs().push_back(mbrTmp2);
    secondNode->getChildren().push_back(children[ind2]);
    *mbr1 *= mbrTmp1;
    *mbr2 *= mbrTmp2;
  } else {
    firstNode->getMBRs().push_back(mbrTmp2);
    firstNode->getChildren().push_back(children[ind2]);
    secondNode->getMBRs().push_back(mbrTmp1);
    secondNode->getChildren().push_back(children[ind1]);
    *mbr1 *= mbrTmp1;
    *mbr2 *= mbrTmp2;
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
      dist_t dist1 = *mbr1 - MBRs[i];
      dist_t dist2 = *mbr2 - MBRs[i];
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
      firstNode->getMBRs().push_back(MBRs[ind]);
      firstNode->getChildren().push_back(children[ind]);
      *mbr1 *= MBRs[ind];
    } else {
      secondNode->getMBRs().push_back(MBRs[ind]);
      secondNode->getChildren().push_back(children[ind]);
      *mbr2 *= MBRs[ind];
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
        firstNode->getMBRs().push_back(MBRs[i]);
        firstNode->getChildren().push_back(children[i]);
        *mbr1 *= MBRs[i];
      } else {
        secondNode->getMBRs().push_back(MBRs[i]);
        secondNode->getChildren().push_back(children[i]);
        *mbr2 *= MBRs[i];
      }
    }
  }
  MBRs.clear();
  children.clear();
  inserted.clear();
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