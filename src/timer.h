#include <time.h>

/*Timer macros. Use as such:
 * 	START_TIMER
 * 	[do stuff]
 * 	STOP_TIMER
 * 	PRINT_TIMER
 */

/* If this is a POSIX system, count in nanoseconds */
#define CAN_USE_NS 0
#if defined(linux) || defined(__linux__) || defined(__posix__) || defined(__unix__)
#undef CAN_USE_NS
#define CAN_USE_NS 1
#endif

#if CAN_USE_NS
/* Using nanoseconds */
#include <sys/time.h>
#define START_TIMER struct timespec KTIMER_start, KTIMER_stop;\
		clock_gettime(CLOCK_MONOTONIC_RAW, &KTIMER_start);
#define STOP_TIMER clock_gettime(CLOCK_MONOTONIC_RAW, &KTIMER_stop);
#define PRINT_TIMER printf("runtime: %lf\n", (KTIMER_stop.tv_sec-KTIMER_start.tv_sec)+(KTIMER_stop.tv_nsec - KTIMER_start.tv_nsec)/1E9);
#else
/* Using whole seconds */
#define START_TIMER time_t KTIMER_start = time(NULL);time_t KTIMER_stop;
#define STOP_TIMER  KTIMER_stop = time(NULL);
#define PRINT_TIMER printf("runtime: %ld\n", KTIMER_stop - KTIMER_start);
#endif

#undef CAN_USE_NS
