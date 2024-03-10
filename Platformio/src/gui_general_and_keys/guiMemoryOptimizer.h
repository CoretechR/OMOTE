#ifndef __GUIMEMORYOPTIMIZER_H__
#define __GUIMEMORYOPTIMIZER_H__

void gui_memoryOptimizer_prepare_startup();
void gui_memoryOptimizer_doAfterSliding_deletionAndCreation(lv_obj_t** tabview, int oldTabID, int newTabID, lv_obj_t** panel, lv_obj_t** img1, lv_obj_t** img2);

#endif /*__GUIMEMORYOPTIMIZER_H__*/
