#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

size_t DEFAULT_OUTPUT_NUM = 1024;
char DEFAULT_OUTPUT_FILE[100] = "./interval_tree.random\0";

#define RANGE 10000

void help()
{
    printf(" --- interval tree test data generator ---\n");
    printf("./[exe] data_number output_file_name\n");
    printf(" -----------------------------------------\n");
}

int main(int argc, char * argv[] )
{
    size_t OUTPUT_NUM = DEFAULT_OUTPUT_NUM ;
    if( argc > 1 ){
        OUTPUT_NUM = atoi( argv[1] );
    }else{
        printf("[WRONG] not enough arguments\n");
        help();
        printf("using default value : data_number = %lu\n", OUTPUT_NUM );
    }
    printf("Generate random data : %lu\n", OUTPUT_NUM );

    char* OUTPUT_FILE = DEFAULT_OUTPUT_FILE;

    if( argc > 2 ){
        OUTPUT_FILE = argv[2];
    }else{
        printf("[WRONG] not enough arguments\n");
        help();
        printf("using default value : output_file_name = %s\n", OUTPUT_FILE );
    }
    printf("Generate data into %s\n", OUTPUT_FILE);
    
    srand( ( unsigned int )time( 0 ) );

    ssize_t low  = 0;
    ssize_t high = 0;

    int fd = open( OUTPUT_FILE, O_RDWR | O_CREAT , 0x7777 );

    size_t sizeof_ssize = sizeof( ssize_t );
    size_t sizeof_size  = sizeof( size_t );

    size_t buf_size = sizeof_ssize * 2;
    char* buf = (char* )malloc( buf_size );

    // how many [ a , b ]
    memcpy( buf, &OUTPUT_NUM, sizeof_size );
    write( fd, buf, sizeof_size );

    // each [ a , b ] is low + high
    for( size_t i = 0; i < OUTPUT_NUM; ++i ){
        low  = rand() % RANGE + 1;
        high = rand() % RANGE + low + 1;

        memcpy( buf, &low, sizeof_ssize );
        memcpy( (char*)buf + sizeof_ssize , &high, sizeof_ssize );
        write( fd, buf, buf_size );
    }

    sync();

    close(fd);

    return 0;
}
