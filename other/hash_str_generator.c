#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define OUTPUT_FILE "./hash_str.random"

#define OUTPUT_NUM 1024
#define STR_MAX_LEN 1024

/*
 * each string is length + charactors
 * 
 * size_t length : n
 * char str[n] : 'a''b''c'...'n'
 */
int main()
{
    srand( (unsigned int)time(0) );

    int fd = open( OUTPUT_FILE, O_RDWR | O_CREAT , 0x7777 );

    char* str = (char*)malloc( STR_MAX_LEN );
    size_t n = 0;

    void* buf = malloc( sizeof(size_t) + STR_MAX_LEN );

    size_t offset = 0;
    size_t size_  = 0;

    for(size_t i = 0; i < OUTPUT_NUM; ++i){
        n = rand()%STR_MAX_LEN;
        for(size_t j = 0; j < n; ++j){
            str[ j ] = rand()%128;
        }

        offset = 0;

        size_ = sizeof( size_t );
        memcpy( (char*)buf + offset, &n, size_ );
        offset += size_;

        size_ = n;
        memcpy( (char*)buf + offset, str, size_ );
        offset += size_;

        write( fd, buf, offset );
    }

    sync();

    free(str);
    free(buf);

    return 0;
}
