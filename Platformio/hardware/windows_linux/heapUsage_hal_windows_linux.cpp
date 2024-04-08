#include <malloc.h>

#if defined(WIN32)
// https://www.daniweb.com/programming/software-development/threads/135188/calculate-the-amount-of-heap-memory
// returns used heap size in bytes or negative if heap is corrupted.
long HeapUsed()
{
    _HEAPINFO info = { 0, 0, 0 };
    long used = 0;
    int rc;

    while ((rc=_heapwalk(&info)) == _HEAPOK)
    {
        if (info._useflag == _USEDENTRY)
            used += info._size;
        }
    if (rc != _HEAPEND && rc != _HEAPEMPTY)
        used = (used?-used:-1);

    return used;
}

#elif defined(__linux__)
long HeapUsed() {
  // don't know how to get used heap size in linux
  return 80000;
}
#endif

void get_heapUsage_HAL(unsigned long *heapSize, unsigned long *freeHeap, unsigned long *maxAllocHeap, unsigned long *minFreeHeap) {
  *freeHeap =  HeapUsed();
  // Don't know how big heap can get including swap. So we report twice the size of used heap. With that, free heap is the same as used heap.
  *heapSize = 2 * *freeHeap;
  *maxAllocHeap = 0;
  *minFreeHeap = 0;
}
