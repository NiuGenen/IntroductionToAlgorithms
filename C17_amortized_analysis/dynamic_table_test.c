#include "dynamic_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void value_pr( void * value )
{
    printf("%-5d", *((int*)value));
}

#define TEST_SIZE 1024

int main()
{
    struct dynamic_table * dnmc = dnmc_table_alloc( 2, value_pr );

    srand( (unsigned int)time(0) );

    for( size_t i = 0; i < TEST_SIZE; ++i ){
        int * value = ( int* )malloc( sizeof(int) );
        *value = i;
        dnmc->push( dnmc, value );
        /*dnmc->pr( dnmc );*/
        __dynamic_counter_pr();
    }

    for( size_t i = 0; i < TEST_SIZE; ++i ){
        dnmc->pop( dnmc );
        /*dnmc->pr( dnmc );*/
        __dynamic_counter_pr();
    }

    return 0;
}
