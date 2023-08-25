#include "SimulatorSPSCQueue.hpp"

template <typename T>
SimulatorSPSCQueue<T>::SimulatorSPSCQueue(uint32_t size)
{
    this->size = size;
    this->data = new T[](this->size + 1);
    this->rd_index = 0;
    this->wr_index = 0;
}

template <typename T>
SimulatorSPSCQueue<T>::~SimulatorSPSCQueue()
{
    free(this->data);
}

template <typename T>
bool SimulatorSPSCQueue<T>::isFull()
{
    return ((this->wr_index + 1) % this->size) == this->rd_index;
}

template <typename T>
bool SimulatorSPSCQueue<T>::isEmpty()
{
    return this->rd_index == this->wr_index;
}

template <typename T>
bool SimulatorSPSCQueue<T>::push(T obj)
{
    bool retval = false;
    if (!this->isFull())
    {
        retval = true;
        this->data[this->wr_index] = obj;
        this->wr_index = this->incrementIndex(this->wr_index);
    }
    return retval;
}

template <typename T>
std::optional<T> SimulatorSPSCQueue<T>::pop()
{
    std::optional<T> retval;

    retval = this->peek();

    this->rd_index = this->incrementIndex(this->rd_index);
    return retval;
}

template <typename T>
std::optional<T> SimulatorSPSCQueue<T>::peek()
{
    if (this->isEmpty())
    {
        return std::nullopt;
    }
    return this->data[this->rd_index];
}

template <typename T>
uint32_t SimulatorSPSCQueue<T>::incrementIndex(uint32_t index)
{
    return (index + 1) % this->size;
}