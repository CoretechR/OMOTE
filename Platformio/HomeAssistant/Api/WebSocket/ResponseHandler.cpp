#include "HomeAssistant/Api/WebSocket/ResponseHandler.hpp"

#include "HomeAssistant/Api/WebSocket/ChunkForwarder.hpp"
#include "HomeAssistant/Api/WebSocket/Message/Message.hpp"
#include "HomeAssistant/Api/WebSocket/Session/ISession.hpp"

using namespace HAL::WebSocket::Json;

namespace HomeAssist::WebSocket {

ResponseHandler::ResponseHandler(Api& aApi)
    : IProcessMessage(
          [this](const auto& aDoc) { return ProcessResponseDoc(aDoc); },
          std::make_unique<ChunkForwarder>(aApi)),
      mApi(aApi) {}

bool ResponseHandler::ProcessResponseDoc(
    const MemConciousDocument& aDocFromSocket) {
  // auto prettyDebugString = ToPrettyString(aDocFromSocket);
  // HardwareFactory::getAbstract().debugPrint("%s", prettyDebugString.c_str());

  if (HandleRedirectToChunkProcessor(aDocFromSocket)) {
    return true;
  }

  auto messageObj = std::make_unique<Message>(aDocFromSocket);
  if (mApi.PreProcessMessage(*messageObj)) {
    return true;
  }
  mApi.mIncomingMessageQueue.push(std::move(messageObj));
  return true;
}

bool ResponseHandler::HandleRedirectToChunkProcessor(
    const MemConciousDocument& aDoc) {
  if (aDoc.HasMember("id") && aDoc["id"].IsInt()) {
    auto sessionId = aDoc["id"].GetInt();
    if (auto& session = mApi.mSessions[sessionId]; session) {
      if (session->IsPreferringChunkProcessing()) {
        if (auto processor = session->GetChunkProcessor(); processor) {
          aDoc.Accept(*processor);
          return true;
        }
      }
    }
  }
  return false;
}

ResponseHandler::~ResponseHandler() {}

}  // namespace HomeAssist::WebSocket
