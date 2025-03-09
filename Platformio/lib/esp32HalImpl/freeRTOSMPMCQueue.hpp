#pragma once
#include "MPMCQueueInterface.hpp"
#include "Arduino.h"
template <typename T>
class freeRTOSMPMCQueue: public MPMCQueueInterface<T>
{
    public:
        freeRTOSMPMCQueue(uint32_t size);
        ~freeRTOSMPMCQueue();
        bool push (T obj);
        bool push (T obj, bool overwrite);
        std::optional<T> pop();
        std::optional<T> peek();
        bool isFull();
        bool isEmpty();
    private:
        QueueHandle_t queue;
};