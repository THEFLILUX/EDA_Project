#pragma once

#include "Point.hpp"
#define PATH_LENGTH 49
namespace utec {
namespace spatial {

class Trip {
 public:
  Trip() : point(Point(0.0, 0.0)), tripIni(0), tripOffset(0) {
    for (int i = 0; i < PATH_LENGTH; i++) this->path[i] = '.';
    this->path[PATH_LENGTH] = '\0';
  }
  Trip(dist_t lon, dist_t lat, std::string path, uint tripIni, uint tripOffset)
      : point(Point(lon, lat)), tripIni(tripIni), tripOffset(tripOffset) {
    for (int i = 0; i < PATH_LENGTH; i++) {
      if (i < path.size())
        this->path[i] = path[i];
      else
        this->path[i] = '.';
    }
    this->path[PATH_LENGTH] = '\0';
  }
  Trip(std::string path) {
    for (int i = 0; i < PATH_LENGTH; i++) {
      if (i < path.size())
        this->path[i] = path[i];
      else
        this->path[i] = '.';
    }
    this->path[PATH_LENGTH] = '\0';
    this->point.setLon(0);
    this->point.setLat(0);
    this->tripIni = 0;
    this->tripOffset = 0;
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
  Point& getPoint() { return this->point; }
  dist_t getLon() { return this->point.getLon(); }
  dist_t getLat() { return this->point.getLat(); }
  void setParam(dist_t lon, dist_t lat, std::string path, uint tripIni,
                uint tripOffset) {
    this->point = Point(lon, lat);
    strncpy(this->path, path.c_str(), path.size());
    this->tripIni = tripIni;
    this->tripOffset = tripOffset;
  }
  void setParamNotPath(dist_t lon, dist_t lat, uint tripIni, uint tripOffset) {
    this->point.setLon(lon);
    this->point.setLat(lat);
    this->tripIni = tripIni;
    this->tripOffset = tripOffset;
  }
  void print() {
    std::cout << " PATH: " << this->path << "(" << strlen(this->path) << ")\n";
  }
  friend dist_t operator-(const Trip& a, const Trip& b) {
    return a.point - b.point;
  }
  friend dist_t operator-(const Trip& a, const Point& b) { return a.point - b; }
  friend dist_t operator-(const Point& a, const Trip& b) { return a - b.point; }

 private:
  Point point;
  char path[PATH_LENGTH + 1];
  uint tripIni;
  uint tripOffset;
};

}  // namespace spatial
}  // namespace utec