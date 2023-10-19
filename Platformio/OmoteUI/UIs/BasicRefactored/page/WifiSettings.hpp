#pragma once
#include "PageBase.hpp"
#include "wifiHandlerInterface.h"

namespace UI::Widget {
class List;
class Label;
class Keyboard;
} // namespace UI::Widget

namespace UI::Page {
class WifiSettings : public Base {
using WifiInfo = wifiHandlerInterface::WifiInfo;
public:
  WifiSettings(std::shared_ptr<wifiHandlerInterface> aWifi);

  std::string GetTitle() override { return "Wifi Settings"; };

  void SetHeight(lv_coord_t aHeight) override;

protected:
  void StartHandlingStatusUpdates();

  void OpenPasswordKeyboard(WifiInfo aNetworkToConnectTo);

private:

  std::shared_ptr<wifiHandlerInterface> mWifi;
  Handler<wifiHandlerInterface::ScanDoneDataTy> mScanCompleteHandler;
  Handler<wifiHandlerInterface::wifiStatus> mScanStatusHandler;

  UI::Widget::Label *mScanningText;
  UI::Widget::List *mWifiNetworks;
  UI::Widget::Keyboard *mPasswordGetter;
};

} // namespace UI::Page