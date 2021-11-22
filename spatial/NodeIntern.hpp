#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

class NodeIntern : public NodeBase {
 public:
  NodeIntern() {}
  NodeIntern(std::string path, uint M = 0, uint m = 0) {
    // Load or download NodeIntern from path
  }
  std::string getPath() override { return ""; }

 private:
  uint M;
  uint m;
  std::vector<std::string> chilren;
  MBR mbr;

  void load() override {
    // load nodeLeaf info
  }
  void download() override {
    // load nodeLeaf info
  }
};

}  // namespace spatial
}  // namespace utec