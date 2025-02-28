#pragma once
#include <iostream>
#include <thread>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

#include "webSocketInterface.hpp"

class webSocketSimulator : public webSocketInterface {
 public:
  webSocketSimulator();
  ~webSocketSimulator() override;

  void connect(const std::string& url) override;
  void disconnect() override;
  void sendMessage(const std::string& message) override;
  void setMessageCallback(MessageCallback callback) override;
  bool isConnected() const override;

 private:
  using client = websocketpp::client<websocketpp::config::asio_client>;
  client wsClient;
  websocketpp::connection_hdl connectionHandle;
  std::thread wsThread;
  bool connected = false;
  MessageCallback messageCallback;

  void onMessage(websocketpp::connection_hdl hdl, client::message_ptr msg);
};
