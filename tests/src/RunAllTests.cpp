/**
 *  @brief RunAllTests
 *
 *  @author Dustin Biser
 */
#include "gtest/gtest.h"

int main(int argc, char** argv) {
  // Disables elapsed time by default.
  ::testing::GTEST_FLAG(print_time) = false;

  // This allows the user to override the flag on the command line.
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
