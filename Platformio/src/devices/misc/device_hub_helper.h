#pragma once

#include <string>
#include <list>
#include "applicationInternal/commandHandler.h"

#if (ENABLE_HUB_COMMUNICATION == 1)
// Helper function to register hub commands for devices
void register_hub_command(uint16_t *command, const char *device, const char *cmd) {
  std::list<std::string> commandPayload = {device, cmd};
  register_command(command, makeCommandData(HUB, commandPayload));
}

// Helper function to register hub commands with additional parameters
void register_hub_command_with_params(uint16_t *command, const char *device, 
                                    const char *cmd, std::list<std::string> params) {
  std::list<std::string> commandPayload = {device, cmd};
  
  // Add all parameters to the command payload
  commandPayload.insert(commandPayload.end(), params.begin(), params.end());
  
  register_command(command, makeCommandData(HUB, commandPayload));
}
#endif 