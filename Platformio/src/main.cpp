// OMOTE firmware for ESP32
// 2023 Maximilian Kern


#include <lvgl.h>
#include "HardwareRevX.hpp"
#include "OmoteUI/OmoteUI.hpp"
#include "omoteconfig.h"

std::shared_ptr<HardwareRevX> hal = nullptr;

void setup() {

  hal = HardwareRevX::getInstance();
  hal->init();

  auto ui = OmoteUI::getInstance(hal);
  ui->layout_UI();


  lv_timer_handler(); // Run the LVGL UI once before the loop takes over

  Serial.print("Setup finised in ");
  Serial.print(millis());
  Serial.println("ms.");
}

void loop() {

  HardwareRevX::getInstance()->handleLoop();
  // IR Test
  // tft.drawString("IR Command: ", 10, 90, 1);
  // decode_results results;
  // if (IrReceiver.decode(&results)) {
  // IrReceiver.resume(); // Enable receiving of the next value
  //}  //tft.drawString(String(results.command) + "        ", 80, 90, 1);
  // 
}