#include "wifiHandlerSim.hpp"

using WifiInfo = wifiHandlerInterface::WifiInfo;

wifiHandlerSim::wifiHandlerSim() {}

void wifiHandlerSim::begin() {}

void wifiHandlerSim::connect(std::string ssid, std::string password) {
  while (!mIsStatusThreadDone.load()) {
  }
  if (mFakeStatusThread.joinable()) {
    mFakeStatusThread.join();
  }
  mCurrentStatus.ssid = ssid;
  mCurrentStatus.isConnected = true;
  mIsStatusThreadDone = false;
  mFakeStatusThread = std::thread([this] {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mStatusUpdate->notify(mCurrentStatus);
    mIsStatusThreadDone = true;
  });
}

static const WifiInfo wifis[] = {
    WifiInfo("High Signal Wifi", -49), WifiInfo("Mid Signal Wifi", -55),
    WifiInfo("Low Signal Wifi", -65), WifiInfo("No Signal Wifi", -90)};

void wifiHandlerSim::scan() {
  while (!mIsScanThreadDone.load()) {
  }
  if (mFakeScanThread.joinable()) {
    mFakeScanThread.join();
  }
  mIsScanThreadDone = false;
  mFakeScanThread = std::thread([this] {
    std::vector<WifiInfo> info =
        std::vector(std::begin(wifis), std::end(wifis));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    mScanNotification->notify(info);
    mIsScanThreadDone = true;
  });
}
