#include <BleKeyboard.h>
#include "commandHandler.h"
#include "device_keyboard_ble/device_keyboard_ble.h"
#include "gui_general_and_keys/guiBase.h"
#include "hardware/battery.h"

#ifdef ENABLE_KEYBOARD_BLE

BleKeyboard bleKeyboard("OMOTE Keyboard", "CoretechR");

void register_device_keyboard_ble() {
  commands[KEYBOARD_BLE_UP]                  = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_UP});
  commands[KEYBOARD_BLE_DOWN]                = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_DOWN});
  commands[KEYBOARD_BLE_RIGHT]               = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_RIGHT});
  commands[KEYBOARD_BLE_LEFT]                = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_LEFT});
  commands[KEYBOARD_BLE_SELECT]              = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_SELECT});
  commands[KEYBOARD_BLE_SENDSTRING]          = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_SENDSTRING});           // payload must be set when calling commandHandler
  commands[KEYBOARD_BLE_BACK]                = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_BACK});
  commands[KEYBOARD_BLE_HOME]                = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_HOME});
  commands[KEYBOARD_BLE_MENU]                = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_MENU});
  commands[KEYBOARD_BLE_SCAN_PREVIOUS_TRACK] = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_SCAN_PREVIOUS_TRACK});
  commands[KEYBOARD_BLE_REWIND_LONG]         = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_REWIND_LONG});
  commands[KEYBOARD_BLE_REWIND]              = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_REWIND});
  commands[KEYBOARD_BLE_PLAYPAUSE]           = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_PLAYPAUSE});
  commands[KEYBOARD_BLE_FASTFORWARD]         = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_FASTFORWARD});
  commands[KEYBOARD_BLE_FASTFORWARD_LONG]    = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_FASTFORWARD_LONG});
  commands[KEYBOARD_BLE_SCAN_NEXT_TRACK]     = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_SCAN_NEXT_TRACK});
  commands[KEYBOARD_BLE_MUTE]                = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_MUTE});
  commands[KEYBOARD_BLE_VOLUME_INCREMENT]    = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_VOLUME_INCREMENT});
  commands[KEYBOARD_BLE_VOLUME_DECREMENT]    = makeCommandData(BLE_KEYBOARD, {KEYBOARD_BLE_VOLUME_DECREMENT});

  bleKeyboard.begin();
}

// if bluetooth is in pairing mode (pairing mode is always on, if not connected), but not connected, then blink
unsigned long blinkBluetoothLabelLastChange = millis();
bool blinkBluetoothLabelIsOn = false;

void update_keyboard_ble_status() {
  if (bleKeyboard.isConnected()) {
    if (BluetoothLabel != NULL) {lv_label_set_text(BluetoothLabel, LV_SYMBOL_BLUETOOTH);}
    bleKeyboard.setBatteryLevel(battery_percentage);

  } else {
    if(millis() - blinkBluetoothLabelLastChange >= 1000){
      blinkBluetoothLabelIsOn = !blinkBluetoothLabelIsOn;
      if (blinkBluetoothLabelIsOn) {
        if (BluetoothLabel != NULL) {lv_label_set_text(BluetoothLabel, LV_SYMBOL_BLUETOOTH);}
      } else {
        if (BluetoothLabel != NULL) {lv_label_set_text(BluetoothLabel, "");}
      }
      blinkBluetoothLabelLastChange = millis();
    }
  }
}

void keyboard_ble_write(uint8_t c) {
  bleKeyboard.write(c);
}

void keyboard_ble_press(uint8_t c) {
  bleKeyboard.press(c);
  bleKeyboard.release(c);
}

void keyboard_ble_longpress(uint8_t c) {
  bleKeyboard.press(c);
  delay(1000);
  bleKeyboard.release(c);
}

void keyboard_ble_home() {
  bleKeyboard.press(KEY_LEFT_ALT);
  bleKeyboard.press(KEY_ESC);
  bleKeyboard.releaseAll();
}

void keyboard_ble_sendString(const String &s) {
  bleKeyboard.print(s);
}

void consumerControl_ble_write(const MediaKeyReport value) {
  bleKeyboard.write(value);
}

void consumerControl_ble_press(const MediaKeyReport value) {
  bleKeyboard.press(value);
  bleKeyboard.release(value);
}

void consumerControl_ble_longpress(const MediaKeyReport value) {
  bleKeyboard.press(value);
  delay(1000);
  bleKeyboard.release(value);
}

