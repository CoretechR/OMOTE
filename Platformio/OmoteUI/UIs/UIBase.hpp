// OMOTE UI
// 2023 Matthew Colvin

#pragma once
#include "HardwareAbstract.hpp"
#include <memory>

class UIBase{
public:
    UIBase(std::shared_ptr<HardwareAbstract> aHardware):
        mHardware(aHardware){}
    
protected:
    std::shared_ptr<HardwareAbstract> mHardware;

};