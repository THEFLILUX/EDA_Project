#pragma once

#include "NodeBase.hpp"

namespace utec {
namespace spatial {

class NodeIntern : public NodeBase {
 public:
  NodeIntern() {}
  NodeIntern(std::string path, uint M = 0, uint m = 0) {
    f.open(this->path, std::ios::binary | std::ios::in | std::ios::out);
    if (!f) {
      std::cout << "Not exists, creating file in nodeIntern constructor\n";
      f.open(this->path, std::ios::binary | std::ios::in | std::ios::out);
      // New nodeIntern
      this->M = M;
      this->m = M / 2;
      this->download();
    } else {
      // Load existing nodeIntern
      std::cout << "Load nodeIntern\n";
      this->load();
    }
    f.close();
  }
  std::string getPath() override { return ""; }

  void printNode() override {}
  void insertTrip(Trip trip) override {}

  void readToFile() override {}
  void writeToFile() override {}

  uint getSize() override { return 0; }

  void insertMBR(MBR mbr){
    if(!MBRs.empty()){
      this->MBRs.push_back(mbr);
      this->children.push_back(this->MBRs.size());
    } else{
      std::cout << "Split nodeIntern";
    }
  }

 private:
  uint M;
  uint m;
  std::vector<MBR> MBRs;
  std::vector<uint> children;
  std::string path;
  std::fstream f;

  void load() override {
    // load nodeLeaf info
  }

  void download() override {
    write(f, this->M);
    write(f, this->m);
    for (size_t i = 0; i < this->MBRs.size(); i++){
      double parDouble = this->MBRs[i].getIniLon();
      write(f, parDouble);
      parDouble = this->MBRs[i].getIniLat();
      write(f, parDouble);
      parDouble = this->MBRs[i].getFinLon();
      write(f, parDouble);
      parDouble = this->MBRs[i].getFinLat();
      write(f, parDouble);
    }

    for (size_t i = 0; i < this->children.size(); i++){
      int childrenID = i;
      this->children[i] = i;
      write(f, childrenID);
    }
    
  }
};

}  // namespace spatial
}  // namespace utec