#pragma once

#include "Point.hpp"

namespace utec {
namespace spatial {

class Trip {
 public:
  Trip() : point(Point(0.0, 0.0)), path(""), line(0) {}
  Trip(double lon, double lat, std::string path, uint line)
      : point(Point(lon, lat)), path(path), line(line) {}
  Trip(const Trip& o) : point(o.point), path(o.path), line(o.line) {}
  Trip& operator=(const Trip& o) {
    this->point = o.point;
    this->path = o.path;
    this->line = o.line;
    return *this;
  }

  std::string getPath() { return this->path; }
  uint getLine() { return this->line; }
  Point getPoint() { return this->point; }
  double getLon() { return this->point.getLon(); }
  double getLat() { return this->point.getLat(); }
  void setParam(double lon, double lat, std::string path, uint line) {
    this->point = Point(lon, lat);
    this->path = path;
    this->line = line;
  }

 private:
  Point point;
  std::string path;
  uint line;
};

}  // namespace spatial
}  // namespace utec