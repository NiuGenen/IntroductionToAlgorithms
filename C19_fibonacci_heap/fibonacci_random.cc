#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )
#define _FREE_(p) do{ \
    if( p != NULL ){ \
        free(p); \
        p = NULL; \
    } \
}while(0);

#define _EXCHANGE_(t,x,y) { \
    t _EXCHANGE_TEMP_;   \
    _EXCHANGE_TEMP_ = x; \
    x = y;               \
    y = _EXCHANGE_TEMP_; \
}

int main( int argc, char *argv[] )
{
    if( argc < 4 ){
        printf("[ERROR] please input range : ./fhran [MIN] [MAX] [OUTPUTFILENAME]\n");
        return -1;
    }
    // get range
    size_t min = atoi( argv[1] );
    size_t max = atoi( argv[2] );
    if( min > max ){
        _EXCHANGE_( size_t , min, max );
    }
    printf("Range : MIN = %-8lu MAX = %-8lu\n", min, max);
    // output file
    const char * filename = argv[3];
    printf("generate random data into %s\n", filename );
    int fd = open( filename, O_CREAT | O_WRONLY, 0666 );
    if( fd < 0 ){
        printf("[ERROR] fail to open output file\n");
        return -2;
    }
    // random 
    srand( (unsigned int )time(0) );
    // prepare to generate
    size_t total = max - min + 1;
    printf("Total number : %lu\n", total);
    size_t * num = _MALLOC_( size_t , total );
    memset( num, 0, total * sizeof(size_t) );
    for( size_t i = 0; i < total; ++i ){
        num[i] = i;
    }
    // generate
    write( fd, &min, sizeof(total) );
    write( fd, &max, sizeof(total) );
    write( fd, &total, sizeof(total) );
    while( total > 0 ){
        size_t index = rand() % total;
        size_t n = num[index];
        _EXCHANGE_( size_t, num[index], num[total-1] );
        total -= 1;
        write( fd, &n, sizeof(size_t) );
    }
    // flush 
    fsync( fd );
    printf("generate over!\n");
    // return
    close( fd );
    _FREE_( num );
    return 0;
}
