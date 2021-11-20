#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

/**
 * RTree implementation
 */
class RTree {
 public:
  RTree() {}
  RTree(std::string path) : rootPath(path) {
    std::fstream f;
    f.open(path, std::ios::binary || std::ios::in);
    if (f.is_open()) {
      if (!is_empty(f)) {
        std::cout << "Load root.\n";
      } else
        std::cout << "Open file error in RTree root path.\n";
    }
  }

  void insertTrip(Trip trip) {}
  void rangeSearch(MBR mbr);

 private:
  NodeBase *root;
  std::string rootPath;  // file with root info
  bool isRootANodeLeaf;
};

}  // namespace spatial
}  // namespace utec
