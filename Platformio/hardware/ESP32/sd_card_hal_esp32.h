#pragma once

extern uint8_t SD_EN_GPIO;
extern uint8_t SD_CS_GPIO;
extern uint8_t SD_MISO_GPIO;
extern uint8_t SD_MOSI_GPIO;
extern uint8_t SD_SCK_GPIO;

void init_SD_HAL(void);
