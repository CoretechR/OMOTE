#pragma once
#include <mutex>

#include "oldHAL/MPMCQueueInterface.hpp"
#include "oldHAL/Targets/Simulator/SimulatorSPSCQueue.hpp"

template <typename T>
class SimulatorMPMCQueue : public SimulatorSPSCQueue<T>,
                           public MPMCQueueInterface<T> {
 public:
  SimulatorMPMCQueue(uint32_t size) : SimulatorSPSCQueue<T>(size) {};
  bool push(T obj);
  bool push(T obj, bool overwrite);
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
  std::mutex mtx;
};