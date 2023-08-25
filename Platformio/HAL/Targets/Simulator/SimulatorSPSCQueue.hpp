#pragma once
#include "SPSCQueueInterface.hpp"
#include <cstdint>

template <typename T>
class SimulatorSPSCQueue: public SPSCQueueInterface<T>
{
    public:
        SimulatorSPSCQueue(uint32_t size);
        ~SimulatorSPSCQueue();
        bool push (T obj);
        std::optional<T> pop();
        std::optional<T> peek();
        bool isFull();
        bool isEmpty();
    private:
        T* data;
        uint32_t size;
        uint32_t rd_index;
        uint32_t wr_index;
        uint32_t incrementIndex(uint32_t index);
};