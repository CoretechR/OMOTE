#include "esp32WebSocket.hpp"

#include "HardwareFactory.hpp"
#include "esp_event.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "esp32WebSocket";

esp32WebSocket::esp32WebSocket(std::shared_ptr<wifiHandler> aWifiHandler)
    : mWifiHandler(aWifiHandler), client(nullptr), mIncomingMessage() {}

void esp32WebSocket::connect(const std::string &url) {
  if (client && !isConnected()) {
    disconnect();
  }
  if (mWifiHandler->GetStatus().isConnected) {
    // Extract host and port from URL
    // std::string protocol, host, path;
    // int port;
    // size_t pos = url.find("://");
    // if (pos != std::string::npos) {
    //   protocol = url.substr(0, pos);
    //   pos += 3;
    // } else {
    //   pos = 0;
    // }

    // size_t host_end = url.find(':', pos);
    // if (host_end != std::string::npos) {
    //   host = url.substr(pos, host_end - pos);
    //   size_t port_end = url.find('/', host_end);
    //   if (port_end != std::string::npos) {
    //     port = std::stoi(url.substr(host_end + 1, port_end - host_end - 1));
    //     path = url.substr(port_end);
    //   } else {
    //     port = std::stoi(url.substr(host_end + 1));
    //   }
    // } else {
    //   size_t path_start = url.find('/', pos);
    //   if (path_start != std::string::npos) {
    //     host = url.substr(pos, path_start - pos);
    //     path = url.substr(path_start);
    //   } else {
    //     host = url.substr(pos);
    //   }
    //   port = (protocol == "wss") ? 443 : 80;
    // }

    // ESP_LOGI(TAG, "Attempting Connect to host: %s, port: %d", host.c_str(),
    //          port);

    // mConfig.port = port;
    // mConfig.host = host.c_str();
    // mConfig.transport = protocol == "ws" ? WEBSOCKET_TRANSPORT_OVER_TCP
    //                                      : WEBSOCKET_TRANSPORT_OVER_SSL;
    mConfig.uri = url.c_str();

    client = esp_websocket_client_init(&mConfig);
    if (!client) {
      ESP_LOGI(TAG, "Failed to init Client");
      return;
    }

    auto err = esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY,
                                             websocket_event_handler, this);
    if (err != ESP_OK) {
      ESP_LOGI(TAG, "Failed to register event handler");
      return;
    }
    err = esp_websocket_client_start(client);
    if (err != ESP_OK) {
      ESP_LOGI(TAG, "Failed to start Client");
      return;
    }

  } else {
    ESP_LOGI(TAG, "Cannot Connect Websocket Without wifi connected");
  }
}

void esp32WebSocket::disconnect() {
  if (client) {
    esp_websocket_client_stop(client);
    esp_websocket_client_destroy(client);
    client = nullptr;
  }
}

void esp32WebSocket::sendMessage(const std::string &message) {
  if (isConnected() && client) {
    ESP_LOGI(TAG, "Sending: %s", message.c_str());
    esp_websocket_client_send_text(client, message.c_str(), message.length(),
                                   portMAX_DELAY);
  }
}

void esp32WebSocket::setMessageCallback(MessageCallback callback) {
  messageCallback = callback;
}

void esp32WebSocket::proccessEventData(esp_websocket_event_data_t *aEventData) {
  // Todo is this a standard timeout message?
  if (aEventData->op_code == 0x08 && aEventData->data_len == 2) {
    ESP_LOGI(TAG, "Received closed message with code=%d",
             256 * aEventData->data_ptr[0] + aEventData->data_ptr[1]);
    return;
  }
  if (aEventData->data_len < 0) {
    return;
  }
  auto dataLength = static_cast<uint>(aEventData->data_len);
  auto nextStep = getNextStep(aEventData);
  printDebugInfo(aEventData, nextStep);
  using Step = ProcessingStep;
  switch (nextStep) {
    case Step::Reserve:
      mIncomingMessage.reserve(aEventData->payload_len);
      [[fallthrough]];
    case Step::Append:
      mIncomingMessage += {aEventData->data_ptr, dataLength};
      break;
    case Step::Partial:
      if (mJsonHandler) {
        auto procStatus =
            mJsonHandler->ProcessChunk({aEventData->data_ptr, dataLength});
      }
      break;
    case Step::Drop:
      break;
    default:
      break;
  }

  auto isMessageGathered = aEventData->payload_len == mIncomingMessage.length();
  if (isMessageGathered) {
    processStoredMessage();
    mIncomingMessage.clear();
    mIncomingMessage.shrink_to_fit();
  }
}

