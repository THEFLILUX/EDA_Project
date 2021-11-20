#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

class NodeIntern : public NodeBase {
 public:
  NodeIntern() {}
  NodeIntern(std::string path) {
    // Load NodeIntern from path
  }

 private:
  uint M;
  uint m;
  std::vector<std::string> chilren;
  MBR mbr;
};

}  // namespace spatial
}  // namespace utec