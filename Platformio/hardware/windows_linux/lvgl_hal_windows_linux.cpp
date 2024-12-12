#include <SDL2/SDL.h>
#include <lvgl.h>
#include <stdlib.h>

#include "keypad_gui/keypad_gui.h"

void init_lvgl_HAL() {
  // Workaround for sdl2 `-m32` crash
  // https://bugs.launchpad.net/ubuntu/+source/libsdl2/+bug/1775067/comments/7
  #ifndef WIN32
    setenv("DBUS_FATAL_WARNINGS", "0", 1);
  #endif

  // https://github.com/lvgl/lv_platformio/issues/59
  // https://github.com/lvgl/lv_port_pc_eclipse/blob/master/main.c#L111
  // https://github.com/lvgl/lv_port_pc_eclipse/blob/master/lv_conf.h#L832-L839
  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(SDL_HOR_RES, SDL_VER_RES);
  lv_display_set_default(disp);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);

  // LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  // lv_obj_t * cursor_obj;
  // cursor_obj = lv_image_create(lv_screen_active());   /*Create an image object for the cursor */
  // lv_image_set_src(cursor_obj, &mouse_cursor_icon);   /*Set the image source*/
  // lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * keyboard = lv_sdl_keyboard_create();
  lv_indev_set_display(keyboard, disp);
  lv_indev_set_group(keyboard, lv_group_get_default());

  lv_sdl_window_set_title(disp, "OMOTE simulator");
  lv_sdl_window_set_zoom(disp, SDL_ZOOM);

  // Get the SDL window via an event
  SDL_Event aWindowIdFinder;
  SDL_PollEvent(&aWindowIdFinder);
  SDL_Window *mSimWindow = SDL_GetWindowFromID(aWindowIdFinder.window.windowID);

  // Setup the keypad window
  SDL_Window *keypadWindow = keypad_gui_setup();

  // Raise the simulator window, sometimes it will get buried on macOS
  SDL_RaiseWindow(mSimWindow);

  // Arrange our windows nicely
  int x, y;
  SDL_GetWindowPosition(keypadWindow, &x, &y);
  SDL_SetWindowPosition(mSimWindow, x - (SDL_HOR_RES * SDL_ZOOM) / 2 - 10, y);
  SDL_SetWindowPosition(keypadWindow, x + (SDL_HOR_RES * SDL_ZOOM) / 2 + 10, y);

}
