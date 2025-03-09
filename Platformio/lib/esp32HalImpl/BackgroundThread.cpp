
#include "BackgroundThread.hpp"

#include <Arduino.h>

/**
 * Class puts given task on the background and control
 * its life time with the scope of this class unless
 * Detact() is called
 */

Task::Task(TaskCallableType aCallable) : mTaskCallable(aCallable) {}

void Task::run() {
  if (mTaskCallable) {
    mTaskCallable();
  }
};

void Task::complete() {
  if (mCompleteCallable) {
    mCompleteCallable();
  }
}

std::string Task::GetName() { return ""; }

std::shared_ptr<BackgroundThread> BackgroundThread::Create(Task aNewTask) {
  return std::shared_ptr<BackgroundThread>(new BackgroundThread(aNewTask));
}

BackgroundThread::~BackgroundThread() {
  Serial.println("Destruct BG Task");
  Stop();
}
/**
 * True - the thread is running
 */
bool BackgroundThread::IsRunning() { return mFreeRTOSHandle != nullptr; }

void BackgroundThread::Start() {
  if (IsRunning()) {
    return;
  }
  mIsStopping = false;
  Serial.println("Start BG Task");
  xTaskCreate(&BackgroundThread::ThreadImpl,
              std::string("Background Thread Task:" + mTask.GetName()).c_str(),
              1024, this, 5, &mFreeRTOSHandle);
}

void BackgroundThread::Stop() {
  if (mIsStopping) {
    return;
  }
  mIsStopping = true;

  std::string stopLog = "Stop BG Task)";
  stopLog += mKeepAliveSelf ? "self->use_count:" +
                                  std::to_string(mKeepAliveSelf.use_count())
                            : "";
  Serial.println(stopLog.c_str());

  // We have not detached thread since KeepAlive is null so need to kill
  // If we did detach we will allow FreeRtos Task to clean its self up in
  // BackgroundThread::ThreadImpl
  if (mKeepAliveSelf == nullptr && mFreeRTOSHandle != nullptr) {
    vTaskDelete(mFreeRTOSHandle);
  }
};

/**
 * Do no cancel task if this object gets destroyed
 */
void BackgroundThread::Detach() { mKeepAliveSelf = shared_from_this(); }

BackgroundThread::BackgroundThread(Task aTaskToBackground)
    : mTask(aTaskToBackground) {
  Serial.println("create BG Task");
}

void BackgroundThread::RunTaskOnNewThread() {
  mTask.run();
  mTask.complete();
}

/*Static*/
void BackgroundThread::ThreadImpl(void *aThread) {
  auto *self = static_cast<BackgroundThread *>(aThread);
  self->RunTaskOnNewThread();
  self->Stop();
  self->mKeepAliveSelf = nullptr;
  vTaskDelete(nullptr);
}
