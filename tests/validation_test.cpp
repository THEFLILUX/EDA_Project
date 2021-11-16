#include <gtest/gtest.h>

#include <iostream>

#include "PNMImage.hpp"
#include "PRQuadTreeImage.hpp"

// Referencia: file = "../tests/images/test8/img8";

using namespace utec::spatial;

TEST(QuadTest8, BasicTest) { EXPECT_TRUE(1); }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}