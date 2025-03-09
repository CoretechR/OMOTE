#pragma once
#include "SPSCQueueInterface.hpp"

template <typename T>
class MPMCQueueInterface: public SPSCQueueInterface<T>
{
    public:
        bool push(T obj, bool overwrite = false);
};