#include "bb.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[] )
{
    if( argc < 3 ) return -1;

    char * filename  = argv[1];
    char * filename_ = argv[2];

    printf("Read  data from : %s\n", filename  );
    printf("Write data from : %s\n", filename_ );

    int fd = open( filename, O_RDONLY, 0666 );
    if( fd < 0 ){
        printf("[ERROR] fail to open file : %s", filename );
        return 0;
    }

    struct bb_rw * bb = bb_rw_alloc( fd, 1, 1, 4096 );
    printf("--- test read bits ---\n");
    printf("rd_size = %lu\n", bb->rd_size );
    int count = 0;
    while( !(bb->rd_end) ){
        printf("%1d", bb->rd_bit(bb) );
        count += 1;
        if( count % 8  == 0 ) printf(" ");
        if( count % 32 == 0 ) printf("\n");
    }
    printf("\n");
    bb_rw_free( bb );
    lseek( fd, 0, SEEK_SET );

    printf("--- test read byte ---\n");
    bb = bb_rw_alloc( fd, 1, 0, 4096 );
    count = 0;
    while( !(bb->rd_end) ){
        printf("%-8d ", bb->rd_byte(bb) );
        count += 1;
        if( count % 4 == 0 ) printf("\n");
    }
    printf("\n");
    bb_rw_free( bb );
    lseek( fd, 0, SEEK_SET );

    unlink( filename_ );
    int fd_ = open( filename_, O_CREAT | O_WRONLY, 0666 );
    struct bb_rw * bb_ = bb_rw_alloc( fd_, 0, 1, 4096 );

    bb = bb_rw_alloc( fd, 1, 1, 4096 );
    while( !(bb->rd_end) ){
        bb_->wr_bit( bb_, bb->rd_bit(bb) );
    }
    bb_->flush( bb_ );
    bb_rw_free( bb_ );
    bb_rw_free( bb  );

    close( fd_ );
    close( fd );
    return 0;
}
