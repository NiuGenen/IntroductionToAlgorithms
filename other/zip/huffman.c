#include "huffman.h"
#include <string.h>
#include <stdio.h>
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

void hfm_dy_add( struct huffman_dynamic * hfm_dy, uint8_t c );
void hfm_dy_clear( struct huffman_dynamic * dy);
void hfm_dy_pr( struct huffman_dynamic * dy);
void hfm_dy_code( struct huffman_dynamic * hfm_dy, uint8_t c, uint8_t * code, int * length );

void hfm_gen_code( struct huffman_dynamic * dy );

struct huffman_dynamic * huffman_dynamic_alloc()
{
    struct huffman_dynamic * dy = _MALLOC_( struct huffman_dynamic, 1 );
    dy->n = 1;
    dy->c_empty = 256;
    for( int i = 0; i < 257; ++i ){
        dy->counter[i] = 0;
        dy->sort[i] = 0;
        dy->code[i][0] = '\0';
        dy->code[i][256] = '\0';
        dy->code_length[i] = 0;
    }
    dy->counter[ dy->c_empty ] = 0;
    dy->sort[0] = dy->c_empty;
    // operations
    dy->add    = hfm_dy_add   ;
    dy->encode = hfm_dy_code  ;
    dy->pr     = hfm_dy_pr    ;
    dy->clear  = hfm_dy_clear ;
    hfm_gen_code( dy );
    return dy;
}

void huffman_dynamic_free( struct huffman_dynamic * hfm_dy )
{
    _FREE_( hfm_dy );
}

struct huffman_sort_entry{
    uint16_t c[256];
    int c_nr;
    size_t count;
};

void hfm_gen_code( struct huffman_dynamic * dy )
{
    // 1. clear code
    for( int i = 0; i < 257; ++i ){
        dy->code[i][0] = '\0';
        dy->code[i][256] = '\0';
        dy->code_length[i] = 0;
    }
    // 2. build entry
    struct huffman_sort_entry * entry = _MALLOC_( struct huffman_sort_entry, dy->n );
    for( int i = 0; i < dy->n; ++i ){
        entry[i].c_nr  = 1;
        entry[i].c[0]  = dy->sort[i];
        entry[i].count = dy->counter[ dy->sort[i] ];
    }
    // 3. huffman code
    int size = dy->n;
    while( size >= 2 ){
        // 3.1 get two smallest entry
        struct huffman_sort_entry * entry1 = &(entry[size - 1]);
        struct huffman_sort_entry * entry2 = &(entry[size - 2]);
        // 3.2 code '0'
        for( int i = 0; i < entry1->c_nr; ++i ){
            uint16_t cc = entry1->c[i];
            size_t cc_code_size = dy->code_length[cc];
            while( cc_code_size > 0 ){
                dy->code[cc][cc_code_size] = dy->code[cc][cc_code_size-1];
                cc_code_size -= 1;
            }
            dy->code[cc][0] = '0';
            dy->code_length[cc] += 1;
            dy->code[cc][ dy->code_length[cc] ] = '\0';
        }
        // 3.3 code '1'
        for( int i = 0; i < entry2->c_nr; ++i ){
            uint16_t cc = entry2->c[i];
            size_t cc_code_size = dy->code_length[cc];
            while( cc_code_size > 0 ){
                dy->code[cc][cc_code_size] = dy->code[cc][cc_code_size-1];
                cc_code_size -= 1;
            }
            dy->code[cc][0] = '1';
            dy->code_length[cc] += 1;
            dy->code[cc][ dy->code_length[cc] ] = '\0';
        }
        // 3.4 merge entry
        for( int i = 0; i < entry1->c_nr; ++i ){
            entry2->c[ entry2->c_nr + i ] = entry1->c[ i ];
        }
        entry2->c_nr += entry1->c_nr;
        if( entry2->count == 0 || entry1->count == 0 )
            entry2->count = 0;
        else
            entry2->count += entry1->count;
        // 3.5 sort
        size -= 1;
        if( size < 2 ) break;
        size_t _index = size - 1;
        struct huffman_sort_entry _entry = entry[ _index ];
        _index -= 1;
        while( _index > 0 ){
            if( _entry.count > entry[ _index ].count != 0 && ( _entry.count == 0 || _entry.count > entry[ _index ].count ) )
                entry[ _index + 1 ] = entry[ _index ];
            else break;
            _index -= 1;
        }
        entry[ _index + 1 ] = _entry;
    }
    free( entry );
}

void hfm_dy_add( struct huffman_dynamic * hfm_dy, uint8_t _c )
{
    uint16_t c = _c;
    hfm_dy->counter[ c ] += 1;
    // update sort
    if( hfm_dy->counter[ c ] == 1 ){
        hfm_dy->sort[ hfm_dy->n ] = c;
        hfm_dy->n += 1;
    }else{
        int index = hfm_dy->n - 1;
        while( index >= 0 && hfm_dy->sort[ index ] != c ) index -= 1;
        // sort[index] == c
        uint8_t c_code[257], t_code[257];
        memcpy( c_code, hfm_dy->code[ c ], 257 );
        while( index > 0 && hfm_dy->counter[ hfm_dy->sort[ index - 1 ] ] != 0 &&
            ( hfm_dy->counter[ c ] == 0 || hfm_dy->counter[ c ] > hfm_dy->counter[ hfm_dy->sort[ index - 1 ] ]  ) ){
            memcpy( t_code, hfm_dy->code[ hfm_dy->sort[index-1] ], 257 );
            memcpy( hfm_dy->code[ hfm_dy->sort[index-1] ], c_code , 257 );
            memcpy( c_code, t_code, 257 );
            hfm_dy->sort[ index ] = hfm_dy->sort[ index - 1 ];
            index -= 1;
        }
        memcpy( hfm_dy->code[c], c_code , 257 );
        hfm_dy->sort[ index ] = c;
        return;
    }
    // update code
    hfm_gen_code( hfm_dy );
}

// uint8_t code[ 256 + 1 ]
void hfm_dy_code( struct huffman_dynamic * dy, uint8_t c, uint8_t * code, int * length )
{
    // search 'c'
    int index = 0;
    for( ; index < dy->n; ++index){
        if( dy->sort[ index ] == c ) break;
    }
    // not found
    if( index == dy->n ) return;
    // found at sort[index]
    memcpy( code, dy->code[c], 257 );
    *length = dy->code_length[c];
    return;
    // special huffman code : in zip
    *length = 0;
    while( *length < index ){ code[ *length ] = '1'; *length += 1; }
    if( *length == dy->n - 1 ) code[ *length - 1 ] = '1';
    else{ code[ *length ] = '0'; *length += 1; }
    code[ *length ] = '\0';
}

void hfm_dy_pr( struct huffman_dynamic * dy)
{
    printf("--- huffman code table ---\n");
    uint8_t code[257];
    int length = 0;
    for( int i = 0; i < dy->n; ++i ){
        uint16_t c = dy->sort[ i ];
        if( c < 256 ) printf("[%3d]%c  ", (int)c, c );
        else          printf("[EMPTY] ");
        printf("count = %-8lu  ", dy->counter[c] );
        dy->encode( dy, c, code, &length );
        printf("code = %s\n", code);
    }
    printf("--------------------------\n");
}

void hfm_dy_clear( struct huffman_dynamic * dy)
{
    dy->n = 0;
    for( int i = 0; i < 256; ++i ) dy->counter[ i ] = 0;
}


void hfm_st_code( uint8_t c, uint8_t * code, size_t * length )
{

}

void hfm_st_pr( struct huffman_static * st )
{

}