void esp32WebSocket::processStoredMessage() {
  // Give JsonHandler a crack at it and
  // if not pass it down to the string handler
  if (mJsonHandler && mJsonHandler->ProcessJsonAsDoc(mIncomingMessage)) {
    return;
  }
  if (messageCallback) {
    messageCallback(mIncomingMessage);
  }
}

esp32WebSocket::ProcessingStep esp32WebSocket::getNextStep(
    esp_websocket_event_data_t *aEventData) {
  if (aEventData->data_len == 0) {
    return ProcessingStep::Drop;
  }

  if (mJsonHandler && mJsonHandler->IsChunkProcessingPrefered()) {
    return ProcessingStep::Partial;
  }

  if (mIncomingMessage.empty() &&
      aEventData->payload_len == aEventData->data_len) {
    return ProcessingStep::Reserve;
  }

  const auto IsStartOfMultiEventData =
      aEventData->payload_len > aEventData->data_len &&
      aEventData->payload_offset == 0;
  if (IsStartOfMultiEventData) {
    return getStartStep(aEventData);
  }

  if (aEventData->payload_offset > 0 && mIncomingMessage.empty()) {
    return ProcessingStep::Partial;
  }

  return !mIncomingMessage.empty() ? ProcessingStep::Append
                                   : ProcessingStep::Drop;
}

esp32WebSocket::ProcessingStep esp32WebSocket::getStartStep(
    esp_websocket_event_data_t *aEventData) {
  auto step = ProcessingStep::Drop;

  auto freeHeap = esp_get_free_heap_size();
  auto isProccessInMemory = aEventData->payload_len < (freeHeap / 2);
  if (isProccessInMemory) {
    step = ProcessingStep::Reserve;
  } else {
    if (mJsonHandler && mJsonHandler->HasChunkProcessor()) {
      step = ProcessingStep::Partial;
    } else {
      ESP_LOGI(TAG,
               "Cannot proccess not enough heap and no partial handler:%d  Msg "
               "Size:%d",
               freeHeap, aEventData->payload_len);
    }
  }
  return step;
}

void esp32WebSocket::printDebugInfo(esp_websocket_event_data_t *aEventData,
                                    ProcessingStep aNextStep) {
  if (!aEventData) {
    return;
  }
  ESP_LOGI(TAG,
           "Received aEventData: opcode=%d, payload_len=%d, data_len=%d, "
           "data_offset=%d ,NextStep =%d",
           aEventData->op_code, aEventData->payload_len, aEventData->data_len,
           aEventData->payload_offset, aNextStep);
  if (aEventData->data_ptr) {
    // ESP_LOGI(TAG, "Message: %s", aEventData->data_ptr);
  }
}

void esp32WebSocket::websocket_event_handler(void *handler_args,
                                             esp_event_base_t base,
                                             int32_t event_id,
                                             void *event_data) {
  esp32WebSocket *self = static_cast<esp32WebSocket *>(handler_args);

  switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
      ESP_LOGI(TAG, "WebSocket connected");
      self->Connected();
      break;
    case WEBSOCKET_EVENT_DISCONNECTED:
      ESP_LOGI(TAG, "WebSocket disconnected");
      self->Disconnected();
      break;
    case WEBSOCKET_EVENT_ERROR:
      ESP_LOGI(TAG, "WebSocket error");
      break;
    case WEBSOCKET_EVENT_DATA:
      self->proccessEventData(
          static_cast<esp_websocket_event_data_t *>(event_data));
      break;
    default:
      break;
  }
}
