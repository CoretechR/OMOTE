#include "keys.hpp"

Keys::Keys() {}
void Keys::HandleKeyPresses(){

};
void Keys::QueueKeyEvent(KeyEvent aJustOccuredKeyEvent){

};

void Keys::GrabKeys() {
  customKeypad.getKey(); // Populate key list
  for (int i = 0; i < LIST_MAX;
       i++) { // Handle multiple keys (Not really necessary in this case)
    if (customKeypad.key[i].kstate == PRESSED ||
        customKeypad.key[i].kstate == HOLD) {
      // May need to think about resetting sleep timer in key handler....
      //   standbyTimer =
      //       sleepTimeout; // Reset the sleep timer when a button is pressed
      int keyCode = customKeypad.key[i].kcode;
      // Queue Keys here!!
      Serial.println(customKeypad.key[i].kchar);
    }
  }
}