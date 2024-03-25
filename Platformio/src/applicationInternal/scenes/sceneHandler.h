#pragma once

#include <string>
#include "applicationInternal/commandHandler.h"

void handleScene(uint16_t command, commandData commandData, std::string additionalPayload = "");
void setLabelCurrentScene();
