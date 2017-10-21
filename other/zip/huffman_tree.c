#include "huffman_tree.h"
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

int hfm_encode( struct huffman_tree * h, uint8_t c, uint8_t * code_in_bit );
void hfm_add(   struct huffman_tree * h, uint8_t c );
void hfm_pr(    struct huffman_tree * h );

void hfm_code_gen( struct huffman_tree * h );

struct huffman_tree_counter * huffman_tree_counter_alloc()
{
    struct huffman_tree_counter * counter = _MALLOC_( struct huffman_tree_counter, 1 );
    counter->node = NULL;
    counter->count = 0;
    return counter;
}

struct huffman_tree_node * huffman_tree_node_alloc()
{
    struct huffman_tree_node * node = _MALLOC_( struct huffman_tree_node, 1 );
    node->index = -1;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->is_leaf = 0;
    node->is_empty_node = 0;
    node->count = 0;
    node->c = 0;
    return node;
}

struct huffman_tree * huffman_tree_alloc( int dynamic )
{
    struct huffman_tree * h = _MALLOC_( struct huffman_tree, 1 );
    // sort
    h->sort = _MALLOC_( struct huffman_tree_node * , 256 * 2 - 1 );
    for( int i = 0; i < 511; ++i ) h->sort[i] = NULL;
    // counter
    h->counter = _MALLOC_( struct huffman_tree_counter*, 256 );
    for( int i = 0; i < 256; ++i ) h->counter[i] = NULL;
    // tree
    h->root = NULL;
    h->n = 0;
    // operations
    h->encode = hfm_encode;
    h->add    = hfm_add;
    h->pr     = hfm_pr;
    // dynamic huffman tree
    h->dynamic = dynamic;
    if( dynamic ){
        // alloc empty node
        h->sort[0] = huffman_tree_node_alloc();
        h->sort[0]->index = 0;
        h->sort[0]->count = 0;
        h->sort[0]->is_leaf = 1;
        h->sort[0]->is_empty_node = 1;
        h->sort[0]->parent = NULL;
        // set huffman tree
        h->root = h->sort[0];
        h->n = 1;
        h->empty_node = h->sort[0];
    }
    // return
    return h;
}

void huffman_tree_free( struct huffman_tree * h )
{
    for( int i = 0; i < 511; ++i ){
        if( h->sort[i] != NULL ){
            _FREE_( h->sort[i] );
        }
        if( i < 256 && h->counter[i] != NULL ){
            _FREE_( h->counter[i] );
        }
    }
    _FREE_( h->sort );
    _FREE_( h );
}

int hfm_encode( struct huffman_tree * h, uint8_t c, uint8_t * code_in_bit )
{
    if( !(h->dynamic) ){
        // build huffman tree
    }
    for( int i = 0; i < 256 * 2 - 1; ++i ){
        struct huffman_tree_node * node = h->sort[i];
        if( node->is_leaf && !(node->is_empty_node) && node->c == c )
        {
            memcpy( code_in_bit, node->code_in_bit, 32 );
            return node->code_length;
        }
    }
    return 0;
}

void hfm_code_gen_or( struct huffman_tree_node * node , uint8_t buf[32], int length, int bit )
{
    if( node == NULL ) return;
    buf[ length++ ] = bit + '0';
    buf[ length ] = '\0';
    if( node->is_leaf ){
        node->code_length = length;
        memcpy( node->code_in_bit, buf, 32 );
    }else{
        hfm_code_gen_or( node->left,  buf, length, 0 );
        hfm_code_gen_or( node->right, buf, length, 1 );
    }
}

void hfm_code_gen( struct huffman_tree * h )
{
    uint8_t code[32];
    hfm_code_gen_or( h->root->left , code, 0, 0 );
    hfm_code_gen_or( h->root->right, code, 0, 1 );
}

