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
  void loadFile(std::string path, std::string strLon, std::string strLat) {
    /*std::ifstream file(path);
    std::string line;
    long first = 0;
    long last = 0;

    std::getline(file, line);
    last += line.size();
    last += 1;
    int fila = 1;
    int rejected = 0;
    while (std::getline(file, line)) {
      first = last;
      last += line.size() + 1;
      for (int i = 0; i < 5; i++) {
        line.replace(0, line.find(",") + 1, "");
      };
      std::string lon = line.substr(0, line.find(","));
      line.replace(0, line.find(",") + 1, "");
      std::string lat = line.substr(0, line.find(","));
      std::cout << fila++ << "\n";
      if (lon == "0" || lon == "" || lat == "0" || lat == "")
        rejected += 1;
      else {
        Trip* trip =
            new Trip(std::stod(lon), std::stod(lat), path, first, last);
        this->rtree->insertTrip(trip);
      }
    }
    std::cout << "Rejected: " << rejected << "\n";
    rtree->writeToFile();*/

    CSVFormat format;
    format.delimiter(',').no_header();
    // CSVReader reader("../tests/RTree/testfew.csv", format);
    CSVReader reader(path, format);
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
    Trip trip(path);
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
      std::cout << fila++ << "\t" << std::stod(lon) << "\t" << std::stod(lat)
                << "\t" << first << "\t" << last << "\n";
    }
    std::cout << "Rejected: " << rejected << "\n";
    rtree->writeToFile();
  }
  void loadFiles(std::vector<std::string> paths, std::string strLon,
                 std::string strLat) {
    for (auto path : paths) {
      loadFile(path, strLon, strLat);
    }
  }

 private:
  RTree*& rtree;
  std::string path;
};

}  // namespace spatial
}  // namespace utec