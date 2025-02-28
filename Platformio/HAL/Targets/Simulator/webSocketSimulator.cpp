#include "webSocketSimulator.hpp"

void webSocketSimulator::connect(const std::string& url) {
  connected = true;
  std::cout << "Connected to " << url << std::endl;
}

void webSocketSimulator::disconnect() {
  connected = false;
  std::cout << "Disconnected" << std::endl;
}

void webSocketSimulator::sendMessage(const std::string& message) {
  if (connected && messageCallback) {
    messageCallback(message);
  }
}

void webSocketSimulator::setMessageCallback(MessageCallback callback) {
  messageCallback = callback;
}

bool webSocketSimulator::isConnected() const { return connected; }
