#include "freeRTOSMPMCQueue.hpp"

template <typename T>
freeRTOSMPMCQueue<T>::freeRTOSMPMCQueue(uint32_t size)
{
    this->queue = xQueueCreate(size, sizeof(T));
}

template <typename T>
freeRTOSMPMCQueue<T>::~freeRTOSMPMCQueue()
{
    vQueueDelete(this->queue);
}

template <typename T>
bool freeRTOSMPMCQueue<T>::push(T obj)
{
    return xQueueSendToBack(this->queue, &obj, 0) == pdTRUE;
}

template <typename T>
bool freeRTOSMPMCQueue<T>::push(T obj, bool overwrite)
{
    if (overwrite == true)
    {
        xQueueOverwrite(this->queue, obj);
        return true;
    }
    else
    {
        return this->push(obj);
    }
}

template <typename T>
std::optional<T> freeRTOSMPMCQueue<T>::pop()
{
    T retval;

    if (xQueueReceive(this->queue, &retval, 0) == pdTRUE)
    {
        return retval;
    }

    return std::nullopt;
}

template <typename T>
std::optional<T> freeRTOSMPMCQueue<T>::peek()
{
    T retval;

    if (xQueuePeek(this->queue, &retval, 0) == pdTRUE)
    {
        return retval;
    }
    return std::nullopt;
}

template <typename T>
bool freeRTOSMPMCQueue<T>::isFull()
{
    return (xQueueIsQueueFullFromISR(this->queue) == pdTRUE);
}

template <typename T>
bool freeRTOSMPMCQueue<T>::isEmpty()
{
    return (xQueueIsQueueEmptyFromISR(this->queue) == pdTRUE);
}