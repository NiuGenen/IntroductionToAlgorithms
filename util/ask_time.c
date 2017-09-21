#include "ask_time.h"
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define ASKER_MAX_NUM 64
/* 7 6 5 4 3 2 1 0
 * x x x x x x x x 
 * | type | stat |
 */ 
uint8_t  asker_s[ ASKER_MAX_NUM + 1 ];
void * asker_start_time[ ASKER_MAX_NUM + 1 ];
void * asker__end__time[ ASKER_MAX_NUM + 1 ];

size_t asker_max_num()
{
    return ASKER_MAX_NUM;
}

#ifndef NULL 
#define NULL 0
#endif

#define MASK_TYPE 0xf0
#define TYPE_OFFSET 4
#define TYPE_LENGTH 4

#define MASK_STAT 0x0f
#define STAT_OFFSET 0
#define STAT_LENGTH 4

int get_asker_type( uint8_t asker )
{
    return ( asker & MASK_TYPE ) >> TYPE_OFFSET ;
}

int get_asker_stat( uint8_t asker )
{
    return ( asker & MASK_STAT ) >> STAT_OFFSET ;
}

void set_asker_type( uint8_t* asker , int type )
{
    // clear type 
    *asker &= ~MASK_TYPE;
    // shift type 
    uint8_t _type = (uint8_t)type;
    for(size_t i = 0; i < TYPE_OFFSET; ++i)
        _type = _type << 1;
    // set type 
    *asker |= _type;
}

void set_asker_stat( uint8_t* asker , int stat )
{
    // clear stat 
    *asker &= ~MASK_STAT;
    uint8_t _stat = (uint8_t)stat;
    // shift stat
    for(size_t i = 0; i < STAT_OFFSET; ++i)
        _stat = _stat << 1;
    // set stat 
    *asker |= _stat;
}

void reset_all_asker()
{
    for(size_t i = 1; i <= ASKER_MAX_NUM; ++i){
        /*asker_s[ i ] = 0;*/
        set_asker_stat( asker_s + i, ASKER_STAT_FREE );
        asker_start_time[ i ] = NULL;
        asker__end__time[ i ] = NULL;
    }
}

time_asker_t asker_alloc( int asker_type )
{
    int type = ASKER_TYPE_CLOCK;
    switch( asker_type )
    {
        default:
        case ASKER_TYPE_CLOCK: type = ASKER_TYPE_CLOCK; break;
        case ASKER_TYPE_GET_TIME_OF_DAY: type = ASKER_TYPE_GET_TIME_OF_DAY; break;
        case ASKER_TYPE_TIMES: type = ASKER_TYPE_TIMES; break;
        case ASKER_TYPE_TIME:  type = ASKER_TYPE_TIME;  break;
        case ASKER_TYPE_REAL:  type = ASKER_TYPE_REAL;  break;
    }
    for( size_t i = 1; i <= ASKER_MAX_NUM; ++i ){
        if( get_asker_stat( asker_s[i] ) == ASKER_STAT_FREE ){
            set_asker_stat( asker_s + i, ASKER_STAT_USED );
            set_asker_type( asker_s + i, type );
            return i;
        }
    }
    return 0;
}

void asker_free( time_asker_t asker )
{
    /*asker_s[ asker ] = 0;*/
    set_asker_stat( asker_s + asker , ASKER_STAT_FREE );

    if( asker_start_time[ asker ] != NULL ) {
        free( asker_start_time[ asker ] );
        asker_start_time[ asker ] = NULL;
    }
    if( asker__end__time[ asker ] != NULL ){
        free( asker__end__time[ asker ] );
        asker__end__time[ asker ] = NULL;
    }
}

