#pragma once

#include <memory>

namespace HomeAssist::WebSocket {

class WebSocketMessage;
class WebSocketMessageHandler;
class WebSocketRequest;

class IWebSocketSession {
 public:
  virtual ~IWebSocketSession() = default;

  virtual WebSocketRequest* BorrowStartRequest() = 0;
  virtual WebSocketRequest* BorrowEndRequest() = 0;

  virtual bool ProcessMessage(const WebSocketMessage& aMessage) = 0;
  virtual bool IsComplete() const = 0;
};

}  // namespace HomeAssist::WebSocket