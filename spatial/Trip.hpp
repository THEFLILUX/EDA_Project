#pragma once

#include "Point.hpp"
#define PATH_LENGTH 15
namespace utec {
namespace spatial {

class Trip {
 public:
  Trip() : point(Point(0.0, 0.0)), tripIni(0), tripOffset(0) {
    for (int i = 0; i < PATH_LENGTH; i++) this->path[i] = '.';
    this->path[PATH_LENGTH] = '\0';
  }
  Trip(double lon, double lat, std::string path, uint tripIni, uint tripOffset)
      : point(Point(lon, lat)), tripIni(tripIni), tripOffset(tripOffset) {
    for (int i = 0; i < PATH_LENGTH; i++) {
      if (i < path.size())
        this->path[i] = path[i];
      else
        this->path[i] = '.';
    }
    this->path[PATH_LENGTH] = '\0';
  }
  Trip& operator=(const Trip& o) {
    this->point = o.point;
    for (int i = 0; i < PATH_LENGTH; i++) this->path[i] = o.path[i];
    this->path[PATH_LENGTH] = '\0';
    this->tripIni = o.tripIni;
    this->tripOffset = o.tripOffset;
    return *this;
  }

  std::string getPath() { return this->path; }
  uint getTripInit() { return this->tripIni; }
  uint getTripOffset() { return this->tripOffset; }
  Point getPoint() { return this->point; }
  double getLon() { return this->point.getLon(); }
  double getLat() { return this->point.getLat(); }
  void setParam(double lon, double lat, std::string path, uint tripIni,
                uint tripOffset) {
    this->point = Point(lon, lat);
    strncpy(this->path, path.c_str(), path.size());
    this->tripIni = tripIni;
    this->tripOffset = tripOffset;
  }
  friend double operator-(const Trip& a, const Trip& b) {
    return a.point - b.point;
  }
  void print() {
    std::cout << " PATH: " << this->path << "(" << strlen(this->path) << ")\n";
  }

 private:
  Point point;
  char path[PATH_LENGTH + 1];
  uint tripIni;
  uint tripOffset;
};

}  // namespace spatial
}  // namespace utec