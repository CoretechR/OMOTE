#include <sys/time.h>
#include <cstddef>

long long current_timestamp_hal_windowsLinux() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\r\n", milliseconds);
    return milliseconds;
}