#include "HomeAssist/WebSocket/Api.hpp"

#include "HardwareFactory.hpp"
#include "HomeAssist/WebSocket/Message/Message.hpp"
#include "HomeAssist/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssist/WebSocket/Session/AuthSession.hpp"
#include "HomeAssist/WebSocket/Session/ISession.hpp"
#include "RapidJsonUtilty.hpp"
#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

Api::Api(std::shared_ptr<webSocketInterface> socket)
    : mHomeAssistSocket(socket) {
  if (mHomeAssistSocket) {
    mHomeAssistSocket->setMessageCallback(
        [this](const std::string& messageStr) {
          ParseIncomingMessage(messageStr);
        });
    mHomeAssistSocket->connect("ws://192.168.86.49:8123/api/websocket");
    mLastConnectRetry = HardwareFactory::getAbstract().execTime();
    mAuthSession = std::make_unique<AuthSession>(mHomeAssistSocket);
  }
}

Api::~Api() {}

void Api::Proccess() {
  auto execTime = HardwareFactory::getAbstract().execTime();
  if (!mHomeAssistSocket) {
    return;
  }
  auto tenSecondsSinceRetry = [this, execTime]() {
    return execTime > mLastConnectRetry + std::chrono::seconds(10);
  };
  if (!mHomeAssistSocket->isConnected() && tenSecondsSinceRetry()) {
    mHomeAssistSocket->connect("ws://192.168.86.49:8123/api/websocket");
    mLastConnectRetry = execTime;
  }
  if (mAuthSession && mHomeAssistSocket->isConnected() &&
      !mAuthSession->IsAuthSent()) {
    mAuthSession->SendAuth();
  }
  ProccessSessions();
  ProccessMessages();
}

void Api::ProccessSessions() {
  CleanUpSessions();
  // Bail we are not connected don't start sessions
  if (mConnectionStatus != ConnectionStatus::Connected) {
    return;
  }
  for (auto& session : mSessions) {
    if (!session.second->IsRunning()) {
      if (auto* request = session.second->BorrowStartRequest(); request) {
        mHomeAssistSocket->sendMessage(request->GetRequestMessage());
        session.second->MarkStarted();
      }
    }
  }
}

void Api::ProccessMessages() {
  if (!mHomeAssistSocket) {
    return;
  }
  while (mIncomingMessageQueue.size() > 0) {
    auto message = std::move(mIncomingMessageQueue.front());
    mIncomingMessageQueue.pop();
    auto& session = mSessions[message->GetId()];
    if (!session) {
      return;
    }
    session->ProcessMessage(*message);
    if (session->IsComplete()) {
      if (auto* request = session->BorrowEndRequest(); request) {
        mHomeAssistSocket->sendMessage(request->GetRequestMessage());
      }
      session->MarkComplete();
    }
  }
}

void Api::CleanUpSessions() {
  for (auto sessionIter = mSessions.begin(); sessionIter != mSessions.end();) {
    if ((*sessionIter).second == nullptr) {
      sessionIter = mSessions.erase(sessionIter);
    } else {
      ++sessionIter;
    }
  }
}

bool Api::PreProccessMessage(Message& aMessage) {
  if (mAuthSession && mAuthSession->ProcessMessage(aMessage)) {
    if (mAuthSession->IsComplete()) {
      mConnectionStatus = mAuthSession->GetConnectionStatus();
      mAuthSession = nullptr;
    }
    return true;
  }
  return false;
}

void Api::ParseIncomingMessage(const std::string& messageStr) {
  MemConciousDocument messageJson;
  messageJson.Parse(messageStr.c_str(), messageStr.size());
  // auto prettyDebugString = ToPrettyString(messageJson);
  // HardwareFactory::getAbstract().debugPrint("%s", prettyDebugString.c_str());
  auto messageObj = std::make_unique<Message>(messageJson);
  if (PreProccessMessage(*messageObj)) {
    return;
  }
  mIncomingMessageQueue.push(std::move(messageObj));
}

void Api::AddSession(std::unique_ptr<ISession> aNewSession) {
  if (aNewSession->BorrowStartRequest() == nullptr) {
    HardwareFactory::getAbstract().debugPrint("Session Missing Start Request!");
  }
  auto newSessionId = mNextRequestId++;
  aNewSession->BorrowStartRequest()->SetId(newSessionId);
  mSessions[newSessionId] = std::move(aNewSession);
}

}  // namespace HomeAssist::WebSocket