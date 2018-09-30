#include "circular_buffer.h"
#include <stdio.h>
#include <string.h>
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

// push at [ed]
void cb_push( struct circular_buffer * cb, void * elem )
{
    if( elem == NULL ) return;
    if( cb->n >= cb->elem_nr ) return;
    memcpy( cb->buf + cb->ed * cb->elem_size, elem, cb->elem_size );
    cb->ed = ( cb->ed + 1 ) % cb->elem_nr;
    cb->n += 1;
}

// pop [st]
void cb_pop( struct circular_buffer * cb )
{
    if( cb == NULL ) return;
    if( cb->n == 0 ) return;
    cb->st = ( cb->st + 1 ) % cb->elem_nr;
    cb->n -= 1;
}

void * cb_at( struct circular_buffer * cb, size_t index )
{
    if( cb == NULL ) return NULL;
    if( index >= cb->n ) return NULL;
    void * ret = _MALLOC_( uint8_t, cb->elem_size );
    index = ( cb->st + index ) % cb->elem_nr;
    memcpy( ret, cb->buf + index * cb->elem_size, cb->elem_size );
    return ret;
}

struct circular_buffer * cb_alloc( size_t elem_nr, size_t elem_size )
{
    struct circular_buffer * cb = _MALLOC_( struct circular_buffer, 1 );
    // buffer 
    cb->buf_size  = elem_nr * elem_size  ;
    cb->buf = _MALLOC_( uint8_t , cb->buf_size ) ;
    // elem
    cb->elem_size = elem_size ;
    cb->elem_nr   = elem_nr   ;
    // index
    cb->n  = 0 ;
    cb->ed = 0 ;
    cb->st = 0 ;
    // ops 
    cb->push = cb_push ;
    cb->pop  = cb_pop  ; 
    cb->at   = cb_at   ;
    return cb;
}

void cb_free( struct circular_buffer * cb )
{
    if( cb == NULL ) return;
    _FREE_( cb->buf );
    _FREE_( cb );
}
