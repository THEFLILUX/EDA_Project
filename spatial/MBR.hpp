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
  MBR(const Point& ini, const Point& fin) : ini(ini), fin(fin) {}
  MBR(const Point& point) : ini(point), fin(point) {}
  MBR& operator=(const MBR& o) {
    this->ini = o.ini;
    this->fin = o.fin;
    return *this;
  }
  void setIni(Point ini) { this->ini = ini; }
  void setIni(double lon, double lat) { this->ini = Point(lon, lat); }
  void setFin(Point fin) { this->fin = fin; }
  void setFin(double lon, double lat) { this->fin = Point(lon, lat); }
  Point getIni() { return this->ini; }
  Point getFin() { return this->fin; }
  double getIniLon() { this->ini.getLon(); }
  double getIniLat() { this->ini.getLat(); }
  double getFinLon() { this->fin.getLon(); }
  double getFinLat() { this->fin.getLat(); }

 private:
  Point ini;
  Point fin;
};

}  // namespace spatial
}  // namespace utec