#include "HomeAssistant/Api/WebSocket/Api.hpp"

#include "HardwareFactory.hpp"
#include "HomeAssistant/Api/WebSocket/Message/Message.hpp"
#include "HomeAssistant/Api/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssistant/Api/WebSocket/ResponseHandler.hpp"
#include "HomeAssistant/Api/WebSocket/Session/AuthSession.hpp"
#include "HomeAssistant/Api/WebSocket/Session/ISession.hpp"
#include "HomeAssistant/RapidJsonUtilty.hpp"
#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

Api::Api(std::shared_ptr<webSocketInterface> socket)
    : mHomeAssistSocket(socket),
      mConnectionNotification(
          std::make_shared<Notification<ConnectionStatus>>()) {
  if (mHomeAssistSocket) {
    mHomeAssistSocket->SetJsonHandler(std::make_unique<ResponseHandler>(*this));

    mHomeAssistSocket->OnConnect([this]() {
      mConnectionTime = HardwareFactory::getAbstract().execTime();
    });

    mHomeAssistSocket->OnDisconnect([this]() { /*TODO Reattempt Connection*/ });
    mHomeAssistSocket->connect("ws://192.168.86.49:8123/api/websocket");
    mLastConnectRetry = HardwareFactory::getAbstract().execTime();
    mAuthSession = std::make_unique<AuthSession>(mHomeAssistSocket);
  }
}

Api::~Api() {
  // Unregsiter on destruction
  if (mHomeAssistSocket) {
    mHomeAssistSocket->OnConnect();
    mHomeAssistSocket->OnDisconnect();
    mHomeAssistSocket->SetJsonHandler();
  }
}

std::shared_ptr<Notification<Api::ConnectionStatus>>
Api::GetConnectionNotification() {
  return mConnectionNotification;
}

void Api::UpdateConnectionStatus(ConnectionStatus aNewStatus) {
  mConnectionStatus = aNewStatus;
  // mConnectionNotification->notify(aNewStatus);
}

void Api::Process() {
  auto execTime = HardwareFactory::getAbstract().execTime();
  if (!mHomeAssistSocket) {
    return;
  }
  auto fiveSecondsSinceRetry = [this, execTime]() {
    return execTime > mLastConnectRetry + std::chrono::seconds(5);
  };
  if (!mHomeAssistSocket->isConnected() && fiveSecondsSinceRetry()) {
    mHomeAssistSocket->connect("ws://192.168.86.49:8123/api/websocket");
    mLastConnectRetry = execTime;
  }

  auto threeSecondsSinceConnection = [this, execTime]() {
    return execTime > mConnectionTime + std::chrono::seconds(3);
  };
  if (mAuthSession && mHomeAssistSocket->isConnected() &&
      !mAuthSession->IsAuthSent() && threeSecondsSinceConnection()) {
    mAuthSession->SendAuth();
  }

  ProcessSessions();
  ProcessMessages();
}

void Api::ProcessSessions() {
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

void Api::ProcessMessages() {
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

bool Api::PreProcessMessage(Message& aMessage) {
  if (mAuthSession && mAuthSession->ProcessMessage(aMessage)) {
    if (mAuthSession->IsComplete()) {
      UpdateConnectionStatus(mAuthSession->GetConnectionStatus());
      mAuthSession = nullptr;
    }
    return true;
  }
  return false;
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