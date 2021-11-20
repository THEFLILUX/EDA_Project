#pragma once

#include "Point.hpp"

namespace utec {
namespace spatial {

class Trip {
 public:
  Trip() : point(Point(0.0, 0.0)), path(""), line(0) {}
  Trip(double lon, double lat, std::string path, uint line)
      : point(Point(lon, lat)), path(path), line(line) {}
  std::string getPath() { return this->path; }
  uint getLine() { return this->line; }

 private:
  Point point;
  std::string path;
  uint line;
};

}  // namespace spatial
}  // namespace utec