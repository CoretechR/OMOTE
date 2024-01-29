#ifndef __SCENEHANDLER_H__
#define __SCENEHANDLER_H__

#include <string>
#include "commandHandler.h"

extern std::string currentScene; // Current scene that is active

void handleScene(std::string command, commandData commandData, std::string additionalPayload = "");
void setLabelCurrentScene();

#endif /*__SCENEHANDLER_H__*/
