#pragma once

#include <memory>

#include "HomeAssistant/Api/WebSocket/Request.hpp"
#include "HomeAssistant/Api/WebSocket/Session/ISession.hpp"
#include "IChunkProcessor.hpp"

namespace HomeAssist::WebSocket {

class Message;
class MessageHandler;
class Request;

class Session : public ISession {
 public:
  Session(std::unique_ptr<Request> aRequest,
          std::shared_ptr<MessageHandler> aMessageHandler = nullptr,
          std::shared_ptr<HAL::WebSocket::Json::IChunkProcessor>
              aChunkProcessor = nullptr);

  Request* BorrowStartRequest() override;
  Request* BorrowEndRequest() override;

  bool ProcessMessage(const Message& aMessage) override;
  bool IsComplete() const override;
  bool IsPreferringChunkProcessing() override;

  std::shared_ptr<HAL::WebSocket::Json::IChunkProcessor> GetChunkProcessor();

 private:
  std::unique_ptr<Request> mStartRequest = nullptr;
  std::unique_ptr<Request> mEndRequest = nullptr;
  std::weak_ptr<MessageHandler> mMessageHandler;
  std::weak_ptr<HAL::WebSocket::Json::IChunkProcessor> mChunkProcessor;
};

}  // namespace HomeAssist::WebSocket