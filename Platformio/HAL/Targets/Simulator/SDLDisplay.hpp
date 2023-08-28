#pragma once
#include "DisplayAbstract.h"

class SDLDisplay : public DisplayAbstract{

public:
    static std::shared_ptr<SDLDisplay> getInstance();

    virtual void setBrightness(uint8_t brightness) override;
    virtual uint8_t getBrightness() override;
    virtual void turnOff() override;

protected:
    virtual void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) override;
    virtual void screenInput(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) override;

private:
    SDLDisplay();

};