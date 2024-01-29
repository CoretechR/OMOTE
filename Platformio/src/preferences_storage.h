#ifndef __PREFERENCES_STORAGE_H__
#define __PREFERENCES_STORAGE_H__

#include <Preferences.h>

extern Preferences preferences;

void init_preferences(void);
void save_preferences(void);

#endif /*__PREFERENCES_STORAGE_H__*/
