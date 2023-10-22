#include "WifiSettings.hpp"
#include "Keyboard.hpp"
#include "Label.hpp"
#include "List.hpp"
#include "LvglResourceManager.hpp"

using namespace UI;
using namespace UI::Page;

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
    if (aWifiInfos.empty()) {
      mScanningText->SetText("No Networks Found");
      return;
    }
    mScanningText->SetText("Networks Found");
    // Create List of wifi infos that when pressed a Keyboard opens
    for (WifiInfo wifiInfo : aWifiInfos) {
      mWifiNetworks->AddItem(wifiInfo.ssid, LV_SYMBOL_WIFI, [this, wifiInfo] {
        OpenPasswordKeyboard(wifiInfo);
      });
    }
  };

  mWifi->scan();
}

void WifiSettings::OpenPasswordKeyboard(WifiInfo aNetworkToConnectTo) {
  // We already have a Keyboard don't launch another one.
  if (mPasswordGetter) {
    return;
  }
  auto keyboard = std::make_unique<Widget::Keyboard>(
      [this, aNetworkToConnectTo](auto aUserEnteredPassword) {
        // Attempt Connection when user finishes up with keyboard input
        mWifi->connect(aNetworkToConnectTo.ssid, aUserEnteredPassword);
        mScanningText->SetText("Attempting Connection to " +
                               aNetworkToConnectTo.ssid);
        mPasswordGetter->AnimateOut();
        StartHandlingStatusUpdates();
      },
      "Password:");
  keyboard->OnKeyboardAnimatedOut([this] {
    // Keyboard is done animating out remove it and null the ref
    RemoveElement(mPasswordGetter);
    mPasswordGetter = nullptr;
  });
  mPasswordGetter = AddElement<Widget::Keyboard>(std::move(keyboard));
}

void WifiSettings::StartHandlingStatusUpdates() {
  mScanStatusHandler = [this](auto aWifiStatus) {
    if (aWifiStatus.isConnected) {
      mScanningText->SetText("Connected to " + aWifiStatus.ssid);
    } else {
      mScanningText->SetText("Failed To Connect To " + aWifiStatus.ssid);
    }
  };
}

void WifiSettings::SetHeight(lv_coord_t aHeight) {
  Base::SetHeight(aHeight);
  mScanningText->AlignTo(this, LV_ALIGN_TOP_MID);
  mScanningText->SetHeight(20);
  mScanningText->SetLongMode(LV_LABEL_LONG_SCROLL);
  const auto padding = 10;
  mWifiNetworks->AlignTo(mScanningText, LV_ALIGN_OUT_BOTTOM_MID, 0, padding);
  mWifiNetworks->SetHeight(GetContentHeight() - mScanningText->GetBottom() -
                           padding);
};
