#include <SDL2/SDL.h>

enum guiKeyStates {GUI_KEY_IDLE, GUI_KEY_PRESSED, GUI_KEY_HOLD, GUI_KEY_RELEASED};

struct ActiveKey {
  char key;
  int keyCode;
  guiKeyStates state;
  bool changed;
};

struct KeyState {
  ActiveKey key;
  bool stateChanged;
};

SDL_Window* keypad_gui_setup();

KeyState pumpKeys();