#pragma once
#include "KeyPressAbstract.hpp"
#include "omoteconfig.h"
#include <Keypad.h> // modified for inverted logic

class Keys : public KeyPressAbstract {
public:
  Keys();
  void HandleKeyPresses() override;
  void QueueKeyEvent(KeyEvent aJustOccuredKeyEvent) override;

protected:
  void GrabKeys();

private:
  QueueHandle_t mKeyPressQueueHandle;
  TaskHandle_t mKeyGrabbingTask;
  TaskHandle_t mKeyHandlingTask;

  // Keypad declarations
  static const byte ROWS = 5; // four rows
  static const byte COLS = 5; // four columns
  // define the symbols on the buttons of the keypads
  char hexaKeys[ROWS][COLS] = {
      {'s', '^', '-', 'm', 'r'}, //  source, channel+, Volume-,   mute, record
      {'i', 'r', '+', 'k', 'd'}, //    info,    right, Volume+,     OK,   down
      {'4', 'v', '1', '3', '2'}, //    blue, channel-,     red, yellow,  green
      {'>', 'o', 'b', 'u', 'l'}, // forward,      off,    back,     up,   left
      {'?', 'p', 'c', '<', '='}  //       ?,     play,  config, rewind,   stop
  };
  byte rowPins[ROWS] = {SW_A, SW_B, SW_C, SW_D,
                        SW_E}; // connect to the row pinouts of the keypad
  byte colPins[COLS] = {SW_1, SW_2, SW_3, SW_4,
                        SW_5}; // connect to the column pinouts of the keypad
  Keypad customKeypad =
      Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
};