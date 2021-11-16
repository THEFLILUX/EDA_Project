#include <gtest/gtest.h>

#include <iostream>

// Referencia: file = "../tests/images/test8/img8";

// using namespace utec::spatial;

TEST(FirstTest, BasicTest) { EXPECT_TRUE(1); }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}