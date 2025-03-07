#pragma once

#include <WiFiUdp.h>  // Required before esp_websocket_client.h include
// Issue seemed fixed but unsure current status
// https://github.com/espressif/arduino-esp32/issues/4405

#include <memory>

#include "esp_websocket_client.h"
#include "websockets/webSocketInterface.hpp"
#include "wifihandler.hpp"

class esp32WebSocket : public webSocketInterface {
 public:
  enum class ProcessingStep { Append, Drop, Partial, Reserve };

  esp32WebSocket(std::shared_ptr<wifiHandler> aWifiHandler);

  void connect(const std::string &url) override;
  void disconnect() override;
  void sendMessage(const std::string &message) override;
  void setMessageCallback(MessageCallback callback) override;

 private:
  void proccessEventData(esp_websocket_event_data_t *aEventData);
  void printDebugInfo(esp_websocket_event_data_t *aEventData,
                      ProcessingStep aNextStep);

  ProcessingStep getNextStep(esp_websocket_event_data_t *aEventData) const;
  ProcessingStep getStartStep(esp_websocket_event_data_t *aEventData) const;

  void processStoredMessage();

  static void websocket_event_handler(void *handler_args, esp_event_base_t base,
                                      int32_t event_id, void *event_data);

  std::shared_ptr<wifiHandler> mWifiHandler;
  Handler<wifiHandlerInterface::wifiStatus> mWifiStatusUpdateHandler;

  esp_websocket_client_handle_t client;
  esp_websocket_client_config_t mConfig{};

  std::string mUri;
  bool connected = false;
  bool mPartialProcessingFailed = false;
  std::string mIncomingMessage{};
  MessageCallback messageCallback{};
};