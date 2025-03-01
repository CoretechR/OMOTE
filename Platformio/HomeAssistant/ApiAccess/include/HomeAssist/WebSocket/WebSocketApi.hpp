#pragma once

#include <memory>
#include <queue>

#include "webSocketInterface.hpp"

namespace HomeAssist::WebSocket {

class WebSocketMessage;
class IWebSocketSession;

class WebSocketApi {
 public:
  enum class ConnectionStatus { Initializing, Connected, Disconnected, Failed };

  WebSocketApi(std::shared_ptr<webSocketInterface> socket);
  virtual ~WebSocketApi();

  void ProccessMessages();

 protected:
  /**
   * @brief Used for preproccessing message before we store it on the queue
   * @return true the message was preproccessed and should not be stored
   */
  bool PreProccessMessage(WebSocketMessage& aMessage);
  void ParseIncomingMessage(const std::string& messageStr);

  void SetupSession() {}
  void CleanUpSessions();

 private:
  ConnectionStatus mConnectionStatus = ConnectionStatus::Initializing;
  std::shared_ptr<webSocketInterface> mHomeAssistSocket = nullptr;
  std::queue<std::unique_ptr<WebSocketMessage>> mIncomingMessageQueue;
  std::vector<std::unique_ptr<IWebSocketSession>> mSessions;
};

}  // namespace HomeAssist::WebSocket
