#include "bb.h"
#include <string.h>
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
    if( !(bb->is_rd && bb->is_bit && !(bb->rd_end) ) ) return -1;
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
        bb->offset_in_buf = 0;
    }
    return bit;
}

int bb_wr_bit( struct bb_rw * bb , int bit)
{
    if( !(bb->is_wr && bb->is_bit) ) return -1;
    int byte = bit & 1;
    int index_of_byte = bb->offset_in_buf / 8;
    int index_of_bit  = bb->offset_in_buf % 8;
    byte = byte << index_of_bit;
    bb->w_buf[ index_of_byte ] |= byte;
    bb->offset_in_buf += 1;
    if( bb->offset_in_buf == (size_t)bb->rd_size * 8 ){
        bb->wr_size = write( bb->fd, bb->w_buf, bb->buf_size );
        if( bb->wr_size < 0 ){
            printf("[ERROR] fail to write data bit\n");
            return -2;
        }
        bb->offset_in_buf = 0;
        bb->offset_in_file += bb->rd_size;
    }
    return 0;
}

uint16_t bb_rd_byte( struct bb_rw * bb )
{
    if( !(bb->is_rd && bb->is_byte && !(bb->rd_end) ) ) return 256;
    uint8_t byte = 0;
    int index_of_byte = bb->offset_in_buf;
    byte = bb->r_buf[ index_of_byte ];
    bb->offset_in_buf += 1;
    if( index_of_byte == bb-> rd_size ){
        bb->rd_size = read( bb->fd, bb->r_buf, bb->buf_size );
        if( bb->rd_size <= 0 ){
            bb->rd_end = 1;
        }
        bb->offset_in_buf = 0;
        bb->offset_in_file += bb->rd_size;
    }
    return byte;
}

uint16_t bb_wr_byte( struct bb_rw * bb , uint8_t byte )
{
    if( !(bb->is_wr && bb->is_byte) ) return 256;
    int index_of_byte = bb->offset_in_buf;
    bb->w_buf[ index_of_byte ] = byte;
    bb->offset_in_buf += 1;
    if( bb->offset_in_buf == bb->buf_size ){
        bb->wr_size = write( bb->fd, bb->w_buf, bb->buf_size );
        if( bb->wr_size < 0 ){
            printf("[ERROR] fail to write data byte\n");
            return 257;
        }
        bb->offset_in_buf = 0;
        bb->offset_in_file += bb->wr_size;
    }
    return byte;
}

void bb_flush( struct bb_rw * bb )
{
    if( !(bb->is_wr) ) return;
    if( bb->offset_in_buf == 0 ) return; // nothing to flush
    if( bb->is_bit ){
        // flush bits
        int size = bb->offset_in_buf / 8;
        if( bb->offset_in_buf % 8 ) size += 1;
        bb->wr_size = write( bb->fd, bb->w_buf, size );
        if( bb->wr_size < 0 ){
            printf("[ERROR] fail to flush data\n");
            return;
        }
    }else{
        // flush bytes
        bb->wr_size = write( bb->fd, bb->w_buf, bb->offset_in_buf );
        if( bb->wr_size < 0 ){
            printf("[ERROR] fail to flush data\n");
            return;
        }
    }
    bb->offset_in_file += bb->wr_size;
    bb->offset_in_buf = 0;
    fsync( bb->fd );
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
        if( bb_rw->rd_size > 0 ) bb_rw->rd_end = 0;
        else  bb_rw->rd_end = 1;
        bb_rw->offset_in_buf  = 0;
        bb_rw->offset_in_file = bb_rw->rd_size;
    }
    else if( bb_rw->is_wr ){
        /*bb_rw->w_buf_ = _MALLOC_( uint8_t, bb_rw->buf_size );*/
        bb_rw->w_buf  = _MALLOC_( uint8_t, bb_rw->buf_size );
        memset( bb_rw->w_buf, 0, bb_rw->buf_size );
        bb_rw->offset_in_file = 0;
        bb_rw->offset_in_buf  = 0;
    }
    // opsrations
    bb_rw->rd_bit  =  bb_rd_bit  ;
    bb_rw->wr_bit  =  bb_wr_bit  ;
    bb_rw->rd_byte =  bb_rd_byte ;
    bb_rw->wr_byte =  bb_wr_byte ;
    bb_rw->flush   =  bb_flush   ;
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
