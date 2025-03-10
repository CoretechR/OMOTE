#pragma once

#include <functional>
#include <memory>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Task {
 public:
  using TaskCallableType = std::function<void()>;
  using TaskCompleteCallableType = std::function<void()>;

  Task(TaskCallableType aCallable);
  virtual ~Task() = default;

  std::string GetName();

  void run();
  void complete();

 private:
  TaskCallableType mTaskCallable;
  TaskCompleteCallableType mCompleteCallable;
  std::string mTaskName;
};

/**
 * Class owns a given task and controls a FreeRtos thread
 * in order to execute that Task.
 *
 * Task will be ran for the lifetime of this class unless Detach is called
 * When Detach is called the BackgroundThread will keep its self alive until
 * the given task has completed
 */
class BackgroundThread : public std::enable_shared_from_this<BackgroundThread> {
 public:
  static std::shared_ptr<BackgroundThread> Create(Task aNewTask);

  virtual ~BackgroundThread();

  bool IsRunning();

  void Start();
  void Stop();
  void Detach();

 private:
  BackgroundThread(Task aTaskToBackground);

  void RunTaskOnNewThread();
  static void ThreadImpl(void *aThread);

 private:
  TaskHandle_t mFreeRTOSHandle = nullptr;
  Task mTask;
  std::shared_ptr<BackgroundThread> mKeepAliveSelf = nullptr;
  bool mIsStopping = false;
};