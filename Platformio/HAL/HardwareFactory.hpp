#include "HardwareAbstract.hpp"
#include <memory>
/**
 * @brief The HardwareFactory is responsible for making the
 */
class HardwareFactory {
public:
  static HardwareAbstract &getAbstract();

  static std::unique_ptr<HardwareAbstract> mHardware;
};