void hfm_dy_update( struct huffman_tree * h, int index )
{
    if( !(h->dynamic) ) return;
    struct huffman_tree_node * node = h->sort[ index ];
    while( node != NULL){
        int _index = index;
        while( _index >= 0 && h->sort[_index]->count == node->count )
            _index -= 1;
        _index += 1;
        if( _index != index ){
            struct huffman_tree_node * _node = h->sort[ _index ];
            // exchange node in sort
            h->sort[ index ] = _node; _node->index = index;
            h->sort[ _index ] = node; node->index = _index;
            // exchange node in tree
            struct huffman_tree_node * parent  = node->parent ;
            struct huffman_tree_node * _parent = _node->parent;
            if( parent == _parent && parent != NULL ){
                _node = parent->left;
                parent->left = parent->right;
                parent->right = _node;
            }
            else{
                _node->parent = parent;
                node->parent = _parent;
                if( parent != NULL ){
                    if( parent->left == node ) parent->left = _node;
                    if( parent->right == node ) parent->right = _node;
                }
                if( _parent != NULL ){
                    if( _parent->left == _node ) _parent->left = node;
                    if( _parent->right == _node ) _parent->right = node;
                }
            }
        }
        // increase count
        h->sort[ _index ]->count += 1;
        if( h->sort[ _index ]->is_leaf )
            h->counter[ h->sort[ _index ]->c ]->count += 1;
        // next loop
        node = node->parent;
        if( node != NULL )
            index = node->index;
    }
    // update root
    h->root = h->sort[0];
    // generate code
    hfm_code_gen( h );
}

void hfm_add( struct huffman_tree * h, uint8_t c )
{
    if( h->counter[c] != NULL ){
        if( !(h->dynamic) ){
            // update counter
            h->counter[c]->count += 1;
            // update node count
            h->counter[c]->node->count += 1;
            // update sort
            int index = h->counter[c]->node->index;
            while( index > 0 && h->sort[index]->count > h->sort[index-1]->count ){
                h->sort[index] = h->sort[index-1];
                index -= 1;
            }
            h->sort[index] = h->counter[c]->node;
        }else{
            int index = h->n - 1;
            // find index where h->sort[ index ].c == c
            while( index > 0 && h->sort[index]->c != c ) index -= 1;
            // update tree with [index]
            hfm_dy_update( h, index );
        }
    }else{
        struct huffman_tree_node * node =  huffman_tree_node_alloc();
        node->count = 1;
        node->c = c;
        node->is_leaf = 1;
        struct huffman_tree_counter * counter = huffman_tree_counter_alloc();
        counter->count = 1;
        counter->node = node;
        // set counter
        h->counter[c] = counter;
        // set sort
        h->sort[ h->n ] = node;
        node->index = h->n;
        if( h->dynamic ){
            h->sort[ h->n + 1 ] = h->empty_node;
            h->empty_node->index = h->n + 1;
            struct huffman_tree_node * n_parent = huffman_tree_node_alloc();
            struct huffman_tree_node * parent = h->empty_node->parent;
            n_parent->left = h->empty_node;
            n_parent->right = node;
            n_parent->is_leaf = 0;
            h->empty_node->parent = n_parent;
            node->parent = n_parent;
            n_parent->parent = parent;
            if( parent != NULL ) parent->left = n_parent;
            h->sort[ h->n - 1 ] = n_parent;
            n_parent->index = h->n - 1;
            // update tree with [ h->n - 1 ]
            hfm_dy_update( h, h->n - 1 );
            h->n += 1;
        }
        h->n += 1;
    }
/*
    printf("--- print to debug ---\n");
    for( int i = 0; i < h->n; ++i ){
        printf("addr = %-10lu, ", (size_t)h->sort[i] );
        printf("parent = %-10lu, ", (size_t)h->sort[i]->parent );
        if( h->sort[i]->is_leaf ){
            printf("count = %-5lu, char = %c ", h->sort[i]->count , h->sort[i]->c );
            printf("code = %s", h->sort[i]->code_in_bit );
        }
        else{
            printf("left = %-10lu, right = %-10lu, count = %-5lu", (size_t)h->sort[i]->left, (size_t)h->sort[i]->right, h->sort[i]->count );
        }
        printf("\n");
    }
    printf("----------------------\n\n");
    */
}

void hfm_pr( struct huffman_tree * h )
{
    printf("--------- huffman code --------\n");
    size_t bits = 0, char_t_s = 0, char_n_s = 0;
    for( int i = 0; i < h->n; ++i ){
        if( h->sort[i]->is_leaf ){
            char_t_s += 1;
            struct huffman_tree_node * node = h->sort[i];
            printf("char = %3d ", node->c );
            printf("count = %-5lu, ", node->count);
            printf("code_length = %-3d ", node->code_length );
            printf("code = %s ", node->code_in_bit );
            printf("\n");
            bits += node->count * node->code_length;
            char_n_s += node->count;
        }
    }
    printf("--- static huffman analysis ---\n");
    printf("number of char : %lu\n", char_n_s );
    printf("type   of char : %lu\n", char_t_s );
    printf("origin bits    : %lu\n", char_n_s * 8 );
    printf("huffman bits   : %lu\n", bits );
    printf("huffman avg    : %lf\n", (double) bits / (double) char_n_s );
    printf("-------------------------------\n");
}
