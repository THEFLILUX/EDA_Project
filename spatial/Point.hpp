#pragma once

#include <iostream>

namespace utec {
namespace spatial {

class Point {
 public:
  Point() : lon(0.0), lat(0.0) {}
  Point(double lon, double lat) : lon(lon), lat(lat) {}
  Point(const Point& o) : lon(o.lon), lat(o.lat) {}
  double getLon() { return this->lon; }
  double getLat() { return this->lat; }
  void setLon(double lon) { this->lon = lon; }
  void setLat(double lon) { this->lat = lat; }

  Point& operator=(const Point& o) {
    this->lon = o.lon;
    this->lat = o.lat;
    return *this;
  }
  friend bool operator==(const Point& a, const Point& b) {
    return a.lon == b.lon && a.lat == b.lat;
  }
  friend bool operator!=(const Point& a, const Point& b) {
    return a.lon != b.lon || a.lat != b.lat;
  }

 private:
  double lon;
  double lat;
};

}  // namespace spatial
}  // namespace utec