#pragma once

#if(OMOTE_HARDWARE_REV >= 5)

#else
    extern uint8_t SW_1_GPIO; // 1...5: Output
    extern uint8_t SW_2_GPIO;
    extern uint8_t SW_3_GPIO;
    extern uint8_t SW_4_GPIO;
    extern uint8_t SW_5_GPIO;
    extern uint8_t SW_A_GPIO; // A...E: Input
    extern uint8_t SW_B_GPIO;
    extern uint8_t SW_C_GPIO;
    extern uint8_t SW_D_GPIO;
    extern uint8_t SW_E_GPIO;
#endif
extern uint64_t BUTTON_PIN_BITMASK;

void init_keys_HAL(void);
void keys_getKeys_HAL(void* ptr);
