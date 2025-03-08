#include "HomeAssistant/Api/WebSocket/Api.hpp"

#include "HardwareFactory.hpp"
#include "HomeAssistant/Api/WebSocket/Message/Message.hpp"
#include "HomeAssistant/Api/WebSocket/RequestBuilder.hpp"
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

    mHomeAssistSocket->OnDisconnect(
        [this]() { UpdateConnectionStatus(ConnectionStatus::Disconnected); });
    AttemptConnection(false);
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
  if (!mHomeAssistSocket) {
    return;
  }

  if (!mHomeAssistSocket->isConnected()) {
    AttemptConnection();
    return;
  }

  auto threeSecondsSinceWebsocketConnection = [this]() {
    auto execTime = HardwareFactory::getAbstract().execTime();
    return execTime > mConnectionTime + std::chrono::seconds(3);
  };
  if (mAuthSession && mHomeAssistSocket->isConnected() &&
      !mAuthSession->IsAuthSent() && threeSecondsSinceWebsocketConnection()) {
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
  if (!aNewSession || !aNewSession->BorrowStartRequest()) {
    HardwareFactory::getAbstract().debugPrint(
        "Session Missing Start Request or null session provided!");
    return;
  }
  auto newRequestId = mNextRequestId++;
  aNewSession->BorrowStartRequest()->SetId(newRequestId);
  mSessions[newRequestId] = std::move(aNewSession);
}

void Api::AttemptConnection(bool aHonorTimeInterval) {
  auto execTime = HardwareFactory::getAbstract().execTime();
  auto fiveSecondsSinceRetry = [this, execTime]() {
    return execTime > mLastConnectRetry + std::chrono::seconds(5);
  };
  if (mConnectionStatus == ConnectionStatus::Connected ||
      (aHonorTimeInterval && !fiveSecondsSinceRetry())) {
    return;
  }
  mHomeAssistSocket->connect("ws://192.168.86.49:8123/api/websocket");
  mLastConnectRetry = execTime;
  // Session already sent auth so need to reset and retry
  if (!mAuthSession || mAuthSession->IsAuthSent()) {
    mAuthSession = std::make_unique<AuthSession>(mHomeAssistSocket);
  }
}

}  // namespace HomeAssist::WebSocket