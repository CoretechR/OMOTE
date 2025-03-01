#pragma once

#include <memory>

namespace HomeAssist {

class WebSocketMessage;
class WebSocketMessageHandler;
class WebSocketRequest;

class WebSocketSession {
 public:
  WebSocketSession(std::unique_ptr<WebSocketRequest> aRequest,
                   std::shared_ptr<WebSocketMessageHandler> aMessageHandler);

  WebSocketRequest* BorrowStartRequest();
  WebSocketRequest* BorrowEndRequest();

  virtual bool ProcessMessage(const WebSocketMessage& aMessage);
  virtual bool IsComplete() const;

 protected:
  WebSocketSession(std::shared_ptr<WebSocketMessageHandler> aMessageHandler);

 private:
  std::unique_ptr<WebSocketRequest> mStartRequest = nullptr;
  std::unique_ptr<WebSocketRequest> mEndRequest = nullptr;
  std::weak_ptr<WebSocketMessageHandler> mMessageHandler;
};

}  // namespace HomeAssist