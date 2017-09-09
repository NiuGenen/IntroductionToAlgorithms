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
    }
}

void end_ask_time( time_asker_t asker )
{
    if( get_asker_stat( asker_s[ asker ] ) != ASKER_STAT_USED ) return;

    clock_t         *end_clock;
    struct timeval  *end_get_time_of_day;
    struct tms      *end_times;
    time_t          *end_time;

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
    }
}

void reset_asker( time_asker_t asker )
{
    if( get_asker_stat( asker_s[ asker ] ) != ASKER_STAT_USED ) return;

    if( asker_start_time[ asker ] != NULL ){
        free( asker_start_time + asker );
        asker_start_time[ asker ] = NULL;
    }
    if( asker__end__time[ asker ] != NULL ){
        free( asker__end__time + asker );
        asker__end__time[ asker ] = NULL;
    }
}

void ask_time_clock( time_asker_t asker );
void ask_time_get_time_of_day( time_asker_t asker );
void ask_time_time( time_asker_t asker );
void ask_time_times( time_asker_t asker );

void ask_time( time_asker_t asker )
{
    if( get_asker_stat( asker_s[ asker ] ) != ASKER_STAT_USED ) return;

    switch( get_asker_type( asker_s[ asker ] ) )
    {
        default:
        case ASKER_TYPE_CLOCK:
            ask_time_clock( asker );
            break;
        case ASKER_TYPE_TIME:
            ask_time_time( asker );
            break;
        case ASKER_TYPE_TIMES:
            ask_time_times( asker );
            break;
        case ASKER_TYPE_GET_TIME_OF_DAY:
            ask_time_get_time_of_day( asker );
            break;
    }
}

void ask_time_clock( time_asker_t asker )
{
    clock_t* start = (clock_t*)asker_start_time[ asker ];
    clock_t* end   = (clock_t*)asker__end__time[ asker ];

    clock_t interval_clock = *end - *start;
    double interval_s      = (double)( interval_clock /(double)CLOCKS_PER_SEC );
    printf("------ asker clock ------\n");
    printf("CLOCKS_PER_SEC = %ld\n", CLOCKS_PER_SEC );
    printf("start = %ld\n", *start );
    printf("end   = %ld\n", *end   );
    printf("interval(clock) = %ld\n", interval_clock );
    printf("interval(s)     = %lf\n", interval_s );
    printf("-------------------------\n");
}

void ask_time_get_time_of_day( time_asker_t asker )
{
    struct timeval* start = (struct timeval*)asker_start_time[ asker ];
    struct timeval* end   = (struct timeval*)asker__end__time[ asker ];

    long interval_us = end->tv_usec - start->tv_usec 
                        + ( end->tv_sec - start->tv_sec ) * 1000 * 1000 ;
    double interval_s      = (double)( interval_us /(double)1000000 );
    printf("------ asker get_time_of_day ------\n");
    printf("start(s)  = %ld\n", start->tv_sec);
    printf("start(us) = %ld\n", start->tv_usec);
    printf("end(s)    = %ld\n", end->tv_sec);
    printf("end(us)   = %ld\n", end->tv_usec);
    printf("interval(us) = %ld\n", interval_us );
    printf("interval(s)  = %lf\n", interval_s );
    printf("-----------------------------------\n");
}

void ask_time_time( time_asker_t asker )
{
    time_t *start = (time_t*)asker_start_time[ asker ];
    time_t *end   = (time_t*)asker__end__time[ asker ];

    double interval_s = difftime( *end, *start );

    printf("------ asker time ------\n");
    printf("interval(s) = %lf\n", interval_s );
    printf("------------------------\n");
}

void ask_time_times( time_asker_t asker )
{
    struct tms* start = (struct tms*)asker_start_time[ asker ];
    struct tms* end   = (struct tms*)asker__end__time[ asker ];

    const long clktck = sysconf( _SC_CLK_TCK );

    printf("------ asker times ------\n");
    printf("start user (clock)  = %ld\n", start->tms_utime );
    printf("start sys  (clock)  = %ld\n", start->tms_stime );
    printf("start total(clock)  = %ld\n", start->tms_utime + start->tms_stime );
    printf("end user (clock)  = %ld\n", end->tms_utime );
    printf("end sys  (clock)  = %ld\n", end->tms_stime );
    printf("end total(clock)  = %ld\n", end->tms_utime + end->tms_stime );
    printf("user (s) = %ld\n", end->tms_utime - start->tms_utime );
    printf("sys  (s) = %ld\n", end->tms_stime - start->tms_stime );
    printf("total(s) = %ld\n", end->tms_utime + end->tms_stime - start->tms_utime - start->tms_stime );
    printf("user (s) = %lf\n", ( end->tms_utime - start->tms_utime) / (double) clktck );
    printf("sys  (s) = %lf\n", ( end->tms_stime - start->tms_stime) / (double) clktck );
    printf("total(s) = %lf\n", ( end->tms_utime + end->tms_stime - start->tms_utime - start->tms_stime ) / (double) clktck );
    printf("-------------------------\n");
}
