#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stddef.h>
#include <stdint.h>

/*
 * no empry node version, which means it is NOT real dynamic
 * 
 * only to calculate huffman code
 * 
 */
struct huffman_dynamic{
    size_t counter[256]     ; // count  = [ char  ]
    uint8_t sort[256]       ; // char   = [ index ]
    uint8_t code[256][257]  ; // code   = [ char  ]
    int code_length[256]    ; // length = [ char ]
    int n;
    // operations 
    void (*add)( struct huffman_dynamic * dy, uint8_t c );
    void (*encode)( struct huffman_dynamic * dy, uint8_t c, uint8_t * code, int * length );
    void (*pr)( struct huffman_dynamic * dy );
    void (*clear)( struct huffman_dynamic * dy);
};

struct huffman_dynamic * huffman_dynamic_alloc();
void huffman_dynamic_free( struct huffman_dynamic * hfm_dy );

struct huffman_static{
    size_t counter[256];
    uint8_t sort[256];
    int n;
    // operations 
    void (*code)( uint8_t c, uint8_t * code, int * length );
    void (*pr)( struct huffman_static * );
    void (*clear)( struct huffman_static * dy);
};

struct huffman_static * huffman_static_alloc();
void huffman_static_free( struct huffman_static * hfm_dy );

#endif
