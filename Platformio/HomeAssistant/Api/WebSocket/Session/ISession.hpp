#pragma once

#include <memory>

namespace HAL::WebSocket::Json {
class IChunkProcessor;
}

namespace HomeAssist::WebSocket {

class Message;
class MessageHandler;
class Request;

class ISession {
 public:
  virtual ~ISession() = default;

  void MarkStarted();
  void MarkComplete();
  bool IsRunning() const;

  virtual Request* BorrowStartRequest() = 0;
  virtual Request* BorrowEndRequest() = 0;

  virtual bool ProcessMessage(const Message& aMessage) = 0;
  virtual bool IsComplete() const = 0;

  virtual bool IsPreferringChunkProcessing() = 0;
  virtual std::shared_ptr<HAL::WebSocket::Json::IChunkProcessor>
  GetChunkProcessor() = 0;

 protected:
  bool mIsRunning = false;
  bool mIsComplete = false;
};

inline bool ISession::IsRunning() const { return mIsRunning; }
inline void ISession::MarkStarted() { mIsRunning = true; }
inline void ISession::MarkComplete() { mIsComplete = true; }

}  // namespace HomeAssist::WebSocket