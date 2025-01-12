#include <Arduino.h>
#include "sd_card_hal_esp32.h"
#include <SD.h>
#include <SPI.h>

uint8_t SD_EN_GPIO = 16;
uint8_t SD_CS_GPIO = 18;
uint8_t SD_MISO_GPIO = 7;
uint8_t SD_MOSI_GPIO = 17;
uint8_t SD_SCK_GPIO = 15;

void init_SD_HAL(void) {
  pinMode(SD_EN_GPIO, OUTPUT);
  digitalWrite(SD_EN_GPIO, LOW);
  delay(10); // wait for the SD card to power up
  SPI.begin(SD_SCK_GPIO, SD_MISO_GPIO, SD_MOSI_GPIO, SD_CS_GPIO);
  if (!SD.begin(SD_CS_GPIO, SPI, 10000000)) { // 10 MHz
      Serial.println("No SD card found.");
      return;
  }
  else{
    Serial.println("SD card initialized successfully.");    
  }
}