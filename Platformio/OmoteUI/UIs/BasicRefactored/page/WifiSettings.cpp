#include "WifiSettings.hpp"
#include "Keyboard.hpp"
#include "Label.hpp"
#include "List.hpp"
#include "LvglResourceManager.hpp"

using namespace UI;
using namespace UI::Page;
using WifiInfo = wifiHandlerInterface::WifiInfo;

WifiSettings::WifiSettings(std::shared_ptr<wifiHandlerInterface> aWifi)
    : Base(ID::Pages::WifiSettings), mWifi(aWifi),
      mScanCompleteHandler(mWifi->ScanCompleteNotification()),
      mScanStatusHandler(mWifi->WifiStatusNotification()),
      mScanningText(AddElement<Widget::Label>(
          std::make_unique<Widget::Label>("Scanning..."))),
      mWifiNetworks(AddElement<Widget::List>(std::make_unique<Widget::List>())),
      mPasswordGetter(nullptr) {

  // Set Handler for when the wifi scan is done
  mScanCompleteHandler = [this](auto aWifiInfos) {
    mScanningText->SetText("Networks Found");
    // Create List of wifi infos
    for (WifiInfo wifiInfo : aWifiInfos) {
      mWifiNetworks->AddItem(wifiInfo.ssid, LV_SYMBOL_WIFI, [this, wifiInfo] {
        if (!mPasswordGetter) {
          // Launch a Keyboard if we dont already have one when user selects
          // list item
          auto keyboard = std::make_unique<Widget::Keyboard>(
              [this, wifiInfo](auto aUserEnteredPassword) {
                // Attempt Connection when user finishes up with keyboard input
                mWifi->connect(wifiInfo.ssid, aUserEnteredPassword);
                mScanningText->SetText("Attempting Connection to " +
                                       wifiInfo.ssid);
                mPasswordGetter->AnimateOut();
              });
          keyboard->OnKeyboardAnimatedOut([this] {
            // Once keyboard is done animating out remove it and null the ref to
            // it.
            RemoveElement(mPasswordGetter);
            mPasswordGetter = nullptr;
          });
          mPasswordGetter = AddElement<Widget::Keyboard>(std::move(keyboard));
        }
      });
    }
  };

  mScanStatusHandler = [this](auto aWifiStatus) {
    if (aWifiStatus.isConnected) {
      mScanningText->SetText("Connected to " + aWifiStatus.ssid);
    } else {
      mScanningText->SetText("Failed To Connect To" + aWifiStatus.ssid);
    }
  };

  mWifi->scan();
}

void WifiSettings::SetHeight(lv_coord_t aHeight) {
  Base::SetHeight(aHeight);
  mScanningText->AlignTo(this, LV_ALIGN_TOP_MID);
  mScanningText->SetHeight(20);
  const auto padding = 10;
  mWifiNetworks->AlignTo(mScanningText, LV_ALIGN_OUT_BOTTOM_MID, 0, padding);
  mWifiNetworks->SetHeight(GetContentHeight() - mScanningText->GetBottom() -
                           padding);
};
