#include "DisplayAbstract.h"

std::shared_ptr<DisplayAbstract> DisplayAbstract::mInstance = nullptr;

DisplayAbstract::DisplayAbstract(){
    lv_init();

    lv_disp_draw_buf_init(&mdraw_buf, mbufA, mbufB,
                        SCREEN_WIDTH * SCREEN_HEIGHT / 10);

    // Initialize the display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = &DisplayAbstract::flushDisplayImpl;
    disp_drv.draw_buf = &mdraw_buf;
    lv_disp_drv_register(&disp_drv);

    // Initialize the touchscreen driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = &DisplayAbstract::screenInputImpl;
    lv_indev_drv_register(&indev_drv);

}

void DisplayAbstract::flushDisplayImpl(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    mInstance->flushDisplay(disp, area, color_p);
}

void DisplayAbstract::screenInputImpl(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    mInstance->screenInput(indev_driver, data);
}