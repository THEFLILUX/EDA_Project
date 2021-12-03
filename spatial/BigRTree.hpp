#pragma once

#include "RTree.hpp"

namespace utec {
namespace spatial {

/**
 * BigRTree implementation
 */
class BigRTree {
 public:
  BigRTree(RTree &rtree) : rtree(rtree) {}
  ~BigRTree() {}
  void loadFile(std::string path) {
       /*
    std::fstream f;
    f.open(path);
    if (f.is_open()) {
      std::string s;
      while (f >> s) {
        std::cout << s << "\n";
      }
      f.close();
    } else
      std::cout << "Error al abrir " << path << "\n";
      */
  }
  void loadFiles(std::vector<std::string> paths) {}

 private:
  RTree &rtree;
};

}  // namespace spatial
}  // namespace utec