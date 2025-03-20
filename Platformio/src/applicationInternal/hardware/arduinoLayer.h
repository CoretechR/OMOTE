#pragma once

#include <stddef.h>

#if defined(ARDUINO)
  // for env:esp32 we need "Arduino.h" e.g. for Serial, delay(), millis()
  #include <Arduino.h>

#elif defined(WIN32) || defined(__linux__) || defined(__APPLE__)
  #include <stdint.h>
  // For Windows and Linux there is no Arduino framework available. So we have to simulate at least those very few calls to Arduino functions which are left in the code.
  // Note: Of course there is a lot more Arduino code in folder "hardware/ESP32/*", but this code is only active in case of esp32, so we don't have to simulate this in the Arduino layer if Windows/Linux is active.
  
  #ifdef __cplusplus
  extern "C" {
  #endif
  
  void delay(uint32_t ms);
  unsigned long millis();
  
  #ifdef __cplusplus
  }
  #endif
  
  class SerialClass {
  public:
    void begin(unsigned long);
    size_t printf(const char * format, ...)  __attribute__ ((format (printf, 2, 3)));
    size_t println(const char c[]);
    size_t println(int nr);
  };
  extern SerialClass Serial;

#endif
