#ifndef __PREFERENCESSTORAGE_H__
#define __PREFERENCESSTORAGE_H__

#include <Preferences.h>

extern Preferences preferences;

void init_preferences(void);
void save_preferences(void);

#endif /*__PREFERENCESSTORAGE_H__*/
