#include "wifiHandlerSim.hpp"

using WifiInfo = wifiHandlerInterface::WifiInfo;

wifiHandlerSim::wifiHandlerSim() {}

void wifiHandlerSim::begin() {}

void wifiHandlerSim::connect(std::string ssid, std::string password) {
  status.ssid = ssid;
  mStatusUpdate->notify(wifiStatus(status));
}

static const WifiInfo wifis[] = {
    WifiInfo("High Signal Wifi", -49), WifiInfo("Mid Signal Wifi", -55),
    WifiInfo("Low Signal Wifi", -65), WifiInfo("No Signal Wifi", -90)};

void wifiHandlerSim::scan() {
  std::vector<WifiInfo> info = std::vector(std::begin(wifis), std::end(wifis));
  mScanNotification->notify(info);
}

bool wifiHandlerSim::isAvailable() { return false; }
