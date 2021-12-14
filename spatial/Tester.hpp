#pragma once

#include <unordered_map>

#include "RTree.hpp"

namespace utec {
namespace spatial {

/**
 * Tester implementation
 */
class Tester {
 public:
  Tester(std::string path, RTree*& rtree) : path(path), rtree(rtree) {}
  void test(Point point1, Point point2, bool printing = false) {
    std::vector<Trip> trips = rtree->rangeSearch(point1, point2);
    std::cout << "Result: " << trips.size() << "\n\n";

    if (printing) {
      std::unordered_map<std::string, std::vector<Trip>> tripsByFile;
      for (Trip trip : trips) {
        if (tripsByFile.count(trip.getPath())) {
          tripsByFile[trip.getPath()].push_back(trip);
        } else {
          std::vector<Trip> tripsInFile;
          tripsInFile.push_back(trip);
          tripsByFile[trip.getPath()] = tripsInFile;
        }
      }
      std::cout << "Number of files in result: " << tripsByFile.size() << "\n";
      for (auto& pair : tripsByFile) {
        std::fstream f;
        std::string localPath = this->path + pair.first;

        // Processing path
        while (localPath[localPath.size() - 1] == '.') {
          localPath = localPath.substr(0, localPath.size() - 1);
        }

        std::cout << "\n\t\tOpening " << localPath << "\n";
        f.open(localPath, std::ios::in);
        if (f.is_open()) {
          for (Trip& trip : pair.second) {
            /*std::cout << std::setprecision(17) << trip.getLon() << ","
                      << std::setprecision(17) << trip.getLat() << "\t"
                      << trip.getTripInit() << "," << trip.getTripOffset()
                      << "\n";*/
            uint ini = trip.getTripInit();
            uint fin = trip.getTripOffset();
            f.seekg(ini, std::ios::beg);
            uint tam = fin - ini - 1;
            char* buffer = new char[tam + 1];
            f.read(buffer, tam);
            buffer[tam] = '\0';
            std::string row = buffer;
            delete[] buffer;

            // Doing something with row:
            if (row.find("\n") != std::string::npos) {
              uint red =
                  row.substr(row.find("\n"), row.length() - row.find("\n"))
                      .length();
              ini -= red;
              fin -= red;
              f.seekg(ini, std::ios::beg);
              uint tam = fin - ini - 1;
              char* buffer = new char[tam + 1];
              f.read(buffer, tam);
              buffer[tam] = '\0';
              row = buffer;
              delete[] buffer;
            }
            std::cout << row << "\n";
          }
          f.close();
        } else {
          std::cout << pair.first << " not open\n";
        }
      }
      tripsByFile.clear();
    }
    trips.clear();
  }

 private:
  RTree*& rtree;
  std::string path;
};

}  // namespace spatial
}  // namespace utec