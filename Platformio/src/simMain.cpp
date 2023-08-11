#include "HardwareSimulator.hpp"
#include "omoteconfig.h"
#include "OmoteUI.hpp"
#include <memory> 

int main(){
    auto hwSim = std::make_shared<HardwareSimulator>();
    hwSim->init();

    auto ui = OmoteUI::getInstance(hwSim);
    ui->layout_UI();

    while (true){
        ui->loopHandler();
        lv_task_handler();
    }
}