void start_ask_time( time_asker_t asker )
{
    if( get_asker_stat( asker_s[ asker ] ) != ASKER_STAT_USED ) return;

    clock_t         *start_clock;
    struct timeval  *start_get_time_of_day;
    struct tms      *start_times;
    time_t          *start_time;
    struct timespec *start_real;

    switch( get_asker_type( asker_s[ asker ] ) )
    {
        default:
        case ASKER_TYPE_CLOCK:
            start_clock = (clock_t *)malloc( sizeof(clock_t) );
            *start_clock = clock();

            asker_start_time[ asker ] = start_clock;
            break;
        case ASKER_TYPE_GET_TIME_OF_DAY:
            start_get_time_of_day = ( struct timeval* )malloc( sizeof( struct timeval ) );
            gettimeofday( start_get_time_of_day, NULL );

            asker_start_time[ asker ] = start_get_time_of_day;
            break;
        case ASKER_TYPE_TIMES:
            start_times = (struct tms*)malloc( sizeof( struct tms ) );
            times( start_times );

            asker_start_time[ asker ] = start_times;
            break;
        case ASKER_TYPE_TIME:
            start_time = (time_t*)malloc( sizeof( time_t ) );
            *start_time = time(NULL);

            asker_start_time[ asker ] = start_time;
            break;
        case ASKER_TYPE_REAL:
            start_real = (struct timespec*)malloc( sizeof( struct timespec ) );
            clock_gettime( CLOCK_MONOTONIC , start_real );

            asker_start_time[ asker ] = start_real;
            break;
    }
}

void end_ask_time( time_asker_t asker )
{
    if( get_asker_stat( asker_s[ asker ] ) != ASKER_STAT_USED ) return;

    clock_t         *end_clock;
    struct timeval  *end_get_time_of_day;
    struct tms      *end_times;
    time_t          *end_time;
    struct timespec *end_real;

    switch( get_asker_type( asker_s[ asker ] ) )
    {
        default:
        case ASKER_TYPE_CLOCK:
            end_clock = (clock_t *)malloc( sizeof(clock_t) );
            *end_clock = clock();

            asker__end__time[ asker ] = end_clock;
            break;
        case ASKER_TYPE_GET_TIME_OF_DAY:
            end_get_time_of_day = ( struct timeval* )malloc( sizeof( struct timeval ) );
            gettimeofday( end_get_time_of_day, NULL );

            asker__end__time[ asker ] = end_get_time_of_day;
            break;
        case ASKER_TYPE_TIMES:
            end_times = (struct tms*)malloc( sizeof( struct tms ) );
            times( end_times );

            asker__end__time[ asker ] = end_times;
            break;
        case ASKER_TYPE_TIME:
            end_time = (time_t*)malloc( sizeof( time_t ) );
            *end_time = time(NULL);

            asker__end__time[ asker ] = end_time;
            break;
        case ASKER_TYPE_REAL:
            end_real = (struct timespec*)malloc( sizeof(struct timespec) );
            clock_gettime( CLOCK_MONOTONIC, end_real );

            asker__end__time[ asker ] = end_real;
            break;
    }
}

void reset_asker( time_asker_t asker )
{
    if( get_asker_stat( asker_s[ asker ] ) != ASKER_STAT_USED ) return;

    if( asker_start_time[ asker ] != NULL ){
        free( asker_start_time[asker] );
        asker_start_time[ asker ] = NULL;
    }
    if( asker__end__time[ asker ] != NULL ){
        free( asker__end__time[asker] );
        asker__end__time[ asker ] = NULL;
    }
}

void ask_time_clock( time_asker_t asker );
void ask_time_get_time_of_day( time_asker_t asker );
void ask_time_time( time_asker_t asker );
void ask_time_times( time_asker_t asker );
void ask_time_real( time_asker_t asker );

void ask_time( time_asker_t asker )
{
    if( get_asker_stat( asker_s[ asker ] ) != ASKER_STAT_USED ) return;

    switch( get_asker_type( asker_s[ asker ] ) )
    {
        default:
        case ASKER_TYPE_CLOCK: ask_time_clock( asker ); break;
        case ASKER_TYPE_TIME:  ask_time_time( asker );  break;
        case ASKER_TYPE_TIMES: ask_time_times( asker ); break;
        case ASKER_TYPE_GET_TIME_OF_DAY: ask_time_get_time_of_day( asker ); break;
        case ASKER_TYPE_REAL:  ask_time_real( asker );  break;
    }
}

