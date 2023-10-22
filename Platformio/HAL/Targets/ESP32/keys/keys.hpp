#pragma once
#include "KeyPressAbstract.hpp"
#include "omoteconfig.h"
#include <Keypad.h> // modified for inverted logic
#include <map>

class Keys : public KeyPressAbstract {
public:
  Keys();
  void HandleKeyPresses() override;
  void QueueKeyEvent(KeyEvent aJustOccuredKeyEvent) override;

protected:
  void GrabKeys();

private:
  static void KeyGrabberTask(void *aSelf);
  static void KeyProccessor(void *aSelf);

  QueueHandle_t mKeyPressQueueHandle;
  TaskHandle_t mKeyGrabbingTask;
  TaskHandle_t mKeyHandlingTask;

  // Keypad declarations
  static const byte ROWS = 5; // four rows
  static const byte COLS = 5; // four columns
  // define the symbols on the buttons of the keypads
  char hexaKeys[ROWS][COLS] = {
      {'s', '^', '-', 'm', 'r'}, //  source, channel+, Volume-,   mute, record
      {'i', 'R', '+', 'k', 'd'}, //    info,    right, Volume+,     OK,   down
      {'4', 'v', '1', '3', '2'}, //    blue, channel-,     red, yellow,  green
      {'>', 'o', 'b', 'u', 'L'}, // forward,      off,    back,     up,   left
      {'?', 'p', 'c', '<', '='}  //       ?,     play,  config, rewind,   stop
  };
  // TODO what is '?' lol

  // TODO Should be able to optomize this out by reordering Ids at some point
  // or even using interrupts to trigger key press queueing
  static inline const std::map<char, KeyId> charKeyToKeyIds{
      {'o', KeyId::Power},
      // Top 4 Buttons left to right
      {'=', KeyId::Stop},
      {'<', KeyId::Rewind},
      {'p', KeyId::Play},
      {'>', KeyId::FastForward},
      // Buttons around D Pad
      {'c', KeyId::Menu},
      {'i', KeyId::Info},
      {'b', KeyId::Back},
      {'s', KeyId::Source},
      // D Pad
      {'u', KeyId::Up},
      {'d', KeyId::Down},
      {'L', KeyId::Left},
      {'R', KeyId::Right},
      {'k', KeyId::Center},
      // Volume Channel and 2 between
      {'+', KeyId::VolUp},
      {'-', KeyId::VolDown},
      {'m', KeyId::Mute},
      {'r', KeyId::Record},
      {'^', KeyId::ChannelUp},
      {'v', KeyId::ChannelDown},
      // Bottom 4 buttons left to right
      {'1', KeyId::Aux1},
      {'2', KeyId::Aux2},
      {'3', KeyId::Aux3},
      {'4', KeyId::Aux4}};

  byte rowPins[ROWS] = {SW_A, SW_B, SW_C, SW_D,
                        SW_E}; // connect to the row pinouts of the keypad
  byte colPins[COLS] = {SW_1, SW_2, SW_3, SW_4,
                        SW_5}; // connect to the column pinouts of the keypad
  Keypad customKeypad =
      Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
};