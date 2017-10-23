#ifndef _BB_H_
#define _BB_H_

#include <sys/types.h>
#include <stdint.h>

struct bb_rw{
    int fd      ;
    int is_rd   ;
    int is_wr   ;
    int is_bit  ;
    int is_byte ;
    size_t offset_in_file ;
    size_t offset_in_buf  ;
    // read
    uint8_t *r_buf  ;
    ssize_t rd_size  ;
    int rd_end;
    // write
    uint8_t *w_buf  ;
    ssize_t wr_size  ;
    // buffer size
    size_t buf_size ;
    // operations 
    int (*rd_bit)( struct bb_rw * bb )          ;
    int (*wr_bit)( struct bb_rw * bb ,int bit ) ;
    uint16_t (*rd_byte)( struct bb_rw * bb )               ;
    uint16_t (*wr_byte)( struct bb_rw * bb , uint8_t byte) ;
    void (*flush)( struct bb_rw * bb );
};

struct bb_rw * bb_rw_alloc( int fd, int is_rd, int is_bit, size_t buf_size );
void bb_rw_free( struct bb_rw * brw );

#endif
