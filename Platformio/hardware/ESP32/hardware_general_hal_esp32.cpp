#include <Arduino.h>
#include <Wire.h>
#include "tft_hal_esp32.h"

uint8_t SDA_GPIO = 19;
uint8_t SCL_GPIO = 22;

void init_hardware_general_HAL(void) {
  // Make sure ESP32 is running at full speed
  setCpuFrequencyMhz(240);
  
  // For I2C to work correctly, the tft has to be powered.
  // Otherwise the IMU cannot be initialized.
  // The tft touch controller, being on the same I2C bus, seems to disturb if not powered.
  pinMode(LCD_EN_GPIO, OUTPUT);
  digitalWrite(LCD_EN_GPIO, LOW);
  
  // SDA and SCL need to be set explicitly, because for IMU you cannot set it explicitly in the constructor.
  // Configure i2c pins and set frequency to 400kHz
  Wire.begin(SDA_GPIO, SCL_GPIO, 400000);

}