void ask_time_clock( time_asker_t asker )
{
    clock_t* start = (clock_t*)asker_start_time[ asker ];
    clock_t* end   = (clock_t*)asker__end__time[ asker ];

    long cps = CLOCKS_PER_SEC;

    clock_t interval_clock = *end - *start;
    double interval_s      = (double)( interval_clock /(double)cps );

    ITA_DBG_INFO ( ITA_DBG_T_STR  , "------ asker clock ------"            );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "CLOCKS_PER_SEC = "  , &cps            );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start = "           , start           );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "end   = "           , end             );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "interval(clock) = " , &interval_clock );
    ITA_DBG_INFO_( ITA_DBG_T_FLO  , "interval(s)     = " , &interval_s     );
    ITA_DBG_INFO ( ITA_DBG_T_STR  , "------------------------"             );
#ifndef ITA_DBG_ENABLE
    printf("------ asker clock ------\n"              );
    printf("CLOCKS_PER_SEC = %ld\n"  , cps            );
    printf("start = %ld\n"           , *start         );
    printf("end   = %ld\n"           , *end           );
    printf("interval(clock) = %ld\n" , interval_clock );
    printf("interval(s)     = %lf\n" , interval_s     );
    printf("-------------------------\n"              );
#endif
}

void ask_time_get_time_of_day( time_asker_t asker )
{
    struct timeval* start = (struct timeval*)asker_start_time[ asker ];
    struct timeval* end   = (struct timeval*)asker__end__time[ asker ];

    long interval_part_s  = end->tv_sec  - start->tv_sec  ;
    long interval_part_us = end->tv_usec - start->tv_usec ;

    long interval_us  = interval_part_us + interval_part_s * 1000 * 1000 ;
    double interval_s = (double)( interval_us /(double)1000000 );

    ITA_DBG_INFO ( ITA_DBG_T_STR  , "------ asker get_time_of_day ------"   );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start(s)  = "    , &(start->tv_sec)    );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start(us) = "    , &(start->tv_usec)   );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "end(s)    = "    , &(end->tv_sec)      );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "end(us)   = "    , &(end->tv_usec)     );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "interval(us) = " , &interval_us        );
    ITA_DBG_INFO_( ITA_DBG_T_FLO  , "interval(s)  = " , &interval_s         );
    ITA_DBG_INFO ( ITA_DBG_T_STR  , "-----------------------------------"   );

#ifndef ITA_DBG_ENABLE
    printf("------ asker get_time_of_day ------\n" );
    printf("start(s)  = %ld\n"    , start->tv_sec  );
    printf("start(us) = %ld\n"    , start->tv_usec );
    printf("end(s)    = %ld\n"    , end->tv_sec    );
    printf("end(us)   = %ld\n"    , end->tv_usec   );
    printf("interval(us) = %ld\n" , interval_us    );
    printf("interval(s)  = %lf\n" , interval_s     );
    printf("-----------------------------------\n" );
#endif
}

void ask_time_time( time_asker_t asker )
{
    time_t *start = (time_t*)asker_start_time[ asker ];
    time_t *end   = (time_t*)asker__end__time[ asker ];

    double interval_s = difftime( *end, *start );

    ITA_DBG_INFO ( ITA_DBG_T_STR  , "------ asker time ------"      );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start(s)  = "    , start       );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start(us) = "    , end         );
    ITA_DBG_INFO_( ITA_DBG_T_FLO  , "interval(s)  = " , &interval_s );
    ITA_DBG_INFO ( ITA_DBG_T_STR  , "------------------------"      );

#ifndef ITA_DBG_ENABLE
    printf("------ asker time ------\n");
    printf("start = %ld\n", *start );
    printf("end   = %ld\n", *end   );
    printf("interval(s) = %lf\n", interval_s );
    printf("------------------------\n");
#endif
}

