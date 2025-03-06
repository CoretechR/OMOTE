#include "webSocketSimulator.hpp"

webSocketSimulator::webSocketSimulator() {
  wsClient.init_asio();
  wsClient.set_message_handler(std::bind(&webSocketSimulator::onMessage, this,
                                         std::placeholders::_1,
                                         std::placeholders::_2));
  wsClient.set_open_handler([this](auto) { Connected(); });
  wsClient.set_close_handler([this](auto) { Disconnected(); });
  //  maybe TODO wsClient.set_fail_handler();
}

webSocketSimulator::~webSocketSimulator() {
  if (wsThread.joinable()) {
    wsThread.join();
  }
}

void webSocketSimulator::connect(const std::string& url) {
  websocketpp::lib::error_code ec;
  auto con = wsClient.get_connection(url, ec);
  if (ec) {
    std::cout << "Could not create connection because: " << ec.message()
              << std::endl;
    return;
  }

  connectionHandle = con->get_handle();
  wsClient.connect(con);
  wsThread = std::thread([this] { wsClient.run(); });

  std::cout << "Connected to " << url << std::endl;
}

void webSocketSimulator::disconnect() {
  websocketpp::lib::error_code ec;
  wsClient.close(connectionHandle, websocketpp::close::status::going_away, "",
                 ec);
  if (ec) {
    std::cout << "Error closing connection: " << ec.message() << std::endl;
  }
  std::cout << "Disconnected" << std::endl;
}

void webSocketSimulator::sendMessage(const std::string& message) {
  if (isConnected()) {
    websocketpp::lib::error_code ec;
    wsClient.send(connectionHandle, message, websocketpp::frame::opcode::text,
                  ec);
    if (ec) {
      std::cout << "Error sending message: " << ec.message() << std::endl;
    }
  }
}

void webSocketSimulator::setMessageCallback(MessageCallback callback) {
  messageCallback = callback;
}

void webSocketSimulator::onMessage(websocketpp::connection_hdl hdl,
                                   client::message_ptr msg) {
  rapidjson::Document doc;
  doc.Parse(msg->get_payload().c_str());
  std::cout << "Message:" << messageNum << std::endl
            << ToPrettyString(doc) << std::endl
            << "EndMessage:" << messageNum << std::endl;
  messageNum++;

  static constexpr auto TypicalEmbeddedChunkingSize = 40000;
  static constexpr auto ChunkSize = 1024;

  auto embeddedWouldHaveChunked =
      msg->get_payload().size() > TypicalEmbeddedChunkingSize;

  if (mJsonHandler) {
    auto shouldChunkProcess = (mJsonHandler->HasChunkProcessor() &&
                               mJsonHandler->IsChunkProcessingPrefered()) ||
                              embeddedWouldHaveChunked;
    if (shouldChunkProcess) {
      size_t payloadSize = msg->get_payload().size();
      for (size_t i = 0; i < payloadSize; i += 1024) {
        std::string chunk = msg->get_payload().substr(i, 1024);
        auto result = mJsonHandler->ProcessChunk(chunk);
        if (result.mStatus != HAL::WebSocket::Json::IProcessMessage::
                                  ProcessResult::StatusCode::Success) {
          std::cout << "error";
        }
      }
    } else {
      if (mJsonHandler->ProcessJsonAsDoc(msg->get_payload())) {
        // JsonHandler parsed no need to try with the generic message handler
        return;
      }
    }
  }
  if (messageCallback) {
    messageCallback(msg->get_payload());
  }
}
