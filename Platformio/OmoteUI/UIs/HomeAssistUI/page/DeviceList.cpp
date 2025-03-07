#include "DeviceList.hpp"

#include "HomeAssistant/Api/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssistant/Api/WebSocket/Request.hpp"
#include "HomeAssistant/Api/WebSocket/Session/Session.hpp"
#include "List.hpp"
#include "UIElementIds.hpp"

namespace UI::Page {

DeviceList::DeviceList(HomeAssist::WebSocket::Api& aApi)
    : Base(ID::Pages::HomeAssistDeviceList),
      mDeviceList(AddNewElement<Widget::List>()),
      mLoadingArc(AddNewElement<Widget::Arc>()),
      mApi(aApi),
      mDeviceQueryProcessor(std::make_shared<UI::DevicesQueryProcessor>(
          [this](const auto& aEntity) { AddEntity(aEntity); })) {
  mLoadingArc->SetRange(0, 100);
  mLoadingArc->SetWidth(GetContentWidth() / 2);
  mLoadingArc->SetHeight(GetContentHeight() / 4);
  mLoadingArc->AlignTo(this, LV_ALIGN_TOP_MID);
  mDeviceList->AlignTo(mDeviceList, LV_ALIGN_OUT_BOTTOM_MID);

  mDeviceQueryProcessor->setRequestProcessCompleteCallback(
      [this](auto aResult) {
        if (aResult) {
          mLoadingArc->SetVisiblity(false);
          mDeviceList->AlignTo(this, LV_ALIGN_TOP_MID);
        } else {
          // Todo throw an error up?
        }
      });

  mDeviceQueryProcessor->setPercentCompleteCallback(
      [this](const auto& aPercentComplete) {
        LvglResourceManager::GetInstance().AttemptNow(
            [this, aPercentComplete]() {
              mLoadingArc->SetValue(aPercentComplete);
            });
      });

  auto entityRequest = std::make_unique<HomeAssist::WebSocket::Request>(
      HomeAssist::GetEntityMessage);

  mApi.AddSession(std::make_unique<HomeAssist::WebSocket::Session>(
      std::move(entityRequest), nullptr, mDeviceQueryProcessor));

  InitializeUI();
}

bool DeviceList::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return false;
}

void DeviceList::AddEntity(const std::string& aEntity) {
  std::string entityDeepCopy = aEntity;
  LvglResourceManager::GetInstance().AttemptNow([this, entityDeepCopy]() {
    constexpr auto lightStr = "light";
    if (entityDeepCopy.rfind(lightStr, 0) == 0) {
      mDeviceList->AddItem(entityDeepCopy, LV_SYMBOL_OK, [] {});
    }
  });
}

void DeviceList::InitializeUI() {
  // Initialize UI elements here
}

}  // namespace UI::Page
