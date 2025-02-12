#pragma once

#include <cstdint>
#include <utility>

class ControllerData {
 public:
  ControllerData() = default;
  virtual ~ControllerData() = default;

  void SetA(bool aIsADown);
  void SetB(bool aIsBDown);
  void SetX(bool aIsXDown);
  void SetY(bool aIsYDown);

  void SetJoystickOne(int16_t aX, int16_t aY);

 private:
  std::pair<int16_t, int16_t> mJoyStickOneCoords{0, 0};
  std::pair<int16_t, int16_t> mJoyStickTwoCoords{0, 0};

  bool mADown{false};
  bool mBDown{false};
  bool mXDown{false};
  bool mYDown{false};
};

inline void ControllerData::SetA(bool aIsADown) { mADown = aIsADown; }
inline void ControllerData::SetB(bool aIsBDown) { mBDown = aIsBDown; }
inline void ControllerData::SetX(bool aIsXDown) { mXDown = aIsXDown; }
inline void ControllerData::SetY(bool aIsYDown) { mYDown = aIsYDown; }
inline void ControllerData::SetJoystickOne(int16_t aX, int16_t aY) {
  mJoyStickOneCoords = {aX, aY};
}