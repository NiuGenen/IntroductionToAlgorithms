#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

// 1.
//   [0] [1] [2] [3] [4] [5] ...... [n-1]
//   |    |---------------|             |
//       st               ed         
// 
// 2.
//   [0] [1] [2] [3] [4] [5] ...... [n-1]
//   |--------|       |-----------------|
//            ed      st             
// 
struct circular_buffer{
    uint8_t * buf   ;
    size_t buf_size ;
    size_t elem_nr  ;
    size_t elem_size; // 1 2 4 8
    size_t n;
    size_t st;
    size_t ed;
    // operations
    void (*push)( struct circular_buffer * cb, void * elem  );
    void (*pop) ( struct circular_buffer * cb               );
    void *(*at) ( struct circular_buffer * cb, size_t index );
};

struct circular_buffer * cb_alloc( size_t elem_nr, size_t elem_size );
void cb_free( struct circular_buffer * cb );

#endif