void keyboard_ble_executeCommand(std::string command, std::string additionalPayload) {
  bool doLog = false;

  if (doLog) {
    if (bleKeyboard.isConnected()) {
      Serial.println("BLE keyboard connected, could send key");
    } else {
      Serial.println("BLE keyboard NOT connected, cannot send key");
    }
  }

  if (command == KEYBOARD_BLE_UP) {
    if (doLog) {Serial.printf("UP received\r\n");}
    keyboard_ble_write(KEY_UP_ARROW);

  } else if (command == KEYBOARD_BLE_DOWN) {
    if (doLog) {Serial.printf("DOWN received\r\n");}
    keyboard_ble_write(KEY_DOWN_ARROW);

  } else if (command == KEYBOARD_BLE_RIGHT) {
    if (doLog) {Serial.printf("RIGHT received\r\n");}
    keyboard_ble_write(KEY_RIGHT_ARROW);

  } else if (command == KEYBOARD_BLE_LEFT) {
    if (doLog) {Serial.printf("LEFT received\r\n");}
    keyboard_ble_write(KEY_LEFT_ARROW);

  } else if (command == KEYBOARD_BLE_SELECT) {
    if (doLog) {Serial.printf("SELECT received\r\n");}
    keyboard_ble_write(KEY_RETURN);

  } else if (command == KEYBOARD_BLE_SENDSTRING) {
    if (doLog) {Serial.printf("SENDSTRING received\r\n");}
    if (additionalPayload != "") {
      keyboard_ble_sendString(additionalPayload.c_str());
    }



  } else if (command == KEYBOARD_BLE_BACK) {
    if (doLog) {Serial.printf("BACK received\r\n");}
    // test which one works best for your device
    // keyboard_ble_write(KEY_ESC);
    consumerControl_ble_write(KEY_MEDIA_WWW_BACK);

  } else if (command == KEYBOARD_BLE_HOME) {
    if (doLog) {Serial.printf("HOME received\r\n");}
    // test which one works best for your device
    // keyboard_ble_home();
    consumerControl_ble_write(KEY_MEDIA_WWW_HOME);

  } else if (command == KEYBOARD_BLE_MENU) {
    if (doLog) {Serial.printf("MENU received\r\n");}
    keyboard_ble_write(0xED); // 0xDA + 13 = 0xED



  // for more consumerControl codes see
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/USB/src/USBHIDConsumerControl.h
  // https://github.com/adafruit/Adafruit_CircuitPython_HID/blob/main/adafruit_hid/consumer_control_code.py
  } else if (command == KEYBOARD_BLE_SCAN_PREVIOUS_TRACK) {
    if (doLog) {Serial.printf("SCAN_PREVIOUS_TRACK received\r\n");}
    consumerControl_ble_write(KEY_MEDIA_PREVIOUS_TRACK);

  } else if (command == KEYBOARD_BLE_REWIND_LONG) {
    if (doLog) {Serial.printf("REWIND_LONG received\r\n");}
    //keyboard_ble_longpress(KEY_LEFT_ARROW);
    consumerControl_ble_longpress(KEY_MEDIA_REWIND);

  } else if (command == KEYBOARD_BLE_REWIND) {
    if (doLog) {Serial.printf("REWIND received\r\n");}
    //keyboard_ble_write(KEY_LEFT_ARROW);
    consumerControl_ble_write(KEY_MEDIA_REWIND);

  } else if (command == KEYBOARD_BLE_PLAYPAUSE) {
    if (doLog) {Serial.printf("PLAYPAUSE received\r\n");}
    consumerControl_ble_write(KEY_MEDIA_PLAY_PAUSE);

  } else if (command == KEYBOARD_BLE_FASTFORWARD) {
    if (doLog) {Serial.printf("FASTFORWARD received\r\n");}
    //keyboard_ble_write(KEY_RIGHT_ARROW);
    consumerControl_ble_write(KEY_MEDIA_FASTFORWARD);

  } else if (command == KEYBOARD_BLE_FASTFORWARD_LONG) {
    if (doLog) {Serial.printf("FASTFORWARD_LONG received\r\n");}
    //keyboard_ble_longpress(KEY_RIGHT_ARROW);
    consumerControl_ble_longpress(KEY_MEDIA_FASTFORWARD);

  } else if (command == KEYBOARD_BLE_SCAN_NEXT_TRACK) {
    if (doLog) {Serial.printf("SCAN_NEXT_TRACK received\r\n");}
    consumerControl_ble_write(KEY_MEDIA_NEXT_TRACK);



  } else if (command == KEYBOARD_BLE_MUTE) {
    if (doLog) {Serial.printf("MUTE received\r\n");}
    consumerControl_ble_write(KEY_MEDIA_MUTE);

  } else if (command == KEYBOARD_BLE_VOLUME_INCREMENT) {
    if (doLog) {Serial.printf("VOLUME_INCREMENT received\r\n");}
    consumerControl_ble_write(KEY_MEDIA_VOLUME_UP);

  } else if (command == KEYBOARD_BLE_VOLUME_DECREMENT) {
    if (doLog) {Serial.printf("VOLUME_DECREMENT received\r\n");}
    consumerControl_ble_write(KEY_MEDIA_VOLUME_DOWN);

  }
}

#endif
/*
const MediaKeyReport KEY_MEDIA_NEXT_TRACK = {1, 0};
const MediaKeyReport KEY_MEDIA_PREVIOUS_TRACK = {2, 0};
const MediaKeyReport KEY_MEDIA_STOP = {4, 0};
const MediaKeyReport KEY_MEDIA_PLAY_PAUSE = {8, 0};
const MediaKeyReport KEY_MEDIA_MUTE = {16, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_UP = {32, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {64, 0};
const MediaKeyReport KEY_MEDIA_WWW_HOME = {128, 0};
const MediaKeyReport KEY_MEDIA_LOCAL_MACHINE_BROWSER = {0, 1}; // Opens "My Computer" on Windows
// original from BLE-Keyboard
// const MediaKeyReport KEY_MEDIA_CALCULATOR = {0, 2};
// changed for usage in OMOTE
const MediaKeyReport KEY_MEDIA_FASTFORWARD = {0, 2};
const MediaKeyReport KEY_MEDIA_WWW_BOOKMARKS = {0, 4};
const MediaKeyReport KEY_MEDIA_WWW_SEARCH = {0, 8};
const MediaKeyReport KEY_MEDIA_WWW_STOP = {0, 16};
const MediaKeyReport KEY_MEDIA_WWW_BACK = {0, 32};
const MediaKeyReport KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION = {0, 64}; // Media Selection
// original from BLE-Keyboard
// const MediaKeyReport KEY_MEDIA_EMAIL_READER = {0, 128};
// changed for usage in OMOTE
const MediaKeyReport KEY_MEDIA_REWIND = {0, 128};
*/
