#pragma once

#include <string>

void init_preferences_HAL(void);
void save_preferences_HAL(void);

std::string get_currentScene_HAL();
void set_currentScene_HAL(std::string aCurrentScene);
std::string get_currentGUIname_HAL();
void set_currentGUIname_HAL(std::string aCurrentGUIname);
