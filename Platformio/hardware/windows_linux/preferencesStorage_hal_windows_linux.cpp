#include <string>

std::string currentScene;
std::string currentGUIname;

void init_preferences_HAL(void) {
}
void save_preferences_HAL(void) {
}

std::string get_currentScene_HAL() {
  return currentScene;
}
void set_currentScene_HAL(std::string aCurrentScene) {
  currentScene = aCurrentScene;
}
std::string get_currentGUIname_HAL(){
  return currentGUIname;
}
void set_currentGUIname_HAL(std::string aCurrentGUIname) {
  currentGUIname = aCurrentGUIname;
}
