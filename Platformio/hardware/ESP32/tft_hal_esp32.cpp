#include <Arduino.h>
#include "driver/ledc.h"
#include "tft_hal_esp32.h"
#include "sleep_hal_esp32.h"

uint8_t SDA_GPIO = 19;
uint8_t SCL_GPIO = 22;

uint8_t LCD_BL_GPIO = 4;
uint8_t LCD_EN_GPIO = 10;
uint8_t LCD_CS_GPIO = 5;
uint8_t LCD_MOSI_GPIO = 23;
uint8_t LCD_SCK_GPIO = 18;
uint8_t LCD_DC_GPIO = 9;

LGFX::LGFX(void) {
  {
    auto cfg = _bus_instance.config();
    cfg.freq_write = SPI_FREQUENCY;
    cfg.freq_read  = 16000000;
    cfg.dma_channel = SPI_DMA_CH_AUTO;
    cfg.pin_sclk = LCD_SCK_GPIO;
    cfg.pin_mosi = LCD_MOSI_GPIO;
    cfg.pin_dc   = LCD_DC_GPIO;
    _bus_instance.config(cfg);
    _panel_instance.setBus(&_bus_instance);
  }
  {
    auto cfg = _panel_instance.config();
    cfg.pin_cs           = LCD_CS_GPIO;
    cfg.pin_rst          = -1;
    cfg.pin_busy         = -1;
    cfg.memory_width     = SCR_WIDTH;
    cfg.memory_height    = SCR_HEIGHT;
    cfg.panel_width      = SCR_WIDTH;
    cfg.panel_height     = SCR_HEIGHT;
    cfg.offset_rotation  = 2;
    _panel_instance.config(cfg);
  }
  {
    auto cfg = _touch_instance.config();
    cfg.i2c_addr = 0x38;
    cfg.i2c_port = 0;
    cfg.pin_sda = SDA_GPIO;
    cfg.pin_scl = SCL_GPIO;
    cfg.freq = 400000;
    cfg.x_min = 0;
    cfg.x_max = SCR_WIDTH-1;
    cfg.y_min = 0;
    cfg.y_max = SCR_HEIGHT-1;
    _touch_instance.config(cfg);
    _panel_instance.setTouch(&_touch_instance);
  }
  setPanel(&_panel_instance);
}
LGFX tft;
byte backlightBrightness = 255;

void init_tft(void) {
  // Configure the backlight PWM
  // Manual setup because ledcSetup() briefly turns on the backlight
  ledc_channel_config_t ledc_channel_left;
  ledc_channel_left.gpio_num = (gpio_num_t)LCD_BL_GPIO;
  ledc_channel_left.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_channel_left.channel = LEDC_CHANNEL_5;
  ledc_channel_left.intr_type = LEDC_INTR_DISABLE;
  ledc_channel_left.timer_sel = LEDC_TIMER_1;
  // LEDC channel duty, the range of duty setting is [0, (2**duty_resolution)]
  ledc_channel_left.duty = 0;
  // needs to be set to 0, otherwise log message "E (324) ledc: ledc_set_duty_with_hpoint(699): hpoint argument is invalid"
  // https://github.com/mudassar-tamboli/ESP32-OV7670-WebSocket-Camera/issues/13
  // LEDC channel hpoint value, the max value is 0xfffff
  ledc_channel_left.hpoint = 0;
  ledc_channel_left.flags.output_invert = 1; // Can't do this with ledcSetup()
  // hpoint and duty explained:
  // https://miro.medium.com/v2/resize:fit:1400/1*ViqSTFdH9COZ51iKYrIyMA.png
  ledc_channel_config(&ledc_channel_left);

  ledc_timer_config_t ledc_timer;
  ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_timer.duty_resolution = LEDC_TIMER_8_BIT;
  ledc_timer.timer_num = LEDC_TIMER_1;
  ledc_timer.freq_hz = 640;
  // https://github.com/mudassar-tamboli/ESP32-OV7670-WebSocket-Camera/issues/13
  // otherwise crash with "assert failed: ledc_clk_cfg_to_global_clk ledc.c:444 (false)"
  ledc_timer.clk_cfg = LEDC_USE_APB_CLK;
  esp_err_t err = ledc_timer_config(&ledc_timer);
  if (err != ESP_OK) {
    Serial.println("Error when calling ledc_timer_config!");
  }  

  #if (OMOTE_HARDWARE_REV == 1)
  // Slowly charge the VSW voltage to prevent a brownout
  // Workaround for hardware rev 1!
  Serial.println("Will slowly charge VSW voltage to prevent that screen is completely bright, with no content");
  for(int i = 0; i < 100; i++) {
    digitalWrite(LCD_EN_GPIO, HIGH);  // LCD Logic off
    delayMicroseconds(1);
    digitalWrite(LCD_EN_GPIO, LOW);   // LCD Logic on
  }
  #else
  Serial.println("Will immediately charge VSW voltage. If screen is completely bright, with no content, then this is the reason.");
  digitalWrite(LCD_EN_GPIO, LOW);
  #endif

  // https://github.com/CoretechR/OMOTE/issues/70#issuecomment-2016763291
  delay(5); // Wait for the LCD driver to power on
  tft.init();
  tft.initDMA();
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
}

void update_backligthBrighness_HAL(void) {
  // A variable declared static inside a function is visible only inside that function, exists only once (not created/destroyed for each call) and is permanent. It is in a sense a private global variable.
  static int fadeInTimer = millis(); // fadeInTimer = time after setup
  if (millis() < fadeInTimer + backlightBrightness) {
    // after boot or wakeup, fade in backlight brightness
    // fade in lasts for <backlightBrightness> ms
    ledcWrite(5, millis() - fadeInTimer);
  } else {
    if (millis() - get_lastActivityTimestamp() > get_sleepTimeout_HAL() - 2000) {
      // less than 2000 ms until standby
      // dim backlight
      ledcWrite(5, get_backlightBrightness_HAL() * 0.3);
    } else {
      // normal mode, set full backlightBrightness
      ledcWrite(5, backlightBrightness);
    }
  }
}

uint8_t get_backlightBrightness_HAL() {
  return backlightBrightness;
};
void set_backlightBrightness_HAL(uint8_t aBacklightBrightness) {
  backlightBrightness = aBacklightBrightness;
};
