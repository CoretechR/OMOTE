#pragma once

void gui_memoryOptimizer_prepare_startup();
void gui_memoryOptimizer_doAfterSliding_deletionAndCreation(lv_obj_t** tabview, int oldTabID, int newTabID, bool newGuiList, lv_obj_t** panel, lv_obj_t** img1, lv_obj_t** img2);
bool gui_memoryOptimizer_isTabIDInMemory(int tabID);
bool gui_memoryOptimizer_isGUInameInMemory(std::string guiName);
