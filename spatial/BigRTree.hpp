#pragma once

#include <unistd.h>

#include <sstream>

#include "RTree.hpp"
#include "csv.hpp"

using namespace csv;

namespace utec {
namespace spatial {

/**
 * BigRTree implementation
 */
class BigRTree {
 public:
  BigRTree(std::string path, RTree*& rtree) : rtree(rtree), path(path) {}
  ~BigRTree() {}
  int loadFile(std::string name, std::string strLon, std::string strLat) {
    CSVFormat format;
    format.delimiter(',').no_header();
    CSVReader reader(this->path + name, format);
    uint colLon = 0;
    uint colLat = 0;
    long first = 0;
    long last = 0;

    for (CSVRow& row : reader) {
      uint i = 0;
      for (CSVField& field : row) {
        if (colLon == 0 && field.get<>() == strLon) colLon = i;
        if (colLat == 0 && field.get<>() == strLat) colLat = i;
        i++;
        last += field.get<>().size();
        last += 1;
      }
      break;
    }
    last += 1;  // Debido al espacio al final del header

    int fila = 1;
    int rejected = 0;
    std::string lon, lat;
    Trip trip(name);
    for (CSVRow& row : reader) {
      first = last;
      uint i = 0;
      for (CSVField& field : row) {
        if (i == colLon) lon = field.get<>();
        if (i == colLat) lat = field.get<>();
        i++;
        last += field.get<>().size();
        last += 1;
      }
      last += 1;
      if (lon == "0" || lon == "" || lat == "0" || lat == "")
        rejected += 1;
      else {
        trip.setParamNotPath(std::stod(lon), std::stod(lat), first, last);
        this->rtree->insertTrip(trip);
      }
      std::cout << name << "\t" << fila++ << "\t" << std::stod(lon) << "\t"
                << std::stod(lat) << "\n";
    }
    std::cout << "Rejected: " << rejected << "\n";
    rtree->writeToFile();
    return rejected;
  }
  void loadFiles(std::vector<std::string> names, std::string strLon,
                 std::string strLat) {
    int totalrejected = 0;
    for (std::string name : names) {
      totalrejected += loadFile(name, strLon, strLat);
    }
    std::cout << "Total rejected: " << totalrejected << "\n";
  }

 private:
  RTree*& rtree;
  std::string path;
};

}  // namespace spatial
}  // namespace utec