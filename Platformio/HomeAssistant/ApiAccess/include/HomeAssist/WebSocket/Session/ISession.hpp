#pragma once

#include <memory>

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

 private:
  bool mIsRunning = false;
};

inline bool ISession::IsRunning() const { return mIsRunning; }
inline void ISession::MarkStarted() { mIsRunning = true; }
inline void ISession::MarkComplete() { mIsRunning = false; }

}  // namespace HomeAssist::WebSocket