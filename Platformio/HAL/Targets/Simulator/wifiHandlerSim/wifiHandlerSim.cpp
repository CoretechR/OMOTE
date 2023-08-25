#include "wifiHandlerSim.hpp"

std::shared_ptr<wifiHandlerSim> mInstance;

std::shared_ptr<wifiHandlerSim> wifiHandlerSim::getInstance()
{
    if(mInstance)
    {
      return mInstance;
    }
    mInstance = std::make_shared<wifiHandlerSim>(wifiHandlerSim());
    return mInstance;
};

wifiHandlerSim::wifiHandlerSim(){

}

void wifiHandlerSim::begin(){

}

static wifiStatus status = {
    .isConnected = true
    , .IP = "172.0.0.1"
};

void wifiHandlerSim::connect(std::shared_ptr<std::string> ssid, std::shared_ptr<std::string> password){
    status.ssid = *ssid;
    std::shared_ptr<wifiStatus> new_status = std::make_shared<wifiStatus> (wifiStatus(std::move(status)));
    this->status_update.notify(new_status);
}

static const WifiInfo wifis[] = {
    {
        .ssid = "High Signal Wifi"
        , .rssi = -49
    }
    , {
        .ssid = "Mid Signal Wifi"
        , .rssi = -55
    }
    , {
        .ssid = "Low Signal Wifi"
        , .rssi = -65
    }
    , {
        .ssid = "No Signal Wifi"
        , .rssi = -90
    }
};

void wifiHandlerSim::scan(){
    std::shared_ptr<std::vector<WifiInfo>> info = std::make_shared<std::vector<WifiInfo>>(std::vector(std::begin(wifis), std::end(wifis)));
    this->scan_notification.notify(info);
}

bool wifiHandlerSim::isAvailable(){
    return false;
}

void wifiHandlerSim::onScanDone(std::function<void (std::shared_ptr<std::vector<WifiInfo>>)> function){
    this->scan_notification.onNotify(std::move(function));
}

void wifiHandlerSim::onStatusUpdate(std::function<void (std::shared_ptr<wifiStatus>)> function){
    this->status_update.onNotify(std::move(function));
}