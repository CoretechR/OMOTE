#ifndef __OMOTE_LOG_H__
#define __OMOTE_LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define OMOTE_LOG_LEVEL_NONE       (0)
#define OMOTE_LOG_LEVEL_ERROR      (1)
#define OMOTE_LOG_LEVEL_WARN       (2)
#define OMOTE_LOG_LEVEL_INFO       (3)
#define OMOTE_LOG_LEVEL_DEBUG      (4)
#define OMOTE_LOG_LEVEL_VERBOSE    (5)

#ifndef CONFIG_OMOTE_LOG_DEFAULT_LEVEL
#define CONFIG_OMOTE_LOG_DEFAULT_LEVEL OMOTE_LOG_LEVEL_NONE
#endif

#ifndef OMOTE_LOG_LEVEL
#define OMOTE_LOG_LEVEL CONFIG_OMOTE_LOG_DEFAULT_LEVEL
#endif

// Include the Arduino layer for all platforms
// This provides millis() for both Arduino and non-Arduino platforms
#include "applicationInternal/hardware/arduinoLayer.h"

// Platform-specific logging setup
#if defined(ARDUINO)
  #include <Arduino.h>
  #define OMOTE_LOG_PRINTF(format, ...) Serial.printf(format, ##__VA_ARGS__)
#else
  #include <stdio.h>
  #define OMOTE_LOG_PRINTF(format, ...) printf(format, ##__VA_ARGS__)
#endif

// Common format for all platforms
#define OMOTE_LOG_FORMAT(letter, format) "[OMOTE " #letter "][%8lu]: " format , (unsigned long) millis()

// Log level macros
#if OMOTE_LOG_LEVEL >= OMOTE_LOG_LEVEL_VERBOSE
#define omote_log_v(format, ...) OMOTE_LOG_PRINTF(OMOTE_LOG_FORMAT(V, format), ##__VA_ARGS__)
#else
#define omote_log_v(format, ...)  do {} while(0)
#endif

#if OMOTE_LOG_LEVEL >= OMOTE_LOG_LEVEL_DEBUG
#define omote_log_d(format, ...) OMOTE_LOG_PRINTF(OMOTE_LOG_FORMAT(D, format), ##__VA_ARGS__)
#else
#define omote_log_d(format, ...)  do {} while(0)
#endif

#if OMOTE_LOG_LEVEL >= OMOTE_LOG_LEVEL_INFO
#define omote_log_i(format, ...) OMOTE_LOG_PRINTF(OMOTE_LOG_FORMAT(I, format), ##__VA_ARGS__)
#else
#define omote_log_i(format, ...) do {} while(0)
#endif

#if OMOTE_LOG_LEVEL >= OMOTE_LOG_LEVEL_WARN
#define omote_log_w(format, ...) OMOTE_LOG_PRINTF(OMOTE_LOG_FORMAT(W, format), ##__VA_ARGS__)
#else
#define omote_log_w(format, ...) do {} while(0)
#endif

#if OMOTE_LOG_LEVEL >= OMOTE_LOG_LEVEL_ERROR
#define omote_log_e(format, ...) OMOTE_LOG_PRINTF(OMOTE_LOG_FORMAT(E, format), ##__VA_ARGS__)
#else
#define omote_log_e(format, ...) do {} while(0)
#endif

#if OMOTE_LOG_LEVEL >= OMOTE_LOG_LEVEL_NONE
#define omote_log_n(format, ...) OMOTE_LOG_PRINTF(OMOTE_LOG_FORMAT(E, format), ##__VA_ARGS__)
#else
#define omote_log_n(format, ...) do {} while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __OMOTE_LOG_H__ */
