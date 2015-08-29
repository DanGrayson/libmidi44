#include <stdio.h>
#include "midi.h"
#include "util.h"

void chk(const char *func, int err) {
     if (err) {
	  fprintf(stderr, "Failed to %s.  Error code %d.\n", func,err);
	  exit(ERROR);
	  }
     }

mach_timebase_info_data_t timebase_info;

void  __attribute__((constructor)) timebase_info_init()
{
    mach_timebase_info(&timebase_info);
}

uint64_t abs_to_ns(uint64_t absolute_time)
{
    uint64_t nanoseconds = (__uint128_t)absolute_time * timebase_info.numer / timebase_info.denom;
    return nanoseconds;
}
