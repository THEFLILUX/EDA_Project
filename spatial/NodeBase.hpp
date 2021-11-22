#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "MBR.hpp"
#include "Trip.hpp"

namespace utec {
namespace spatial {

class NodeBase {
 public:
  virtual std::string getPath() = 0;

 private:
  virtual void load() = 0;
  virtual void download() = 0;
};

template <typename T>
void write(std::fstream& f, T& t) {
  f.write((char*)&t, sizeof(t));
}

template <typename T>
void read(std::fstream& f, T& t) {
  f.read((char*)&t, sizeof(t));
}

bool is_empty(std::fstream& f) {
  return f.peek() == std::fstream::traits_type::eof();
}

}  // namespace spatial
}  // namespace utec