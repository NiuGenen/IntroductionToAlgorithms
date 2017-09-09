#include "ask_time.h"

size_t time_consuming()
{
    size_t ret = 0;
    int i = 1;
    while( i < 1000000000 ){
        ret += i;
        ret *= i;
        i += 1;
    }

    return ret;
}

int main()
{
    // clock 
    time_asker_t asker_clock = asker_alloc( ASKER_TYPE_CLOCK );
    start_ask_time( asker_clock );
    time_consuming();
    end_ask_time( asker_clock );
    ask_time( asker_clock );

    // get time of day 
    time_asker_t asker_get_time_of_day = asker_alloc( ASKER_TYPE_GET_TIME_OF_DAY );
    start_ask_time( asker_get_time_of_day );
    time_consuming();
    end_ask_time( asker_get_time_of_day );
    ask_time( asker_get_time_of_day );
    
    // time 
    time_asker_t asker_time = asker_alloc( ASKER_TYPE_TIME );
    start_ask_time( asker_time );
    time_consuming();
    end_ask_time( asker_time );
    ask_time( asker_time );

    // times 
    time_asker_t asker_times = asker_alloc( ASKER_TYPE_TIMES );
    start_ask_time( asker_times );
    time_consuming();
    end_ask_time( asker_times );
    ask_time( asker_times );

    // free asker
    asker_free( asker_clock );
    asker_free( asker_get_time_of_day );
    asker_free( asker_time );
    asker_free( asker_times );
}
