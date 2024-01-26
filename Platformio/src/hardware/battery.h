#ifndef __BATTERY_H__
#define __BATTERY_H__

//#define CRG_STAT 21 // battery charger feedback,                  GPIO21, VSPIHD, EMAC_TX_EN
#define ADC_BAT 36  // Battery voltage sense input (1/2 divider), GPIO36, ADC1_CH0, RTC_GPIO0

extern int battery_percentage;

void init_battery(void);
void update_battery_stats(void);

#endif /*__BATTERY_H__*/
