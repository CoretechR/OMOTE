#include "BasicUI.hpp"
#include "HardwareSimulator.hpp"
#include "omoteconfig.h"
#include <memory>

int main() {
  auto hwSim = std::make_shared<HardwareSimulator>();
  hwSim->init();

  auto ui = UI::BasicUI(hwSim);
  // ui->layout_UI();

  while (true) {
    ui.loopHandler();
  }
}