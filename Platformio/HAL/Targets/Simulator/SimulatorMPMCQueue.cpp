#include "SimulatorMPMCQueue.hpp"

template <typename T>
bool SimulatorMPMCQueue<T>::push(T obj)
{
    return this->push(obj, false);
}

template <typename T>
bool SimulatorMPMCQueue<T>::push(T obj, bool overwrite)
{
    bool retval = false;
    if (this->mtx.try_lock())
    {
        if (this->isFull() && overwrite)
        {
            /* If we should overwrite already written data and the buffer is full, we increment the rd_index as well.
            This has to be done in the mutex so we do not overwrite data which is already being read*/
            this->rd_index = this->incrementIndex(this->rd_index);
        }

        /* If the buffer is full, we can not write to the buffer. If overwrite is set to true, this check will never
        fail as we move the rd_index if the buffer would otherwise be full*/
        if (!this->isFull())
        {
                retval = true;
                this->data[this->wr_index] = obj;
                this->wr_index = this->incrementIndex(this->wr_index);
        }
        this->mtx.unlock();
    }
    return retval;
}

template <typename T>
std::optional<T> SimulatorMPMCQueue<T>::pop()
{
    T retval;
    if (this->mtx.try_lock()){

        if (this->isEmpty())
        {
            return std::nullopt;
        }

        retval = this->data[this->rd_index];
        this->rd_index = this->incrementIndex(this->rd_index);
        this->mtx.unlock();
        return retval;

    }
    return std::nullopt;
}

template <typename T>
std::optional<T> SimulatorMPMCQueue<T>::peek()
{
    T retval;
    if (this->mtx.try_lock())
    {
        if (this->isEmpty())
        {
            return std::nullopt;
        }

        retval = this->data[this->rd_index];
        this->mtx.unlock();
        return retval;
    }
    return std::nullopt;
}

template <typename T>
uint32_t SimulatorMPMCQueue<T>::incrementIndex(uint32_t index)
{
    return (index + 1) % this->size;
}