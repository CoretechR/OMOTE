#ifndef __MEMORYUSAGE_H__
#define __MEMORYUSAGE_H__

//#define SHOW_USED_MEMORY_INSTEAD_OF_FREE_IN_GUI // comment it out to see free memory instead of used memory in GUI. Serial log will always show both.

bool getShowMemoryUsage();
void setShowMemoryUsage(bool aShowMemoryUsage);
void doLogMemoryUsage(void);

#endif /*__MEMORYUSAGE_H__*/
