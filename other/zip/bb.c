#include "bb.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )
#define _FREE_(p) do{\
    if( p != NULL ){\
        free(p); \
        p = NULL; \
    }\
}while(0);

int bb_rd_bit( struct bb_rw * bb )
{
    if( !(bb->is_rd && bb->is_bit) ) return -1;
    if( bb->rd_size <= 0 ) return -2;
    int index_of_byte = bb->offset_in_buf / 8;
    int index_of_bit  = bb->offset_in_buf % 8;
    int bit = ( bb->r_buf[ index_of_byte ] >> index_of_bit ) & 1;
    bb->offset_in_buf += 1;
    if( bb->offset_in_buf == (size_t)bb->rd_size * 8 ){
        bb->rd_size = read( bb->fd, bb->r_buf, bb->buf_size );
        if( bb->rd_size <= 0 ){
            bb->rd_end = 1;
        }
    }
    return bit;
}

int bb_wr_bit( struct bb_rw * bb )
{
    if( !(bb->is_wr && bb->is_bit) ) return -1;
    int bit = 0;
    return bit;
}

uint16_t bb_rd_byte( struct bb_rw * bb )
{
    if( !(bb->is_rd && bb->is_byte) ) return 256;
    uint8_t byte = 0;
    return byte;
}

uint16_t bb_wr_byte( struct bb_rw * bb )
{
    if( !(bb->is_wr && bb->is_byte) ) return 256;
    uint8_t byte = 0;
    return byte;
}

struct bb_rw * bb_rw_alloc( int fd, int is_rd, int is_bit, size_t buf_size )
{
    struct bb_rw * bb_rw = _MALLOC_( struct bb_rw, 1 );
    bb_rw->fd = fd;
    bb_rw->buf_size = buf_size;
    // rd/wr
    bb_rw->is_wr = !is_rd ;
    bb_rw->is_rd = is_rd  ;
    // bit/byte 
    bb_rw->is_bit  = is_bit  ;
    bb_rw->is_byte = !is_bit ;
    // init buffer
    if( bb_rw->is_rd ){
        /*bb_rw->r_buf_ = _MALLOC_( uint8_t, bb_rw->buf_size );*/
        bb_rw->r_buf  = _MALLOC_( uint8_t, bb_rw->buf_size );
        bb_rw->rd_size = read( fd, bb_rw->r_buf, bb_rw->buf_size );
        printf("%lu\n", bb_rw->rd_size);
        if( bb_rw->rd_size > 0 ) bb_rw->rd_end = 0;
        else  bb_rw->rd_end = 1;
        bb_rw->offset_in_buf  = 0;
        bb_rw->offset_in_file = bb_rw->rd_size;
    }
    else if( bb_rw->is_wr ){
        /*bb_rw->w_buf_ = _MALLOC_( uint8_t, bb_rw->buf_size );*/
        bb_rw->w_buf  = _MALLOC_( uint8_t, bb_rw->buf_size );
        bb_rw->offset_in_file = 0;
        bb_rw->offset_in_buf  = 0;
    }
    // opsrations
    bb_rw->rd_bit  =  bb_rd_bit  ;
    bb_rw->wr_bit  =  bb_wr_bit  ;
    bb_rw->rd_byte =  bb_wr_byte ;
    bb_rw->wr_byte =  bb_wr_byte ;
    return bb_rw;
}

void bb_rw_free( struct bb_rw * bb_rw )
{
    /*_FREE_( bb_rw->r_buf_ ) ;*/
    _FREE_( bb_rw->r_buf  ) ;
    /*_FREE_( bb_rw->w_buf_ ) ;*/
    _FREE_( bb_rw->w_buf  ) ;
    _FREE_( bb_rw )         ;
}
