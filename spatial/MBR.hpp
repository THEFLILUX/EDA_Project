#pragma once

#include "Trip.hpp"
#define MBRNULL -1
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
  MBR(dist_t coord) : ini(Point(coord, coord)), fin(Point(coord, coord)) {}
  MBR(dist_t coord1, dist_t coord2)
      : ini(Point(coord1, coord1)), fin(Point(coord2, coord2)) {}
  MBR& operator=(const MBR& o) {
    this->ini = o.ini;
    this->fin = o.fin;
    return *this;
  }
  friend bool operator==(const MBR& a, const MBR& b) {
    return a.ini == b.ini && a.fin == b.fin;
  }
  friend bool operator!=(const MBR& a, const MBR& b) {
    return a.ini != b.ini || a.fin != b.fin;
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
  double area() {
    return (this->fin.getLon() - this->ini.getLon()) *
           (this->fin.getLat() - this->ini.getLat());
  }
  void print() {
    std::cout << "(" << this->ini.getLon() << ", " << this->ini.getLat()
              << ") (" << this->fin.getLon() << ", " << this->fin.getLat()
              << ")";
  }
  friend std::ostream& operator<<(std::ostream& os, MBR& mbr) {
    os << "(" << mbr.getIniLon() << ", " << mbr.getIniLat() << ") ("
       << mbr.getFinLon() << ", " << mbr.getFinLat() << ")";
    return os;
  }
  /*
  Operador *=: Expande el mismo mbr dado un punto o trip
  */
  MBR& operator*=(Trip trip) {
    if (this->isNull()) {
      this->ini = trip.getPoint();
      this->fin = trip.getPoint();
      return *this;
    }
    (*this) *= trip.getPoint();
    /*
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
    }*/
    return *this;
  }
  MBR& operator*=(Point point) {
    if (this->isNull()) {
      this->ini = point;
      this->fin = point;
      return *this;
    }
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
  /*
  Operador *: Expande el mbr dado un punto o trip y devuelve otro
  */
  MBR operator*(Trip trip) {
    MBR newMBR(*this);
    if (this->isNull()) {
      std::cout << "\nERROR: está multiplicando un MBR nulo\n";
      return newMBR;
    }
    newMBR = (*this) * trip.getPoint();
    /*if (trip.getLon() > this->fin.getLon()) {
      if (trip.getLat() > this->fin.getLat()) {
        newMBR.setFin(Point(trip.getLon(), trip.getLat()));
      } else if (trip.getLat() < this->ini.getLat()) {
        newMBR.setIni(Point(this->ini.getLon(), trip.getLat()));
        newMBR.setFin(Point(trip.getLon(), this->fin.getLat()));
      } else {
        newMBR.setFin(Point(trip.getLon(), this->fin.getLat()));
      }
    } else if (trip.getLon() < this->ini.getLon()) {
      if (trip.getLat() > this->fin.getLat()) {
        newMBR.setIni(Point(trip.getLon(), this->ini.getLat()));
        newMBR.setFin(Point(this->fin.getLon(), trip.getLat()));
      } else if (trip.getLat() < this->ini.getLat()) {
        newMBR.setIni(Point(trip.getLon(), trip.getLat()));
      } else {
        newMBR.setIni(Point(trip.getLon(), this->ini.getLat()));
      }
    } else {
      if (trip.getLat() > this->fin.getLat()) {
        newMBR.setFin(Point(this->fin.getLon(), trip.getLat()));
      } else if (trip.getLat() < this->ini.getLat()) {
        newMBR.setIni(Point(this->ini.getLon(), trip.getLat()));
      } else {
        // Está dentro del mbr
      }
    }*/
    return newMBR;
  }
  MBR operator*(Point point) {
    MBR newMBR(*this);
    if (this->isNull()) {
      std::cout << "\nERROR: está multiplicando un MBR nulo\n";
      return newMBR;
    }
    if (point.getLon() > this->fin.getLon()) {
      if (point.getLat() > this->fin.getLat()) {
        newMBR.setFin(Point(point.getLon(), point.getLat()));
      } else if (point.getLat() < this->ini.getLat()) {
        newMBR.setIni(Point(this->ini.getLon(), point.getLat()));
        newMBR.setFin(Point(point.getLon(), this->fin.getLat()));
      } else {
        newMBR.setFin(Point(point.getLon(), this->fin.getLat()));
      }
    } else if (point.getLon() < this->ini.getLon()) {
      if (point.getLat() > this->fin.getLat()) {
        newMBR.setIni(Point(point.getLon(), this->ini.getLat()));
        newMBR.setFin(Point(this->fin.getLon(), point.getLat()));
      } else if (point.getLat() < this->ini.getLat()) {
        newMBR.setIni(Point(point.getLon(), point.getLat()));
      } else {
        newMBR.setIni(Point(point.getLon(), this->ini.getLat()));
      }
    } else {
      if (point.getLat() > this->fin.getLat()) {
        newMBR.setFin(Point(this->fin.getLon(), point.getLat()));
      } else if (point.getLat() < this->ini.getLat()) {
        newMBR.setIni(Point(this->ini.getLon(), point.getLat()));
      } else {
        // Está dentro del mbr
      }
    }
    return newMBR;
  }
  /*
  Operador -: Calcula la mínima distancia de un mbr a un trip o punto
  */
  friend dist_t operator-(MBR mbr, Trip trip) {
    if (mbr.isNull()) {
      std::cout << "\nERROR: está calculando distancia a un MBR nulo\n";
      return 0.0;
    }
    return mbr - trip.getPoint();
    /*
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
    }*/
  }
  friend dist_t operator-(MBR mbr, Point point) {
    if (mbr.isNull()) {
      std::cout << "\nERROR: está calculando distancia a un MBR nulo\n";
      return 0.0;
    }
    if (point.getLon() > mbr.getFinLon()) {
      if (point.getLat() > mbr.getFinLat())
        return mbr.getFin() - point;
      else if (point.getLat() < mbr.getIniLat())
        return Point(mbr.getFinLon(), mbr.getIniLat()) - point;
      else
        return point.getLon() - mbr.getFinLon();
    } else if (point.getLon() < mbr.getIniLon()) {
      if (point.getLat() > mbr.getFinLat())
        return Point(mbr.getIniLon(), mbr.getFinLat()) - point;
      else if (point.getLat() < mbr.getIniLat())
        return mbr.getIni() - point;
      else
        return mbr.getIniLon() - point.getLon();
    } else {
      if (point.getLat() > mbr.getFinLat())
        return point.getLat() - mbr.getFinLat();
      else if (point.getLat() < mbr.getIniLat())
        return mbr.getIniLat() - point.getLat();
      else
        return 0.0;
    }
  }
  /*
  Operador /: Calcula el incremento del mbr expandido por un trip o punto.
  */
  friend dist_t operator/(MBR mbr, Trip trip) {
    if (mbr.isNull()) {
      std::cout << "\nERROR: está calculando incremento de un MBR nulo\n";
      return 0.0;
    }
    return mbr / trip.getPoint();
    /*
    MBR newMBR(mbr);
    if (trip.getLon() > mbr.getFinLon()) {
      if (trip.getLat() > mbr.getFinLat()) {
        newMBR.setFin(Point(trip.getLon(), trip.getLat()));
      } else if (trip.getLat() < mbr.getIniLat()) {
        newMBR.setIni(Point(mbr.getIniLon(), trip.getLat()));
        newMBR.setFin(Point(trip.getLon(), mbr.getFinLat()));
      } else {
        newMBR.setFin(Point(trip.getLon(), mbr.getFinLat()));
      }
    } else if (trip.getLon() < mbr.getIniLon()) {
      if (trip.getLat() > mbr.getFinLat()) {
        newMBR.setIni(Point(trip.getLon(), mbr.getIniLat()));
        newMBR.setFin(Point(mbr.getFinLon(), trip.getLat()));
      } else if (trip.getLat() < mbr.getIniLat()) {
        newMBR.setIni(Point(trip.getLon(), trip.getLat()));
      } else {
        newMBR.setIni(Point(trip.getLon(), mbr.getIniLat()));
      }
    } else {
      if (trip.getLat() > mbr.getFinLat()) {
        newMBR.setFin(Point(mbr.getFinLon(), trip.getLat()));
      } else if (trip.getLat() < mbr.getIniLat()) {
        newMBR.setIni(Point(mbr.getIniLon(), trip.getLat()));
      } else {
        // Está dentro del mbr (Nunca entra acá)
      }
    }
    double areaIncrease = newMBR.area() - mbr.area();
    if (areaIncrease < 0) {
      std::cout << "\n\t\tERROR DIFERENCIA AREA NEGATIVA\n";
      std::cout << "\t" << mbr << "\n";
      std::cout << "\t" << newMBR << "\n";
      return -areaIncrease;
    }
    return areaIncrease;*/
  }
  friend dist_t operator/(MBR mbr, Point point) {
    if (mbr.isNull()) {
      std::cout << "\nERROR: está calculando incremento de un MBR nulo\n";
      return 0.0;
    }
    MBR newMBR(mbr);
    if (point.getLon() > mbr.getFinLon()) {
      if (point.getLat() > mbr.getFinLat()) {
        newMBR.setFin(Point(point.getLon(), point.getLat()));
      } else if (point.getLat() < mbr.getIniLat()) {
        newMBR.setIni(Point(mbr.getIniLon(), point.getLat()));
        newMBR.setFin(Point(point.getLon(), mbr.getFinLat()));
      } else {
        newMBR.setFin(Point(point.getLon(), mbr.getFinLat()));
      }
    } else if (point.getLon() < mbr.getIniLon()) {
      if (point.getLat() > mbr.getFinLat()) {
        newMBR.setIni(Point(point.getLon(), mbr.getIniLat()));
        newMBR.setFin(Point(mbr.getFinLon(), point.getLat()));
      } else if (point.getLat() < mbr.getIniLat()) {
        newMBR.setIni(Point(point.getLon(), point.getLat()));
      } else {
        newMBR.setIni(Point(point.getLon(), mbr.getIniLat()));
      }
    } else {
      if (point.getLat() > mbr.getFinLat()) {
        newMBR.setFin(Point(mbr.getFinLon(), point.getLat()));
      } else if (point.getLat() < mbr.getIniLat()) {
        newMBR.setIni(Point(mbr.getIniLon(), point.getLat()));
      } else {
        // Está dentro del mbr (Nunca entra acá)
      }
    }
    double areaIncrease = newMBR.area() - mbr.area();
    if (areaIncrease < 0) {
      std::cout << "\n\t\tERROR DIFERENCIA AREA NEGATIVA\n";
      std::cout << "\t" << mbr << "\n";
      std::cout << "\t" << newMBR << "\n";
      return -areaIncrease;
    }
    return areaIncrease;
  }
  /*
  Operador *= (MBR)): Expande el mbr actual dado otro mbr
  */
  MBR& operator*=(MBR mbr) {
    if (mbr.isNull()) {
      this->ini = mbr.getIni();
      this->fin = mbr.getIni();
      std::cout
          << "\nERROR: está calculando incremento a partir de un MBR nulo\n";
      return *this;
    }
    if (this->isNull()) {
      this->ini = mbr.getIni();
      this->fin = mbr.getIni();
      return *this;
    }
    (*this) *= mbr.getIni();
    (*this) *= mbr.getFin();
    return *this;
  }
  /*
  Operador - MBR: Calcula la mínima distancia entre MBRs
  */
  friend dist_t operator-(MBR mbr1, MBR mbr2) {
    if (mbr1.isNull() || mbr2.isNull()) {
      std::cout << "\nERROR: está calculando distancia entre algún MBR nulo\n";
      return 0.0;
    }
    Point pivote(mbr1.getIni());
    dist_t dist = mbr2 - pivote;
    pivote = mbr1.getFin();
    dist = std::min(dist, mbr2 - pivote);
    pivote.setParam(mbr1.getIniLon(), mbr1.getFinLat());
    dist = std::min(dist, mbr2 - pivote);
    pivote.setParam(mbr1.getFinLon(), mbr1.getIniLat());
    dist = std::min(dist, mbr2 - pivote);

    pivote = mbr2.getIni();
    dist = std::min(dist, mbr1 - pivote);
    pivote = mbr2.getFin();
    dist = std::min(dist, mbr1 - pivote);
    pivote.setParam(mbr2.getIniLon(), mbr2.getFinLat());
    dist = std::min(dist, mbr1 - pivote);
    pivote.setParam(mbr2.getFinLon(), mbr2.getIniLat());
    dist = std::min(dist, mbr1 - pivote);

    return dist;
  }

 private:
  Point ini;
  Point fin;
  bool isNull() {
    return ini.getLon() == MBRNULL && ini.getLat() == MBRNULL &&
           fin.getLon() == MBRNULL && fin.getLat() == MBRNULL;
  }
};

}  // namespace spatial
}  // namespace utec