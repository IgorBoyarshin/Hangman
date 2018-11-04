// Unit tests for classes of Hangman project.

#include "../src/test.h"

#include "gtest/gtest.h"


TEST (SumTest, PositiveNums) {
    EXPECT_EQ (8, sum(3, 5));
    EXPECT_NE (4, sum(0, 3));
}


TEST (SumTest, NegativeNums) {
    EXPECT_EQ (-1, sum(2, -3));
    EXPECT_NE (-5, sum(-10, -10));
}


int main(int argc, char **argv)
{
  printf("Running main() from gtest.cpp\n");

  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
