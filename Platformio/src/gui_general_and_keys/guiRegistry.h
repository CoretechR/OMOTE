#ifndef __GUIREGISTRY_H__
#define __GUIREGISTRY_H__

/*
  If you want to create a new GUI for the touch screen, then
  - copy one of the existing GUIs, e.g. gui_numpad.cpp and gui_numpad.h
  - place the files in the right folder:
    - in folder "gui_general_and_keys" if they are of general purpose, not only for a specific device
    - in the device folder, if the GUI is specific for a certain device
    - in folder "scenes" if the GUI is intended for a specific scene
    It does not really matter where the files are placed, but doing so helps to keep a clean structure
  - rename the functions, they must not have the same name as the ones in the file you copied from
  - call "register_gui_<nameOfGUI>();" in main.cpp

  Technically spoken, to register a GUI, you need to provide two function pointers:
  - one for creating the tab
  - one for creating the page indicator at the bottom of the tab
*/
#include <string>
#include <lvgl.h>

typedef void (*init_gui_tab)(lv_obj_t* tabview);
typedef void (*init_gui_pageIndicator)(lv_obj_t* tabview);

void register_gui(init_gui_tab a_init_gui_tab, init_gui_pageIndicator a_gui_pageIndicator);
void create_gui_tabs_from_gui_registry(lv_obj_t* tabview);
void create_gui_pageIndicators_from_gui_registry(lv_obj_t* panel);

#endif /*__GUIREGISTRY_H__*/
