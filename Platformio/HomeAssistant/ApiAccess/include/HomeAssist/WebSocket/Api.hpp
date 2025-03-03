#pragma once

#include <chrono>
#include <map>
#include <memory>
#include <queue>

#include "webSocketInterface.hpp"

namespace HomeAssist::WebSocket {

class Message;
class ISession;
class AuthSession;

class Api {
 public:
  enum class ConnectionStatus { Initializing, Connected, Disconnected, Failed };

  Api(std::shared_ptr<webSocketInterface> socket);
  virtual ~Api();

  void Proccess();

  void AddSession(std::unique_ptr<ISession> aSession);

 protected:
  /**
   * @brief Used for preproccessing message before we store it on the queue
   * @return true the message was preproccessed and should not be stored
   */
  bool PreProccessMessage(Message& aMessage);
  void ParseIncomingMessage(const std::string& messageStr);

  void ProccessSessions();
  void CleanUpSessions();

  void ProccessMessages();

 private:
  std::chrono::milliseconds mLastConnectRetry;
  ConnectionStatus mConnectionStatus = ConnectionStatus::Initializing;
  std::shared_ptr<webSocketInterface> mHomeAssistSocket = nullptr;
  std::queue<std::unique_ptr<Message>> mIncomingMessageQueue;
  std::map<int, std::unique_ptr<ISession>> mSessions;
  std::unique_ptr<AuthSession> mAuthSession;
  int mNextRequestId = 1;
};

}  // namespace HomeAssist::WebSocket
