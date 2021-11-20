#pragma once

#include "Point.hpp"

namespace utec {
namespace spatial {

class MBR {
 public:
  MBR() : ini(Point(0.0, 0.0)), fin(Point(0.0, 0.0)) {}
  MBR(long iniLon, long iniLat, long finLon, long finLat)
      : ini(Point(iniLon, iniLat)), fin(Point(finLon, finLat)) {}
  MBR(const MBR& o) : ini(o.ini), fin(o.fin) {}
  MBR& operator=(const MBR& o) {
    this->ini = o.ini;
    this->fin = o.fin;
    return *this;
  }

 private:
  Point ini;
  Point fin;
};

}  // namespace spatial
}  // namespace utec