void ask_time_times( time_asker_t asker )
{
    struct tms* start = (struct tms*)asker_start_time[ asker ];
    struct tms* end   = (struct tms*)asker__end__time[ asker ];

    const long clktck = sysconf( _SC_CLK_TCK );

    long start_total_clock = start->tms_utime + start->tms_stime;
    long end_total_clock   = end->tms_utime + end->tms_stime;

    long user_clock  = end->tms_utime  - start->tms_utime;
    long sys_clock   = end->tms_stime  - start->tms_stime;
    long total_clock = end_total_clock - start_total_clock;
    
    double user_s  = ( end->tms_utime - start->tms_utime) / (double) clktck ;
    double sys_s   = ( end->tms_stime - start->tms_stime) / (double) clktck ;
    double total_s = ( end->tms_utime + end->tms_stime - start->tms_utime - start->tms_stime ) / (double) clktck ;

    ITA_DBG_INFO ( ITA_DBG_T_STR  , "------ asker times ------");
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "clock per second = "    , &clktck              );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start user (clock)  = " , &(start->tms_utime)  );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start sys  (clock)  = " , &(start->tms_stime)  );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start total(clock)  = " , &(start_total_clock) );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "end   user (clock)  = " , &(end->tms_utime)    );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "end   sys  (clock)  = " , &(end->tms_stime)    );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "end   total(clock)  = " , &(end_total_clock)   );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "user (clock) = ", &(user_clock)  );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "sys  (clock) = ", &(sys_clock)   );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "total(clock) = ", &(total_clock) );
    ITA_DBG_INFO_( ITA_DBG_T_FLO  , "user (s) = ", &(user_s)   );
    ITA_DBG_INFO_( ITA_DBG_T_FLO  , "sys  (s) = ", &(sys_s)    );
    ITA_DBG_INFO_( ITA_DBG_T_FLO  , "total(s) = ", &(total_s)  );
    ITA_DBG_INFO ( ITA_DBG_T_STR  , "-------------------------");

#ifndef ITA_DBG_ENABLE
    printf("------ asker times ------\n");
    printf("clock per second = %ld\n", clktck);
    printf("start user (clock)  = %ld\n", start->tms_utime );
    printf("start sys  (clock)  = %ld\n", start->tms_stime );
    printf("start total(clock)  = %ld\n", start_total_clock);
    printf("end user (clock)  = %ld\n", end->tms_utime );
    printf("end sys  (clock)  = %ld\n", end->tms_stime );
    printf("end total(clock)  = %ld\n", end_total_clock);
    printf("user (clock) = %ld\n", user_clock );
    printf("sys  (clock) = %ld\n", sys_clock  );
    printf("total(clck) = %ld\n", total_clock );
    printf("user (s) = %lf\n", user_s );
    printf("sys  (s) = %lf\n", sys_s  );
    printf("total(s) = %lf\n", total_s);
    printf("-------------------------\n");
#endif
}

void ask_time_real( time_asker_t asker )
{
    struct timespec* start = (struct timespec*)asker_start_time[ asker ];
    struct timespec* end   = (struct timespec*)asker__end__time[ asker ];

    long interval_part_s  = end->tv_sec  - start->tv_sec  ;
    long interval_part_ns = end->tv_nsec - start->tv_nsec ;

    long interval_ns  = interval_part_ns + interval_part_s * 1000 * 1000 * 1000 ;
    double interval_s = (double)( interval_ns /(double)1000000000 );

    ITA_DBG_INFO ( ITA_DBG_T_STR  , "------ asker real time ------"    );
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start(s)  = " , &(start->tv_sec)  ); 
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "start(ns) = " , &(start->tv_nsec) ); 
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "end(s)    = " , &(end->tv_sec)    ); 
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "end(ns)   = " , &(end->tv_nsec)   ); 
    ITA_DBG_INFO_( ITA_DBG_T_UINT , "interval(ns) = " , &interval_ns   ); 
    ITA_DBG_INFO_( ITA_DBG_T_FLO  , "interval(s)  = " , &interval_s    );
    ITA_DBG_INFO ( ITA_DBG_T_STR  , "----------------------------"     );

#ifndef ITA_DBG_ENABLE
    printf("------ asker real time ------\n");
    printf("start(s)  = %ld\n", start->tv_sec);
    printf("start(ns) = %ld\n", start->tv_nsec);
    printf("end(s)    = %ld\n", end->tv_sec);
    printf("end(ns)   = %ld\n", end->tv_nsec);
    printf("interval(ns) = %ld\n", interval_ns );
    printf("interval(s)  = %lf\n", interval_s );
    printf("-----------------------------\n");
#endif
}
