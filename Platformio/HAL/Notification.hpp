#pragma once
#include <vector>
#include <functional>

template <class... notifyData>
class Notification{
    public:
        Notification() = default;
        void onNotify(std::function<void(notifyData...)> aHandler);
        void notify(notifyData... notifySendData);

    private:
        std::vector<std::function<void(notifyData...)>> mFunctionHandlers;
};

template <class... handlerData>
void Notification<handlerData...>::onNotify(std::function<void(handlerData...)> aHandler){
    mFunctionHandlers.push_back(std::move(aHandler));
}

template <class... outboundData>
void Notification<outboundData...>::notify(outboundData... notifySendData){
    for (auto handler : mFunctionHandlers){
        handler(notifySendData...);
    }
}
