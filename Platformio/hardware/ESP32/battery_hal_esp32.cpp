#include <Arduino.h>

// uint8_t CRG_STAT_GPIO = 21; // battery charger feedback,                  GPIO21, VSPIHD, EMAC_TX_EN
uint8_t ADC_BAT_GPIO = 36;  // Battery voltage sense input (1/2 divider), GPIO36, ADC1_CH0, RTC_GPIO0

void init_battery_HAL(void) {
  // Currently the battery charge status cannot be recognized in a reliable way due to a design flaw in the PCB.
  // See https://github.com/CoretechR/OMOTE/issues/55
  // So charge status is deactivated for now.
  //pinMode(CRG_STAT_GPIO, INPUT_PULLUP);
  pinMode(ADC_BAT_GPIO, INPUT);
}

void get_battery_status_HAL(int *battery_voltage, int *battery_percentage) {
  int battery_analogRead = 0;

  battery_analogRead = analogRead(ADC_BAT_GPIO);
  // original values
  // battery_voltage = battery_analogRead*2*3300/4095 + 350; // 350mV ADC offset
  // adjusted values due to new measurements
  *battery_voltage = battery_analogRead*2*3350/4095 + 325;
  *battery_percentage = constrain(map(*battery_voltage, 3700, 4200, 0, 100), 0, 100);
  // Check if battery is charging, fully charged or disconnected
  /*
    "disconnected" cannot be recognized
    https://electronics.stackexchange.com/questions/615215/level-shifting-of-a-3-state-pin
    https://electrical.codidact.com/posts/286209
    https://how2electronics.com/lithium-ion-battery-charger-circuit-using-mcp73831/
  */
  //*battery_ischarging = !digitalRead(CRG_STAT_GPIO);

}
