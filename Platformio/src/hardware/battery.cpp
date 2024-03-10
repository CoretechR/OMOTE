#include <Arduino.h>
#include "hardware/battery.h"
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/gui_settings.h"

void init_battery(void) {
  // Currently the battery charge status cannot be recognized in a reliable way due to a design flaw in the PCB.
  // See https://github.com/CoretechR/OMOTE/issues/55
  // So charge status is deactivated for now.
  //pinMode(CRG_STAT, INPUT_PULLUP);
  pinMode(ADC_BAT, INPUT);
}

// Battery declares
int battery_analogRead = 0;
int battery_voltage = 0;
int battery_percentage = 100;
//bool battery_ischarging = false;

void update_battery_stats(void) {
  battery_analogRead = analogRead(ADC_BAT);
  // original values
  // battery_voltage = battery_analogRead*2*3300/4095 + 350; // 350mV ADC offset
  // adjusted values due to new measurements
  battery_voltage = battery_analogRead*2*3350/4095 + 325;
  battery_percentage = constrain(map(battery_voltage, 3700, 4200, 0, 100), 0, 100);
  // Check if battery is charging, fully charged or disconnected
  /*
    "disconnected" cannot be recognized
    https://electronics.stackexchange.com/questions/615215/level-shifting-of-a-3-state-pin
    https://electrical.codidact.com/posts/286209
    https://how2electronics.com/lithium-ion-battery-charger-circuit-using-mcp73831/
  */
  //battery_ischarging = !digitalRead(CRG_STAT);
  
  char buffer1[20];
  sprintf(buffer1, "Voltage: %.2f V", (float)battery_voltage / 1000);

  // GUI settings
  if (objBattSettingsVoltage    != NULL) {lv_label_set_text_fmt(objBattSettingsVoltage,    buffer1);}
  if (objBattSettingsPercentage != NULL) {lv_label_set_text_fmt(objBattSettingsPercentage, "Percentage: %d%%", battery_percentage);}
  //lv_label_set_text_fmt(objBattSettingsIscharging, "Is charging: %s",  battery_ischarging ? "yes" : "no");
  
  // GUI status bar at the top
  char buffer2[12];
  // Voltage and percentage
  // sprintf(buffer2, "%.1fV, %d%%", (float)battery_voltage / 1000, battery_percentage);
  // only percentage
  sprintf(buffer2, "%d%%", battery_percentage);
  // sprintf(buffer, "%d%%", battery_percentage);
  for (int i=0; i<strlen(buffer2); i++) {
    if (buffer2[i] == '.') {
      buffer2[i] = ',';
    }
  }

  // if (battery_ischarging /*|| (!battery_ischarging && battery_voltage > 4350)*/){
  //   // if (BattPercentageLabel != NULL) {lv_label_set_text(BattPercentageLabel, "");}
  //   // lv_label_set_text_fmt(BattPercentageLabel, "%d%%", battery_percentage);
  //   // lv_label_set_text_fmt(BattPercentageLabel, "%.1f, %d%%", (float)battery_voltage / 1000, battery_percentage);
  //   if (BattPercentageLabel != NULL) {lv_label_set_text(BattPercentageLabel, buffer2);}
  //   if (BattIconLabel != NULL) {lv_label_set_text(BattIconLabel, LV_SYMBOL_USB);}
  // } else
  {
    // Update status bar battery indicator
    // lv_label_set_text_fmt(BattPercentageLabel, "%.1f, %d%%", (float)battery_voltage / 1000, battery_percentage);
    if (BattPercentageLabel != NULL) {lv_label_set_text(BattPercentageLabel, buffer2);}
    if (BattIconLabel != NULL) {
           if(battery_percentage > 95) lv_label_set_text(BattIconLabel, LV_SYMBOL_BATTERY_FULL);
      else if(battery_percentage > 75) lv_label_set_text(BattIconLabel, LV_SYMBOL_BATTERY_3);
      else if(battery_percentage > 50) lv_label_set_text(BattIconLabel, LV_SYMBOL_BATTERY_2);
      else if(battery_percentage > 25) lv_label_set_text(BattIconLabel, LV_SYMBOL_BATTERY_1);
      else                             lv_label_set_text(BattIconLabel, LV_SYMBOL_BATTERY_EMPTY);
    }
  }
}

