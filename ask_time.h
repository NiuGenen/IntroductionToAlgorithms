#ifndef _ASK_TIME_H_
#define _ASK_TIME_H_ 

#define ITA_DBG_ENABLE
#include "dbg_info.h"

#define ASKER_TYPE_CLOCK 0
/* time.h
 * 
 * clock_t clock();
 * 
 * CLOCKS_PER_SECOND
 * 
 */
#define ASKER_TYPE_GET_TIME_OF_DAY 1
/* time.h
 * 
 * int gettimeofday( struct timeval* tv, NULL ); 
 * 
 * struct timeval{
 *     long tv_sec;  
 *     longtv_usec; 
 * }
 * 
 */
#define ASKER_TYPE_TIMES 2
/* sys/times.h
 * 
 * clock_t times(struct tms* buf );
 * 
 * struct tms{
 *     clock_ttms_utime;    // user   time
 *     clock_ttms_stime;    // system time
 *     clock_ttms_cutime;   // user   time of reaped children
 *     clock_ttms_cstime;   // system time of reaped children
 * }
 */
#define ASKER_TYPE_TIME 3
/* time.h
 * 
 * time_t time(time_t* timer);
 * 
 * double difftime(time_t time1, time_t time0); // second
 * 
 */ 
#define ASKER_TYPE_REAL 4
/* time.h
 * 
 * int clock_gettime(clockid_t clk_id, struct timespec *tp);
 * 
 * strace timespec{
 *     time_t tv_sec;
 *     long tv_nsec;
 * }
 * 
 * CLOCK_REALTIME ; standard
 * CLOCK_MONOTONIC
 * CLOCK_PROCESS_CPUTIME_ID
 * CLOCK_THREAD_CPUTIME_ID 
 */

#define ASKER_STAT_FREE 0
#define ASKER_STAT_USED 1

#include <stdio.h>

typedef size_t time_asker_t;

size_t asker_max_num();
void reset_all_asker();
time_asker_t asker_alloc( int asker_type );
void asker_free( time_asker_t asker );
int asker_valid( time_asker_t asker );

void start_ask_time( time_asker_t asker );
void end_ask_time( time_asker_t asker );
void reset_asker( time_asker_t asker );
void ask_time( time_asker_t asker );

#endif
