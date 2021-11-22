#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

class NodeLeaf : public NodeBase {
 public:
  NodeLeaf() {}
  NodeLeaf(std::string path, uint M = 0) : path(path) {
    f.open(this->path, std::ios::binary | std::ios::in | std::ios::out);
    if (!f) {
      std::cout << "Not exists, creating file in nodeLeaf constructor\n";
      f.open(this->path, std::ios::binary | std::ios::in | std::ios::out);
      // New nodeLeaf
      this->M = M;
      this->m = M / 2;
      this->mbr = MBR(Point(-1, -1), Point(-1, -1));
      this->download();
    } else {
      // Load existing nodeLeaf
      std::cout << "Load nodeLeaf\n";
      this->load();
    }
    f.close();

    /*f.open(this->path, std::ios::binary | std::ios::in | std::ios::out);
    if (f.is_open()) {
      if (is_empty(f)) {
        // New nodeLeaf
        this->M = M;
        this->m = M / 2;
        this->mbr = MBR(Point(-1, -1), Point(-1, -1));
        this->download();
      } else {
        // Load existing nodeLeaf
        this->load();
      }
      f.close();
    } else
      std::cout << "Open file error in NodeLeaf constructor.\n";
      */
  }

  std::string getPath() override { return this->path; }
  uint getSize() { return this->trips.size(); }
  MBR getMBR() { return this->mbr; }
  void insertTrip(Trip trip) {
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

  void writeToFile() {
    std::cout << "Writing to file\n";
    f.open(this->path, std::ios::binary | std::ios::out | std::ios::trunc);
    if (f.is_open()) {
      // Format:M, m, mbr.ini.long, mbr.ini.lat, mbr.fin.long, mbr.fin.lat,
      //#trips (uint), trip: point.long, point.lat, uint(size of str),
      // char*, uint line
      this->download();
      f.close();
    } else
      std::cout << "Open file error in NodeLeaf writeToFIle\n";
  }

  void printNode() {
    std::cout << "\n";
    std::cout << "M: " << M << "\n";
    std::cout << "m: " << m << "\n";
    std::cout << "MBR: (" << mbr.getIniLon() << ", " << mbr.getIniLat() << ") ("
              << mbr.getFinLon() << ", " << mbr.getFinLat() << ")\n";
    std::cout << "path: " << path << "\n";
    std::cout << "trips: " << trips.size() << "\n";
    for (int i = 0; i < trips.size(); i++) {
      std::cout << "\tTrip " << i << ": (" << trips[i].getLon() << ", "
                << trips[i].getLat() << ") line: " << trips[i].getLine()
                << " path: " << trips[i].getPath() << "\n";
    }
  }

 private:
  uint M;
  uint m;
  MBR mbr;
  std::string path;
  std::vector<Trip> trips;
  std::fstream f;

  void load() override {
    // Format:M, m, mbr.ini.long, mbr.ini.lat, mbr.fin.long, mbr.fin.lat,
    //#trips (uint), trip: point.long, point.lat, uint(size of str),
    // char*, uint line
    this->trips.clear();
    double pointLon, pointLat;
    read(f, this->M);
    read(f, this->m);
    read(f, pointLon);
    read(f, pointLat);
    this->mbr.setIni(Point(pointLon, pointLat));
    read(f, pointLon);
    read(f, pointLat);
    this->mbr.setFin(Point(pointLon, pointLat));

    uint size;
    read(f, size);
    for (int i = 0; i < size; i++) {
      // Read trips
      read(f, pointLon);
      read(f, pointLat);

      uint sizeBuffer;
      std::string tripPath;
      read(f, sizeBuffer);
      char* buffer = new char[sizeBuffer + 1];
      f.read(buffer, sizeBuffer);
      buffer[sizeBuffer] = '\0';
      tripPath = buffer;
      delete buffer;

      uint line;
      read(f, line);

      Trip trip(pointLon, pointLat, tripPath, line);
      this->trips.push_back(trip);
    }
  }
  void download() override {
    write(f, this->M);
    write(f, this->m);
    double parDouble = this->mbr.getIni().getLon();
    write(f, parDouble);
    parDouble = this->mbr.getIni().getLat();
    write(f, parDouble);
    parDouble = this->mbr.getFin().getLon();
    write(f, parDouble);
    parDouble = this->mbr.getFin().getLat();
    write(f, parDouble);

    uint parUint = this->trips.size();
    write(f, parUint);

    for (Trip trip : trips) {
      parDouble = trip.getPoint().getLon();
      write(f, parDouble);
      parDouble = trip.getPoint().getLat();
      write(f, parDouble);

      parUint = trip.getPath().size();
      write(f, parUint);
      f.write(trip.getPath().c_str(), parUint);

      parUint = trip.getLine();
      write(f, parUint);
    }
  }
};

}  // namespace spatial
}  // namespace utec