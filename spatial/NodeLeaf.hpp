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
      this->mbr = MBR(Point(-1, -1));

      this->nodeLeafBucket.M = M;
      this->nodeLeafBucket.m = M / 2;
      this->nodeLeafBucket.vectorsSize = 0;
      this->download(isNew);
    } else {
      // Load existing nodeLeaf
      std::cout << "Load nodeLeaf\n";
      this->load();
    }
  }

  uint getSize() override { return this->trips.size(); }
  MBR getMBR() { return this->mbr; }

  void insertTrip(Trip trip) override {
    this->trips.push_back(trip);
    if (trips.size() == 1) {
      this->mbr = MBR(trip.getPoint());
    } else {
      if (trip.getLon() > this->mbr.getFinLon()) {
        if (trip.getLat() > this->mbr.getFinLat()) {
          this->mbr.setFin(trip.getLon(), trip.getLat());
        } else if (trip.getLat() < this->mbr.getIniLat()) {
          this->mbr.setIni(this->mbr.getIniLon(), trip.getLat());
          this->mbr.setFin(trip.getLon(), this->mbr.getFinLat());
        } else {
          this->mbr.setFin(trip.getLon(), this->mbr.getFinLat());
        }
      } else if (trip.getLon() < this->mbr.getIni().getLon()) {
        if (trip.getLat() > this->mbr.getFinLat()) {
          this->mbr.setIni(trip.getLon(), this->mbr.getIniLat());
          this->mbr.setFin(this->mbr.getFinLon(), trip.getLat());
        } else if (trip.getLat() < this->mbr.getIniLat()) {
          this->mbr.setIni(trip.getLon(), trip.getLat());
        } else {
          this->mbr.setIni(trip.getLon(), this->mbr.getIniLat());
        }
      } else {
        if (trip.getLat() > this->mbr.getFinLat()) {
          this->mbr.setFin(this->mbr.getFinLon(), trip.getLat());
        } else if (trip.getLat() < this->mbr.getIniLat()) {
          this->mbr.setIni(this->mbr.getIniLon(), trip.getLat());
        } else {
          // Está dentro del mbr
        }
      }
    }
  }
  void readToFile() override {}

  void writeToFile() override {
    std::cout << "Writing NodeLeaf to file\n";
    this->download(false);
  }

  void printNode() override {
    std::cout << "\nNode Leaf\n";
    std::cout << "nodeID: " << this->nodeID << "\n";
    std::cout << "MBR: (" << this->mbr.getIniLon() << ", "
              << this->mbr.getIniLat() << ") (" << this->mbr.getFinLon() << ", "
              << this->mbr.getFinLat() << ")\n";
    std::cout << "\tPrint bucket:\n";
    this->nodeLeafBucket.print();
    for (int i = 0; i < trips.size(); i++) {
      std::cout << "\tTrip " << i << ": (" << trips[i].getLon() << ", "
                << trips[i].getLat() << ") tripInit: " << trips[i].getTripInit()
                << " tripOffset: " << trips[i].getTripOffset();
      trips[i].print();
    }
  }

 private:
  uint nodeID;
  NodeBucket nodeLeafBucket;
  std::vector<Trip> trips;
  MBR mbr;
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

    this->mbr = MBR(this->nodeLeafBucket.iniLon, this->nodeLeafBucket.iniLat,
                    this->nodeLeafBucket.finLon, this->nodeLeafBucket.finLat);
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
    this->nodeLeafBucket.iniLon = this->mbr.getIniLon();
    this->nodeLeafBucket.iniLat = this->mbr.getIniLat();
    this->nodeLeafBucket.finLon = this->mbr.getFinLon();
    this->nodeLeafBucket.finLat = this->mbr.getFinLat();

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