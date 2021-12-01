#pragma once

#include <math.h>

#include <iostream>
#define dist_t double

namespace utec {
namespace spatial {

class Point {
 public:
  Point() : lon(0.0), lat(0.0) {}
  Point(dist_t lon, dist_t lat) : lon(lon), lat(lat) {}
  Point(const Point& o) : lon(o.lon), lat(o.lat) {}
  dist_t getLon() { return this->lon; }
  dist_t getLat() { return this->lat; }
  void setLon(dist_t lon) { this->lon = lon; }
  void setLat(dist_t lon) { this->lat = lat; }

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
  friend dist_t operator-(const Point& a, const Point& b) {
    return sqrt((a.lon - b.lon) * (a.lon - b.lon) +
                (a.lat - b.lat) * (a.lat - b.lat));
  }

 private:
  dist_t lon;
  dist_t lat;
};

}  // namespace spatial
}  // namespace utec