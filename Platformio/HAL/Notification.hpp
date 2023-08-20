#pragma once
#include <vector>
#include <functional>

template <class... notifyData>
class Notification{
    public:
        typedef std::function<void(notifyData...)> HandlerTy;

        Notification() = default;
        void onNotify(HandlerTy aHandler);
        void notify(notifyData... notifySendData);

    private:
        std::vector<HandlerTy> mFunctionHandlers;
};


template <class... handlerData>
void Notification<handlerData...>::onNotify(HandlerTy aHandler){
    mFunctionHandlers.push_back(std::move(aHandler));
}

template <class... outboundData>
void Notification<outboundData...>::notify(outboundData... notifySendData){
    for (auto handler : mFunctionHandlers){
        handler(notifySendData...);
    }
}