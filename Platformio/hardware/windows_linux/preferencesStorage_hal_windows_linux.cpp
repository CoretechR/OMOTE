#include <string>

enum GUIlists {
// MAIN_GUI_LIST: we are in the main_gui_list (with the scene selector as first gui), either if a scene is active or not
// SCENE_GUI_LIST: a scene is active and we are not in the main_gui_list. In that case, we try to use the scene specific gui list, if the scene defined one.
  MAIN_GUI_LIST,
  SCENE_GUI_LIST
};

/*
Possible example:
main_gui_list : "Scene selection", "Smart Home", "Settings", "IR Receiver"};

"Off"        :
"TV"         : "Numpad"
"Fire TV"    : "Numpad",   "Settings"
"Chromecast" : 
"Apple TV"   : "Apple TV", "Settings", "IR Receiver"
*/
std::string activeScene;
std::string activeGUIname;
int activeGUIlist;

void init_preferences_HAL(void) {
  // set some values for tests
  activeScene = "";              // "Off", "TV", "Fire TV", "Chromecast", "Apple TV";
  activeGUIname = "";            // "Scene selection", "Smart Home", "Settings", "IR Receiver"    // "Numpad", "Apple TV"
  activeGUIlist = MAIN_GUI_LIST; // // MAIN_GUI_LIST, SCENE_GUI_LIST;
}
void save_preferences_HAL(void) {
}

std::string get_activeScene_HAL() {
  return activeScene;
}
void set_activeScene_HAL(std::string anActiveScene) {
  activeScene = anActiveScene;
}
std::string get_activeGUIname_HAL(){
  return activeGUIname;
}
void set_activeGUIname_HAL(std::string anActiveGUIname) {
  activeGUIname = anActiveGUIname;
}
int get_activeGUIlist_HAL() {
  return activeGUIlist;
}
void set_activeGUIlist_HAL(int anActiveGUIlist) {
  activeGUIlist = anActiveGUIlist;
}
