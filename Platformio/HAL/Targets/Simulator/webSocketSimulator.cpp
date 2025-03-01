#include "webSocketSimulator.hpp"

webSocketSimulator::webSocketSimulator() {
  wsClient.init_asio();
  wsClient.set_message_handler(std::bind(&webSocketSimulator::onMessage, this,
                                         std::placeholders::_1,
                                         std::placeholders::_2));
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

  connected = true;
  std::cout << "Connected to " << url << std::endl;
}

void webSocketSimulator::disconnect() {
  websocketpp::lib::error_code ec;
  wsClient.close(connectionHandle, websocketpp::close::status::going_away, "",
                 ec);
  if (ec) {
    std::cout << "Error closing connection: " << ec.message() << std::endl;
  }
  connected = false;
  std::cout << "Disconnected" << std::endl;
}

void webSocketSimulator::sendMessage(const std::string& message) {
  if (connected) {
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

bool webSocketSimulator::isConnected() const { return connected; }

void webSocketSimulator::onMessage(websocketpp::connection_hdl hdl,
                                   client::message_ptr msg) {
  if (messageCallback) {
    messageCallback(msg->get_payload());
  }
}
