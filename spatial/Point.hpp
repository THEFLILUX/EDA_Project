#pragma once

#include <math.h>

#include <iostream>
#define dist_t double
#define PI 3.14159265358979323846
#define EARTH_RADIUS_IN_METERS 6372797.560856

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
  void setLat(dist_t lat) { this->lat = lat; }
  void setParam(dist_t lon, dist_t lat) {
    this->lon = lon;
    this->lat = lat;
  }

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
    // return sqrt((a.lon - b.lon) * (a.lon - b.lon) + (a.lat - b.lat) * (a.lat
    // - b.lat));
    dist_t lat_arc = (a.lat - b.lat) * PI / 180;
    dist_t lon_arc = (a.lon - b.lon) * PI / 180;
    dist_t lat_h = sin(lat_arc * 0.5);
    dist_t lon_h = sin(lon_arc * 0.5);

    return EARTH_RADIUS_IN_METERS * 2.0 *
           asin(sqrt(lat_h * lat_h +
                     (cos(a.lat * PI / 180) * cos(b.lat * PI / 180)) * lon_h *
                         lon_h));
  }

 private:
  dist_t lon;
  dist_t lat;
};

}  // namespace spatial
}  // namespace utec