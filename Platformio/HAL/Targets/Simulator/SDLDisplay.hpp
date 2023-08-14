#include "DisplayAbstract.h"

class SDLDisplay : public DisplayAbstract{

public:
    static std::shared_ptr<SDLDisplay> getInstance(){
        if (!DisplayAbstract::mInstance){
            DisplayAbstract::mInstance = std::shared_ptr<SDLDisplay>(new SDLDisplay());
        }
        return std::static_pointer_cast<SDLDisplay>(mInstance);
    }

    virtual void setBrightness(uint8_t brightness) override {};
    virtual void turnOff() override {};

protected:
    virtual void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) override
    { sdl_display_flush(disp,area,color_p); };
    virtual void screenInput(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) override
    { sdl_mouse_read(indev_driver,data);}

private:
    SDLDisplay(): DisplayAbstract(){
        sdl_init();
    }

};