#pragma once

#include "Trip.hpp"

namespace utec {
namespace spatial {

class MBR {
 public:
  MBR() : ini(Point(0.0, 0.0)), fin(Point(0.0, 0.0)) {}
  MBR(dist_t iniLon, dist_t iniLat, dist_t finLon, dist_t finLat)
      : ini(Point(iniLon, iniLat)), fin(Point(finLon, finLat)) {}
  MBR(const MBR& o) : ini(o.ini), fin(o.fin) {}
  MBR(const Point& ini, const Point& fin) : ini(ini), fin(fin) {}
  MBR(const Point& point) : ini(point), fin(point) {}
  MBR& operator=(const MBR& o) {
    this->ini = o.ini;
    this->fin = o.fin;
    return *this;
  }
  MBR& operator*(Point point) {
    if (point.getLon() > fin.getLon()) {
      if (point.getLat() > fin.getLat()) {
        this->fin = Point(point.getLon(), point.getLat());
      } else if (point.getLat() < this->ini.getLat()) {
        this->ini = Point(this->ini.getLon(), point.getLat());
        this->fin = Point(point.getLon(), this->fin.getLat());
      } else {
        this->fin = Point(point.getLon(), this->fin.getLat());
      }
    } else if (point.getLon() < this->ini.getLon()) {
      if (point.getLat() > this->fin.getLat()) {
        this->ini = Point(point.getLon(), this->ini.getLat());
        this->fin = Point(this->fin.getLon(), point.getLat());
      } else if (point.getLat() < this->ini.getLat()) {
        this->ini = Point(point.getLon(), point.getLat());
      } else {
        this->ini = Point(point.getLon(), this->ini.getLat());
      }
    } else {
      if (point.getLat() > this->fin.getLat()) {
        this->fin = Point(this->fin.getLon(), point.getLat());
      } else if (point.getLat() < this->ini.getLat()) {
        this->ini = Point(this->ini.getLon(), point.getLat());
      } else {
        // Está dentro del mbr
      }
    }
    return *this;
  }
  MBR& operator*(Trip trip) {
    if (trip.getLon() > fin.getLon()) {
      if (trip.getLat() > fin.getLat()) {
        this->fin = Point(trip.getLon(), trip.getLat());
      } else if (trip.getLat() < this->ini.getLat()) {
        this->ini = Point(this->ini.getLon(), trip.getLat());
        this->fin = Point(trip.getLon(), this->fin.getLat());
      } else {
        this->fin = Point(trip.getLon(), this->fin.getLat());
      }
    } else if (trip.getLon() < this->ini.getLon()) {
      if (trip.getLat() > this->fin.getLat()) {
        this->ini = Point(trip.getLon(), this->ini.getLat());
        this->fin = Point(this->fin.getLon(), trip.getLat());
      } else if (trip.getLat() < this->ini.getLat()) {
        this->ini = Point(trip.getLon(), trip.getLat());
      } else {
        this->ini = Point(trip.getLon(), this->ini.getLat());
      }
    } else {
      if (trip.getLat() > this->fin.getLat()) {
        this->fin = Point(this->fin.getLon(), trip.getLat());
      } else if (trip.getLat() < this->ini.getLat()) {
        this->ini = Point(this->ini.getLon(), trip.getLat());
      } else {
        // Está dentro del mbr
      }
    }
    return *this;
  }
  void setIni(Point ini) { this->ini = ini; }
  void setIni(dist_t lon, dist_t lat) { this->ini = Point(lon, lat); }
  void setFin(Point fin) { this->fin = fin; }
  void setFin(dist_t lon, dist_t lat) { this->fin = Point(lon, lat); }
  Point getIni() { return this->ini; }
  Point getFin() { return this->fin; }
  dist_t getIniLon() { this->ini.getLon(); }
  dist_t getIniLat() { this->ini.getLat(); }
  dist_t getFinLon() { this->fin.getLon(); }
  dist_t getFinLat() { this->fin.getLat(); }
  friend dist_t operator-(MBR mbr, Trip trip) {
    if (trip.getLon() > mbr.getFinLon()) {
      if (trip.getLat() > mbr.getFinLat())
        return mbr.getFin() - trip;
      else if (trip.getLat() < mbr.getIniLat())
        return Point(mbr.getFinLon(), mbr.getIniLat()) - trip;
      else
        return trip.getLon() - mbr.getFinLon();
    } else if (trip.getLon() < mbr.getIniLon()) {
      if (trip.getLat() > mbr.getFinLat())
        return Point(mbr.getIniLon(), mbr.getFinLat()) - trip;
      else if (trip.getLat() < mbr.getIniLat())
        return mbr.getIni() - trip;
      else
        return mbr.getIniLon() - trip.getLon();
    } else {
      if (trip.getLat() > mbr.getFinLat())
        return trip.getLat() - mbr.getFinLat();
      else if (trip.getLat() < mbr.getIniLat())
        return mbr.getIniLat() - trip.getLat();
      else
        return 0.0;
    }
  }

 private:
  Point ini;
  Point fin;
};

}  // namespace spatial
}  // namespace utec