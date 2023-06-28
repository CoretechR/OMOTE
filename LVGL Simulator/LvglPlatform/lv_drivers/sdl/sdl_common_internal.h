/**
 * @file sdl_common_internal.h
 * Provides SDL related functions which are only used internal.
 *
 */

#ifndef SDL_COMMON_INTERNAL_H
#define SDL_COMMON_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "sdl_common.h"

#if USE_SDL || USE_SDL_GPU

#include SDL_INCLUDE_PATH

/**********************
 * GLOBAL PROTOTYPES
 **********************/
int quit_filter(void * userdata, SDL_Event * event);

void mouse_handler(SDL_Event * event);
void mousewheel_handler(SDL_Event * event);
uint32_t keycode_to_ctrl_key(SDL_Keycode sdl_key);
void keyboard_handler(SDL_Event * event);

#endif /* USE_SDL || USE_SDL_GPU */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SDL_COMMON_INTERNAL_H */
