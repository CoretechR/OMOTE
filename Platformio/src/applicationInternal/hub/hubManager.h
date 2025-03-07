#pragma once

#include "hubBackendBase.h"
#include <nlohmann/json.hpp>
#include <memory>

using json = nlohmann::json;

class HubManager {
private:
  std::unique_ptr<HubBackendBase> activeBackend;
  HubBackend currentBackend;  // Store the current backend type

  // Private constructor for singleton
  HubManager();
  
  // Factory method for creating backends
  static std::unique_ptr<HubBackendBase> createBackend(HubBackend backend);

public:
  static HubManager& getInstance();
  
  // Delete copy constructor and assignment operator
  HubManager(const HubManager&) = delete;
  HubManager& operator=(const HubManager&) = delete;
  
  ~HubManager() = default;  // Smart pointer handles cleanup
  
  // Initialize with the preferred backend
  bool init(HubBackend backend);
  
  // Process communication tasks
  void process();
  
  // Send a message using the active backend
  bool sendMessage(const json& payload);
  
  // Check if communication is ready
  bool isReady() const;
  
  // Shutdown communication
  void shutdown();
  
  // Check if the hub is initialized
  bool isInitialized() const;
  
  // Get the current backend type
  HubBackend getCurrentBackend() const;
}; 