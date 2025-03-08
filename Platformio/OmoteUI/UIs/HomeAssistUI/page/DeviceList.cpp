#include "DeviceList.hpp"

#include "HomeAssistant/Api/WebSocket/Request.hpp"
#include "HomeAssistant/Api/WebSocket/RequestBuilder.hpp"
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

  {
    using namespace HomeAssist::WebSocket;
    auto request =
        RequestBuilder()
            .SetType(RequestTypes::CONFIG_ENTITY_REGISTRY_LIST_DISPLAY)
            .BuildUnique();

    mApi.AddSession(std::make_unique<Session>(std::move(request), nullptr,
                                              mDeviceQueryProcessor));
  }
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

}  // namespace UI::Page
