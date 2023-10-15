#include "WifiSettings.hpp"
#include "Label.hpp"
#include "List.hpp"
#include "LvglResourceManager.hpp"
//#include <Arduino.h>

using namespace UI;
using namespace UI::Page;

WifiSettings::WifiSettings(std::shared_ptr<wifiHandlerInterface> aWifi)
    : Base(ID::Pages::WifiSettings), mWifi(aWifi),
      mScanCompleteHandler(mWifi->ScanCompleteNotification()),
      mScanningText(AddElement<Widget::Label>(
          std::make_unique<Widget::Label>("Scanning..."))),
      mWifiNetworks(
          AddElement<Widget::List>(std::make_unique<Widget::List>())) {

  mScanCompleteHandler = [this](auto aWifiInfos) {
    // Serial.println("populating UI");
    mScanningText->SetText("Networks Found");
    for (WifiInfo wifiInfo : aWifiInfos) {
      mWifiNetworks->AddItem(wifiInfo.ssid, LV_SYMBOL_WIFI, [] {});
    }
  };

  mWifi->scan();

  // mWifi->onScanDone([this](auto aWifiInfos) {
  //
  // });
}

void WifiSettings::SetHeight(lv_coord_t aHeight) {
  Base::SetHeight(aHeight);
  mScanningText->AlignTo(this, LV_ALIGN_TOP_MID);
  mScanningText->SetHeight(15);
  const auto padding = 10;
  mWifiNetworks->AlignTo(mScanningText, LV_ALIGN_OUT_BOTTOM_MID, 0, padding);
  mWifiNetworks->SetHeight(GetContentHeight() - mScanningText->GetBottom() -
                           padding);
};
