#pragma once

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
  BigRTree(RTree& rtree) : rtree(rtree) {}
  ~BigRTree() {}
  void loadFile(std::string path) {
    CSVReader reader(path);
    for (CSVRow& row : reader) {  // Input iterator
      for (CSVField& field : row) {
        // By default, get<>() produces a std::string.
        // A more efficient get<string_view>() is also available, where the
        // resulting string_view is valid as long as the parent CSVRow is alive
        std::cout << field.get<>() << "\n";
      }
    }
    /*
        std::fstream f;
        f.open(path);
        if (f.is_open()) {
          std::string s;
          f.seekg(2);

          int tam = 5;
          char* buffer = new char[tam + 1];
          f.read(buffer, tam);
          buffer[tam] = '\0';
          s = buffer;
          delete buffer;

          std::cout << s << "-\n";

          f.close();
        } else
          std::cout << "Error al abrir " << path << "\n";*/
  }
  void loadFiles(std::vector<std::string> paths) {}

 private:
  RTree& rtree;
};

}  // namespace spatial
}  // namespace utec