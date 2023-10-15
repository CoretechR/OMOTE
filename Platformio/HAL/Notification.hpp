#pragma once
#include <functional>
#include <map>
#include <memory>

template <class... notifyData> class Handler;
template <class... notifyData> class Notification {
  friend class Handler<notifyData...>;

public:
  typedef std::function<void(notifyData...)> HandlerTy;
  typedef int HandlerID;

  Notification() { mIdMaker = 0; };
  void notify(notifyData... notifySendData);

protected:
  HandlerID onNotify(HandlerTy aHandler);
  void unregister(HandlerID aHandler);

private:
  std::map<HandlerID, HandlerTy> mFunctionHandlers;
  HandlerID mIdMaker;
};

template <class... handlerData>
int Notification<handlerData...>::onNotify(HandlerTy aHandler) {
  if (aHandler) {
    mFunctionHandlers[++mIdMaker] = std::move(aHandler);
    return mIdMaker;
  } else {
    return -1;
  }
}

template <class... outboundData>
void Notification<outboundData...>::notify(outboundData... notifySendData) {
  for (auto handler : mFunctionHandlers) {
    handler.second(notifySendData...);
  }
}

template <class... handlerData>
void Notification<handlerData...>::unregister(HandlerID aHandlerId) {
  auto handlerToUnRegister =
      std::find_if(mFunctionHandlers.begin(), mFunctionHandlers.end(),
                   [aHandlerId](auto registeredHandler) {
                     return aHandlerId == registeredHandler.first;
                   });
  if (handlerToUnRegister != mFunctionHandlers.end()) {
    mFunctionHandlers.erase(handlerToUnRegister);
  }
}

template <class... notifyData> class Handler {
public:
  typedef std::function<void(notifyData...)> callableTy;
  void operator=(Handler &other) = delete;

  Handler() = default;
  Handler(std::shared_ptr<Notification<notifyData...>> aNotification,
          callableTy aCallable = nullptr)
      : mNotification(aNotification),
        mHandlerId(aNotification->onNotify(aCallable)) {}

  virtual ~Handler() {
    if (mHandlerId >= 0) {
      mNotification->unregister(mHandlerId);
    }
  }

  void operator=(callableTy aHandler) {
    if (mHandlerId >= 0) {
      mNotification->unregister(mHandlerId);
      mHandlerId = -1;
    }
    if (aHandler) {
      mHandlerId = mNotification->onNotify(aHandler);
    }
  }

  void
  SetNotification(std::shared_ptr<Notification<notifyData...>> aNotification) {
    mNotification = aNotification;
  }

private:
  std::shared_ptr<Notification<notifyData...>> mNotification = nullptr;
  int mHandlerId = -1;
};