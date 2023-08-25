#pragma once
#include <optional>

template <typename T>
class SPSCQueueInterface {
    public:
        virtual bool push(T obj) = 0;
        virtual std::optional<T> pop() = 0;
        virtual std::optional<T> peek() = 0;
        virtual bool isFull() = 0;
        virtual bool isEmpty() = 0;
};