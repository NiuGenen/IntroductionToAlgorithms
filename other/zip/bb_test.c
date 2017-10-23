#include "bits.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

const char * filename = "/home/ng/asd.bit\0";

int main()
{
    int fd = open( filename, O_RDONLY, 0x0777 );
    if( fd < 0 ){
        printf("[ERROR] fail to open file : %s", filename );
        return 0;
    }

    struct Bb_rw * bb = Bb_rw_alloc( fd, 1, 1, 4096 );
    printf("rd_size = %lu\n", bb->rd_size );

    int count = 0;
    while( !(bb->rd_end) ){
        printf("%d", bb->rd_bit(bb) );
        count += 1;
        if( count % 8 == 0 ) printf("\n");
    }

    Bb_rw_free( bb );

    return 0;
}
