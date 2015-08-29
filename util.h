#include <mach/mach.h>
#include <mach/mach_time.h>
extern void chk(const char *func, int err);
extern mach_timebase_info_data_t timebase_info;
extern void  __attribute__((constructor)) timebase_info_init();
extern uint64_t abs_to_ns(uint64_t absolute_time);

