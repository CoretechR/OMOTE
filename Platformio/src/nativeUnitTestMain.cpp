#include <gmock/gmock.h>
#include <gtest/gtest.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  if (RUN_ALL_TESTS())
    ;
  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
