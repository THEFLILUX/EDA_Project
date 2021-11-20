#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

class NodeLeaf : public NodeBase {
 public:
  NodeLeaf() {}
  NodeLeaf(std::string path, bool load = false) : path(path) {
    // Load nodeLeaf from path
    if (load) {
        }
  }
  void load() override {
    // load nodeLeaf info
  }
  std::string getPath() override { return this->path; }

 private:
  uint M;
  uint m;
  MBR mbr;
  std::string path;
  std::vector<Trip> trips;
};

}  // namespace spatial
}  // namespace utec