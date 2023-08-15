// OMOTE UI Images
// 2023 Matthew Colvin
#pragma once
#include "lvgl.h"

class Images{

public:
    Images();

    lv_obj_t* addAppleTVIcon(lv_obj_t* parent);
    lv_obj_t* addAppleDisplayImage(lv_obj_t* parent);
    lv_obj_t* addAppleBackIcon(lv_obj_t* parent);
    
    lv_obj_t* addLowBrightnessIcon(lv_obj_t* parent);
    lv_obj_t* addHighBrightnessIcon(lv_obj_t* parent);
    lv_obj_t* addLightBulbIcon(lv_obj_t* parent);
    
    lv_obj_t* addLeftGradiant(lv_obj_t* parent);
    lv_obj_t* addRightGradiant(lv_obj_t* parent);

private:
    // Make Image based on anImageDesc then 
    // add that image to parent. 
    lv_obj_t* addImg(lv_obj_t* parent, lv_img_dsc_t* anImgDesc);
    void setupImageDescriptions();

    lv_img_dsc_t appleTvIcon;
    lv_img_dsc_t appleDisplayIcon;
    lv_img_dsc_t appleBackIcon;

    lv_img_dsc_t low_brightness;
    lv_img_dsc_t high_brightness;
    lv_img_dsc_t lightbulb_icon;

    lv_img_dsc_t gradientLeft;
    lv_img_dsc_t gradientRight;

    

};