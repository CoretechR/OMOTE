#pragma once
#include "PageBase.hpp"
#include "wifiHandlerInterface.h"

namespace UI::Widget {
class List;
class Label;
} // namespace UI::Widget

namespace UI::Page {
class WifiSettings : public Base {
public:
  WifiSettings(std::shared_ptr<wifiHandlerInterface> aWifi);

  std::string GetTitle() override { return "Wifi Settings"; };

  void SetHeight(lv_coord_t aHeight) override;

private:
  std::shared_ptr<wifiHandlerInterface> mWifi;
  Handler<wifiHandlerInterface::ScanDoneDataTy> mScanCompleteHandler;

  UI::Widget::Label *mScanningText;
  UI::Widget::List *mWifiNetworks;
};

} // namespace UI::Page