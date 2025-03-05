#pragma once

#include <chrono>
#include <map>
#include <memory>
#include <queue>

#include "websockets/webSocketInterface.hpp"

namespace HomeAssist::WebSocket {

class Message;
class ISession;
class AuthSession;

class Api {
 public:
  enum class ConnectionStatus { Initializing, Connected, Disconnected, Failed };

  Api(std::shared_ptr<webSocketInterface> socket);
  virtual ~Api();

  void Process();

  void AddSession(std::unique_ptr<ISession> aSession);

 protected:
  /**
   * @brief Used for preProcessing message before we store it on the queue
   * @return true the message was preProcessed and should not be stored
   */
  bool PreProcessMessage(Message& aMessage);
  void ParseIncomingMessage(const std::string& messageStr);
  bool ProcessDocument(const MemConciousDocument& aDocFromSocket);

  void ProcessSessions();
  void CleanUpSessions();

  void ProcessMessages();

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
