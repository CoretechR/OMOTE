#pragma once

#include "hubInterface.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class HubManager {
private:
  HubInterface* activeBackend = nullptr;
  HubBackend currentBackend;  // Store the current backend type

public:
  static HubManager& getInstance();
  
  HubManager();
  ~HubManager();
  
  // Initialize with the preferred backend
  bool init(HubBackend backend);
  
  // Process communication tasks
  void process();
  
  // Send a message using the active backend
  bool sendMessage(const char* device, const char* command, json payload);
  
  // Check if communication is ready
  bool isReady() const;
  
  // Shutdown communication
  void shutdown();
  
  // Check if the hub is initialized
  bool isInitialized() const;
  
  // Get the current backend type
  HubBackend getCurrentBackend() const;
}; 