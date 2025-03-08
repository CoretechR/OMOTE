

#include <gmock/gmock.h>

#if (defined(IS_SIMULATOR))
int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  if (RUN_ALL_TESTS())
    ;
  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
#else
#include <Arduino.h>
void setup() {
  // MatthewColvin/OMOTE#9
  Serial.begin(115200);
  ::testing::InitGoogleMock();
}

void loop() {
  // Run tests
  if (RUN_ALL_TESTS())
    ;

  // sleep for 1 sec
  delay(1000);
}

#endif